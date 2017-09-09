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
   �ú�����������DIBͼ�񣬷���������DIB�ľ����
************************************************************************/

HGLOBAL WINAPI ZoomDIB(LPSTR lpDIB, float fXZoomRatio, float fYZoomRatio)
{
	
	// Դͼ��Ŀ�Ⱥ͸߶�
	LONG	lWidth;
	LONG	lHeight;
	
	// ���ź�ͼ��Ŀ�Ⱥ͸߶�
	LONG	lNewWidth;
	LONG	lNewHeight;
	
	// ���ź�ͼ��Ŀ�ȣ�lNewWidth'��������4�ı�����
	LONG	lNewLineBytes;
	
	// ָ��Դͼ���ָ��
	LPSTR	lpDIBBits;
	
	// ָ��Դ���ص�ָ��
	LPSTR	lpSrc;
	
	// ���ź���DIB���
	HDIB	hDIB;
	
	// ָ������ͼ���Ӧ���ص�ָ��
	LPSTR	lpDst;
	
	// ָ������ͼ���ָ��
	LPSTR	lpNewDIB;
	LPSTR	lpNewDIBBits;
	
	// ָ��BITMAPINFO�ṹ��ָ�루Win3.0��
	LPBITMAPINFOHEADER lpbmi;
	
	// ָ��BITMAPCOREINFO�ṹ��ָ��
	LPBITMAPCOREHEADER lpbmc;
	
	// ѭ����������������DIB�е����꣩
	LONG	i;
	LONG	j;
	
	// ������ԴDIB�е�����
	LONG	i0;
	LONG	j0;
	
	// ͼ��ÿ�е��ֽ���
	LONG lLineBytes;
	
	// �ҵ�ԴDIBͼ��������ʼλ��
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// ��ȡͼ��Ŀ��
	lWidth = ::DIBWidth(lpDIB);
	
	// ����ͼ��ÿ�е��ֽ���
	lLineBytes = WIDTHBYTES(lWidth * 8);
	
	// ��ȡͼ��ĸ߶�
	lHeight = ::DIBHeight(lpDIB);
	
	// �������ź��ͼ��ʵ�ʿ��
	// �˴�ֱ�Ӽ�0.5������ǿ������ת��ʱ���������룬����ֱ�ӽ�ȥС������
	lNewWidth = (LONG) (::DIBWidth(lpDIB) * fXZoomRatio + 0.5);
	
	// ������ͼ��ÿ�е��ֽ���
	lNewLineBytes = WIDTHBYTES(lNewWidth * 8);
	
	// �������ź��ͼ��߶�
	lNewHeight = (LONG) (lHeight * fYZoomRatio + 0.5);
	
	// �����ڴ棬�Ա�����DIB
	hDIB = (HDIB) ::GlobalAlloc(GHND, lNewLineBytes * lNewHeight + *(LPDWORD)lpDIB + ::PaletteSize(lpDIB));
	
	// �ж��Ƿ��ڴ����ʧ��
	if (hDIB == NULL)
	{
		// �����ڴ�ʧ��
		return NULL;
	}
	
	// �����ڴ�
	lpNewDIB =  (char * )::GlobalLock((HGLOBAL) hDIB);
	
	// ����DIB��Ϣͷ�͵�ɫ��
	memcpy(lpNewDIB, lpDIB, *(LPDWORD)lpDIB + ::PaletteSize(lpDIB));
	
	// �ҵ���DIB������ʼλ��
	lpNewDIBBits = ::FindDIBBits(lpNewDIB);
	
	// ��ȡָ��
	lpbmi = (LPBITMAPINFOHEADER)lpNewDIB;
	lpbmc = (LPBITMAPCOREHEADER)lpNewDIB;
	
	// ����DIB��ͼ��ĸ߶ȺͿ��
	if (IS_WIN30_DIB(lpNewDIB))
	{
		// ����Windows 3.0 DIB
		lpbmi->biWidth = lNewWidth;
		lpbmi->biHeight = lNewHeight;
	}
	else
	{
		// ����������ʽ��DIB
		lpbmc->bcWidth = (unsigned short) lNewWidth;
		lpbmc->bcHeight = (unsigned short) lNewHeight;
	}
	
	// ���ͼ��ÿ�н��в���
	for(i = 0; i < lNewHeight; i++)
	{
		// ���ͼ��ÿ�н��в���
		for(j = 0; j < lNewWidth; j++)
		{
			
			// ָ����DIB��i�У���j�����ص�ָ��
			// ע��˴���Ⱥ͸߶�����DIB�Ŀ�Ⱥ͸߶�
			lpDst = (char *)lpNewDIBBits + lNewLineBytes * (lNewHeight - 1 - i) + j;
			
			// �����������ԴDIB�е�����
			i0 = (LONG) (i / fYZoomRatio + 0.5);
			j0 = (LONG) (j / fXZoomRatio + 0.5);
			
			// �ж��Ƿ���Դͼ��Χ��
			if( (j0 >= 0) && (j0 < lWidth) && (i0 >= 0) && (i0 < lHeight))
			{
				
				// ָ��ԴDIB��i0�У���j0�����ص�ָ��
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
	
	// ����
	return hDIB;
}


/////////////////////////////////////////////////////////////////////////////
// CMyDIPView

IMPLEMENT_DYNCREATE(CMyDIPView, CScrollView)

BEGIN_MESSAGE_MAP(CMyDIPView, CScrollView)
	//{{AFX_MSG_MAP(CMyDIPView)
	ON_COMMAND(ID_MENUITEM32778, OnMenuitem32778)
	ON_COMMAND(ID_MENUITEM32779, OnMenuitem32779)
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
	// ͼ������

	// ��ȡ�ĵ�
	CMyDIPDoc* pDoc = GetDocument();
	
	// ָ��DIB��ָ��
	LPSTR lpDIB;
	
	// ����DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());
	
	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ�����ţ������Ŀ������ƣ�
	if (::DIBNumColors(lpDIB) != 256)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ�����ţ�", "ϵͳ��ʾ" , MB_ICONINFORMATION | MB_OK);

		// �������
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// ����
		return;
	}
	
	// ���ű���
	float fXZoomRatio;
	float fYZoomRatio;

	//������
	fXZoomRatio = 0.8;
	fYZoomRatio = 0.8;

	
	// ������DIB
	HDIB hNewDIB = NULL;
	
	// ���Ĺ����״
	BeginWaitCursor();
	
	// ����ZoomDIB()����ת��DIB
	hNewDIB = (HDIB) ZoomDIB(lpDIB, fXZoomRatio, fYZoomRatio);
	
	// �ж������Ƿ�ɹ�
	if (hNewDIB != NULL)
	{
		
		// �滻DIB��ͬʱ�ͷž�DIB����
		pDoc->ReplaceHDIB(hNewDIB);

		// ����DIB��С�͵�ɫ��
		pDoc->InitDIBData();
		
		// ��������
		pDoc->SetModifiedFlag(TRUE);
		
		// �������ù�����ͼ��С
		SetScrollSizes(MM_TEXT, pDoc->GetDocSize());

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
	// ͼ������

	// ��ȡ�ĵ�
	CMyDIPDoc* pDoc = GetDocument();
	
	// ָ��DIB��ָ��
	LPSTR lpDIB;
	
	// ����DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());
	
	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ�����ţ������Ŀ������ƣ�
	if (::DIBNumColors(lpDIB) != 256)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ�����ţ�", "ϵͳ��ʾ" , MB_ICONINFORMATION | MB_OK);

		// �������
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// ����
		return;
	}
	
	// ���ű���
	float fXZoomRatio;
	float fYZoomRatio;

	//������
	fXZoomRatio = 1.25;
	fYZoomRatio = 1.25;

	
	// ������DIB
	HDIB hNewDIB = NULL;
	
	// ���Ĺ����״
	BeginWaitCursor();
	
	// ����ZoomDIB()����ת��DIB
	hNewDIB = (HDIB) ZoomDIB(lpDIB, fXZoomRatio, fYZoomRatio);
	
	// �ж������Ƿ�ɹ�
	if (hNewDIB != NULL)
	{
		
		// �滻DIB��ͬʱ�ͷž�DIB����
		pDoc->ReplaceHDIB(hNewDIB);

		// ����DIB��С�͵�ɫ��
		pDoc->InitDIBData();
		
		// ��������
		pDoc->SetModifiedFlag(TRUE);
		
		// �������ù�����ͼ��С
		SetScrollSizes(MM_TEXT, pDoc->GetDocSize());

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


