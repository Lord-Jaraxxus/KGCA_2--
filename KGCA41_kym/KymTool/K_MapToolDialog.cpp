// K_MapToolDialog.cpp: 구현 파일
//

#include "pch.h"

#include "KymTool.h"
#include "K_MapToolDialog.h"
#include "afxdialogex.h"


// K_MapToolDialog 대화 상자

IMPLEMENT_DYNAMIC(K_MapToolDialog, CDialogEx)

K_MapToolDialog::K_MapToolDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
	, m_iRows(_T(""))
	, m_iCollumns(_T(""))
{

}

K_MapToolDialog::~K_MapToolDialog()
{
}

void K_MapToolDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, m_iRows);
	DDX_Text(pDX, IDC_EDIT1, m_iCollumns);
}


BEGIN_MESSAGE_MAP(K_MapToolDialog, CDialogEx)
	//ON_NOTIFY(NM_OUTOFMEMORY, IDC_HOTKEY1, &K_MapToolDialog::OnNMOutofmemoryHotkey1)
	ON_EN_CHANGE(IDC_EDIT1, &K_MapToolDialog::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT2, &K_MapToolDialog::OnEnChangeEdit2)
END_MESSAGE_MAP()


// K_MapToolDialog 메시지 처리기


void K_MapToolDialog::OnNMOutofmemoryHotkey1(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}


void K_MapToolDialog::OnEnChangeEdit1()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void K_MapToolDialog::OnEnChangeEdit2()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


BOOL K_MapToolDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_iRows = L"5";
	m_iCollumns = L"5";
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
