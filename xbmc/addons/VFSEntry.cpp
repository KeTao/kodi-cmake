/*
 *      Copyright (C) 2013 Arne Morten Kvarving
 *
 *  This Program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This Program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with XBMC; see the file COPYING.  If not, see
 *  <http://www.gnu.org/licenses/>.
 *
 */
#include "VFSEntry.h"
#include "AddonManager.h"
#include "utils/StringUtils.h"
#include "URL.h"

namespace ADDON
{

class CVFSURLWrapper
{
  public:
    CVFSURLWrapper(const CURL& url2)
    {
      m_strings.push_back(url2.Get());
      m_strings.push_back(url2.GetDomain());
      m_strings.push_back(url2.GetHostName());
      m_strings.push_back(url2.GetFileName());
      m_strings.push_back(url2.GetOptions());
      m_strings.push_back(url2.GetUserName());
      m_strings.push_back(url2.GetPassWord());
      m_strings.push_back(url2.GetRedacted());
      m_strings.push_back(url2.GetShareName());

      url.url = m_strings[0].c_str();
      url.domain = m_strings[1].c_str();
      url.hostname = m_strings[2].c_str();
      url.filename = m_strings[3].c_str();
      url.port = url2.GetPort();
      url.options = m_strings[4].c_str();
      url.username = m_strings[5].c_str();
      url.password = m_strings[6].c_str();
      url.redacted = m_strings[7].c_str();
      url.sharename = m_strings[8].c_str();
    }

    VFSURL url;
  protected:
    std::vector<std::string> m_strings;
};

CVFSEntry::CVFSEntry(const cp_extension_t* ext)
 : VFSEntryDll(ext),
   m_protocols(CAddonMgr::Get().GetExtValue(ext->configuration, "@protocols")),
   m_extensions(CAddonMgr::Get().GetExtValue(ext->configuration, "@extensions")),
   m_files(CAddonMgr::Get().GetExtValue(ext->configuration, "@files") == "true"),
   m_directories(CAddonMgr::Get().GetExtValue(ext->configuration, "@directories") == "true"),
   m_filedirectories(CAddonMgr::Get().GetExtValue(ext->configuration, "@filedirectories") == "true")
{
}

AddonPtr CVFSEntry::Clone() const
{
  // Copy constructor is generated by compiler and calls parent copy constructor
  return AddonPtr(new CVFSEntry(*this));
}

void* CVFSEntry::Open(const CURL& url)
{
  if (!Initialized())
    return NULL;

  CVFSURLWrapper url2(url);
  return m_pStruct->Open(&url2.url);
}

void* CVFSEntry::OpenForWrite(const CURL& url, bool bOverWrite)
{
  if (!Initialized())
    return NULL;

  CVFSURLWrapper url2(url);
  return m_pStruct->OpenForWrite(&url2.url, bOverWrite);
}

bool CVFSEntry::Exists(const CURL& url)
{
  if (!Initialized())
    return false;

  CVFSURLWrapper url2(url);
  return m_pStruct->Exists(&url2.url);
}

int CVFSEntry::Stat(const CURL& url, struct __stat64* buffer)
{
  if (!Initialized())
    return -1;

  CVFSURLWrapper url2(url);
  return m_pStruct->Stat(&url2.url, buffer);
}

ssize_t CVFSEntry::Read(void* ctx, void* lpBuf, size_t uiBufSize)
{
  if (!Initialized())
    return 0;

  return m_pStruct->Read(ctx, lpBuf, uiBufSize);
}

ssize_t CVFSEntry::Write(void* ctx, void* lpBuf, size_t uiBufSize)
{
  if (!Initialized())
    return 0;

  return m_pStruct->Write(ctx, lpBuf, uiBufSize);
}

int64_t CVFSEntry::Seek(void* ctx, int64_t position, int whence)
{
  if (!Initialized())
    return 0;

  return m_pStruct->Seek(ctx, position, whence);
}

int CVFSEntry::Truncate(void* ctx, int64_t size)
{
  if (!Initialized())
    return 0;

  return m_pStruct->Truncate(ctx, size);
}

void CVFSEntry::Close(void* ctx)
{
  if (!Initialized())
    return;

  m_pStruct->Close(ctx);
}

int64_t CVFSEntry::GetPosition(void* ctx)
{
  if (!Initialized())
    return 0;

  return m_pStruct->GetPosition(ctx);
}

int CVFSEntry::GetChunkSize(void* ctx)
{
  if (!Initialized())
    return 0;

  return m_pStruct->GetChunkSize(ctx);
}

int64_t CVFSEntry::GetLength(void* ctx)
{
  if (!Initialized())
    return 0;

  return m_pStruct->GetLength(ctx);
}

int CVFSEntry::IoControl(void* ctx, XFILE::EIoControl request, void* param)
{
  if (!Initialized())
    return -1;

  return m_pStruct->IoControl(ctx, request, param);
}

bool CVFSEntry::Delete(const CURL& url)
{
  if (!Initialized())
    return false;

  CVFSURLWrapper url2(url);
  return m_pStruct->Delete(&url2.url);
}

bool CVFSEntry::Rename(const CURL& url, const CURL& url2)
{
  if (!Initialized())
    return false;

  CVFSURLWrapper url3(url);
  CVFSURLWrapper url4(url2);
  return m_pStruct->Rename(&url3.url, &url4.url);
}

void CVFSEntry::ClearOutIdle()
{
  if (!Initialized())
    return;

  m_pStruct->ClearOutIdle();
}

void CVFSEntry::DisconnectAll()
{
  if (!Initialized())
    return;

  m_pStruct->DisconnectAll();
}

bool CVFSEntry::DirectoryExists(const CURL& url)
{
  if (!Initialized())
    return false;

  CVFSURLWrapper url2(url);
  return m_pStruct->DirectoryExists(&url2.url);
}

bool CVFSEntry::RemoveDirectory(const CURL& url)
{
  if (!Initialized())
    return false;

  CVFSURLWrapper url2(url);
  return m_pStruct->RemoveDirectory(&url2.url);
}

bool CVFSEntry::CreateDirectory(const CURL& url)
{
  if (!Initialized())
    return false;

  CVFSURLWrapper url2(url);
  return m_pStruct->CreateDirectory(&url2.url);
}

static void VFSDirEntriesToCFileItemList(int num_entries,
                                         VFSDirEntry* entries,
                                         CFileItemList& items)
{
  for (int i=0;i<num_entries;++i)
  {
    CFileItemPtr item(new CFileItem());
    item->SetLabel(entries[i].label);
    item->SetPath(entries[i].path);
    item->m_dwSize = entries[i].size;
    item->m_dateTime = entries[i].mtime;
    item->m_bIsFolder = entries[i].folder;
    if (entries[i].title)
      item->m_strTitle = entries[i].title;
    for (int j=0;j<entries[i].num_props;++j)
    {
      if (strcasecmp(entries[i].properties[j].name, "propmisusepreformatted") == 0)
      {
        if (strcasecmp(entries[i].properties[j].name, "true") == 0)
          item->SetLabelPreformated(true);
        else
          item->SetLabelPreformated(false);
      } else
        item->SetProperty(entries[i].properties[j].name,
                          entries[i].properties[j].val);
    }
    items.Add(item);
  }
}

bool CVFSEntry::GetDirectory(const CURL& url, CFileItemList& items,
                             void* ctx)
{
  if (!Initialized())
    return false;

  VFSCallbacks callbacks;
  callbacks.ctx = ctx;
  callbacks.GetKeyboardInput = CVFSEntryIDirectoryWrapper::DoGetKeyboardInput;
  callbacks.SetErrorDialog = CVFSEntryIDirectoryWrapper::DoSetErrorDialog;
  callbacks.RequireAuthentication = CVFSEntryIDirectoryWrapper::DoRequireAuthentication;

  VFSDirEntry* entries;
  int num_entries;
  CVFSURLWrapper url2(url);
  void* ctx2 = m_pStruct->GetDirectory(&url2.url, &entries, &num_entries, &callbacks);
  if (ctx2)
  {
    VFSDirEntriesToCFileItemList(num_entries, entries, items);
    m_pStruct->FreeDirectory(ctx2);

    return true;
  }

  return false;
}

bool CVFSEntry::ContainsFiles(const CURL& url, CFileItemList& items)
{
  if (!Initialized())
    Create();

  VFSDirEntry* entries;
  int num_entries;

  CVFSURLWrapper url2(url);
  char rootpath[1024];
  rootpath[0] = 0;
  void* ctx = m_pStruct->ContainsFiles(&url2.url, &entries, &num_entries, rootpath);
  if (!ctx)
    return false;

  VFSDirEntriesToCFileItemList(num_entries, entries, items);
  m_pStruct->FreeDirectory(ctx);
  if (strlen(rootpath))
    items.SetPath(rootpath);

  return true;
}

bool CVFSEntry::NextChannel(void* ctx, bool preview)
{
  if (!Initialized())
    return false;

  return m_pStruct->NextChannel(ctx, preview);
}

bool CVFSEntry::PrevChannel(void* ctx, bool preview)
{
  if (!Initialized())
    return false;

  return m_pStruct->PrevChannel(ctx, preview);
}

bool CVFSEntry::SelectChannel(void* ctx, unsigned int channel)
{
  if (!Initialized())
    return false;

  return m_pStruct->SelectChannel(ctx, channel);
}

int CVFSEntry::GetTotalTime(void* ctx)
{
  if (!Initialized())
    return false;

  return m_pStruct->GetTotalTime(ctx);
}

int CVFSEntry::GetStartTime(void* ctx)
{
  if (!Initialized())
    return false;

  return m_pStruct->GetStartTime(ctx);
}

bool CVFSEntry::UpdateItem(void* ctx, CFileItem& item)
{
  if (!Initialized())
    return false;

  return false;//m_pStruct->UpdateItem(ctx);
}

CVFSEntryIFileWrapper::CVFSEntryIFileWrapper(VFSEntryPtr ptr) :
  m_context(NULL), m_addon(ptr)
{
}

CVFSEntryIFileWrapper::~CVFSEntryIFileWrapper()
{
  Close();
}

bool CVFSEntryIFileWrapper::Open(const CURL& url)
{
  m_context = m_addon->Open(url);
  return m_context != NULL;
}

bool CVFSEntryIFileWrapper::OpenForWrite(const CURL& url, bool bOverWrite)
{
  m_context = m_addon->OpenForWrite(url, bOverWrite);
  return m_context != NULL;
}

bool CVFSEntryIFileWrapper::Exists(const CURL& url)
{
  return m_addon->Exists(url);
}

int CVFSEntryIFileWrapper::Stat(const CURL& url, struct __stat64* buffer)
{
  return m_addon->Stat(url, buffer);
}

int CVFSEntryIFileWrapper::Truncate(int64_t size)
{
  return m_addon->Truncate(m_context, size);
}

ssize_t CVFSEntryIFileWrapper::Read(void* lpBuf, size_t uiBufSize)
{
  if (!m_context)
    return 0;

  return m_addon->Read(m_context, lpBuf, uiBufSize);
}

ssize_t CVFSEntryIFileWrapper::Write(void* lpBuf, size_t uiBufSize)
{
  if (!m_context)
    return 0;

  return m_addon->Write(m_context, lpBuf, uiBufSize);
}

int64_t CVFSEntryIFileWrapper::Seek(int64_t iFilePosition, int iWhence)
{
  if (!m_context)
    return 0;

  return m_addon->Seek(m_context, iFilePosition, iWhence);
}

void CVFSEntryIFileWrapper::Close()
{
  if (m_context)
  {
    m_addon->Close(m_context);
    m_context = NULL;
  }
}

int64_t CVFSEntryIFileWrapper::GetPosition()
{
  if (!m_context)
    return 0;

  return m_addon->GetPosition(m_context);
}

int CVFSEntryIFileWrapper::GetChunkSize()
{
  if (!m_context)
    return 0;

  return m_addon->GetChunkSize(m_context);
}

int64_t CVFSEntryIFileWrapper::GetLength()
{
  if (!m_context)
    return 0;

  return m_addon->GetLength(m_context);
}

int CVFSEntryIFileWrapper::IoControl(XFILE::EIoControl request, void* param)
{
  if (!m_context)
    return 0;

  return m_addon->IoControl(m_context, request, param);
}

bool CVFSEntryIFileWrapper::Delete(const CURL& url)
{
  return m_addon->Delete(url);
}

bool CVFSEntryIFileWrapper::Rename(const CURL& url, const CURL& url2)
{
  return m_addon->Rename(url, url2);
}

CVFSEntryILiveTVWrapper::CVFSEntryILiveTVWrapper(VFSEntryPtr ptr) :
  CVFSEntryIFileWrapper(ptr)
{
}

bool CVFSEntryILiveTVWrapper::NextChannel(bool preview)
{
  if (!m_context)
    return false;

  return m_addon->NextChannel(m_context, preview);
}

bool CVFSEntryILiveTVWrapper::PrevChannel(bool preview)
{
  if (!m_context)
    return false;

  return m_addon->PrevChannel(m_context, preview);
}

bool CVFSEntryILiveTVWrapper::SelectChannel(unsigned int channel)
{
  if (!m_context)
    return false;

  return m_addon->SelectChannel(m_context, channel);
}

int CVFSEntryILiveTVWrapper::GetTotalTime()
{
  if (!m_context)
    return 0;

  return m_addon->GetTotalTime(m_context);
}

int CVFSEntryILiveTVWrapper::GetStartTime()
{
  if (!m_context)
    return 0;

  return m_addon->GetStartTime(m_context);
}

bool CVFSEntryILiveTVWrapper::UpdateItem(CFileItem& item)
{
  if (!m_context)
    return false;

  return m_addon->UpdateItem(m_context, item);
}

CVFSEntryIDirectoryWrapper::CVFSEntryIDirectoryWrapper(VFSEntryPtr ptr) :
  m_addon(ptr)
{
}

bool CVFSEntryIDirectoryWrapper::Exists(const CURL& url)
{
  return m_addon->DirectoryExists(url);
}

bool CVFSEntryIDirectoryWrapper::Remove(const CURL& url)
{
  return m_addon->RemoveDirectory(url);
}

bool CVFSEntryIDirectoryWrapper::Create(const CURL& url)
{
  return m_addon->CreateDirectory(url);
}

bool CVFSEntryIDirectoryWrapper::GetDirectory(const CURL& url,
                                              CFileItemList& items)
{
  return m_addon->GetDirectory(url, items, this);
}

bool CVFSEntryIDirectoryWrapper::DoGetKeyboardInput(void* ctx,
                                                    const char* heading,
                                                    char** input)
{
  return ((CVFSEntryIDirectoryWrapper*)ctx)->GetKeyboardInput2(heading, input);
}

bool CVFSEntryIDirectoryWrapper::GetKeyboardInput2(const char* heading,
                                                   char** input)
{
  std::string inp;
  bool result;
  if ((result=GetKeyboardInput(CVariant(std::string(heading)), inp)))
    *input = strdup(inp.c_str());

  return result;
}

void CVFSEntryIDirectoryWrapper::DoSetErrorDialog(void* ctx, const char* heading,
                                                  const char* line1,
                                                  const char* line2,
                                                  const char* line3)
{
  ((CVFSEntryIDirectoryWrapper*)ctx)->SetErrorDialog2(heading, line1,
                                                      line2, line3);
}

void CVFSEntryIDirectoryWrapper::SetErrorDialog2(const char* heading,
                                                 const char* line1,
                                                 const char* line2,
                                                 const char* line3)
{
  CVariant l2=0, l3=0;
  if (line2)
    l2 = std::string(line2);
  if (line3)
    l3 = std::string(line3);
  if (m_flags & XFILE::DIR_FLAG_ALLOW_PROMPT)
    SetErrorDialog(CVariant(std::string(heading)),
                   CVariant(std::string(line1)), l2, l3);
}

void CVFSEntryIDirectoryWrapper::DoRequireAuthentication(void* ctx,
                                                         const char* url)
{
  ((CVFSEntryIDirectoryWrapper*)ctx)->RequireAuthentication2(CURL(url));
}

void CVFSEntryIDirectoryWrapper::RequireAuthentication2(const CURL& url)
{
  if (m_flags & XFILE::DIR_FLAG_ALLOW_PROMPT)
    RequireAuthentication(url);
}
                                
CVFSEntryManager::CVFSEntryManager()
{
}

CVFSEntryManager& CVFSEntryManager::Get()
{
  static CVFSEntryManager instance;

  return instance;
}

VFSEntryPtr CVFSEntryManager::GetAddon(const std::string& id)
{
  if (m_addons.find(id) == m_addons.end())
  {
    AddonPtr addon;
    CAddonMgr::Get().GetAddon(id, addon, ADDON_VFS);
    if (addon)
    {
      VFSEntryPtr ptr = std::static_pointer_cast<CVFSEntry>(addon);
      ptr->Create();
      m_addons.insert(make_pair(id, ptr));
      return m_addons[id];
    }
    return VFSEntryPtr();
  }

  return m_addons[id];
}

void CVFSEntryManager::ClearOutIdle()
{
  for (std::map<std::string, VFSEntryPtr>::iterator it  = m_addons.begin();
                                                    it != m_addons.end(); ++it)
    it->second->ClearOutIdle();
}

void CVFSEntryManager::DisconnectAll()
{
  for (std::map<std::string, VFSEntryPtr>::iterator it  = m_addons.begin();
                                                    it != m_addons.end(); ++it)
    it->second->DisconnectAll();
}

} /*namespace ADDON*/

