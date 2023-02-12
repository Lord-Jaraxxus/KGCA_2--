#include "K_Window.h"
HWND g_hWnd;
RECT g_rtClient;
K_Window* g_pWindow = nullptr;

//// 얘는 반드시 이 규격이여야 함? 외부 함수여야 한다;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // 핸들링(중계)
    _ASSERT(g_pWindow);
    return g_pWindow->MsgProc(hWnd, message, wParam, lParam);
}

LRESULT K_Window::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0); // 메세지큐에 직접 WM_QUIT
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam); // 메세지 내가 처리 불가, 니가 대신 '해줘'
    }
    return 0;
}

bool K_Window::TCoreInit()
{
    return false;
}

bool K_Window::TCoreFrame()
{
    return false;
}

bool K_Window::TCoreRender()
{
    return false;
}

bool K_Window::TCoreRelease()
{
    return false;
}

K_Window::K_Window()
{
    g_pWindow = this;
}

bool K_Window::Run()
{
    if (TCoreInit() == false) return false; // 이 Init은 밑에 샘플껏이 돌게될껐이다?
    //K_Window::Init();

    MSG msg = { 0, };
    // 기본 메시지 루프입니다:
    bool m_bGameRun = true;
    while (m_bGameRun)
    {
        if (msg.message == WM_QUIT)  break;

        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg); // 메세지 번역
            DispatchMessage(&msg);  // 메세지 프로시저에 전달
        }
        else // 처리할 메세지가 없을때 프레임 렌더 실행, 이게 게임이지 ㅋㅋ
        {
            if (!TCoreFrame() || !TCoreRender()) m_bGameRun = false;
        }

    }
    TCoreRelease();
    return true;
}

void K_Window::CenterWindow()
{
    UINT iScreenWidth = GetSystemMetrics(SM_CXFULLSCREEN);
    UINT iScreenHeight = GetSystemMetrics(SM_CYFULLSCREEN);
    UINT cx = (iScreenWidth - m_iClientWidth) / 2;
    UINT cy = (iScreenHeight - m_iClientHeight) / 2;
    MoveWindow(m_hWnd, cx, cy, m_iClientWidth, m_iClientHeight, TRUE);
}

bool K_Window::SetWindow(HINSTANCE hInstance, const WCHAR* szTitle, UINT iWidth, UINT iHeight)
{
    // 윈도우 등록
    WORD ret = MyRegisterClass();
    // 애플리케이션 초기화를 수행합니다:
    if (!InitInstance(szTitle, iWidth, iHeight))
    {
        return FALSE;
    }
    return false;
}

// 운영체제에 윈도우 등록
ATOM K_Window::MyRegisterClass()
{
    WNDCLASSEXW wcex; // 윈도우 클래스, 얘의 멤버들을 다 챙겨줘야 윈도우가 만들어짐
    ZeroMemory(&wcex, sizeof(WNDCLASSEX));

    // 요거 네개는 필수
    wcex.cbSize = sizeof(WNDCLASSEX); // 윈도우클래스 구조체 전체의 크기
    wcex.style = CS_HREDRAW | CS_VREDRAW; // 묻지도 따지지도 말고 무조건, 가로 세로로 갱신하겠다는 뜻? 일단 넘어가고
    wcex.hInstance = m_hInstance;
    //wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.hbrBackground = CreateSolidBrush(RGB(0, 255, 0)); // RGB는 시프트연산?

    // 요거 두개가 핵심
    // 윈도우 프로시저 함수 포인터, 윈도우 메세지를 받을 함수를 지정한다. 
    wcex.lpfnWndProc = WndProc; // 전화번호
    wcex.lpszClassName = L"킹영민 윈도우"; // 윈도우 이름
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW); // 마우스 커서 모냥

    // 이 밑에는 리소스가 있어야 한다, 나중에 하게씀
    //wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_KWINDOW));
    //wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_KWINDOW);
    //wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL K_Window::InitInstance(const WCHAR* szTitle, UINT iWidth, UINT iHeight)
{
    m_csStyle = WS_OVERLAPPEDWINDOW;

    RECT rc = { 100, 100, iWidth + 100, iHeight + 100 };
    AdjustWindowRect(&rc, m_csStyle, FALSE);

    // 운영체제에 등록한 윈도우를 생성한다
    m_hWnd = CreateWindowW(
        L"킹영민 윈도우",
        szTitle,
        m_csStyle, // 윈도우 스타일. 최소/최대창, 아이콘, 시스템 메뉴 등  이거저거 손볼 수 있음 - 게임에서는 팝업윈도우를 많이쓴다
        rc.left, rc.top,
        (rc.right - rc.left), (rc.bottom - rc.top),
        nullptr, nullptr,
        m_hInstance, nullptr);

    if (!m_hWnd)
    {
        return FALSE;
    }

    g_hWnd = m_hWnd;
    //g_rtClient = m_rtClient; //************** 이것 위치때문에 Draw가 안됐었는데, 대체 왜임??????

    ShowWindow(m_hWnd, SW_SHOW); // 윈도우가 화면에 뜬다. 쇼윈도에걸린너의셔츠를보며
    GetWindowRect(m_hWnd, &m_rtWindow); // 이거저거 다 있는 모든 화면영역
    GetClientRect(m_hWnd, &m_rtClient); // 이거저거 뺀 순수 출력화면
    g_rtClient = m_rtClient;

    m_iClientWidth = m_rtClient.right - m_rtClient.left;
    m_iClientHeight = m_rtClient.bottom - m_rtClient.top;
    CenterWindow();

    m_csStyle = GetWindowLong(m_hWnd, GWL_STYLE);

    //UpdateWindow(hWnd);
    return TRUE;
}