#pragma once
#pragma comment(lib, "fmod_vc.lib")

#include "K_Std.h"
#include "fmod.h"
#include "fmod.hpp"
#include "fmod_errors.h"

#include <codecvt> // wtm()
#include <tchar.h> // _stprintf_s

class K_Sound
{
public:
	FMOD::System*	m_pSystem = nullptr;
	FMOD::Sound*	m_pSound = nullptr;
	FMOD::Channel*	m_pChannel = nullptr;

public:
	unsigned int    m_TotalTime;
	std::wstring    m_szBuffer;

public:
	virtual bool Init();
	virtual bool Frame();
	virtual bool Render();
	virtual bool Release();

public:
	virtual bool Load(std::wstring filename);
	virtual void Play(bool bLoop);
	virtual void PlayEffect();
	virtual void Stop();
	virtual bool Pause();
	virtual bool Volume(bool bVolume);
	virtual bool SetLoop(bool bLoop);
};


static std::string wtm(std::wstring str)
{
	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
	return conv.to_bytes(str);
}

static const char* wtm_filename(std::wstring str) 
{
	static std::string filename = wtm(str);
	const char* fcc = filename.c_str();
	return fcc;
}
