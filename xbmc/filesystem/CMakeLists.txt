set(SOURCES AddonsDirectory.cpp
            AudioBookFileDirectory.cpp
            CacheStrategy.cpp
            CDDADirectory.cpp
            CDDAFile.cpp
            CircularCache.cpp
            CurlFile.cpp
            DAVCommon.cpp
            DAVDirectory.cpp
            DAVFile.cpp
            DirectoryCache.cpp
            Directory.cpp
            DirectoryFactory.cpp
            DirectoryHistory.cpp
            DllLibCurl.cpp
            EventsDirectory.cpp
            FavouritesDirectory.cpp
            FileCache.cpp
            File.cpp
            FileDirectoryFactory.cpp
            FileFactory.cpp
            FileReaderFile.cpp
            FTPDirectory.cpp
            FTPParse.cpp
            HTTPDirectory.cpp
            HTTPFile.cpp
            IDirectory.cpp
            IFile.cpp
            ImageFile.cpp
            iso9660.cpp
            ISO9660Directory.cpp
            ISOFile.cpp
            LibraryDirectory.cpp
            MemBufferCache.cpp
            MultiPathDirectory.cpp
            MultiPathFile.cpp
            MusicDatabaseDirectory.cpp
            MusicDatabaseFile.cpp
            MusicFileDirectory.cpp
            MusicSearchDirectory.cpp
            NptXbmcFile.cpp
            OverrideDirectory.cpp
            OverrideFile.cpp
            PipeFile.cpp
            PipesManager.cpp
            PlaylistDirectory.cpp
            PlaylistFileDirectory.cpp
            PluginDirectory.cpp
            PVRDirectory.cpp
            PVRFile.cpp
            ResourceDirectory.cpp
            ResourceFile.cpp
            RSSDirectory.cpp
            SAPDirectory.cpp
            SAPFile.cpp
            ShoutcastFile.cpp
            SmartPlaylistDirectory.cpp
            SMBDirectory.cpp
            SMBFile.cpp
            SourcesDirectory.cpp
            SpecialProtocol.cpp
            SpecialProtocolDirectory.cpp
            SpecialProtocolFile.cpp
            StackDirectory.cpp
            udf25.cpp
            UDFDirectory.cpp
            UDFFile.cpp
            UPnPDirectory.cpp
            UPnPFile.cpp
            VideoDatabaseDirectory.cpp
            VirtualDirectory.cpp
            ZeroconfDirectory.cpp
            XbtDirectory.cpp
            XbtFile.cpp
            XbtManager.cpp
            ZipDirectory.cpp
            ZipFile.cpp
            ZipManager.cpp)

if(OS STREQUAL "Android")
  list(APPEND SOURCES AndroidAppDirectory.cpp
                      AndroidAppFile.cpp
                      APKDirectory.cpp
                      APKFile.cpp)
endif()

if(BLURAY_FOUND)
  list(APPEND SOURCES BlurayDirectory.cpp
                      BlurayFile.cpp)
endif()

include_directories(${CORE_SOURCE_DIR}/lib/libUPnP
                    ${CORE_SOURCE_DIR}/lib/libUPnP/Platinum/Source/Core
                    ${CORE_SOURCE_DIR}/lib/libUPnP/Platinum/Source/Platinum
                    ${CORE_SOURCE_DIR}/lib/libUPnP/Platinum/Source/Devices/MediaConnect
                    ${CORE_SOURCE_DIR}/lib/libUPnP/Platinum/Source/Devices/MediaRenderer
                    ${CORE_SOURCE_DIR}/lib/libUPnP/Platinum/Source/Devices/MediaServer
                    ${CORE_SOURCE_DIR}/lib/libUPnP/Platinum/Source/Extras
                    ${CORE_SOURCE_DIR}/lib/libUPnP/Neptune/Source/System/Posix
                    ${CORE_SOURCE_DIR}/lib/libUPnP/Neptune/Source/Core)

add_definitions(-DNPT_CONFIG_ENABLE_LOGGING)

core_add_library(filesystem)
add_dependencies(filesystem libcpluff ffmpeg)
