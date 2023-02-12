#include "K_SoundManager.h"
#include <tchar.h>

bool K_SoundManager::Init()
{
    FMOD::System_Create(&m_pSystem);
    m_pSystem->init(32, FMOD_INIT_NORMAL, 0);

    return true;
}

bool K_SoundManager::Frame()
{

    m_pSystem->update();
    for (auto data : m_List)
    {
        K_Sound* pData = data.second;
        if (pData) pData->Frame();
    }
    return true;
}

bool K_SoundManager::Render()
{
    return true;
}

bool K_SoundManager::Release()
{
    for (auto data : m_List)
    {
        K_Sound* pData = data.second;
        if (pData) pData->Release();
        delete pData;
    }
    m_List.clear();
    return true;
}

K_SoundManager::K_SoundManager()
{
}

K_SoundManager::~K_SoundManager()
{
}


K_Sound* K_SoundManager::Load(std::wstring name)
{
    bool hr;

    //중복 제거
    auto iter = m_List.find(name);
    if (iter != m_List.end()) return iter->second;

    K_Sound* pNewData = new K_Sound;
    if (pNewData)
    {
        hr = pNewData->Load (m_pSystem, name);
        if (hr) m_List.insert(std::make_pair(name, pNewData));
    }
    return pNewData;
}

void	K_SoundManager::LoadDir(std::wstring path)
{
    W_STR dirpath = path + L"*.*";
    intptr_t handle;
    struct _wfinddata_t fd;
    handle = _wfindfirst(dirpath.c_str(), &fd);
    if (handle == -1L) return;

    do {
        // 디렉토리 라면
        if ((fd.attrib & _A_SUBDIR)
            && fd.name[0] != '.')
        {
            LoadDir(path + fd.name + L"/");
        }
        else if (fd.name[0] != '.')
        {
            m_fileList.push_back(path + fd.name);
        }
    } while (_wfindnext(handle, &fd) == 0);
    _findclose(handle);
}
void	K_SoundManager::LoadAll(std::wstring path)
{
    LoadDir(path);
    for (auto& data : m_fileList)
    {
        Load(data);
    }
}
K_Sound* K_SoundManager::GetPtr(W_STR name)
{
    auto iter = m_List.find(name);
    if (iter != m_List.end())
    {
        K_Sound* pSound = iter->second;
        return pSound;
    }
    return nullptr;
}

W_STR K_SoundManager::GetSplitName(std::wstring fullpath)
{
    W_STR name;
    TCHAR dirve[MAX_PATH] = { 0, };
    TCHAR dir[MAX_PATH] = { 0, };
    TCHAR filename[MAX_PATH] = { 0, };
    TCHAR ext[MAX_PATH] = { 0, };
    _tsplitpath_s(fullpath.c_str(),
        dirve, dir, filename, ext);
    name = filename;
    name += ext;
    return name;
}