// TypeRecView.cpp : implementation of the CTypeRecView class
//

#include "stdafx.h"
#include "TypeRec.h"
#include "mainfrm.h"

#include "io.h"
#include "direct.h"
#include "string.h"

#include "TypeRecDoc.h"
#include "TypeRecView.h"

#include "ColorTable.h"

//#include "suanfa1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTypeRecView

IMPLEMENT_DYNCREATE(CTypeRecView, CView)

BEGIN_MESSAGE_MAP(CTypeRecView, CView)
//{{AFX_MSG_MAP(CTypeRecView)
ON_WM_ERASEBKGND()
ON_COMMAND(ID_FILE_256ToGray, OnFILE256ToGray)
ON_COMMAND(ID_FILE_24ToGray, OnFILE24ToGray)
ON_COMMAND(ID_TEMP_SUBRECT, OnTempSubrect)
ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
ON_COMMAND(ID_TEMP_PALETTE, OnTempPalette)
ON_COMMAND(ID_TEMP_GRAY, OnTempGray)
ON_COMMAND(ID_TEMP_ERROR, OnTempError)
ON_COMMAND(ID_TEST_1_1, OnTest11)
ON_COMMAND(ID_TEST_1_2, OnTest12)
ON_COMMAND(ID_TEST_1_3, OnTest13)
ON_COMMAND(ID_TEST_1_4, OnTest14)
ON_COMMAND(ID_TEST_1_5, OnTest15)
ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
ON_COMMAND(ID_TEST_1_45, OnTest145)
ON_COMMAND(ID_TEST_1_6, OnTest16)
//}}AFX_MSG_MAP
// Standard printing commands
ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTypeRecView construction/destruction

CTypeRecView::CTypeRecView()
{
	m_ipzLeft=0;
	m_ipzRight=0;
	m_ipzBottom=0;
	m_ipzTop=0;
}

CTypeRecView::~CTypeRecView()
{
}

BOOL CTypeRecView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	
	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CTypeRecView drawing

void CTypeRecView::OnDraw(CDC* pDC)
{
	// ��ʾ�ȴ����
	BeginWaitCursor();
	
	// ��ȡ�ĵ�
	CTypeRecDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	// ��ȡDIB
	HDIB hDIB = pDoc->GetHDIB();
	
	// �ж�DIB�Ƿ�Ϊ��
	if (hDIB != NULL)
	{
		LPSTR lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);
		
		// ��ȡDIB���
		int cxDIB = (int) ::DIBWidth(lpDIB);
		
		// ��ȡDIB�߶�
		int cyDIB = (int) ::DIBHeight(lpDIB);
		
		::GlobalUnlock((HGLOBAL) hDIB);
		
		CRect rcDIB;
		rcDIB.top = rcDIB.left = 0;
		rcDIB.right = cxDIB;
		rcDIB.bottom = cyDIB;
		
		CRect rcDest;
		
		// �ж��Ƿ��Ǵ�ӡ
		if (pDC->IsPrinting())
		{
			// �Ǵ�ӡ���������ͼ���λ�úʹ�С���Ա����ҳ��
			
			// ��ȡ��ӡҳ���ˮƽ���(����)
			int cxPage = pDC->GetDeviceCaps(HORZRES);
			
			// ��ȡ��ӡҳ��Ĵ�ֱ�߶�(����)
			int cyPage = pDC->GetDeviceCaps(VERTRES);
			
			// ��ȡ��ӡ��ÿӢ��������
			int cxInch = pDC->GetDeviceCaps(LOGPIXELSX);
			int cyInch = pDC->GetDeviceCaps(LOGPIXELSY);
			
			// �����ӡͼ���С�����ţ�����ҳ���ȵ���ͼ���С��
			rcDest.top = rcDest.left = 0;
			rcDest.bottom = (int)(((double)cyDIB * cxPage * cyInch)
				/ ((double)cxDIB * cxInch));
			rcDest.right = cxPage;
			
			// �����ӡͼ��λ�ã���ֱ���У�
			int temp = cyPage - (rcDest.bottom - rcDest.top);
			rcDest.bottom += temp/2;
			rcDest.top += temp/2;
			
		}
		else   
			// �Ǵ�ӡ
		{
			// ��������ͼ��
			rcDest = rcDIB;
		}
		
		// ���DIB
		::PaintDIB(pDC->m_hDC, &rcDest, pDoc->GetHDIB(),
			&rcDIB, pDoc->GetDocPalette());
	}
	
	// �ָ��������
	EndWaitCursor();
}

/////////////////////////////////////////////////////////////////////////////
// CTypeRecView printing

BOOL CTypeRecView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTypeRecView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTypeRecView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CTypeRecView diagnostics

#ifdef _DEBUG
void CTypeRecView::AssertValid() const
{
	CView::AssertValid();
}

void CTypeRecView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTypeRecDoc* CTypeRecView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTypeRecDoc)));
	return (CTypeRecDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTypeRecView message handlers

BOOL CTypeRecView::OnEraseBkgnd(CDC* pDC) 
{
	// ��Ҫ��Ϊ�������Ӵ���Ĭ�ϵı���ɫ
	// ����ɫ���ĵ���Ա����m_refColorBKGָ��
	
	// ��ȡ�ĵ�
	CTypeRecDoc* pDoc = GetDocument();
	
	// ����һ��Brush
	CBrush brush(pDoc->m_refColorBKG);                                              
	
	// ������ǰ��Brush
	CBrush* pOldBrush = pDC->SelectObject(&brush);
	
	// ��ȡ�ػ�����
	CRect rectClip;
	pDC->GetClipBox(&rectClip);
	
	// �ػ�
	pDC->PatBlt(rectClip.left, rectClip.top, rectClip.Width(), rectClip.Height(), PATCOPY);
	
	// �ָ���ǰ��Brush
	pDC->SelectObject(pOldBrush);                                                  
	
	// ����
	return TRUE;
	
}

void CTypeRecView::OnFILE256ToGray() 
{
	
	// ��256ɫλͼת���ɻҶ�ͼ
	
	// ��ȡ�ĵ�
	CTypeRecDoc* pDoc = GetDocument();
	
	// ָ��DIB��ָ��
	LPSTR	lpDIB;
	
	// ����DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());
	
	// ��ɫ���е���ɫ��Ŀ
	WORD wNumColors;
	
	// ��ȡDIB����ɫ���е���ɫ��Ŀ
	wNumColors = ::DIBNumColors(lpDIB);
	
	// �ж��Ƿ���8-bppλͼ
	if (wNumColors != 256)
	{
		// ��ʾ�û�
		MessageBox("��256ɫλͼ��", "ϵͳ��ʾ" , MB_ICONINFORMATION | MB_OK);
		
		// �������
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// ����
		return;
	}
	
	// ���Ĺ����״
	BeginWaitCursor();
	
	// ָ��DIB����ָ��
	LPSTR   lpDIBBits;
	
	// ָ��DIB���ص�ָ��
	BYTE *	lpSrc;
	
	// ѭ������
	LONG	i;
	LONG	j;
	
	// ͼ����
	LONG	lWidth;
	
	// ͼ��߶�
	LONG	lHeight;
	
	// ͼ��ÿ�е��ֽ���
	LONG	lLineBytes;
	
	// ָ��BITMAPINFO�ṹ��ָ�루Win3.0��
	LPBITMAPINFO lpbmi;
	
	// ָ��BITMAPCOREINFO�ṹ��ָ��
	LPBITMAPCOREINFO lpbmc;
	
	// �����Ƿ���Win3.0 DIB�ı��
	BOOL bWinStyleDIB;
	
	// ��ȡָ��BITMAPINFO�ṹ��ָ�루Win3.0��
	lpbmi = (LPBITMAPINFO)lpDIB;
	
	// ��ȡָ��BITMAPCOREINFO�ṹ��ָ��
	lpbmc = (LPBITMAPCOREINFO)lpDIB;
	
	// �Ҷ�ӳ���
	BYTE bMap[256];
	
	// �ж��Ƿ���WIN3.0��DIB
	bWinStyleDIB = IS_WIN30_DIB(lpDIB);
	
	// ����Ҷ�ӳ������������ɫ�ĻҶ�ֵ����������DIB��ɫ��
	for (i = 0; i < 256; i ++)
	{
		if (bWinStyleDIB)
		{
			// �������ɫ��Ӧ�ĻҶ�ֵ
			bMap[i] = (BYTE)(0.299 * lpbmi->bmiColors[i].rgbRed +
				0.587 * lpbmi->bmiColors[i].rgbGreen +
				0.114 * lpbmi->bmiColors[i].rgbBlue + 0.5);
			
			// ����DIB��ɫ���ɫ����
			lpbmi->bmiColors[i].rgbRed =(unsigned char) i;
			
			// ����DIB��ɫ����ɫ����
			lpbmi->bmiColors[i].rgbGreen = (unsigned char)i;
			
			// ����DIB��ɫ����ɫ����
			lpbmi->bmiColors[i].rgbBlue = (unsigned char)i;
			
			// ����DIB��ɫ�屣��λ
			lpbmi->bmiColors[i].rgbReserved = 0;
		}
		else
		{
			// �������ɫ��Ӧ�ĻҶ�ֵ
			bMap[i] = (BYTE)(0.299 * lpbmc->bmciColors[i].rgbtRed +
				0.587 * lpbmc->bmciColors[i].rgbtGreen +
				0.114 * lpbmc->bmciColors[i].rgbtBlue + 0.5);
			
			// ����DIB��ɫ���ɫ����
			lpbmc->bmciColors[i].rgbtRed =(unsigned char)i;
			
			// ����DIB��ɫ����ɫ����
			lpbmc->bmciColors[i].rgbtGreen =(unsigned char) i;
			
			// ����DIB��ɫ����ɫ����
			lpbmc->bmciColors[i].rgbtBlue = (unsigned char)i;
		}
	}
	
	// �ҵ�DIBͼ��������ʼλ��
	lpDIBBits = ::FindDIBBits(lpDIB);	
	
	// ��ȡͼ����
	lWidth = ::DIBWidth(lpDIB);
	
	// ��ȡͼ��߶�
	lHeight = ::DIBHeight(lpDIB);
	
	// ����ͼ��ÿ�е��ֽ���
	lLineBytes = WIDTHBYTES(lWidth * 8);
	
	// ����ÿ�����ص���ɫ�����������ջҶ�ӳ����ɻҶ�ֵ��
	// ÿ��
	for(i = 0; i < lHeight; i++)
	{
		// ÿ��
		for(j = 0; j < lWidth; j++)
		{
			// ָ��DIB��i�У���j�����ص�ָ��
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
			
			// �任
			*lpSrc = bMap[*lpSrc];
		}
	}
	
	// �滻��ǰ��ɫ��Ϊ�Ҷȵ�ɫ��
	pDoc->GetDocPalette()->SetPaletteEntries(0, 256, (LPPALETTEENTRY) ColorsTable[0]);
	
	// ��������
	pDoc->SetModifiedFlag(TRUE);
	
	// ʵ���µĵ�ɫ��
	OnDoRealize((WPARAM)m_hWnd,0);
	
	// ������ͼ
	pDoc->UpdateAllViews(NULL);
	
	// �������
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
	
	// �ָ����
	EndWaitCursor();		
	
}

LRESULT CTypeRecView::OnDoRealize(WPARAM wParam, LPARAM)
{
	ASSERT(wParam != NULL);
	
	// ��ȡ�ĵ�
	CTypeRecDoc* pDoc = GetDocument();
	
	// �ж�DIB�Ƿ�Ϊ��
	if (pDoc->GetHDIB() == NULL)
	{
		// ֱ�ӷ���
		return 0L;
	}
	
	// ��ȡPalette
	CPalette* pPal = pDoc->GetDocPalette();
	if (pPal != NULL)
	{
		// ��ȡMainFrame
		CMainFrame* pAppFrame = (CMainFrame*) AfxGetApp()->m_pMainWnd;
		ASSERT_KINDOF(CMainFrame, pAppFrame);
		
		CClientDC appDC(pAppFrame);
		
		// All views but one should be a background palette.
		// wParam contains a handle to the active view, so the SelectPalette
		// bForceBackground flag is FALSE only if wParam == m_hWnd (this view)
		CPalette* oldPalette = appDC.SelectPalette(pPal, ((HWND)wParam) != m_hWnd);
		
		if (oldPalette != NULL)
		{
			UINT nColorsChanged = appDC.RealizePalette();
			if (nColorsChanged > 0)
				pDoc->UpdateAllViews(NULL);
			appDC.SelectPalette(oldPalette, TRUE);
		}
		else
		{
			TRACE0("\tCCh1_1View::OnPaletteChanged�е���SelectPalette()ʧ�ܣ�\n");
		}
	}
	
	return 0L;
	
}

//24λ���ɫͼת����256���Ҷ�ͼ
//
void CTypeRecView::OnFILE24ToGray()    
{
	CTypeRecDoc* pDoc = GetDocument(); //��ȡ�ĵ�
	LPSTR lpDIB;                       //ָ��DIB��ָ��
	
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());
	
	ConvertToGrayScale(lpDIB);
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
	pDoc->SetModifiedFlag(TRUE);
	pDoc->UpdateAllViews(NULL);				
}

//����ָ������ͼ��
//
void CTypeRecView::OnTempSubrect() 
{
	
	CTypeRecDoc* pDoc = GetDocument();
	HDIB hDIB,hNewDIB;
	hDIB=pDoc->GetHDIB();
	
	long lWidth;                    //ͼ���Ⱥ͸߶�
	long lHeight;
	
	// ָ��DIB��ָ��
	LPSTR	lpDIB;
	
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());//��õ�ǰλͼ
	// �ҵ�DIBͼ��������ʼλ��
	lWidth = ::DIBWidth(lpDIB);   //DIB ���
	lHeight = ::DIBHeight(lpDIB); //DIB �߶�
	
    //�ٶ��ļ�������(���Ƹ���)
	//
	CRect rect(m_ipzLeft,m_ipzTop,m_ipzRight,m_ipzBottom);
	//	CRect rect(m_ipzLeft,190,m_ipzRight,240);
	//CRect rect(0,190,lWidth,260);
	//	CRect rect(0,m_ipzTop,lWidth,m_ipzBottom);
	hNewDIB= myCropDIB(hDIB,rect);
	
	if (OpenClipboard())
	{
		EmptyClipboard();
		SetClipboardData (CF_DIB, CopyHandle((HANDLE) hNewDIB ));
		CloseClipboard();
	}
	
}



void CTypeRecView::OnEditCopy() 
{
	// ���Ƶ�ǰͼ��
	
	// ��ȡ�ĵ�
	CTypeRecDoc* pDoc = GetDocument();
	
	// �򿪼�����
	if (OpenClipboard())
	{
		// ���Ĺ����״
		BeginWaitCursor();
		
		// ��ռ�����
		EmptyClipboard();
		
		// ���Ƶ�ǰͼ�񵽼�����
		SetClipboardData (CF_DIB, CopyHandle((HANDLE) pDoc->GetHDIB()) );
		
		// �رռ�����
		CloseClipboard();
		
		// �ָ����
		EndWaitCursor();
	}
	
}

void CTypeRecView::OnTempPalette() 
{
	CDC *pDC=GetDC();
	//	if((!pDC->GetDeviceCaps(RASTERCAPS)) & RC_PALETTE)
	//	{
	//		AfxMessageBox("��ǰ��ʾϵͳ��֧�ֵ�ɫ�塣");
	//		return;
	//	}
	
	CString str;
	int nColorNum=pDC->GetDeviceCaps(SIZEPALETTE);
	str.Format("��ǰϵͳ��ɫ���ͬʱ��ʾ����ɫ��Ϊ%d.",nColorNum);
	AfxMessageBox(str);
	
	int nColorReserved=pDC->GetDeviceCaps(NUMRESERVED);
	str.Format("��ǰϵͳ��ɫ�屣������ɫ��Ϊ%d��",nColorReserved);
	AfxMessageBox(str);
	
	ReleaseDC(pDC);
	
	
}

//�Ҷ�ת������
//

void CTypeRecView::OnTempGray() 
{
	
	CTypeRecDoc* pDoc = GetDocument();
	HDIB hDIB=pDoc->GetHDIB();
	
	long lWidth;                    //ͼ���Ⱥ͸߶�
	long lHeight;
	LONG lLineBytes;
	
	// ָ��DIB��ָ��
	LPSTR	lpDIB;
	unsigned char * lpSrc;          //ָ��ԭͼ�����ص��ָ��
	
	// ָ��DIB����ָ��
	LPSTR   lpDIBBits;
	
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());//��õ�ǰλͼ
	
	// ���Ĺ����״
	BeginWaitCursor();
	// �ҵ�DIBͼ��������ʼλ��
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// �ҵ�DIBͼ��������ʼλ��
	lWidth = ::DIBWidth(lpDIB);   //DIB ���
	lHeight = ::DIBHeight(lpDIB); //DIB �߶�
	lLineBytes=WIDTHBYTES(lWidth*8);	
	
	int i,j;
	
	for(i=0;i<lHeight;i++)
		for(j=0;j<lWidth;j++)
		{
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
			//if((*lpSrc)<255&&(*lpSrc)>190)
			if((*lpSrc)<155&&(*lpSrc)>100)
				*lpSrc=255;
			else
				if((*lpSrc)<255&&(*lpSrc)>190)
					*lpSrc=0;
		}
		
		pDoc->SetModifiedFlag(TRUE);
		pDoc->UpdateAllViews(NULL);
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());	
		
		/*
		// ��ȡ�ĵ�
		CTypeRecDoc* pDoc = GetDocument();
		
		  // ָ��DIB��ָ��
		  LPSTR	lpDIB;
		  
			// ָ��DIB����ָ��
			LPSTR   lpDIBBits;
			
			  unsigned char * lpSrc;          //ָ��ԭͼ�����ص��ָ��
			  
				// ����DIB
				lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());
				
				  // ���Ĺ����״
				  BeginWaitCursor();
				  
					// �ҵ�DIBͼ��������ʼλ��
					lpDIBBits = ::FindDIBBits(lpDIB);
					long lWidth;                    //ͼ���Ⱥ͸߶�
					long lHeight;
					LONG lLineBytes;
					
					  long i,j;           //ѭ������
					  
						lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());//��õ�ǰλͼ
						// �ҵ�DIBͼ��������ʼλ��
						lWidth = ::DIBWidth(lpDIB);   //DIB ���
						lHeight = ::DIBHeight(lpDIB); //DIB �߶�
						
						  lLineBytes=WIDTHBYTES(lWidth*8);
						  
							for(i = 0; i < lHeight; i++)
							{
							// ÿ��
							for(j =  0; j < lWidth; j++)
							{
							lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
							*lpSrc=0;
							}
							}
							
							  // ���Ĺ����״
							  EndWaitCursor();
							  
								pDoc->SetModifiedFlag(TRUE);
								pDoc->UpdateAllViews(NULL);
								::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		*/
}

void CTypeRecView::OnTempError() 
{
	CTypeRecDoc* pDoc = GetDocument();
	HDIB hDIB=pDoc->GetHDIB();
	
	long lWidth;                    //ͼ���Ⱥ͸߶�
	long lHeight;
	LONG lLineBytes;
	
	// ָ��DIB��ָ��
	LPSTR	lpDIB;
	unsigned char * lpSrc;          //ָ��ԭͼ�����ص��ָ��
	
	// ָ��DIB����ָ��
	LPSTR   lpDIBBits;
	
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());//��õ�ǰλͼ
	
	// ���Ĺ����״
	BeginWaitCursor();
	// �ҵ�DIBͼ��������ʼλ��
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// �ҵ�DIBͼ��������ʼλ��
	lWidth = ::DIBWidth(lpDIB);   //DIB ���
	lHeight = ::DIBHeight(lpDIB); //DIB �߶�
	lLineBytes=WIDTHBYTES(lWidth*8);	
	
	int i,j,pixel;
	
	long lHistogram[256];
	for(i=0;i<256;i++)
		lHistogram[i]=0;
	
	//���ֱ��ͼ
	int iMax1GrayValue=0;
	int iMax2GrayValue=0;
	
	for(i=0;i<lHeight;i++)
	{
		for(j=0;j<lWidth;j++)
		{
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
			pixel=(unsigned char)(*lpSrc);
			lHistogram[pixel]++;
		}
	}
	
	for(i=0;i<256;i++)
	{
		//�����Ҷ�ֵ��Ϊ�����Ҷ�ֵ
		if(iMax1GrayValue<lHistogram[i])
		{
			iMax1GrayValue=i;
		}
		else if(iMax2GrayValue<iMax1GrayValue&&iMax2GrayValue-1<iMax2GrayValue&&iMax2GrayValue+1<iMax2GrayValue)
		{
			iMax2GrayValue=i;
		}
	}
	
	
}

//��һ��ʼ��ֵT��ͼ��A���ж�ֵ���õ���ֵ��ͼ��B,��ʼ��ֵT��ȷ��������:ѡ����ֵ
//T=Gmax-(Gmax-Gmin)/3,Gmax��Gmin�ֱ�����ߡ���ͻҶ�ֵ��
//����ֵ�Բ�ͬ������һ������Ӧ��,�ܹ���֤������������Ϊ0,��ͻ����������
//
void CTypeRecView::OnTest11() 
{
	CTypeRecDoc* pDoc=GetDocument();   //����ĵ�
	LPSTR lpDIB;                       //ָ��DIB��ָ��
	LPSTR lpDIBBits;                   //ָ��DIB�����ص�ָ��
	
    LONG lLineBytes;
	unsigned char * lpSrc;             //ָ��ԭͼ�����ص��ָ��
	
    long lWidth;                       //ͼ���Ⱥ͸߶�
	long lHeight;
	
	long i,j;           //ѭ������
	
	OnEditCopy();
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());//��õ�ǰλͼ
	// �ҵ�DIBͼ��������ʼλ��
	lpDIBBits = ::FindDIBBits(lpDIB);
	lWidth = ::DIBWidth(lpDIB);   //DIB ���
	lHeight = ::DIBHeight(lpDIB); //DIB �߶�
	// ����ͼ��ÿ�е��ֽ���
	lLineBytes = WIDTHBYTES(lWidth * 8);
	
	long lCount[256];
	for(i=0;i<256;i++)
	{
		lCount[i]=0;  //����
	}
	if(::DIBNumColors(lpDIB) != 256)  //256ɫλͼ�����κδ���
	{
		return;
	}
	for(i=0;i<lHeight;i++)
	{
		for(j=0;j<lWidth;j++)
		{
			lpSrc=(unsigned char *)lpDIB+lLineBytes*i+j;
			lCount[*(lpSrc)]++;
		}
	}
	
	//�󴰿ڱ任�����޺�����
	//
	long temp[16];
	int k=0;
	
	for(k=0;k<16;k++)
	{
		temp[k]=0;
		for(i=k*16;i<(k+1)*16;i++)
			temp[k]+=lCount[i];
	}
	
	long max=0;
	int t=0;
	for(k=15;k>=0;k--)
	{
		if(temp[k]>max)
		{
			max=temp[k];
			t=k;
		}
		
	}
	
	int bLow=0,bUp=0;
	bLow=(t-1)*16;
	//	bUp=(t+5)*16;
	
	//	bLow=100;
	bUp=255;
	// ��ֵ
	INT	bThre;
	
	bThre=(INT)((2*bUp+bLow)/3);
	
	// ���Ĺ����״
	BeginWaitCursor();
	
	// ����ThresholdTrans()����������ֵ�任
	ThresholdTrans(lpDIBBits, ::DIBWidth(lpDIB), ::DIBHeight(lpDIB), bThre);
	
	pDoc->SetModifiedFlag(TRUE);
	pDoc->UpdateAllViews(NULL);
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
	// �ָ����
	EndWaitCursor();
}

//(2)�����������š���ͼ��B���򵥵��������ػҶ�ֵ���,�õ��µ�ͼ��G,
//��Gi,j=|Pi,j-Pi,j-1|i=0,1,��,439;j=0,1,��,639Gi,0=Pi,0,���Եֱ�Ӹ�ֵ,
//����Ӱ������Ч����
//
void CTypeRecView::OnTest12() 
{
	
	CTypeRecDoc* pDoc=GetDocument();   //����ĵ�
	LPSTR lpDIB;                       //ָ��DIB��ָ��
	
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());//��õ�ǰλͼ
	
	//���Զ����ģ��������������
	myTemplate(lpDIB);
	
	pDoc->SetModifiedFlag(TRUE);
	pDoc->UpdateAllViews(NULL);
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
	
}

//����ҶȻ�������ֵΪ0(ͼ2(f))�����ǵ��������������������,
//������������һ�󲿷��ǹ�������,��ģ��(1,1,1,1,1)T��G������ֵ�˲�,
//�õ������˴󲿷ָ��ŵ�ͼ��C��
//
void CTypeRecView::OnTest13() 
{
	// ��ֵ�˲�
	
	// ��ȡ�ĵ�
	CTypeRecDoc* pDoc = GetDocument();
	
	// ָ��DIB��ָ��
	LPSTR	lpDIB;
	
	// ָ��DIB����ָ��
	LPSTR   lpDIBBits;
	
	// �˲����ĸ߶�
	int iFilterH;
	
	// �˲����Ŀ��
	int iFilterW;
	
	// ����Ԫ�ص�X����
	int iFilterMX;
	
	// ����Ԫ�ص�Y����
	int iFilterMY;
	
	// ����DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());
	
	// �ҵ�DIBͼ��������ʼλ��
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ����ֵ�˲��������Ŀ������ƣ�
	if (::DIBNumColors(lpDIB) != 256)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ����ֵ�˲���", "ϵͳ��ʾ" ,
			MB_ICONINFORMATION | MB_OK);
		
		// �������
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// ����
		return;
	}
	
	// �����Ի���
	
	// ��ʼ������ֵ
	iFilterH = 5;
	iFilterW = 1;
	iFilterMX = 0;
	iFilterMY = 2;
	
	
	// ���Ĺ����״
	BeginWaitCursor();
	
	// ����MedianFilter()������ֵ�˲�
	if (myMedianFilter(lpDIBBits, ::DIBWidth(lpDIB), ::DIBHeight(lpDIB), 
		iFilterH, iFilterW, iFilterMX, iFilterMY))
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

//����ˮƽͶӰ����⳵��ˮƽλ��
//
void CTypeRecView::OnTest14() 
{
	CTypeRecDoc* pDoc=GetDocument();   //����ĵ�
	LPSTR lpDIB;                       //ָ��DIB��ָ��
    long lWidth;                       //ͼ���Ⱥ͸߶�
	long lHeight;
	
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());//��õ�ǰλͼ
	lWidth = ::DIBWidth(lpDIB);   //DIB ���
	lHeight = ::DIBHeight(lpDIB); //DIB �߶�
	
	//ˮƽͶӰ����ȡ������ͼ������±�Եλ��
	//
	myHprojectDIB(lpDIB, lWidth, lHeight,&m_ipzTop, &m_ipzBottom) ;
	m_ipzLeft=0;
	m_ipzRight=lWidth;
	pDoc->UpdateAllViews(NULL);
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());	
	
	//�Ժ�����ͼ����м��ã��õ����Ƹ߶ȣ�ԭͼ���ȵ�ͼ��
	OnTempSubrect();
}

//���ô�ֱͶӰ����⳵�ƴ�ֱλ��
//
void CTypeRecView::OnTest15() 
{
	CTypeRecDoc* pDoc=GetDocument();   //����ĵ�
	LPSTR lpDIB;                       //ָ��DIB��ָ��
	
    long lWidth;                       //ͼ���Ⱥ͸߶�
	long lHeight;
	
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());//��õ�ǰλͼ
	lWidth = ::DIBWidth(lpDIB);   //DIB ���
	lHeight = ::DIBHeight(lpDIB); //DIB �߶�
	
	myVprojectDIB(lpDIB, lWidth, lHeight,&m_ipzLeft, &m_ipzRight) ;
	pDoc->UpdateAllViews(NULL);
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());		
	OnTempSubrect();
}

void CTypeRecView::OnEditPaste() 
{
	// ճ��ͼ��
	
	// ������DIB
	HDIB hNewDIB = NULL;
	
	// �򿪼�����
	if (OpenClipboard())
	{
		// ���Ĺ����״
		BeginWaitCursor();
		
		// ��ȡ�������е�ͼ��
		hNewDIB = (HDIB) CopyHandle(::GetClipboardData(CF_DIB));
		
		// �رռ�����
		CloseClipboard();
		
		// �ж��Ƿ��ȡ�ɹ�
		if (hNewDIB != NULL)
		{
			// ��ȡ�ĵ�
			CTypeRecDoc* pDoc = GetDocument();
			
			// �滻DIB��ͬʱ�ͷž�DIB����
			pDoc->ReplaceHDIB(hNewDIB);
			
			// ����DIB��С�͵�ɫ��
			pDoc->InitDIBData();
			
			// ��������
			pDoc->SetModifiedFlag(TRUE);
			
			// �������ù�����ͼ��С
			//	SetScrollSizes(MM_TEXT, pDoc->GetDocSize());
			
			// ʵ���µĵ�ɫ��
			OnDoRealize((WPARAM)m_hWnd,0);
			
			// ������ͼ
			pDoc->UpdateAllViews(NULL);
		}
		// �ָ����
		EndWaitCursor();
	}	
}

//�ۺϵ�4��5����
//
void CTypeRecView::OnTest145() 
{
	CTypeRecDoc* pDoc=GetDocument();   //����ĵ�
	LPSTR lpDIB;                       //ָ��DIB��ָ��
    long lWidth;                       //ͼ���Ⱥ͸߶�
	long lHeight;
	
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());//��õ�ǰλͼ
	lWidth = ::DIBWidth(lpDIB);   //DIB ���
	lHeight = ::DIBHeight(lpDIB); //DIB �߶�
	
	//ˮƽͶӰ����ȡ������ͼ������±�Եλ��
	//
	myHprojectDIB(lpDIB, lWidth, lHeight,&m_ipzTop, &m_ipzBottom) ;
	m_ipzLeft=0;
	m_ipzRight=lWidth;
	
	//�Ժ�����ͼ����м��ã��õ����Ƹ߶ȣ�ԭͼ���ȵ�ͼ��
	//	OnTempSubrect();
	
	HDIB hDIB;
	HDIB hNewDIB;
	hDIB=pDoc->GetHDIB();
	
    //�ٶ��ļ�������(���Ƹ���)
	//
	CRect rect(m_ipzLeft,m_ipzTop,m_ipzRight,m_ipzBottom);
	hNewDIB= myCropDIB(hDIB,rect);
	
	// �ж��Ƿ���óɹ�
	if (hNewDIB != NULL)
	{
		// ��ȡ�ĵ�
		CTypeRecDoc* pDoc = GetDocument();
		
		// �滻DIB��ͬʱ�ͷž�DIB����
		pDoc->ReplaceHDIB(hNewDIB);
		
		// ����DIB��С�͵�ɫ��
		pDoc->InitDIBData();
		
		// ��������
		pDoc->SetModifiedFlag(TRUE);
		
		// ʵ���µĵ�ɫ��
		OnDoRealize((WPARAM)m_hWnd,0);
		
	}
	
	//5
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());//��õ�ǰλͼ
	lWidth = ::DIBWidth(lpDIB);   //DIB ���
	lHeight =::DIBHeight(lpDIB); //DIB �߶�
	
	myVprojectDIB(lpDIB, lWidth, lHeight,&m_ipzLeft, &m_ipzRight) ;
	
	pDoc->UpdateAllViews(NULL);
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());	
}

//��ȡ������ͼ��
//
void CTypeRecView::OnTest16() 
{
	OnEditPaste();
	OnTempSubrect();
	OnEditPaste();	
}
