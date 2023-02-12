#include "K_Sound.h"

bool K_Sound::Init()
{
    return true;
}

bool K_Sound::Frame()
{
    if (m_pChannel)
    {
        unsigned int ms = 0;
        m_pChannel->getPosition(&ms, FMOD_TIMEUNIT_MS);
        TCHAR szBuffer[256] = { 0, };
        _stprintf_s(szBuffer,
            _T("경과 시간[%02d:%02d:%02d],총 플레이 타임[%02d:%02d:%02d]"),
            ms / 1000 / 60,
            ms / 1000 % 60,
            ms / 10 % 60,
            m_TotalTime / 1000 / 60,
            m_TotalTime / 1000 % 60,
            m_TotalTime / 10 % 60);
        m_szBuffer = szBuffer;
    }

    return true;
}

bool K_Sound::Render()
{
    return true;
}

bool K_Sound::Release()
{
    m_pSound->release();
    m_pSystem->close();
    m_pSystem->release();
    return true;
}

bool K_Sound::Load(FMOD::System* pSystem, std::wstring filename)
{
    m_pSystem = pSystem;

    FMOD_RESULT fr;

    //const char* filename_cc = wtm_filename(filename); // 이거 왜 안됨;
    std::string filename_m = wtm(filename);
    const char* filename_cc = filename_m.c_str();

    fr = m_pSystem->createSound(filename_cc, FMOD_DEFAULT, nullptr, &m_pSound);
    //fr = m_pSystem->createSound(filename_m.c_str(), FMOD_DEFAULT, nullptr, &m_pSound); // 이것도 되긴함

    if (fr == FMOD_OK){ m_pSound->getLength(&m_TotalTime, FMOD_TIMEUNIT_MS); }

    return true;
}

void K_Sound::Play(bool bLoop)
{
    FMOD_RESULT fr;
    bool isPlaying;
    m_pChannel->isPlaying(&isPlaying);

    if (!isPlaying) 
    { 
        fr = m_pSystem->playSound(m_pSound, nullptr, false, &m_pChannel); 
        if (fr == FMOD_OK)
        {
            float fVolume = 0.5f;
            m_pChannel->setVolume(fVolume);
            SetLoop(bLoop);
        }
    }
    //else
    //{
    //    Stop();
    //    fr = m_pSystem->playSound(m_pSound, nullptr, false, &m_pChannel);
    //    if (fr == FMOD_OK)
    //    {
    //        float fVolume = 0.5f;
    //        m_pChannel->setVolume(fVolume);
    //        SetLoop(bLoop);
    //    }
    //}
}

void K_Sound::PlayEffect()
{
    FMOD_RESULT fr;
    fr = m_pSystem->playSound(m_pSound, nullptr, false, &m_pChannel);
}


void K_Sound::Stop()
{
    FMOD_RESULT fr;
    m_pChannel->stop();
}

bool K_Sound::Pause()
{
    bool bPaused;
    m_pChannel->getPaused(&bPaused); // 퍼즈되어있으면 1을, 아니면 0을 가져옴
    m_pChannel->setPaused(!bPaused); // 퍼즈되어있으면 0을 넣어서 다시 틀고, 아니면 1을 넣어서 퍼즈

    return true;
}

// 볼륨 조절, 매개변수가 true면 볼륨업 false면 볼륨다운
bool K_Sound::Volume(bool bVolume)
{
    float fVolume;
    m_pChannel->getVolume(&fVolume);
    if (bVolume)
    {
        fVolume += 0.1f;
        fVolume = min(1.0f, fVolume);
    }
    else 
    {
        fVolume -= 0.1f;
        fVolume = max(0.0f, fVolume);
    }

    m_pChannel->setVolume(fVolume);

    return true;
}

bool K_Sound::SetLoop(bool bLoop)
{
    if (bLoop) m_pSound->setMode(FMOD_LOOP_NORMAL);
    else m_pSound->setMode(FMOD_LOOP_OFF);

    return true;
}

