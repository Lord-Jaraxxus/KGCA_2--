
// KymToolView.h: CKymToolView 클래스의 인터페이스
//

#pragma once


class CKymToolView : public CView
{
protected: // serialization에서만 만들어집니다.
	CKymToolView() noexcept;
	DECLARE_DYNCREATE(CKymToolView)

// 특성입니다.
public:
	CKymToolDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CKymToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

#ifndef _DEBUG  // KymToolView.cpp의 디버그 버전
inline CKymToolDoc* CKymToolView::GetDocument() const
   { return reinterpret_cast<CKymToolDoc*>(m_pDocument); }
#endif

