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

/*************************************************************************
   �ú�������ˮƽ�ƶ�DIBͼ�񡣺�������ı�ͼ��Ĵ�С���Ƴ��Ĳ���ͼ��
  ����ȥ���հײ����ð�ɫ��䡣
 ************************************************************************/

BOOL WINAPI TranslationDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, LONG lXOffset, LONG lYOffset)
{
	// ָ��Դͼ���ָ��
	LPSTR	lpSrc;
	
	// ָ��Ҫ���������ָ��
	LPSTR	lpDst;
	
	// ָ����ͼ���ָ��
	LPSTR	lpNewDIBBits;
	HLOCAL	hNewDIBBits;

	// ��������DIB�е�����
	LONG	i;
	LONG	j;
	
	// ������ԴDIB�е�����
	LONG	i0;
	LONG	j0;
	
	// ͼ��ÿ�е��ֽ���
	LONG lLineBytes;
	
	// ����ͼ��ÿ�е��ֽ���
	lLineBytes = WIDTHBYTES(lWidth * 8);
	
	// ��ʱ�����ڴ棬�Ա�����ͼ��
	hNewDIBBits = LocalAlloc(LHND, lLineBytes * lHeight);
	if (hNewDIBBits == NULL)
	{
		// �����ڴ�ʧ��
		return FALSE;
	}
	
	// �����ڴ�
	lpNewDIBBits = (char * )LocalLock(hNewDIBBits);
	
	// ÿ��
	for(i = 0; i < lHeight; i++)
	{
		// ÿ��
		for(j = 0; j < lWidth; j++)
		{
			// ָ����DIB��i�У���j�����ص�ָ��
			// ע������DIB��ͼ���һ����ʵ���������һ�е�λ�ã����lpDst
			// ֵ����(char *)lpNewDIBBits + lLineBytes * i + j������
			// (char *)lpNewDIBBits + lLineBytes * (lHeight - 1 - i) + j
			lpDst = (char *)lpNewDIBBits + lLineBytes * (lHeight - 1 - i) + j;
			
			// �����������ԴDIB�е�����
			i0 = i - lXOffset;
			j0 = j - lYOffset;
			
			// �ж��Ƿ���Դͼ��Χ��
			if( (j0 >= 0) && (j0 < lWidth) && (i0 >= 0) && (i0 < lHeight))
			{
				// ָ��ԴDIB��i0�У���j0�����ص�ָ��
				// ͬ��Ҫע��DIB���µ��õ�����
				lpSrc = (char *)lpDIBBits + lLineBytes * (lHeight - 1 - i0) + j0;
				
				// ��������
				*lpDst = *lpSrc;
			}
			else
			{
				// ����Դͼ��û�е����أ�ֱ�Ӹ�ֵΪ255
				* ((unsigned char*)lpDst) = 255;
			}
			
		}
	}
	
	// ����ƽ�ƺ��ͼ��
	memcpy(lpDIBBits, lpNewDIBBits, lLineBytes * lHeight);
	
	// �ͷ��ڴ�
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);
	
	// ����
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMyDIPView

IMPLEMENT_DYNCREATE(CMyDIPView, CScrollView)

BEGIN_MESSAGE_MAP(CMyDIPView, CScrollView)
	//{{AFX_MSG_MAP(CMyDIPView)
	ON_COMMAND(ID_MENUITEM32778, OnMenuitem32778)
	ON_COMMAND(ID_MENUITEM32779, OnMenuitem32779)
	ON_COMMAND(ID_MENUITEM32780, OnMenuitem32780)
	ON_COMMAND(ID_MENUITEM32781, OnMenuitem32781)
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


//�Աȶ�����
//DEL void CMyDIPView::OnMenuitem32777() 
//DEL {
//DEL 	
//DEL 	// ��ȡ�ĵ�
//DEL 	CMyDIPDoc* pDoc = GetDocument();
//DEL 	int i,j;
//DEL 	int r1=60,r2=200;
//DEL 	double k=1.5;
//DEL     unsigned char *lpSrc;	
//DEL 	ASSERT_VALID(pDoc);
//DEL 	if(pDoc->m_hDIB == NULL)
//DEL 		return ;
//DEL 	LPSTR lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->m_hDIB);
//DEL 	LPSTR lpDIBBits=::FindDIBBits (lpDIB);
//DEL 	int cxDIB = (int) ::DIBWidth(lpDIB);         // Size of DIB - x
//DEL 	int cyDIB = (int) ::DIBHeight(lpDIB);        // Size of DIB - y
//DEL 	long lLineBytes = WIDTHBYTES(cxDIB * 8);     // ����ͼ��ÿ�е��ֽ���
//DEL 	// ÿ��
//DEL 	for(i = 0; i < cyDIB; i++)
//DEL 	{
//DEL 		// ÿ��
//DEL 		for(j = 0; j < cxDIB; j++)
//DEL 		{
//DEL 			// ָ��DIB��i�У���j�����ص�ָ��
//DEL 			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (cyDIB - 1 - i) + j;
//DEL 			// �����µĻҶ�ֵ
//DEL 			if(*lpSrc<r1) *lpSrc=BYTE(*lpSrc/k);
//DEL 			else if(*lpSrc<r2) *lpSrc= BYTE((*lpSrc-r1)*k+r1/k);
//DEL 			else *lpSrc=BYTE((*lpSrc-r2)/k+255-(255-r2)/k);
//DEL 		}
//DEL 	}
//DEL 	::GlobalUnlock((HGLOBAL) pDoc->m_hDIB);
//DEL     Invalidate(TRUE);
//DEL }

void CMyDIPView::OnMenuitem32778() 
{
	// ƽ��λͼ

	// ��ȡ�ĵ�
	CMyDIPDoc* pDoc = GetDocument();
	
	// ָ��DIB��ָ��
	LPSTR	lpDIB;

	// ָ��DIB����ָ��
	LPSTR   lpDIBBits;
	
	// ����DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());
	
	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ��ƽ�ƣ������Ŀ������ƣ�
	if (::DIBNumColors(lpDIB) != 256)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ��ƽ�ƣ�", "ϵͳ��ʾ" , MB_ICONINFORMATION | MB_OK);

		// �������
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// ����
		return;
	}
	
	LONG lXOffset;
	LONG lYOffset;
	
		
	// ƽ����
	lXOffset = 0;
	lYOffset = -10;

	
	// ���Ĺ����״
	BeginWaitCursor();

	// �ҵ�DIBͼ��������ʼλ��
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// ����TranslationDIB()����ƽ��DIB
	if (TranslationDIB(lpDIBBits, ::DIBWidth(lpDIB), ::DIBHeight(lpDIB), lXOffset, lYOffset))
	{
		
		// ��������
		pDoc->SetModifiedFlag(TRUE);

		// ������ͼ
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// ��ʾ�û�
		MessageBox("�����ڴ�ʧ�ܣ�", "ϵͳ��ʾ" , MB_ICONINFORMATION | MB_OK);
	}
	
	// �������
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());

	// �ָ����
	EndWaitCursor();
}



void CMyDIPView::OnMenuitem32779() 
{
	// ƽ��λͼ

	// ��ȡ�ĵ�
	CMyDIPDoc* pDoc = GetDocument();
	
	// ָ��DIB��ָ��
	LPSTR	lpDIB;

	// ָ��DIB����ָ��
	LPSTR   lpDIBBits;
	
	// ����DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());
	
	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ��ƽ�ƣ������Ŀ������ƣ�
	if (::DIBNumColors(lpDIB) != 256)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ��ƽ�ƣ�", "ϵͳ��ʾ" , MB_ICONINFORMATION | MB_OK);

		// �������
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// ����
		return;
	}
	
	LONG lXOffset;
	LONG lYOffset;
	
		
	// ƽ����
	lXOffset = 0;
	lYOffset = 10;

	
	// ���Ĺ����״
	BeginWaitCursor();

	// �ҵ�DIBͼ��������ʼλ��
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// ����TranslationDIB()����ƽ��DIB
	if (TranslationDIB(lpDIBBits, ::DIBWidth(lpDIB), ::DIBHeight(lpDIB), lXOffset, lYOffset))
	{
		
		// ��������
		pDoc->SetModifiedFlag(TRUE);

		// ������ͼ
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// ��ʾ�û�
		MessageBox("�����ڴ�ʧ�ܣ�", "ϵͳ��ʾ" , MB_ICONINFORMATION | MB_OK);
	}
	
	// �������
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());

	// �ָ����
	EndWaitCursor();
}


void CMyDIPView::OnMenuitem32780() 
{
	// ƽ��λͼ

	// ��ȡ�ĵ�
	CMyDIPDoc* pDoc = GetDocument();
	
	// ָ��DIB��ָ��
	LPSTR	lpDIB;

	// ָ��DIB����ָ��
	LPSTR   lpDIBBits;
	
	// ����DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());
	
	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ��ƽ�ƣ������Ŀ������ƣ�
	if (::DIBNumColors(lpDIB) != 256)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ��ƽ�ƣ�", "ϵͳ��ʾ" , MB_ICONINFORMATION | MB_OK);

		// �������
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// ����
		return;
	}
	
	LONG lXOffset;
	LONG lYOffset;
	
		
	// ƽ����
	lXOffset = -10;
	lYOffset = 0;

	
	// ���Ĺ����״
	BeginWaitCursor();

	// �ҵ�DIBͼ��������ʼλ��
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// ����TranslationDIB()����ƽ��DIB
	if (TranslationDIB(lpDIBBits, ::DIBWidth(lpDIB), ::DIBHeight(lpDIB), lXOffset, lYOffset))
	{
		
		// ��������
		pDoc->SetModifiedFlag(TRUE);

		// ������ͼ
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// ��ʾ�û�
		MessageBox("�����ڴ�ʧ�ܣ�", "ϵͳ��ʾ" , MB_ICONINFORMATION | MB_OK);
	}
	
	// �������
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());

	// �ָ����
	EndWaitCursor();
}


void CMyDIPView::OnMenuitem32781() 
{
	// ƽ��λͼ

	// ��ȡ�ĵ�
	CMyDIPDoc* pDoc = GetDocument();
	
	// ָ��DIB��ָ��
	LPSTR	lpDIB;

	// ָ��DIB����ָ��
	LPSTR   lpDIBBits;
	
	// ����DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());
	
	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ��ƽ�ƣ������Ŀ������ƣ�
	if (::DIBNumColors(lpDIB) != 256)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ��ƽ�ƣ�", "ϵͳ��ʾ" , MB_ICONINFORMATION | MB_OK);

		// �������
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// ����
		return;
	}
	
	LONG lXOffset;
	LONG lYOffset;
	
		
	// ƽ����
	lXOffset = 10;
	lYOffset = 0;

	
	// ���Ĺ����״
	BeginWaitCursor();

	// �ҵ�DIBͼ��������ʼλ��
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// ����TranslationDIB()����ƽ��DIB
	if (TranslationDIB(lpDIBBits, ::DIBWidth(lpDIB), ::DIBHeight(lpDIB), lXOffset, lYOffset))
	{
		
		// ��������
		pDoc->SetModifiedFlag(TRUE);

		// ������ͼ
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// ��ʾ�û�
		MessageBox("�����ڴ�ʧ�ܣ�", "ϵͳ��ʾ" , MB_ICONINFORMATION | MB_OK);
	}
	
	// �������
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());

	// �ָ����
	EndWaitCursor();
}