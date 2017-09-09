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
#include "math.h"

#define PI 3.14159
//�Ƕȵ�����ת���ĺ�
#define RADIAN(angle) ((angle)*PI/180.0) 

/*************************************************************************
 *
 * �������ƣ�
 *   RotateDIB()
 *
 * ����:
 *   LPSTR lpDIB		- ָ��ԴDIB��ָ��
 *   int iRotateAngle	- ��ת�ĽǶȣ�0-360�ȣ�
 *
 * ����ֵ:
 *   HGLOBAL            - ��ת�ɹ�������DIB��������򷵻�NULL��
 *
 * ˵��:
 *   �ú���������ͼ������Ϊ������תDIBͼ�񣬷���������DIB�ľ����
 * ���øú������Զ�����ͼ������ʾ���е����ء������в������ڽ���
 * ֵ�㷨���в�ֵ��
 *
 ************************************************************************/

HGLOBAL WINAPI RotateDIB(LPSTR lpDIB, int iRotateAngle)
{
	
	// Դͼ��Ŀ�Ⱥ͸߶�
	LONG	lWidth;
	LONG	lHeight;
	
	// ��ת��ͼ��Ŀ�Ⱥ͸߶�
	LONG	lNewWidth;
	LONG	lNewHeight;
	
	// ͼ��ÿ�е��ֽ���
	LONG	lLineBytes;
	
	// ��ת��ͼ��Ŀ�ȣ�lNewWidth'��������4�ı�����
	LONG	lNewLineBytes;
	
	// ָ��Դͼ���ָ��
	LPSTR	lpDIBBits;
	
	// ָ��Դ���ص�ָ��
	LPSTR	lpSrc;
	
	// ��ת����DIB���
	HDIB	hDIB;
	
	// ָ����תͼ���Ӧ���ص�ָ��
	LPSTR	lpDst;
	
	// ָ����תͼ���ָ��
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
	
	// ��ת�Ƕȣ����ȣ�
	float	fRotateAngle;
	
	// ��ת�Ƕȵ����Һ�����
	float	fSina, fCosa;
	
	// Դͼ�ĸ��ǵ����꣨��ͼ������Ϊ����ϵԭ�㣩
	float	fSrcX1,fSrcY1,fSrcX2,fSrcY2,fSrcX3,fSrcY3,fSrcX4,fSrcY4;
	
	// ��ת���ĸ��ǵ����꣨��ͼ������Ϊ����ϵԭ�㣩
	float	fDstX1,fDstY1,fDstX2,fDstY2,fDstX3,fDstY3,fDstX4,fDstY4;
	
	// �����м䳣��
	float	f1,f2;
	
	// �ҵ�ԴDIBͼ��������ʼλ��
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// ��ȡͼ���"���"��4�ı�����
	lWidth = ::DIBWidth(lpDIB);
	
	// ����ͼ��ÿ�е��ֽ���
	lLineBytes = WIDTHBYTES(lWidth * 8);
	
	// ��ȡͼ��ĸ߶�
	lHeight = ::DIBHeight(lpDIB);
	
	// ����ת�ǶȴӶ�ת��������
	fRotateAngle = (float) RADIAN(iRotateAngle);
	
	// ������ת�Ƕȵ�����
	fSina = (float) sin((double)fRotateAngle);
	
	// ������ת�Ƕȵ�����
	fCosa = (float) cos((double)fRotateAngle);
	
	// ����ԭͼ���ĸ��ǵ����꣨��ͼ������Ϊ����ϵԭ�㣩
	fSrcX1 = (float) (- (lWidth  - 1) / 2);
	fSrcY1 = (float) (  (lHeight - 1) / 2);
	fSrcX2 = (float) (  (lWidth  - 1) / 2);
	fSrcY2 = (float) (  (lHeight - 1) / 2);
	fSrcX3 = (float) (- (lWidth  - 1) / 2);
	fSrcY3 = (float) (- (lHeight - 1) / 2);
	fSrcX4 = (float) (  (lWidth  - 1) / 2);
	fSrcY4 = (float) (- (lHeight - 1) / 2);
	
	// ������ͼ�ĸ��ǵ����꣨��ͼ������Ϊ����ϵԭ�㣩
	fDstX1 =  fCosa * fSrcX1 + fSina * fSrcY1;
	fDstY1 = -fSina * fSrcX1 + fCosa * fSrcY1;
	fDstX2 =  fCosa * fSrcX2 + fSina * fSrcY2;
	fDstY2 = -fSina * fSrcX2 + fCosa * fSrcY2;
	fDstX3 =  fCosa * fSrcX3 + fSina * fSrcY3;
	fDstY3 = -fSina * fSrcX3 + fCosa * fSrcY3;
	fDstX4 =  fCosa * fSrcX4 + fSina * fSrcY4;
	fDstY4 = -fSina * fSrcX4 + fCosa * fSrcY4;
	
	// ������ת���ͼ��ʵ�ʿ��
	lNewWidth  = (LONG) ( max( fabs(fDstX4 - fDstX1), fabs(fDstX3 - fDstX2) ) + 0.5);
	
	// ������ͼ��ÿ�е��ֽ���
	lNewLineBytes = WIDTHBYTES(lNewWidth * 8);
	
	// ������ת���ͼ��߶�
	lNewHeight = (LONG) ( max( fabs(fDstY4 - fDstY1), fabs(fDstY3 - fDstY2) )  + 0.5);
	
	// �������������������Ժ�ÿ�ζ�������
	f1 = (float) (-0.5 * (lNewWidth - 1) * fCosa - 0.5 * (lNewHeight - 1) * fSina
		+ 0.5 * (lWidth  - 1));
	f2 = (float) ( 0.5 * (lNewWidth - 1) * fSina - 0.5 * (lNewHeight - 1) * fCosa
		+ 0.5 * (lHeight - 1));
	
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
			i0 = (LONG) (-((float) j) * fSina + ((float) i) * fCosa + f2 + 0.5);
			j0 = (LONG) ( ((float) j) * fCosa + ((float) i) * fSina + f1 + 0.5);
			
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
	// ͼ����ת

	// ��ȡ�ĵ�
	CMyDIPDoc* pDoc = GetDocument();
	
	// ָ��DIB��ָ��
	LPSTR lpDIB;
	
	// ����DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());
	
	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ����ת�������Ŀ������ƣ�
	if (::DIBNumColors(lpDIB) != 256)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ����ת��", "ϵͳ��ʾ" , MB_ICONINFORMATION | MB_OK);

		// �������
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// ����
		return;
	}
	
	// ��ת�Ƕ�
	int iRotateAngle;
	
	iRotateAngle=15;
	
	// ������DIB
	HDIB hNewDIB = NULL;
	
	// ���Ĺ����״
	BeginWaitCursor();
	
	// ����RotateDIB()������תDIB
	hNewDIB = (HDIB) RotateDIB(lpDIB, iRotateAngle);
	
	// �ж���ת�Ƿ�ɹ�
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
	// ͼ����ת

	// ��ȡ�ĵ�
	CMyDIPDoc* pDoc = GetDocument();
	
	// ָ��DIB��ָ��
	LPSTR lpDIB;
	
	// ����DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());
	
	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ����ת�������Ŀ������ƣ�
	if (::DIBNumColors(lpDIB) != 256)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ����ת��", "ϵͳ��ʾ" , MB_ICONINFORMATION | MB_OK);

		// �������
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// ����
		return;
	}
	
	// ��ת�Ƕ�
	int iRotateAngle;
	
	iRotateAngle=-15;
	
	// ������DIB
	HDIB hNewDIB = NULL;
	
	// ���Ĺ����״
	BeginWaitCursor();
	
	// ����RotateDIB()������תDIB
	hNewDIB = (HDIB) RotateDIB(lpDIB, iRotateAngle);
	
	// �ж���ת�Ƿ�ɹ�
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