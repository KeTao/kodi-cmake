function(xbmc_add_library name)
  add_library(${name} STATIC ${SOURCES})
  set_target_properties(${name} PROPERTIES PREFIX "")
  if("${ARGN}" STREQUAL "")
    set(xbmc-bin_DEPENDS ${name} ${xbmc-bin_DEPENDS} CACHE STRING "" FORCE)
  endif()
endfunction()

function(xbmc_link_library lib wraplib)
  set(export -Wl,--unresolved-symbols=ignore-all
             `cat ${CMAKE_BINARY_DIR}/${XBMC_BUILD_DIR}/dllloader.dir/exports/wrapper.def`
             ${CMAKE_BINARY_DIR}/${XBMC_BUILD_DIR}/dllloader.dir/exports/CMakeFiles/wrapper.dir/wrapper.c.o)
  set(check_arg "")
  if(TARGET ${lib})
    set(target ${lib})
    set(link_lib ${CMAKE_BINARY_DIR}/${XBMC_BUILD_DIR}/${lib}.dir/${lib}.a)
    set(check_arg ${ARGV2})
    set(data_arg  ${ARGV3})
  else()
    set(target ${ARGV2})
    set(link_lib ${lib})
    set(check_arg ${ARGV3})
    set(data_arg ${ARGV4})
  endif()
  if("${check_arg}" STREQUAL "export")
    set(export ${export} 
        -Wl,--version-script=${ARGV3})
  elseif("${check_arg}" STREQUAL "nowrap")
    set(export ${data_arg})
  elseif("${check_arg}" STREQUAL "extras")
    list(APPEND export ${data_arg})
  endif()
  add_custom_command(OUTPUT ${wraplib}-${ARCH}${CMAKE_SHARED_MODULE_SUFFIX}
                     COMMAND ${CMAKE_C_COMPILER}
                     ARGS    -Wl,--whole-archive
                             ${link_lib}
                             -Wl,--no-whole-archive -lm
                             -shared -o ${CMAKE_BINARY_DIR}/${wraplib}-${ARCH}${CMAKE_SHARED_MODULE_SUFFIX}
                             ${export}
                     DEPENDS ${target} wrapper.def wrapper)
  list(APPEND WRAP_FILES ${wraplib}-${ARCH}${CMAKE_SHARED_MODULE_SUFFIX})
  set(WRAP_FILES ${WRAP_FILES} PARENT_SCOPE)
endfunction()

function(copy_file_to_buildtree file relative)
  string(REPLACE "\(" "\\(" filename ${file})
  string(REPLACE "\)" "\\)" file2 ${filename})
  string(REPLACE "${relative}/" "" file3 ${file2})

  if(NOT ${XBMC_SOURCE_DIR} MATCHES ${CMAKE_BINARY_DIR})
    if (NOT TARGET export-files)
      add_custom_target(export-files ALL COMMENT "Copying files into build tree")
    endif (NOT TARGET export-files)
    add_custom_command(TARGET export-files COMMAND ${CMAKE_COMMAND} -E copy_if_different "${file2}" "${CMAKE_CURRENT_BINARY_DIR}/${file3}")
  endif()
  list(APPEND install_data ${file2})
  set(install_data ${install_data} PARENT_SCOPE)
endfunction()

function(pack_xbt input output relative)
  file(GLOB_RECURSE MEDIA_FILES ${input}/*)
  get_filename_component(dir ${output} PATH)
  add_custom_command(OUTPUT  ${output}
                     COMMAND ${CMAKE_COMMAND} -E make_directory ${dir}
                     COMMAND ${CMAKE_BINARY_DIR}/${XBMC_BUILD_DIR}/texturepacker.dir/TexturePacker
                     ARGS    -input ${input}
                             -output ${output}
                             -dupecheck
                     DEPENDS ${MEDIA_FILES} TexturePacker)
  list(APPEND XBT_FILES ${output})
  set(XBT_FILES ${XBT_FILES} PARENT_SCOPE)
endfunction()

function(copy_skin_to_buildtree skin relative)
  file(GLOB_RECURSE FILES ${skin}/*)
  file(GLOB_RECURSE MEDIA_FILES ${skin}/media/*)
  list(REMOVE_ITEM FILES ${MEDIA_FILES})
  foreach(file ${FILES})
    copy_file_to_buildtree(${file} ${relative})
  endforeach()
  file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/${dest}/media)
  string(REPLACE "${relative}/" "" dest ${skin})
  pack_xbt(${skin}/media
           ${CMAKE_BINARY_DIR}/${dest}/media/Textures.xbt
           ${CMAKE_BINARY_DIR})
                
  set(XBT_FILES ${XBT_FILES} PARENT_SCOPE)
endfunction()

function(xbmc_find_soname lib)
  if(ARGV1)
    set(liblow ${ARGV1})
  else()
    string(TOLOWER ${lib} liblow)
  endif()
  if(${lib}_LDFLAGS)
    set(link_lib "${${lib}_LDFLAGS}")
  else()
    if(IS_ABSOLUTE "${${lib}_LIBRARIES}")
      set(link_lib "${${lib}_LIBRARIES}")
    else()
      set(link_lib -l${${lib}_LIBRARIES})
    endif()
  endif()
  execute_process(COMMAND ${CMAKE_C_COMPILER} -nostdlib -o /dev/null -Wl,-M ${link_lib} 
                  COMMAND grep LOAD.*${liblow}
                  ERROR_QUIET
                  OUTPUT_VARIABLE ${lib}_FILENAME)
  string(REPLACE "LOAD " "" ${lib}_FILENAME "${${lib}_FILENAME}")
  string(STRIP "${${lib}_FILENAME}" ${lib}_FILENAME)
  if(${lib}_FILENAME)
    execute_process(COMMAND objdump -p ${${lib}_FILENAME}
                    COMMAND grep SONAME.*${liblow}
                    ERROR_QUIET
                    OUTPUT_VARIABLE ${lib}_SONAME)
    string(REPLACE "SONAME " "" ${lib}_SONAME ${${lib}_SONAME})
    string(STRIP ${${lib}_SONAME} ${lib}_SONAME)
    message(STATUS "${lib} soname: ${${lib}_SONAME}")
    set(${lib}_SONAME ${${lib}_SONAME} PARENT_SCOPE)
  endif()
endfunction()

macro(export_dep)
  set(SYSTEM_INCLUDES ${SYSTEM_INCLUDES} PARENT_SCOPE)
  set(DEPLIBS ${DEPLIBS} PARENT_SCOPE)
  set(DEP_DEFINES ${DEP_DEFINES} PARENT_SCOPE)
  set(${depup}_FOUND ${${depup}_FOUND} PARENT_SCOPE)
endmacro()

function(xbmc_require_dep dep)
  find_package(${dep} REQUIRED)
  string(TOUPPER ${dep} depup)
  list(APPEND SYSTEM_INCLUDES ${${depup}_INCLUDE_DIRS})
  list(APPEND DEPLIBS ${${depup}_LIBRARIES})
  list(APPEND DEP_DEFINES ${${depup}_DEFINITIONS})
  export_dep()
endfunction()

function(xbmc_require_dyload_dep dep)
  find_package(${dep} REQUIRED)
  string(TOUPPER ${dep} depup)
  list(APPEND SYSTEM_INCLUDES ${${depup}_INCLUDE_DIRS})
  xbmc_find_soname(${depup})
  export_dep()
  set(${depup}_SONAME ${${depup}_SONAME} PARENT_SCOPE)
endfunction()

macro(setup_enable_switch)
  string(TOUPPER ${dep} depup)
  if (ARGV1)
    set(enable_switch ${ARGV1})
  else()
    set(enable_switch ENABLE_${depup})
  endif()
endmacro()

function(xbmc_optional_dep dep)
  setup_enable_switch()
  if(${enable_switch})
    find_package(${dep})
    if(${depup}_FOUND)
      list(APPEND SYSTEM_INCLUDES ${${depup}_INCLUDE_DIRS})
      list(APPEND DEPLIBS ${${depup}_LIBRARIES})
      list(APPEND DEP_DEFINES ${${depup}_DEFINITIONS})
      export_dep()
    endif()
  endif()
endfunction()

function(xbmc_optional_dyload_dep dep)
  setup_enable_switch()
  if(${enable_switch})
    find_package(${dep})
    if(${depup}_FOUND)
      list(APPEND SYSTEM_INCLUDES ${${depup}_INCLUDE_DIRS})
      xbmc_find_soname(${depup})
      list(APPEND DEP_DEFINES ${${depup}_DEFINITIONS})
      export_dep()
      set(${depup}_SONAME ${${depup}_SONAME} PARENT_SCOPE)
    endif()
  endif()
endfunction()
