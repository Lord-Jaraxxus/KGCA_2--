
// KymToolView.cpp: CKymToolView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "KymTool.h"
#endif

#include "KymToolDoc.h"
#include "KymToolView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKymToolView

IMPLEMENT_DYNCREATE(CKymToolView, CView)

BEGIN_MESSAGE_MAP(CKymToolView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CKymToolView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CKymToolView 생성/소멸

CKymToolView::CKymToolView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CKymToolView::~CKymToolView()
{
}

BOOL CKymToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CKymToolView 그리기

void CKymToolView::OnDraw(CDC* /*pDC*/)
{
	CKymToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CKymToolView 인쇄


void CKymToolView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CKymToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CKymToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CKymToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}

void CKymToolView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CKymToolView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CKymToolView 진단

#ifdef _DEBUG
void CKymToolView::AssertValid() const
{
	CView::AssertValid();
}

void CKymToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CKymToolDoc* CKymToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKymToolDoc)));
	return (CKymToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CKymToolView 메시지 처리기


void CKymToolView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CKymToolApp* pApp = (CKymToolApp*)AfxGetApp();
	pApp->m_Sample.ResizeDevice(cx, cy);
}
