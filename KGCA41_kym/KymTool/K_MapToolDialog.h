#pragma once


// K_MapToolDialog 대화 상자

class K_MapToolDialog : public CDialogEx
{
	DECLARE_DYNAMIC(K_MapToolDialog)

public:
	K_MapToolDialog(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~K_MapToolDialog();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnNMOutofmemoryHotkey1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnChangeEdit2();
	CString m_iRows;
	CString m_iCollumns;
	virtual BOOL OnInitDialog();
};
