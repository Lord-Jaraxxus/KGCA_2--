#pragma once
#include "K_Sound.h"
#include <io.h>//_findclose

typedef std::basic_string<TCHAR> T_STR;
typedef std::basic_string<wchar_t> W_STR;
typedef std::basic_string<char>  C_STR;
typedef std::vector<std::basic_string<TCHAR>>		TCHAR_STRING_VECTOR;
typedef std::vector<DWORD>							DWORD_VECTOR;

class K_SoundManager : public K_Singleton<K_SoundManager>
{
public:
	bool Init();
	bool Frame();
	bool Render();
	bool Release();

private:
	FMOD::System* m_pSystem = nullptr;

private:
	friend class K_Singleton<K_SoundManager>;
	std::map<std::wstring, K_Sound*> m_List;
	std::list<std::wstring>  m_fileList;

private:
	K_SoundManager();
	~K_SoundManager();

public:
	K_Sound* Load(std::wstring name);

public:
	T_STR		GetSplitName(std::wstring name);
	void		LoadDir(std::wstring path);
	void		LoadAll(std::wstring path);
	K_Sound*	GetPtr(W_STR name);

};

#define I_Sound K_SoundManager::GetInstance()

