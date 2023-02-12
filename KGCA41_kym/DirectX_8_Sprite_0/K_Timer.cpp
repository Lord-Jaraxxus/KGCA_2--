#include "K_Timer.h"

float g_fGameTimer = 0.0f;
float g_fSecondPerFrame = 0.0f;

bool K_Timer::Init()
{
    m_fGameTimer = 0.0f;
    m_fElapseTimer = 0.0f;
    dwBeforeTime = timeGetTime(); // 1000ÀÌ 1ÃÊ
    return true;
}

bool K_Timer::Frame()
{
    DWORD dwCurrentTime = timeGetTime();
    DWORD dwElapseTime = dwCurrentTime - dwBeforeTime;
    m_fElapseTimer = dwElapseTime / 1000.0f;
    m_fGameTimer += m_fElapseTimer;

    m_iFPSCounter++;
    m_fFPSTimer += m_fElapseTimer;
    if (m_fFPSTimer >= 1.0f)
    {
        m_iFPS = m_iFPSCounter;
        m_iFPSCounter = 0;
        m_fFPSTimer = 0.0f;
    }
    
    g_fGameTimer = m_fGameTimer;
    g_fSecondPerFrame = m_fElapseTimer;
    dwBeforeTime = timeGetTime();

    return true;
}

bool K_Timer::Render()
{
    m_szTimer = std::to_wstring(m_fGameTimer);

    m_szTimer += L" ";
    m_szTimer += std::to_wstring(m_iFPS);
    m_szTimer += L"\n";
    OutputDebugString(m_szTimer.c_str());

    return true;
}

bool K_Timer::Release()
{
    return true;
}
