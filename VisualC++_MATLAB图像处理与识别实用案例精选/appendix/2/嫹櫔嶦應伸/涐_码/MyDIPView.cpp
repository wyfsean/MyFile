// MyDIPView.cpp : implementation of the CMyDIPView class
//

#include "stdafx.h"
#include "MyDIP.h"

#include "MyDIPDoc.h"
#include "MyDIPView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyDIPView

IMPLEMENT_DYNCREATE(CMyDIPView, CScrollView)

BEGIN_MESSAGE_MAP(CMyDIPView, CScrollView)
	//{{AFX_MSG_MAP(CMyDIPView)
	ON_COMMAND(ID_MY_CODING_4mul4, OnMYCODING4mul4)
	ON_COMMAND(ID_MY_CODING_8mul8, OnMYCODING8mul8)
	ON_COMMAND(ID_MY_CODING_2mul2, OnMYCODING2mul2)
	ON_COMMAND(ID_MY_CODING_16mul16, OnMYCODING16mul16)
	ON_COMMAND(ID_MENUITEM32777, OnMenuitem32777)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyDIPView construction/destruction

CMyDIPView::CMyDIPView()
{
	// TODO: add construction code here

}

CMyDIPView::~CMyDIPView()
{
}

BOOL CMyDIPView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMyDIPView drawing

void CMyDIPView::OnDraw(CDC* pDC)
{   
	CMyDIPDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if(pDoc->m_hDIB == NULL)
		return ;
	// TODO: add draw code for native data here
	int i,j;
    unsigned char *lpSrc;
	LPSTR lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->m_hDIB);
	int cxDIB = (int) ::DIBWidth(lpDIB);         // Size of DIB - x
	int cyDIB = (int) ::DIBHeight(lpDIB);        // Size of DIB - y
	LPSTR lpDIBBits=::FindDIBBits (lpDIB);
	// ����ͼ��ÿ�е��ֽ���
	long lLineBytes = WIDTHBYTES(cxDIB * 8);
	// ÿ��
	for(i = 0; i < cyDIB; i++)
	{
		// ÿ��
		for(j = 0; j < cxDIB; j++)
		{
			// ָ��DIB��i�У���j�����ص�ָ��
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (cyDIB - 1 - i) + j;
			// �����µĻҶ�ֵ
			//*(lpSrc) = BYTE(255-*lpSrc);
		}
	}
	::GlobalUnlock((HGLOBAL) pDoc->m_hDIB);
	CRect rect(0,0,cxDIB,cyDIB), rcDIB(0,0,cxDIB,cyDIB);
	::PaintDIB(pDC->m_hDC, &rect, pDoc->m_hDIB, &rcDIB, pDoc->m_palDIB);
}

/////////////////////////////////////////////////////////////////////////////
// CMyDIPView printing

BOOL CMyDIPView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMyDIPView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMyDIPView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CMyDIPView diagnostics

#ifdef _DEBUG
void CMyDIPView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CMyDIPView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CMyDIPDoc* CMyDIPView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMyDIPDoc)));
	return (CMyDIPDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMyDIPView message handlers

void CMyDIPView::OnSize(UINT nType, int cx, int cy)
{
	CScrollView::OnSize(nType, cx, cy);
}

void CMyDIPView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	SetScrollSizes(MM_TEXT, GetDocument()->m_sizeDoc);
}

void CMyDIPView::OnMYCODING4mul4() 
{
	// ��ȡ�ĵ�
	CMyDIPDoc* pDoc = GetDocument();
	
	//���з�����롣���﷽����Ϊ4���߶�Ϊ4
	rectcoding(pDoc->m_hDIB ,4,4);

	//ˢ��
    Invalidate(TRUE);
}

void CMyDIPView::OnMYCODING8mul8() 
{
	// ��ȡ�ĵ�
	CMyDIPDoc* pDoc = GetDocument();
	
	//���з�����롣���﷽����Ϊ8���߶�Ϊ8
	rectcoding(pDoc->m_hDIB ,8,8);

	//ˢ��
    Invalidate(TRUE);
}


void CMyDIPView::OnMYCODING2mul2() 
{
	// ��ȡ�ĵ�
	CMyDIPDoc* pDoc = GetDocument();
	
	//���з�����롣���﷽����Ϊ2���߶�Ϊ2
	rectcoding(pDoc->m_hDIB ,2,2);

	//ˢ��
    Invalidate(TRUE);
}


void CMyDIPView::OnMYCODING16mul16() 
{
	// ��ȡ�ĵ�
	CMyDIPDoc* pDoc = GetDocument();
	
	//���з�����롣���﷽����Ϊ16���߶�Ϊ16
	rectcoding(pDoc->m_hDIB ,16,16);

	//ˢ��
    Invalidate(TRUE);	
}


//�Աȶ�����
void CMyDIPView::OnMenuitem32777() 
{
	
	// ��ȡ�ĵ�
	CMyDIPDoc* pDoc = GetDocument();
	int i,j;
	int r1=60,r2=200;
	double k=1.5;
    unsigned char *lpSrc;	
	ASSERT_VALID(pDoc);
	if(pDoc->m_hDIB == NULL)
		return ;
	LPSTR lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->m_hDIB);
	LPSTR lpDIBBits=::FindDIBBits (lpDIB);
	int cxDIB = (int) ::DIBWidth(lpDIB);         // Size of DIB - x
	int cyDIB = (int) ::DIBHeight(lpDIB);        // Size of DIB - y
	long lLineBytes = WIDTHBYTES(cxDIB * 8);     // ����ͼ��ÿ�е��ֽ���
	// ÿ��
	for(i = 0; i < cyDIB; i++)
	{
		// ÿ��
		for(j = 0; j < cxDIB; j++)
		{
			// ָ��DIB��i�У���j�����ص�ָ��
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (cyDIB - 1 - i) + j;
			// �����µĻҶ�ֵ
			if(*lpSrc<r1) *lpSrc=BYTE(*lpSrc/k);
			else if(*lpSrc<r2) *lpSrc= BYTE((*lpSrc-r1)*k+r1/k);
			else *lpSrc=BYTE((*lpSrc-r2)/k+255-(255-r2)/k);
		}
	}
	::GlobalUnlock((HGLOBAL) pDoc->m_hDIB);
    Invalidate(TRUE);
}
