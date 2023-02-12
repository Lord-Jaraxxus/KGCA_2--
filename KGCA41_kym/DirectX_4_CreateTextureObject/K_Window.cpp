#include "K_Window.h"
HWND g_hWnd;
RECT g_rtClient;
K_Window* g_pWindow = nullptr;

//// ��� �ݵ�� �� �԰��̿��� ��? �ܺ� �Լ����� �Ѵ�;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    // �ڵ鸵(�߰�)
    _ASSERT(g_pWindow);
    return g_pWindow->MsgProc(hWnd, message, wParam, lParam);
}

LRESULT K_Window::MsgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0); // �޼���ť�� ���� WM_QUIT
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam); // �޼��� ���� ó�� �Ұ�, �ϰ� ��� '����'
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
    if (TCoreInit() == false) return false; // �� Init�� �ؿ� ���ò��� ���Եɲ��̴�?
    //K_Window::Init();

    MSG msg = { 0, };
    // �⺻ �޽��� �����Դϴ�:
    bool m_bGameRun = true;
    while (m_bGameRun)
    {
        if (msg.message == WM_QUIT)  break;

        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg); // �޼��� ����
            DispatchMessage(&msg);  // �޼��� ���ν����� ����
        }
        else // ó���� �޼����� ������ ������ ���� ����, �̰� �������� ����
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
    // ������ ���
    WORD ret = MyRegisterClass();
    // ���ø����̼� �ʱ�ȭ�� �����մϴ�:
    if (!InitInstance(szTitle, iWidth, iHeight))
    {
        return FALSE;
    }
    return false;
}

// �ü���� ������ ���
ATOM K_Window::MyRegisterClass()
{
    WNDCLASSEXW wcex; // ������ Ŭ����, ���� ������� �� ì����� �����찡 �������
    ZeroMemory(&wcex, sizeof(WNDCLASSEX));

    // ��� �װ��� �ʼ�
    wcex.cbSize = sizeof(WNDCLASSEX); // ������Ŭ���� ����ü ��ü�� ũ��
    wcex.style = CS_HREDRAW | CS_VREDRAW; // ������ �������� ���� ������, ���� ���η� �����ϰڴٴ� ��? �ϴ� �Ѿ��
    wcex.hInstance = m_hInstance;
    //wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.hbrBackground = CreateSolidBrush(RGB(0, 255, 0)); // RGB�� ����Ʈ����?

    // ��� �ΰ��� �ٽ�
    // ������ ���ν��� �Լ� ������, ������ �޼����� ���� �Լ��� �����Ѵ�. 
    wcex.lpfnWndProc = WndProc; // ��ȭ��ȣ
    wcex.lpszClassName = L"ŷ���� ������"; // ������ �̸�
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW); // ���콺 Ŀ�� ���

    // �� �ؿ��� ���ҽ��� �־�� �Ѵ�, ���߿� �ϰԾ�
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

    // �ü���� ����� �����츦 �����Ѵ�
    m_hWnd = CreateWindowW(
        L"ŷ���� ������",
        szTitle,
        m_csStyle, // ������ ��Ÿ��. �ּ�/�ִ�â, ������, �ý��� �޴� ��  �̰����� �պ� �� ���� - ���ӿ����� �˾������츦 ���̾���
        rc.left, rc.top,
        (rc.right - rc.left), (rc.bottom - rc.top),
        nullptr, nullptr,
        m_hInstance, nullptr);

    if (!m_hWnd)
    {
        return FALSE;
    }

    g_hWnd = m_hWnd;
    //g_rtClient = m_rtClient; //************** �̰� ��ġ������ Draw�� �ȵƾ��µ�, ��ü ����??????

    ShowWindow(m_hWnd, SW_SHOW); // �����찡 ȭ�鿡 ���. ���������ɸ����Ǽ���������
    GetWindowRect(m_hWnd, &m_rtWindow); // �̰����� �� �ִ� ��� ȭ�鿵��
    GetClientRect(m_hWnd, &m_rtClient); // �̰����� �� ���� ���ȭ��
    g_rtClient = m_rtClient;

    m_iClientWidth = m_rtClient.right - m_rtClient.left;
    m_iClientHeight = m_rtClient.bottom - m_rtClient.top;
    CenterWindow();

    m_csStyle = GetWindowLong(m_hWnd, GWL_STYLE);

    //UpdateWindow(hWnd);
    return TRUE;
}