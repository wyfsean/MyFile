// ImageProcessingView.cpp : implementation of the CImageProcessingView class
//

#include "stdafx.h"
#include "ImageProcessing.h"

#include "ImageProcessingDoc.h"
#include "ImageProcessingView.h"
#include  "GlobalApi.h"
#include <complex>
using namespace std;


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImageProcessingView

IMPLEMENT_DYNCREATE(CImageProcessingView, CScrollView)

BEGIN_MESSAGE_MAP(CImageProcessingView, CScrollView)
	//{{AFX_MSG_MAP(CImageProcessingView)
	ON_COMMAND(ID_EDGE_CANNY, OnEdgeCanny)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImageProcessingView construction/destruction

CImageProcessingView::CImageProcessingView()
{
	// ΪС���任���õĲ���
	// ��ʱ���С���任ϵ���ڴ�
	m_pDbImage = NULL;	
	
	// ���õ�ǰ����
	m_nDWTCurDepth = 0;

	// ����С������֧������
	m_nSupp = 1;
}

CImageProcessingView::~CImageProcessingView()
{
	// �ͷ��ѷ����ڴ�
	if(m_pDbImage){
		delete[]m_pDbImage;
		m_pDbImage = NULL;
	}
}

BOOL CImageProcessingView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CImageProcessingView drawing

void CImageProcessingView::OnDraw(CDC* pDC)
{
	CImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	CSize sizeDibDisplay;		
	

	
	if(!pDoc->m_pDibInit->IsEmpty()){	
		sizeDibDisplay = pDoc->m_pDibInit->GetDimensions();
		pDoc->m_pDibInit->Draw(pDC,CPoint(0,0),sizeDibDisplay);	
	}	

}

void CImageProcessingView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	CSize sizeTotal = pDoc->m_pDibInit->GetDimensions();
	SetScrollSizes(MM_TEXT, sizeTotal);

	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

}

/////////////////////////////////////////////////////////////////////////////
// CImageProcessingView printing

BOOL CImageProcessingView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CImageProcessingView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CImageProcessingView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CImageProcessingView diagnostics

#ifdef _DEBUG
void CImageProcessingView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CImageProcessingView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CImageProcessingDoc* CImageProcessingView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImageProcessingDoc)));
	return (CImageProcessingDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CImageProcessingView message handlers









/*************************************************************************
 *
 * \�������ƣ�
 *   OnFft2d()
 *
 * \�������:
 *   ��
 * 
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   ���ж�ά���ٸ���Ҷ�任
 *
 *************************************************************************
 */
//DEL void CImageProcessingView::OnFft2d() 
//DEL {
//DEL 	//ͼ��FFT�任
//DEL 
//DEL 	// ���Ĺ����״
//DEL 	BeginWaitCursor();
//DEL 	
//DEL 	// ѭ�����Ʊ���
//DEL 	int y;
//DEL 	int x;
//DEL 	
//DEL 	// ���Doc���ָ��
//DEL 	CImageProcessingDoc * pDoc = (CImageProcessingDoc *)this->GetDocument();
//DEL 
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 
//DEL 	// ���ͼ���ͷ�ļ���Ϣ
//DEL 	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
//DEL 
//DEL 	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ����ɢ����Ҷ�任��
//DEL 	if (lpBMIH->biBitCount != 8)
//DEL 	{
//DEL 		// ��ʾ�û�
//DEL 		MessageBox("Ŀǰֻ֧��256ɫλͼ����ɢ����Ҷ�任��", "ϵͳ��ʾ" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 						
//DEL 		// ����
//DEL 		return;
//DEL 	}
//DEL 
//DEL 	// ͼ��Ŀ�
//DEL 	CSize sizeImage ;
//DEL 	int nWidth ;
//DEL 	int nHeight;
//DEL 
//DEL 	// ���ͼ��Ŀ�
//DEL 	sizeImage = pDib->GetDimensions() ;
//DEL 		
//DEL 	nWidth = sizeImage.cx;
//DEL 	nHeight= sizeImage.cy;
//DEL 
//DEL 	// ��ʱ����
//DEL 	double	dTmpOne;
//DEL 	double  dTmpTwo;
//DEL 	
//DEL 	// ����Ҷ�任��ֱ�������
//DEL 	int nTransHeight ;
//DEL 
//DEL 	// ����Ҷ�任ˮƽ�������
//DEL 	int nTransWidth  ;	
//DEL 	
//DEL 	// ������и���Ҷ�任�ĵ���	��2���������ݣ�
//DEL 	dTmpOne = log(nWidth)/log(2);
//DEL 	dTmpTwo = ceil(dTmpOne)		   ;
//DEL 	dTmpTwo = pow(2,dTmpTwo)	   ;
//DEL 	nTransWidth = (int) dTmpTwo	   ;
//DEL 	
//DEL 	// ������и���Ҷ�任�ĵ��� ��2���������ݣ�
//DEL 	dTmpOne = log(nHeight)/log(2);
//DEL 	dTmpTwo = ceil(dTmpOne)		   ;
//DEL 	dTmpTwo = pow(2,dTmpTwo)	   ;
//DEL 	nTransHeight = (int) dTmpTwo   ;
//DEL 
//DEL 	// ����ͼ�����ݴ洢ÿ����Ҫ���ֽ���
//DEL 	// BMP�ļ���ÿ�����ݴ洢��DWORD�����
//DEL 	int		nSaveWidth;
//DEL 	nSaveWidth = ( (nWidth << 3) + 31)/32 * 4 ;
//DEL 
//DEL 	// ָ��ͼ�����ݵ�ָ��
//DEL 	LPBYTE lpImage ;
//DEL 	lpImage = pDib->m_lpImage ;
//DEL 
//DEL 	// ͼ������ֵ
//DEL 	unsigned char unchValue;
//DEL 
//DEL 	
//DEL 	// ָ��ʱ�����ݵ�ָ��
//DEL 	complex<double> * pCTData ;
//DEL 
//DEL 	// ָ��Ƶ�����ݵ�ָ��
//DEL 	complex<double> * pCFData ;
//DEL 
//DEL 	// �����ڴ�
//DEL 	pCTData=new complex<double>[nTransWidth * nTransHeight];
//DEL 	pCFData=new complex<double>[nTransWidth * nTransHeight];
//DEL 
//DEL 	// ��ʼ��
//DEL 	// ͼ�����ݵĿ�͸߲�һ����2���������ݣ�����pCTData
//DEL 	// ��һ����������Ҫ��0
//DEL 	for(y=0; y<nTransHeight; y++)
//DEL 	{
//DEL 		for(x=0; x<nTransWidth; x++)
//DEL 		{
//DEL 			pCTData[y*nTransWidth + x]=complex<double>(0,0);
//DEL 		}
//DEL 	}
//DEL 
//DEL 	// ��ͼ�����ݴ���pCTData
//DEL 	for(y=0; y<nHeight; y++)
//DEL 	{
//DEL 		for(x=0; x<nWidth; x++)
//DEL 		{
//DEL 			unchValue = lpImage[y*nSaveWidth +x];
//DEL 			pCTData[y*nTransWidth + x]=complex<double>(unchValue,0);
//DEL 		}
//DEL 	}
//DEL 
//DEL 	// ����Ҷ���任
//DEL 	DIBFFT_2D(pCTData, nWidth, nHeight, pCFData) ;
//DEL 	
//DEL 	// ��ʱ����
//DEL 	double dTmp;
//DEL 
//DEL 	for(y=0; y<nHeight; y++)
//DEL 	{
//DEL 		for(x=0; x<nWidth; x++)
//DEL 		{
//DEL 			dTmp = pCFData[y * nTransWidth + x].real() 
//DEL 				   * pCFData[y * nTransWidth + x].real()
//DEL 				 + pCFData[y * nTransWidth + x].imag() 
//DEL 				   * pCFData[y * nTransWidth + x].imag();
//DEL 			
//DEL 			dTmp = sqrt(dTmp) ;
//DEL 
//DEL 			// Ϊ����ʾ����Ҫ�Է��ȵĴ�С��������
//DEL 			dTmp /= 100        ;
//DEL 
//DEL 			// ����ͼ�����ݵĴ�С
//DEL 			dTmp = min(dTmp, 255) ;
//DEL 
//DEL 			lpImage[y*nSaveWidth +x] = (unsigned char)(int)dTmp;
//DEL 		}
//DEL 	}
//DEL 
//DEL 	// Ϊ������Ļ����ʾ�����ǰѷ���ֵ��Ĳ����ú�ɫ��ʾ
//DEL 	for(y=0; y<nHeight; y++)
//DEL 	{
//DEL 		for(x=0; x<nWidth; x++)
//DEL 		{
//DEL 			lpImage[y*nSaveWidth +x] = 255 - lpImage[y*nSaveWidth +x];
//DEL 		}
//DEL 	}
//DEL 	
//DEL 	// ˢ����Ļ
//DEL 	Invalidate();
//DEL 
//DEL 	// �ͷ��ڴ�
//DEL 	delete pCTData;
//DEL 	delete pCFData;
//DEL 	pCTData = NULL;
//DEL 	pCFData = NULL;
//DEL 
//DEL 	// ��������
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 		
//DEL 	// ������ͼ
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL 
//DEL 	//  �ָ������״
//DEL 	EndWaitCursor();
//DEL }


/*************************************************************************
 *
 * \�������ƣ�
 *   OnDft2d()
 *
 * \�������:
 *   ��
 * 
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   ���ж�ά����Ҷ�任
 *
 *************************************************************************
 */
//DEL void CImageProcessingView::OnDft2d() 
//DEL {
//DEL 	//ͼ����ɢ����Ҷ�任
//DEL 	
//DEL 	//��ʾ�û���ֱ�ӽ�����ɢ����Ҷ�任��ʱ��ܳ�
//DEL 	MessageBox("û��ʹ��FFT��ʱ����ܺܳ���", "������ʾ" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 	
//DEL 	//���Ĺ����״
//DEL 	BeginWaitCursor(); 
//DEL 
//DEL 	// ѭ�����Ʊ���
//DEL 	int y;
//DEL 	int x;
//DEL 	
//DEL 	CImageProcessingDoc * pDoc = (CImageProcessingDoc *)this->GetDocument();
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 
//DEL 	// ���ͼ���ͷ�ļ���Ϣ
//DEL 	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
//DEL 
//DEL 	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ����ɢ����Ҷ�任��
//DEL 	if (lpBMIH->biBitCount != 8)
//DEL 	{
//DEL 		// ��ʾ�û�
//DEL 		MessageBox("Ŀǰֻ֧��256ɫλͼ����ɢ����Ҷ�任��", "ϵͳ��ʾ" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 						
//DEL 		// ����
//DEL 		return;
//DEL 	}
//DEL 
//DEL 	//ͼ��ĳ����С
//DEL 	CSize sizeImage		= pDib->GetDimensions();
//DEL 	int nWidth			= sizeImage.cx		;
//DEL 	int nHeight			= sizeImage.cy		;
//DEL 
//DEL 	// ����ͼ�����ݴ洢ÿ����Ҫ���ֽ���
//DEL 	// BMP�ļ���ÿ�����ݴ洢��DWORD�����
//DEL 	int		nSaveWidth;
//DEL 	nSaveWidth = ( (nWidth << 3) + 31)/32 * 4 ;
//DEL 
//DEL 	// ָ��ͼ�����ݵ�ָ��
//DEL 	LPBYTE lpImage ;
//DEL 	lpImage = pDib->m_lpImage ;
//DEL 
//DEL 	double * pTrRstRpart  = new double [nWidth*nHeight];
//DEL 	double * pTrRstIpart  = new double [nWidth*nHeight];	
//DEL 	
//DEL 	::DIBDFT_2D(pDib, pTrRstRpart,pTrRstIpart);
//DEL 
//DEL 	// ��ʱ����
//DEL 	double dTmp;
//DEL 
//DEL 	for(y=0; y<nHeight; y++)
//DEL 	{
//DEL 		for(x=0; x<nWidth; x++)
//DEL 		{
//DEL 			dTmp = pTrRstRpart[y*nWidth + x] * pTrRstRpart[y*nWidth + x]
//DEL 				 + pTrRstIpart[y*nWidth + x] * pTrRstIpart[y*nWidth + x];
//DEL 			
//DEL 			dTmp = sqrt(dTmp) ;
//DEL 
//DEL 			// Ϊ����ʾ����Ҫ�Է��ȵĴ�С��������
//DEL 			dTmp /= 100        ;
//DEL 
//DEL 			// ����ͼ�����ݵĴ�С
//DEL 			dTmp = min(dTmp, 255) ;
//DEL 
//DEL 			lpImage[y*nSaveWidth +x] = (unsigned char)(int)dTmp;
//DEL 		}
//DEL 	}
//DEL 
//DEL 	// Ϊ������Ļ����ʾ�����ǰѷ���ֵ��Ĳ����ú�ɫ��ʾ
//DEL 	for(y=0; y<nHeight; y++)
//DEL 	{
//DEL 		for(x=0; x<nWidth; x++)
//DEL 		{
//DEL 			lpImage[y*nSaveWidth +x] = 255 - lpImage[y*nSaveWidth +x];
//DEL 		}
//DEL 	}
//DEL 
//DEL 	// �ͷ��ڴ�
//DEL 	delete pTrRstRpart;
//DEL 	pTrRstRpart=NULL  ;
//DEL 	
//DEL 	delete pTrRstIpart;
//DEL 	pTrRstIpart=NULL  ;
//DEL 
//DEL 	// ��������
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 		
//DEL 	// ������ͼ
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL 
//DEL 	// �ָ������״
//DEL 	EndWaitCursor(); 
//DEL 	
//DEL 	// ˢ����Ļ
//DEL 	Invalidate();
//DEL }

//DEL void CImageProcessingView::OnFreqDct() 
//DEL {
//DEL 	// ͼ�����ɢ���ұ任
//DEL 		
//DEL 	// ���Ĺ����״
//DEL 	BeginWaitCursor();
//DEL 
//DEL 	// ��ȡ�ĵ�
//DEL 	CImageProcessingDoc* pDoc = GetDocument();
//DEL 
//DEL 	//  ���ͼ��CDib���ָ��
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 
//DEL 	// ���ͼ���ͷ�ļ���Ϣ
//DEL 	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
//DEL 
//DEL 	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ����ɢ���ұ任��
//DEL 	if (lpBMIH->biBitCount != 8)
//DEL 	{
//DEL 		// ��ʾ�û�
//DEL 		MessageBox("Ŀǰֻ֧��256ɫλͼ����ɢ���ұ任��", "ϵͳ��ʾ" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 						
//DEL 		// ����
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	::DIBDct(pDib);
//DEL 	
//DEL 	// ��������
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 		
//DEL 	// ������ͼ
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL 
//DEL     // �ָ����
//DEL 	EndWaitCursor();
//DEL 	
//DEL }

//DEL void CImageProcessingView::OnFreqHotelling() 
//DEL {
//DEL 	// ͼ������ֱ任
//DEL 	
//DEL 	// ���Ĺ����״
//DEL 	BeginWaitCursor();
//DEL 
//DEL 	// ��ȡ�ĵ�
//DEL 	CImageProcessingDoc* pDoc = GetDocument();
//DEL 
//DEL 	//  ���ͼ��CDib���ָ��
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 
//DEL 	// ���ͼ���ͷ�ļ���Ϣ
//DEL 	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
//DEL 
//DEL 	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ����ɢ�����ֱ任��
//DEL 	if (lpBMIH->biBitCount != 8)
//DEL 	{
//DEL 		// ��ʾ�û�
//DEL 		MessageBox("Ŀǰֻ֧��256ɫλͼ����ɢ�����ֱ任��", "ϵͳ��ʾ" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 						
//DEL 		// ����
//DEL 		return;
//DEL 	}
//DEL 
//DEL 	//  ͼ��Ļ����ֱ任
//DEL 	DIBHOTELLING(pDib);
//DEL 	
//DEL 	// ��������
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 		
//DEL 	// ������ͼ
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL 
//DEL     // �ָ����
//DEL 	EndWaitCursor();
//DEL 	
//DEL }

//DEL void CImageProcessingView::OnFreqWalsh() 
//DEL {
//DEL 	// ͼ����ֶ�ʲ��������任
//DEL 	
//DEL 	// ���Ĺ����״
//DEL 	BeginWaitCursor();
//DEL 
//DEL 	// ��ȡ�ĵ�
//DEL 	CImageProcessingDoc* pDoc = GetDocument();
//DEL 
//DEL 	//  ���ͼ��CDib���ָ��
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 
//DEL 	// ���ͼ���ͷ�ļ���Ϣ
//DEL 	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
//DEL 
//DEL 	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ���ֶ�ʲ��������任��
//DEL 	if (lpBMIH->biBitCount != 8)
//DEL 	{
//DEL 		// ��ʾ�û�
//DEL 		MessageBox("Ŀǰֻ֧��256ɫλͼ����ɢ�ֶ�ʲ�任��", "ϵͳ��ʾ" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 						
//DEL 		// ����
//DEL 		return;
//DEL 	}
//DEL 
//DEL 	//  �����ֶ�ʲ��������任
//DEL 	DIBWalsh(pDib);
//DEL 	
//DEL 	// ��������
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 		
//DEL 	// ������ͼ
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL 
//DEL     // �ָ����
//DEL 	EndWaitCursor();
//DEL 	
//DEL }

/*************************************************************************
 *
 * \�������ƣ�
 *   OnViewHistogram()
 *
 * \�������:
 *   ��
 * 
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   �鿴ֱ��ͼ������ֱ��ͼ��ʾ����
 *
 *************************************************************************
 */
//DEL void CImageProcessingView::OnViewHistogram() 
//DEL {
//DEL 	// ��ȡ�ĵ�
//DEL 	CImageProcessingDoc* pDoc = GetDocument();
//DEL 
//DEL 	// DIB����ɫ��Ŀ
//DEL 	int nColorTableEntries;
//DEL 	nColorTableEntries = pDoc->m_pDibInit->m_nColorTableEntries;
//DEL 		
//DEL 	// �ж��Ƿ���8bppλͼ������ֻ����8bppλͼ��
//DEL 	if ( nColorTableEntries != 256)
//DEL 	{
//DEL 		// ��ʾ�û������ٽ��д���
//DEL 		MessageBox("Ŀǰֻ֧�ֲ鿴256ɫλͼ�Ҷ�ֱ��ͼ��", "ϵͳ��ʾ" , MB_ICONINFORMATION | MB_OK);
//DEL 		
//DEL 		// ����
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	// ���Ĺ����״
//DEL 	BeginWaitCursor();
//DEL 	
//DEL 	// �����Ի���
//DEL 	CDlgHistShow dlgHistShow;
//DEL 	
//DEL 	// ��ʼ������ֵ
//DEL 	dlgHistShow.m_pDib = pDoc->m_pDibInit;
//DEL 		
//DEL 	// ��ʾ�Ի���
//DEL 	if (dlgHistShow.DoModal() != IDOK)
//DEL 	{
//DEL 		// ����
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	// �ָ����
//DEL 	EndWaitCursor();	
//DEL }

/*************************************************************************
 *
 * \�������ƣ�
 *   OnEnhanceSmooth()
 *
 * \�������:
 *   ��
 * 
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   ��ͼ�����ƽ������������ƽ��ģ�����öԻ���
 *
 *************************************************************************
 */
//DEL void CImageProcessingView::OnEnhanceSmooth() 
//DEL {
//DEL 	// TODO: Add your command handler code here
//DEL 		// ͼ��ƽ��
//DEL 	
//DEL 	// ��ȡ�ĵ�
//DEL 	CImageProcessingDoc* pDoc = GetDocument();
//DEL 	
//DEL 	// ģ��߶�
//DEL 	int		nTempHeight;
//DEL 	
//DEL 	// ģ����
//DEL 	int		nTempWidth;
//DEL 	
//DEL 	// ģ��ϵ��
//DEL 	double		dbTempCoef;
//DEL 	
//DEL 	// ģ������Ԫ��X����
//DEL 	int		nTempCenX;
//DEL 	
//DEL 	// ģ������Ԫ��Y����
//DEL 	int		nTempCenY;
//DEL 	
//DEL 	// ģ��Ԫ������
//DEL 	double	pdbTemp[25];
//DEL 	
//DEL 	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ��ƽ���������Ŀ������ƣ�
//DEL 	if(pDoc->m_pDibInit->m_nColorTableEntries != 256)
//DEL 	//if (::DIBNumColors(lpDIB) != 256)
//DEL 	{
//DEL 		// ��ʾ�û�
//DEL 		MessageBox("Ŀǰֻ֧��256ɫλͼ��ƽ����", "ϵͳ��ʾ" , 
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 		
//DEL 		// ����
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	// �����Ի���
//DEL 	CDlgSmooth dlgSmth;
//DEL 	
//DEL 	// ��ģ�����鸳��ֵ��Ϊƽ��ģ�壩
//DEL 	pdbTemp[0] = 1.0;
//DEL 	pdbTemp[1] = 1.0;
//DEL 	pdbTemp[2] = 1.0;
//DEL 	pdbTemp[3] = 0.0;
//DEL 	pdbTemp[4] = 0.0;
//DEL 	pdbTemp[5] = 1.0;
//DEL 	pdbTemp[6] = 1.0;
//DEL 	pdbTemp[7] = 1.0;
//DEL 	pdbTemp[8] = 0.0;
//DEL 	pdbTemp[9] = 0.0;
//DEL 	pdbTemp[10] = 1.0;
//DEL 	pdbTemp[11] = 1.0;
//DEL 	pdbTemp[12] = 1.0;
//DEL 	pdbTemp[13] = 0.0;
//DEL 	pdbTemp[14] = 0.0;
//DEL 	pdbTemp[15] = 0.0;
//DEL 	pdbTemp[16] = 0.0;
//DEL 	pdbTemp[17] = 0.0;
//DEL 	pdbTemp[18] = 0.0;
//DEL 	pdbTemp[19] = 0.0;
//DEL 	pdbTemp[20] = 0.0;
//DEL 	pdbTemp[21] = 0.0;
//DEL 	pdbTemp[22] = 0.0;
//DEL 	pdbTemp[23] = 0.0;
//DEL 	pdbTemp[24] = 0.0;
//DEL 	
//DEL 	// ��ʼ���Ի������ֵ
//DEL 	dlgSmth.m_nTemType  = 0;
//DEL 	dlgSmth.m_nSmthTemHeight  = 3;
//DEL 	dlgSmth.m_nSmthTemWidth  = 3;
//DEL 	dlgSmth.m_nSmthTemCenX = 1;
//DEL 	dlgSmth.m_nSmthTemCenY = 1;
//DEL 	dlgSmth.m_dbSmthTemCoef  = (double) (1.0 / 9.0);
//DEL 	dlgSmth.m_pdbTemp = pdbTemp;
//DEL 	
//DEL 	// ��ʾ�Ի�����ʾ�û��趨ƽ����
//DEL 	if (dlgSmth.DoModal() != IDOK)
//DEL 	{
//DEL 		// ����
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	// ��ȡ�û��趨��ƽ����
//DEL 	nTempHeight   = dlgSmth.m_nSmthTemHeight;
//DEL 	nTempWidth  = dlgSmth.m_nSmthTemWidth;
//DEL 	nTempCenX = dlgSmth.m_nSmthTemCenX;
//DEL 	nTempCenY = dlgSmth.m_nSmthTemCenY;
//DEL 	dbTempCoef  = dlgSmth.m_dbSmthTemCoef;
//DEL 	
//DEL 	// ɾ���Ի���
//DEL 	delete dlgSmth;	
//DEL 	
//DEL 	// ���Ĺ����״
//DEL 	BeginWaitCursor();
//DEL 	
//DEL 	// ����Template()����ƽ��DIB
//DEL 	if (GeneralTemplate(pDoc->m_pDibInit, nTempWidth, nTempHeight, 
//DEL 		nTempCenX, nTempCenY, pdbTemp, dbTempCoef))
//DEL 	{		
//DEL 		// ��������
//DEL 		pDoc->SetModifiedFlag(TRUE);
//DEL 
//DEL 		// ������ͼ
//DEL 		pDoc->UpdateAllViews(NULL);
//DEL 	}
//DEL 	else
//DEL 	{
//DEL 		// ��ʾ�û�
//DEL 		MessageBox("�����ڴ�ʧ�ܣ�", "ϵͳ��ʾ" , MB_ICONINFORMATION | MB_OK);
//DEL 	}
//DEL 	
//DEL 	// �ָ����
//DEL 	EndWaitCursor();	
//DEL 	
//DEL }

/*************************************************************************
 *
 * \�������ƣ�
 *   OnEnhanceMedian()
 *
 * \�������:
 *   ��
 * 
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   ��ͼ�������ֵ�˲���������ƽ��ģ�����öԻ���
 *
 *************************************************************************
 */
//DEL void CImageProcessingView::OnEnhanceMedian() 
//DEL {
//DEL 	// ��ֵ�˲�
//DEL 	
//DEL 	// ��ȡ�ĵ�
//DEL 	CImageProcessingDoc* pDoc = GetDocument();
//DEL 	
//DEL 	// �˲����ĸ߶�
//DEL 	int nFilterHeight;
//DEL 	
//DEL 	// �˲����Ŀ��
//DEL 	int nFilterWidth;
//DEL 	
//DEL 	// ����Ԫ�ص�X����
//DEL 	int nFilterCenX;
//DEL 	
//DEL 	// ����Ԫ�ص�Y����
//DEL 	int nFilterCenY;
//DEL 	
//DEL 	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ��ƽ���������Ŀ������ƣ�
//DEL 	if(pDoc->m_pDibInit->m_nColorTableEntries != 256)
//DEL 	{
//DEL 		// ��ʾ�û�
//DEL 		MessageBox("Ŀǰֻ֧��256ɫλͼ��ƽ����", "ϵͳ��ʾ" , 
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 		
//DEL 		// ����
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	
//DEL 	// �����Ի���
//DEL 	CDlgMedian dlgMedian;
//DEL 	
//DEL 	// ��ʼ������ֵ
//DEL 	dlgMedian.m_nFilterType = 0;
//DEL 	dlgMedian.m_nFilterHeight = 3;
//DEL 	dlgMedian.m_nFilterWidth = 1;
//DEL 	dlgMedian.m_nFilterCenX = 0;
//DEL 	dlgMedian.m_nFilterCenY = 1;
//DEL 	
//DEL 	// ��ʾ�Ի�����ʾ�û��趨ƽ����
//DEL 	if (dlgMedian.DoModal() != IDOK)
//DEL 	{
//DEL 		// ����
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	// ��ȡ�û����趨
//DEL 	nFilterHeight = dlgMedian.m_nFilterHeight;
//DEL 	nFilterWidth = dlgMedian.m_nFilterWidth;
//DEL 	nFilterCenX = dlgMedian.m_nFilterCenX;
//DEL 	nFilterCenY = dlgMedian.m_nFilterCenY;
//DEL 	
//DEL 	// ɾ���Ի���
//DEL 	delete dlgMedian;	
//DEL 	
//DEL 	// ���Ĺ����״
//DEL 	BeginWaitCursor();
//DEL 	
//DEL 	// ����MedianFilter()������ֵ�˲�
//DEL 	if (MedianFilter(pDoc->m_pDibInit, nFilterWidth,
//DEL 		nFilterHeight, nFilterCenX, nFilterCenY ))
//DEL 	
//DEL 	{		
//DEL 		// ��������
//DEL 		pDoc->SetModifiedFlag(TRUE);
//DEL 
//DEL 		// ������ͼ
//DEL 		pDoc->UpdateAllViews(NULL);
//DEL 	}
//DEL 	else
//DEL 	{
//DEL 		// ��ʾ�û�
//DEL 		MessageBox("�����ڴ�ʧ�ܣ�", "ϵͳ��ʾ" , MB_ICONINFORMATION | MB_OK);
//DEL 	}
//DEL 	
//DEL 	// �ָ����
//DEL 	EndWaitCursor();
//DEL 
//DEL 	// ��������
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 	
//DEL 	// ������ͼ
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL 	
//DEL }


//DEL void CImageProcessingView::OnEnhancePseudcolor() 
//DEL {
//DEL 	// α��ɫ����
//DEL 	
//DEL 	// ��ȡ�ĵ�
//DEL 	CImageProcessingDoc* pDoc = GetDocument();
//DEL 	
//DEL 	// �����û�ѡ���α��ɫ���������
//DEL 	int		nColor;
//DEL 	
//DEL 	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ��ƽ���������Ŀ������ƣ�
//DEL 	if(pDoc->m_pDibInit->m_nColorTableEntries != 256)
//DEL 	{
//DEL 		// ��ʾ�û�
//DEL 		MessageBox("Ŀǰֻ֧��256ɫλͼ��α��ɫ�任��", "ϵͳ��ʾ" , 
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 
//DEL 		// ����
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	// �����Ի���
//DEL 	CDlgEnhColor dlgPara;
//DEL 	
//DEL 	// ��ʼ������ֵ
//DEL 	if (pDoc->m_nColorIndex >= 0)
//DEL 	{
//DEL 		// ��ʼѡ�е�ǰ��α��ɫ
//DEL 		dlgPara.m_nColor = pDoc->m_nColorIndex;
//DEL 	}
//DEL 	else
//DEL 	{
//DEL 		// ��ʼѡ�лҶ�α��ɫ�����
//DEL 		dlgPara.m_nColor = 0;
//DEL 	}
//DEL 	
//DEL 	// ָ�����������ָ��
//DEL 	dlgPara.m_lpColorName = (LPSTR) ColorScaleName;
//DEL 	
//DEL 	// α��ɫ������Ŀ
//DEL 	dlgPara.m_nColorCount = COLOR_SCALE_COUNT;
//DEL 	
//DEL 	// �����ַ�������
//DEL 	dlgPara.m_nNameLen = sizeof(ColorScaleName) / COLOR_SCALE_COUNT;
//DEL 	
//DEL 	// ��ʾ�Ի�����ʾ�û��趨ƽ����
//DEL 	if (dlgPara.DoModal() != IDOK)
//DEL 	{
//DEL 		// ����
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	// ��ȡ�û����趨
//DEL 	nColor = dlgPara.m_nColor;
//DEL 	
//DEL 	// ɾ���Ի���
//DEL 	delete dlgPara;	
//DEL 	
//DEL 	// ���Ĺ����״
//DEL 	BeginWaitCursor();
//DEL 	
//DEL 	// �ж�α��ɫ�����Ƿ�Ķ�
//DEL 	if (pDoc->m_nColorIndex != nColor)
//DEL 	{
//DEL 		// ����ReplaceColorPal()�����任��ɫ��
//DEL 		ReplaceDIBColorTable(pDoc->m_pDibInit, (LPBYTE)ColorsTable[nColor]);
//DEL 		
//DEL 		// ���µ�ɫ��
//DEL 		pDoc->m_pDibInit->MakePalette();
//DEL 		
//DEL 		// �������Ա����
//DEL 		pDoc->m_nColorIndex = nColor;
//DEL 		
//DEL 		// ��������
//DEL 		pDoc->SetModifiedFlag(TRUE);
//DEL 				
//DEL 		// ������ͼ
//DEL 		pDoc->UpdateAllViews(NULL);
//DEL 	}	
//DEL 	
//DEL 
//DEL 	// �ָ����
//DEL 	EndWaitCursor();
//DEL }

//DEL void CImageProcessingView::OnTransDwt() 
//DEL {
//DEL 	// ����ĵ���ָ��
//DEL 	CImageProcessingDoc * pDoc = (CImageProcessingDoc *)this->GetDocument();
//DEL 
//DEL 	// ָ��ͼ���ָ��
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 
//DEL 	// ���Ĺ����״
//DEL 	BeginWaitCursor();
//DEL 
//DEL 	// ����С���任
//DEL 	int rsl = DIBDWTStep(pDib,0);
//DEL 
//DEL 	// �ָ������״
//DEL 	EndWaitCursor();
//DEL 
//DEL 	// ���С���任���ɹ�����ֱ�ӷ���
//DEL 	if (!rsl)			
//DEL 		return;
//DEL 
//DEL 	// �������־
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 
//DEL 	// ������ʾ
//DEL 	pDoc->UpdateAllViews(FALSE);
//DEL }

//DEL void CImageProcessingView::OnTransIdwt() 
//DEL {
//DEL 	// ����ĵ���ָ��	
//DEL 	CImageProcessingDoc * pDoc = (CImageProcessingDoc *)this->GetDocument();
//DEL 
//DEL 	// ָ��ͼ���ָ��
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 
//DEL 	// ���Ĺ����״
//DEL 	BeginWaitCursor();
//DEL 
//DEL 	// ����С�����任
//DEL 	int rsl = DIBDWTStep(pDib,1);
//DEL 
//DEL 	// �ָ������״
//DEL 	EndWaitCursor();
//DEL 
//DEL 	// ���С���任���ɹ�����ֱ�ӷ���
//DEL 	if (!rsl)			
//DEL 		return;
//DEL 	pDoc->UpdateAllViews(FALSE);
//DEL 
//DEL 	// ��������
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 	
//DEL 	// ������ͼ
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL }

//DEL BOOL CImageProcessingView::DIBDWTStep(CDib* pDib, int nInv)
//DEL {
//DEL 	// ѭ������
//DEL 	int i, j;
//DEL 
//DEL 	// ��ȡͼ��ĳ��ȺͿ��
//DEL 	int nWidth  = pDib->m_lpBMIH->biWidth;
//DEL 	int nHeight = pDib->m_lpBMIH->biHeight;
//DEL 		
//DEL 	// ��ȡ�任��������
//DEL 	int nMaxWLevel = Log2(nWidth);
//DEL 	int nMaxHLevel = Log2(nHeight);
//DEL 	int nMaxLevel;
//DEL 	if (nWidth == 1<<nMaxWLevel && nHeight == 1<<nMaxHLevel)
//DEL 		nMaxLevel = min(nMaxWLevel, nMaxHLevel);
//DEL 
//DEL 	// ��ȡͼ��Ĵ洢�ߴ�
//DEL 	CSize sizeImageSave = pDib->GetDibSaveDim();
//DEL 
//DEL 	// ��ʱ����
//DEL 	double	*pDbTemp;
//DEL 	BYTE	*pBits;
//DEL 
//DEL 	// ���С���任�Ĵ洢�ڴ滹û�з��䣬�������ڴ�
//DEL 	if(!m_pDbImage){			
//DEL 		m_pDbImage = new double[nWidth*nHeight];
//DEL 		if (!m_pDbImage)	return FALSE;
//DEL 
//DEL 		// ��ͼ�����ݷ���m_pDbImage�� 
//DEL 		for (j=0; j<nHeight; j++)
//DEL 		{
//DEL 			pDbTemp = m_pDbImage + j*sizeImageSave.cx;
//DEL 			pBits = pDib->m_lpImage + (nHeight-1-j)*sizeImageSave.cx;		
//DEL 			for (i=0; i<nWidth; i++)
//DEL 				pDbTemp[i] = pBits[i];
//DEL 		}
//DEL 	}
//DEL 	
//DEL 	// ����С���任���򷴱任��
//DEL 	if (!DWTStep_2D(m_pDbImage, nMaxWLevel-m_nDWTCurDepth, nMaxHLevel-m_nDWTCurDepth,
//DEL 						nMaxWLevel, nMaxHLevel, nInv, 1, m_nSupp))
//DEL 		return FALSE;
//DEL 
//DEL 	// ����Ƿ��任����ǰ������1
//DEL 	if (nInv)
//DEL 		m_nDWTCurDepth --;
//DEL 	// �����1
//DEL 	else
//DEL 		m_nDWTCurDepth ++;
//DEL 
//DEL 	// Ȼ�󣬽����ݿ�����ԭCDib�У���������Ӧ������ת��
//DEL 	int lfw = nWidth>>m_nDWTCurDepth, lfh = nHeight>>m_nDWTCurDepth;
//DEL 	for (j=0; j<nHeight; j++)
//DEL 	{
//DEL 		pDbTemp = m_pDbImage + j*sizeImageSave.cx;
//DEL 		pBits = pDib->m_lpImage + (nHeight-1-j)*sizeImageSave.cx;
//DEL 		for (i=0; i<nWidth; i++)
//DEL 		{
//DEL 			if (j<lfh && i<lfw)
//DEL 				pBits[i] = FloatToByte(pDbTemp[i]);
//DEL 			else
//DEL 				pBits[i] = BYTE(FloatToChar(pDbTemp[i]) ^ 0x80);			
//DEL 		}
//DEL 	}
//DEL 
//DEL 	// ����
//DEL 	return TRUE;
//DEL }
//DEL void CImageProcessingView::OnEnhanceLintrans() 
//DEL {
//DEL 	// ��ȡ�ĵ�
//DEL 	CImageProcessingDoc* pDoc = GetDocument();
//DEL 
//DEL 	// �����Ի���
//DEL 	CDlgEhnLinTrans dlgPara;
//DEL 	
//DEL 	// ��1����
//DEL 	int	nX1;
//DEL 	int	nY1;
//DEL 	
//DEL 	// ��2����
//DEL 	int	nX2;
//DEL 	int	nY2;
//DEL 
//DEL 	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ�ĻҶ����죬�����Ŀ������ƣ�
//DEL 	if(pDoc->m_pDibInit->m_nColorTableEntries != 256)
//DEL 	{
//DEL 		// ��ʾ�û�
//DEL 		MessageBox("Ŀǰֻ֧��256ɫλͼ��α��ɫ�任��", "ϵͳ��ʾ" , 
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 
//DEL 		// ����
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	// ��ʼ������ֵ
//DEL 	dlgPara.m_nX1 = 50;
//DEL 	dlgPara.m_nY1 = 30;
//DEL 	dlgPara.m_nX2 = 200;
//DEL 	dlgPara.m_nY2 = 220;
//DEL 	
//DEL 	// ��ʾ�Ի�����ʾ�û��趨����λ��
//DEL 	if (dlgPara.DoModal() != IDOK)
//DEL 	{
//DEL 		// ����
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	// ��ȡ�û����趨
//DEL 	nX1 = dlgPara.m_nX1;
//DEL 	nY1 = dlgPara.m_nY1;
//DEL 	nX2 = dlgPara.m_nX2;
//DEL 	nY2 = dlgPara.m_nY2;
//DEL 	
//DEL 	// ɾ���Ի���
//DEL 	delete dlgPara;	
//DEL 	
//DEL 	// ���Ĺ����״
//DEL 	BeginWaitCursor();
//DEL 	
//DEL 	// ����GrayStretch()�������лҶ�����
//DEL 	GraySegLinTrans(pDoc->m_pDibInit, nX1, nY1, nX2, nY2);
//DEL 	
//DEL 	// ��������
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 	
//DEL 	// ������ͼ
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL 	
//DEL 	// �ָ����
//DEL 	EndWaitCursor();
//DEL }

//DEL void CImageProcessingView::OnEnhanceHistequ() 
//DEL {
//DEL 	// ֱ��ͼ����
//DEL 
//DEL 	// ��ȡ�ĵ�
//DEL 	CImageProcessingDoc* pDoc = GetDocument();
//DEL 		
//DEL 	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ��ֱ��ͼ���⣬�����Ŀ������ƣ�
//DEL 	if(pDoc->m_pDibInit->m_nColorTableEntries != 256)
//DEL 	{
//DEL 		// ��ʾ�û�
//DEL 		MessageBox("Ŀǰֻ֧��256ɫλͼ��α��ɫ�任��", "ϵͳ��ʾ" , 
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 
//DEL 		// ����
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	// ���Ĺ����״
//DEL 	BeginWaitCursor();
//DEL 	
//DEL 	// ����HistogramEqualize()��������ֱ��ͼ����
//DEL 	HistogramEqualize(pDoc->m_pDibInit);
//DEL 	
//DEL 	// ��������
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 	
//DEL 	// ������ͼ
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL 		
//DEL 	// �ָ����
//DEL 	EndWaitCursor();	
//DEL }

/*************************************************************************
 *
 * \�������ƣ�
 *   OnRegReg()
 *
 * \�������:
 *   ��
 * 
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   �ú���ʵ��ͼ�����׼
 *
 *************************************************************************
 */
//DEL void CImageProcessingView::OnRegReg() 
//DEL {	
//DEL 	// ����ĵ�����
//DEL 	CImageProcessingDoc* pDoc;
//DEL 	pDoc = GetDocument();
//DEL 
//DEL 	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ��ˮƽ���������Ŀ������ƣ�
//DEL 	if(pDoc->m_pDibInit->m_nColorTableEntries != 256)
//DEL 	{
//DEL 		// ��ʾ�û�
//DEL 		MessageBox("Ŀǰֻ֧��256ɫλͼ��ͼ����׼��", "ϵͳ��ʾ" , 
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 		
//DEL 		// ����
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	// ��ͼ����׼�Ի���
//DEL 	CDlgReg* pDlg=new CDlgReg(NULL,pDoc);
//DEL 
//DEL 	pDlg->DoModal();
//DEL 
//DEL 	delete pDlg;
//DEL }

/*************************************************************************
 *
 * \�������ƣ�
 *   OnEnhanceSharp()
 *
 * \�������:
 *   ��
 * 
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   �ú�������Laplacian����ʵ��ͼ�����
 *
 *************************************************************************
 */
//DEL void CImageProcessingView::OnEnhanceSharp() 
//DEL {
//DEL 	//���Ĺ����״
//DEL 	BeginWaitCursor(); 
//DEL 	
//DEL 	CImageProcessingDoc * pDoc = (CImageProcessingDoc *)this->GetDocument();
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 	
//DEL 	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
//DEL 	
//DEL 	// �ж��Ƿ���8-bppλͼ
//DEL 	if (lpBMIH->biBitCount != 8)
//DEL 	{
//DEL 		// ��ʾ�û�
//DEL 		MessageBox("Ŀǰֻ֧��256ɫλͼ��ͼ��ָ", "ϵͳ��ʾ" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 		
//DEL 		// ����
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	// ѭ�����Ʊ���
//DEL 	int y;
//DEL 	int x;
//DEL 	
//DEL 	CSize sizeImage = pDib->GetDimensions();
//DEL 	int nWidth = sizeImage.cx ;
//DEL 	int nHeight= sizeImage.cy ;
//DEL 	
//DEL 	int nSaveWidth = pDib->GetDibSaveDim().cx;
//DEL 	
//DEL 	// �����ڴ棬�洢ͼ������,�����ݵĴ洢����DWORD�����
//DEL 	unsigned char * pUnchImage = new unsigned char[nWidth*nHeight];
//DEL 	
//DEL 	for(y=0; y<nHeight; y++)
//DEL 	{
//DEL 		for(x=0; x<nWidth; x++)
//DEL 		{
//DEL 			pUnchImage[y*nWidth+x] = pDib->m_lpImage[y*nSaveWidth+x];
//DEL 		}
//DEL 	}
//DEL 	
//DEL 	// ����LinearSharpen��������ͼ������ǿ
//DEL 	LinearSharpen(pUnchImage, nWidth, nHeight) ;
//DEL 	
//DEL 	// ��ǿ�Ժ��ͼ�󿽱���pDib�У�������ʾ
//DEL 	for(y=0; y<nHeight; y++)
//DEL 	{
//DEL 		for(x=0; x<nWidth; x++)
//DEL 		{
//DEL 			pDib->m_lpImage[y*nSaveWidth+x] = pUnchImage[y*nWidth+x];
//DEL 		}
//DEL 	}
//DEL 	
//DEL 	// �ͷ��ڴ�
//DEL 	delete []pUnchImage;
//DEL 	pUnchImage = NULL  ;
//DEL 	
//DEL 	// �ָ������״
//DEL 	EndWaitCursor(); 
//DEL 	
//DEL 	// ��������
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 	
//DEL 	// ������ͼ
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL 	
//DEL }

/*************************************************************************
*
* \�������ƣ�
*   OnEnhanceSmoothFr()
*
* \�������:
*   ��
*
* \����ֵ:
*   ��
*
* \˵��:
*   �ú������õ�ͨ�˲�ʵ��ͼ��ƽ��
*
************************************************************************
*/
//DEL void CImageProcessingView::OnEnhanceSmoothFr() 
//DEL {
//DEL 	// TODO: Add your command handler code here
//DEL 	//���Ĺ����״
//DEL 	BeginWaitCursor(); 
//DEL 	
//DEL 	CImageProcessingDoc * pDoc = (CImageProcessingDoc *)this->GetDocument();
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 	
//DEL 	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
//DEL 	
//DEL 	// �ж��Ƿ���8-bppλͼ
//DEL 	if (lpBMIH->biBitCount != 8)
//DEL 	{
//DEL 		// ��ʾ�û�
//DEL 		MessageBox("Ŀǰֻ֧��256ɫλͼ��ͼ��ָ", "ϵͳ��ʾ" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 		
//DEL 		// ����
//DEL 		return;
//DEL 	}
//DEL 
//DEL 	// ѭ�����Ʊ���
//DEL 	int y;
//DEL 	int x;
//DEL 	
//DEL 	CSize sizeImage = pDib->GetDimensions();
//DEL 	int nWidth = sizeImage.cx ;
//DEL 	int nHeight= sizeImage.cy ;
//DEL 
//DEL 	int nSaveWidth = pDib->GetDibSaveDim().cx;
//DEL 	
//DEL 	// �����ڴ棬�洢ͼ������,�����ݵĴ洢����DWORD�����
//DEL 	unsigned char * pUnchImage = new unsigned char[nWidth*nHeight];
//DEL 	
//DEL 	for(y=0; y<nHeight; y++)
//DEL 	{
//DEL 		for(x=0; x<nWidth; x++)
//DEL 		{
//DEL 			pUnchImage[y*nWidth+x] = pDib->m_lpImage[y*nSaveWidth+x];
//DEL 		}
//DEL 	}
//DEL 
//DEL 	// ���õ�ͨ�˲���������ͼ����ǿ
//DEL 	LowPassFilterEnhance(pUnchImage, nWidth, nHeight, nWidth/16) ;
//DEL 	
//DEL 	// ��ǿ�Ժ��ͼ�󿽱���pDib�У�������ʾ
//DEL 	for(y=0; y<nHeight; y++)
//DEL 	{
//DEL 		for(x=0; x<nWidth; x++)
//DEL 		{
//DEL 			pDib->m_lpImage[y*nSaveWidth+x] = pUnchImage[y*nWidth+x];
//DEL 		}
//DEL 	}
//DEL 	
//DEL 	// �ͷ��ڴ�
//DEL 	delete []pUnchImage;
//DEL 	pUnchImage = NULL  ;
//DEL 	
//DEL 	// �ָ������״
//DEL 	EndWaitCursor(); 
//DEL 	
//DEL 	// ��������
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 	
//DEL 	// ������ͼ
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL }

/*************************************************************************
*
* \�������ƣ�
*   OnEnhanceButtLow()
*
* \�������:
*   ��
*
* \����ֵ:
*   ��
*
* \˵��:
*   �ú�������Butterworth��ͨ�˲�ʵ��ͼ��ƽ��
*
************************************************************************
*/
//DEL void CImageProcessingView::OnEnhanceButtLow() 
//DEL {
//DEL 	// TODO: Add your command handler code here
//DEL 	//���Ĺ����״
//DEL 	BeginWaitCursor(); 
//DEL 	
//DEL 	CImageProcessingDoc * pDoc = (CImageProcessingDoc *)this->GetDocument();
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 	
//DEL 	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
//DEL 	
//DEL 	// �ж��Ƿ���8-bppλͼ
//DEL 	if (lpBMIH->biBitCount != 8)
//DEL 	{
//DEL 		// ��ʾ�û�
//DEL 		MessageBox("Ŀǰֻ֧��256ɫλͼ��ͼ��ָ", "ϵͳ��ʾ" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 		
//DEL 		// ����
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	// ѭ�����Ʊ���
//DEL 	int y;
//DEL 	int x;
//DEL 	
//DEL 	CSize sizeImage = pDib->GetDimensions();
//DEL 	int nWidth = sizeImage.cx ;
//DEL 	int nHeight= sizeImage.cy ;
//DEL 	
//DEL 	int nSaveWidth = pDib->GetDibSaveDim().cx;
//DEL 	
//DEL 	// �����ڴ棬�洢ͼ������,�����ݵĴ洢����DWORD�����
//DEL 	unsigned char * pUnchImage = new unsigned char[nWidth*nHeight];
//DEL 	
//DEL 	for(y=0; y<nHeight; y++)
//DEL 	{
//DEL 		for(x=0; x<nWidth; x++)
//DEL 		{
//DEL 			pUnchImage[y*nWidth+x] = pDib->m_lpImage[y*nSaveWidth+x];
//DEL 		}
//DEL 	}
//DEL 	
//DEL 	// ����ButterWorth��ͨ�˲���������ͼ����ǿ
//DEL 	ButterWorthLowPass(pUnchImage, nWidth, nHeight, nWidth/2) ;
//DEL 	
//DEL 	// ��ǿ�Ժ��ͼ�󿽱���pDib�У�������ʾ
//DEL 	for(y=0; y<nHeight; y++)
//DEL 	{
//DEL 		for(x=0; x<nWidth; x++)
//DEL 		{
//DEL 			pDib->m_lpImage[y*nSaveWidth+x] = pUnchImage[y*nWidth+x];
//DEL 		}
//DEL 	}
//DEL 	
//DEL 	// �ͷ��ڴ�
//DEL 	delete []pUnchImage;
//DEL 	pUnchImage = NULL  ;
//DEL 	
//DEL 	// �ָ������״
//DEL 	EndWaitCursor(); 
//DEL 	
//DEL 	// ��������
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 	
//DEL 	// ������ͼ
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL }

/*************************************************************************
*
* \�������ƣ�
*   OnEnhanceSharpFreq()
*
* \�������:
*   ��
*
* \����ֵ:
*   ��
*
* \˵��:
*   �ú������ø�ͨ�˲�ʵ��ͼ����ǿ
*
************************************************************************
*/
//DEL void CImageProcessingView::OnEnhanceSharpFreq() 
//DEL {
//DEL 	// TODO: Add your command handler code here
//DEL 	//���Ĺ����״
//DEL 	BeginWaitCursor(); 
//DEL 	
//DEL 	CImageProcessingDoc * pDoc = (CImageProcessingDoc *)this->GetDocument();
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 	
//DEL 	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
//DEL 	
//DEL 	// �ж��Ƿ���8-bppλͼ
//DEL 	if (lpBMIH->biBitCount != 8)
//DEL 	{
//DEL 		// ��ʾ�û�
//DEL 		MessageBox("Ŀǰֻ֧��256ɫλͼ��ͼ��ָ", "ϵͳ��ʾ" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 		
//DEL 		// ����
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	// ѭ�����Ʊ���
//DEL 	int y;
//DEL 	int x;
//DEL 	
//DEL 	CSize sizeImage = pDib->GetDimensions();
//DEL 	int nWidth = sizeImage.cx ;
//DEL 	int nHeight= sizeImage.cy ;
//DEL 	
//DEL 	int nSaveWidth = pDib->GetDibSaveDim().cx;
//DEL 	
//DEL 	// �����ڴ棬�洢ͼ������,�����ݵĴ洢����DWORD�����
//DEL 	unsigned char * pUnchImage = new unsigned char[nWidth*nHeight];
//DEL 	
//DEL 	for(y=0; y<nHeight; y++)
//DEL 	{
//DEL 		for(x=0; x<nWidth; x++)
//DEL 		{
//DEL 			pUnchImage[y*nWidth+x] = pDib->m_lpImage[y*nSaveWidth+x];
//DEL 		}
//DEL 	}
//DEL 	
//DEL 	// ���ø�ͨ�˲���������ͼ����ǿ
//DEL 	HighPassFilterEnhance(pUnchImage, nWidth, nHeight, 50) ;
//DEL 	
//DEL 	// ��ǿ�Ժ��ͼ�󿽱���pDib�У�������ʾ
//DEL 	for(y=0; y<nHeight; y++)
//DEL 	{
//DEL 		for(x=0; x<nWidth; x++)
//DEL 		{
//DEL 			pDib->m_lpImage[y*nSaveWidth+x] = pUnchImage[y*nWidth+x];
//DEL 		}
//DEL 	}
//DEL 	
//DEL 	// �ͷ��ڴ�
//DEL 	delete []pUnchImage;
//DEL 	pUnchImage = NULL  ;
//DEL 	
//DEL 	// �ָ������״
//DEL 	EndWaitCursor(); 
//DEL 	
//DEL 	// ��������
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 	
//DEL 	// ������ͼ
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL }

/*************************************************************************
*
* \�������ƣ�
*   OnEnhanceButtHight()
*
* \�������:
*   ��
*
* \����ֵ:
*   ��
*
* \˵��:
*   �ú�������Butterworth��ͨ�˲�ʵ��ͼ��ƽ��
*
************************************************************************
*/
//DEL void CImageProcessingView::OnEnhanceButtHight() 
//DEL {	
//DEL 	//���Ĺ����״
//DEL 	BeginWaitCursor(); 
//DEL 	
//DEL 	CImageProcessingDoc * pDoc = (CImageProcessingDoc *)this->GetDocument();
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 	
//DEL 	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
//DEL 	
//DEL 	// �ж��Ƿ���8-bppλͼ
//DEL 	if (lpBMIH->biBitCount != 8)
//DEL 	{
//DEL 		// ��ʾ�û�
//DEL 		MessageBox("Ŀǰֻ֧��256ɫλͼ��ͼ��ָ", "ϵͳ��ʾ" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 		
//DEL 		// ����
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	// ѭ�����Ʊ���
//DEL 	int y;
//DEL 	int x;
//DEL 	
//DEL 	CSize sizeImage = pDib->GetDimensions();
//DEL 	int nWidth = sizeImage.cx ;
//DEL 	int nHeight= sizeImage.cy ;
//DEL 	
//DEL 	int nSaveWidth = pDib->GetDibSaveDim().cx;
//DEL 	
//DEL 	// �����ڴ棬�洢ͼ������,�����ݵĴ洢����DWORD�����
//DEL 	unsigned char * pUnchImage = new unsigned char[nWidth*nHeight];
//DEL 	
//DEL 	for(y=0; y<nHeight; y++)
//DEL 	{
//DEL 		for(x=0; x<nWidth; x++)
//DEL 		{
//DEL 			pUnchImage[y*nWidth+x] = pDib->m_lpImage[y*nSaveWidth+x];
//DEL 		}
//DEL 	}
//DEL 	
//DEL 	// ����ButterWorth��ͨ�˲���������ͼ����ǿ
//DEL 	ButterWorthHighPass(pUnchImage, nWidth, nHeight, nWidth/2) ;
//DEL 	
//DEL 	// ��ǿ�Ժ��ͼ�󿽱���pDib�У�������ʾ
//DEL 	for(y=0; y<nHeight; y++)
//DEL 	{
//DEL 		for(x=0; x<nWidth; x++)
//DEL 		{
//DEL 			pDib->m_lpImage[y*nSaveWidth+x] = pUnchImage[y*nWidth+x];
//DEL 		}
//DEL 	}
//DEL 	
//DEL 	// �ͷ��ڴ�
//DEL 	delete []pUnchImage;
//DEL 	pUnchImage = NULL  ;
//DEL 	
//DEL 	// �ָ������״
//DEL 	EndWaitCursor(); 
//DEL 	
//DEL 	// ��������
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 	
//DEL 	// ������ͼ
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL 	
//DEL }

/*************************************************************************
*
* \�������ƣ�
*   OnRegionsegFix()
*
* \�������:
*   ��
*
* \����ֵ:
*   ��
*
* \˵��:
*   ʵ�ֲ�������ָ�
*
************************************************************************
*/
//DEL void CImageProcessingView::OnRegionsegFix() 
//DEL {
//DEL 	// TODO: Add your command handler code here
//DEL 
//DEL 	//���Ĺ����״
//DEL 	BeginWaitCursor(); 
//DEL 
//DEL 	CImageProcessingDoc * pDoc = (CImageProcessingDoc *)this->GetDocument();
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 
//DEL 	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
//DEL 	
//DEL 	// �ж��Ƿ���8-bppλͼ
//DEL 	if (lpBMIH->biBitCount != 8)
//DEL 	{
//DEL 		// ��ʾ�û�
//DEL 		MessageBox("Ŀǰֻ֧��256ɫλͼ��ͼ��ָ", "ϵͳ��ʾ" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 					
//DEL 		// ����
//DEL 		return;
//DEL 	}
//DEL 
//DEL 	// ���ú���ʵ�̶ֹ���ֵ�ָ�
//DEL 	RegionSegFixThreshold(pDib,200);
//DEL 
//DEL 	// �ָ������״
//DEL 	EndWaitCursor(); 
//DEL 	
//DEL 	// ��������
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 	
//DEL 	// ������ͼ
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL }

/*************************************************************************
*
* \�������ƣ�
*   OnAdaRegionSeg()
*
* \�������:
*   ��
*
* \����ֵ:
*   ��
*
* \˵��:
*   ʵ������Ӧ����ָ��㷨
*
************************************************************************
*/
//DEL void CImageProcessingView::OnAdaRegionSeg() 
//DEL {
//DEL 	// TODO: Add your command handler code here
//DEL 	
//DEL 	//���Ĺ����״
//DEL 	BeginWaitCursor(); 
//DEL 	
//DEL 	CImageProcessingDoc * pDoc = (CImageProcessingDoc *)this->GetDocument();
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 	
//DEL 	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
//DEL 	
//DEL 	// �ж��Ƿ���8-bppλͼ
//DEL 	if (lpBMIH->biBitCount != 8)
//DEL 	{
//DEL 		// ��ʾ�û�
//DEL 		MessageBox("Ŀǰֻ֧��256ɫλͼ��ͼ��ָ", "ϵͳ��ʾ" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 		
//DEL 		// ����
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	// ����Ӧ����ָ�
//DEL 	RegionSegAdaptive(pDib);
//DEL 	
//DEL 	// �ָ������״
//DEL 	EndWaitCursor(); 
//DEL 	
//DEL 		// ��������
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 	
//DEL 	// ������ͼ
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL 	
//DEL }
/*************************************************************************
*
* \�������ƣ�
*   OnEdgeRoberts()
*
* \�������:
*   ��
*
* \����ֵ:
*   ��
*
* \˵��:
*   ʵ�ֲ��б߽�ָRoberts����
*
************************************************************************
*/
//DEL void CImageProcessingView::OnEdgeRoberts() 
//DEL {
//DEL 	// TODO: Add your command handler code here
//DEL 
//DEL 	//���Ĺ����״
//DEL 	BeginWaitCursor(); 
//DEL 
//DEL 	// ѭ�����Ʊ���
//DEL 	int y;
//DEL 	int x;
//DEL 	
//DEL 	CImageProcessingDoc * pDoc = (CImageProcessingDoc *)this->GetDocument();
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 
//DEL 	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
//DEL 	
//DEL 	// �ж��Ƿ���8-bppλͼ
//DEL 	if (lpBMIH->biBitCount != 8)
//DEL 	{
//DEL 		// ��ʾ�û�
//DEL 		MessageBox("Ŀǰֻ֧��256ɫλͼ��ͼ��ָ", "ϵͳ��ʾ" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 		
//DEL 		// ����
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	// ͼ��ĳ����С
//DEL 	CSize sizeImage		= pDib->GetDimensions();
//DEL 	int nWidth			= sizeImage.cx		;
//DEL 	int nHeight			= sizeImage.cy		;
//DEL 	
//DEL 	// ָ���ݶ����ݵ�ָ��
//DEL 	double * pdGrad;
//DEL 	
//DEL 	// ����ͼ��Ĵ�С�����ڴ�ռ䣬�洢�ݶȼ���Ľ��
//DEL 	pdGrad=new double[nHeight*nWidth];
//DEL 	
//DEL 	//ͼ�����ݵ�ָ��
//DEL 	LPBYTE  pImageData = pDib->m_lpImage;
//DEL 	
//DEL 	// ͼ���ڼ�����ڴ洢�е�ʵ�ʴ�С
//DEL 	CSize sizeImageSave	= pDib->GetDibSaveDim();
//DEL 	
//DEL 	// ͼ�����ڴ���ÿһ������ռ�õ�ʵ�ʿռ�
//DEL 	int nSaveWidth = sizeImageSave.cx;
//DEL 	
//DEL 	// Ӧ��Roberts�������ݶ�
//DEL 	RobertsOperator(pDib, pdGrad);
//DEL 	
//DEL 	
//DEL 	for(y=0; y<nHeight ; y++ )
//DEL 		for(x=0 ; x<nWidth ; x++ )
//DEL 		{
//DEL 			if(*(pdGrad+y*nWidth+x)>50)
//DEL 				*(	pImageData+y*nSaveWidth+x	)=0;
//DEL 			else
//DEL 				*(	pImageData+y*nSaveWidth+x	)=255;			
//DEL 		}
//DEL 		
//DEL   //�ͷ��ݶȽ��ʹ�õ��ڴ�ռ�
//DEL 	delete pdGrad;
//DEL 	pdGrad=NULL;
//DEL 
//DEL 	// �ָ������״
//DEL 	EndWaitCursor(); 
//DEL 	
//DEL 	// ��������
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 	
//DEL 	// ������ͼ
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL }


/*************************************************************************
*
* \�������ƣ�
*   OnEdgeSobel()
*
* \�������:
*   ��
*
* \����ֵ:
*   ��
*
* \˵��:
*   ʵ�ֲ��б߽�ָSobel����
*
************************************************************************
*/
//DEL void CImageProcessingView::OnEdgeSobel() 
//DEL {
//DEL 	// TODO: Add your command handler code here
//DEL 	
//DEL 	//���Ĺ����״
//DEL 	BeginWaitCursor(); 
//DEL 
//DEL 	// ѭ�����Ʊ���
//DEL 	int y;
//DEL 	int x;
//DEL 	
//DEL 	CImageProcessingDoc * pDoc = (CImageProcessingDoc *)this->GetDocument();
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 
//DEL 	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
//DEL 	
//DEL 	// �ж��Ƿ���8-bppλͼ
//DEL 	if (lpBMIH->biBitCount != 8)
//DEL 	{
//DEL 		// ��ʾ�û�
//DEL 		MessageBox("Ŀǰֻ֧��256ɫλͼ��ͼ��ָ", "ϵͳ��ʾ" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 		
//DEL 		// ����
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	// ͼ��ĳ����С
//DEL 	CSize sizeImage		= pDib->GetDimensions();
//DEL 	int nWidth			= sizeImage.cx		;
//DEL 	int nHeight			= sizeImage.cy		;
//DEL 	
//DEL 	// ָ���ݶ����ݵ�ָ��
//DEL 	double * pdGrad;
//DEL 	
//DEL 	// ����ͼ��Ĵ�С�����ڴ�ռ䣬�洢�ݶȼ���Ľ��
//DEL 	pdGrad=new double[nHeight*nWidth];
//DEL 	
//DEL 	//ͼ�����ݵ�ָ��
//DEL 	LPBYTE  lpImage = pDib->m_lpImage;
//DEL 	
//DEL 	// ͼ���ڼ�����ڴ洢�е�ʵ�ʴ�С
//DEL 	CSize sizeImageSave	= pDib->GetDibSaveDim();
//DEL 	
//DEL 	// ͼ�����ڴ���ÿһ������ռ�õ�ʵ�ʿռ�
//DEL 	int nSaveWidth = sizeImageSave.cx;
//DEL 	
//DEL 	// Ӧ��Sobel�������ݶ�
//DEL 	SobelOperator(pDib, pdGrad);
//DEL 	
//DEL 	
//DEL 	for(y=0; y<nHeight ; y++ )
//DEL 		for(x=0 ; x<nWidth ; x++ )
//DEL 		{
//DEL 			if(*(pdGrad+y*nWidth+x)>50)
//DEL 				*(	lpImage+y*nSaveWidth+x	)=0;
//DEL 			else
//DEL 				*(	lpImage+y*nSaveWidth+x	)=255;			
//DEL 		}
//DEL 		
//DEL   //�ͷ��ڴ�ռ�
//DEL 	delete []pdGrad;
//DEL 	pdGrad=NULL;
//DEL 	
//DEL 	// �ָ������״
//DEL 	EndWaitCursor(); 
//DEL 	
//DEL 	// ��������
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 	
//DEL 	// ������ͼ
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL }
/*************************************************************************
*
* \�������ƣ�
*   OnEdgePrewitt()
*
* \�������:
*   ��
*
* \����ֵ:
*   ��
*
* \˵��:
*   ʵ�ֲ��б߽�ָPrewitt����
*
************************************************************************
*/
//DEL void CImageProcessingView::OnEdgePrewitt() 
//DEL {
//DEL 	// TODO: Add your command handler code here
//DEL 	
//DEL 	//���Ĺ����״
//DEL 	BeginWaitCursor(); 
//DEL 
//DEL 	// ѭ�����Ʊ���
//DEL 	int y;
//DEL 	int x;
//DEL 	
//DEL 	CImageProcessingDoc * pDoc = (CImageProcessingDoc *)this->GetDocument();
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 
//DEL 	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
//DEL 	
//DEL 	// �ж��Ƿ���8-bppλͼ
//DEL 	if (lpBMIH->biBitCount != 8)
//DEL 	{
//DEL 		// ��ʾ�û�
//DEL 		MessageBox("Ŀǰֻ֧��256ɫλͼ��ͼ��ָ", "ϵͳ��ʾ" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 		
//DEL 		// ����
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	// ͼ��ĳ����С
//DEL 	CSize sizeImage		= pDib->GetDimensions();
//DEL 	int nWidth			= sizeImage.cx		;
//DEL 	int nHeight			= sizeImage.cy		;
//DEL 	
//DEL 	// ָ���ݶ����ݵ�ָ��
//DEL 	double * pdGrad;
//DEL 	
//DEL 	// ����ͼ��Ĵ�С�����ڴ�ռ䣬�洢�ݶȼ���Ľ��
//DEL 	pdGrad=new double[nHeight*nWidth];
//DEL 	
//DEL 	//ͼ�����ݵ�ָ��
//DEL 	LPBYTE  lpImage = pDib->m_lpImage;
//DEL 	
//DEL 	// ͼ���ڼ�����ڴ洢�е�ʵ�ʴ�С
//DEL 	CSize sizeImageSave	= pDib->GetDibSaveDim();
//DEL 	
//DEL 	// ͼ�����ڴ���ÿһ������ռ�õ�ʵ�ʿռ�
//DEL 	int nSaveWidth = sizeImageSave.cx;
//DEL 	
//DEL 	// Ӧ��Prewitt�������ݶ�
//DEL 	PrewittOperator(pDib, pdGrad);
//DEL 	
//DEL 	
//DEL 	for(y=0; y<nHeight ; y++ )
//DEL 		for(x=0 ; x<nWidth ; x++ )
//DEL 		{
//DEL 			if(*(pdGrad+y*nWidth+x)>50)
//DEL 				*(	lpImage+y*nSaveWidth+x	)=0;
//DEL 			else
//DEL 				*(	lpImage+y*nSaveWidth+x	)=255;			
//DEL 		}
//DEL 	
//DEL   //�ͷ��ڴ�ռ�
//DEL 	delete []pdGrad;
//DEL 	pdGrad=NULL;
//DEL 	
//DEL 	// �ָ������״
//DEL 	EndWaitCursor(); 
//DEL 	
//DEL 	// ��������
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 	
//DEL 	// ������ͼ
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL 		
//DEL }

/*************************************************************************
*
* \�������ƣ�
*   OnEdgeLaplace()
*
* \�������:
*   ��
*
* \����ֵ:
*   ��
*
* \˵��:
*   ʵ�ֲ��б߽�ָ������˹����
*
************************************************************************
*/
//DEL void CImageProcessingView::OnEdgeLaplace() 
//DEL {
//DEL 	// TODO: Add your command handler code here
//DEL 	
//DEL 	//���Ĺ����״
//DEL 	BeginWaitCursor(); 
//DEL 	
//DEL 	// ѭ�����Ʊ���
//DEL 	int y;
//DEL 	int x;
//DEL 	
//DEL 	CImageProcessingDoc * pDoc = (CImageProcessingDoc *)this->GetDocument();
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 	
//DEL 	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
//DEL 	
//DEL 	// �ж��Ƿ���8-bppλͼ
//DEL 	if (lpBMIH->biBitCount != 8)
//DEL 	{
//DEL 		// ��ʾ�û�
//DEL 		MessageBox("Ŀǰֻ֧��256ɫλͼ��ͼ��ָ", "ϵͳ��ʾ" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 		
//DEL 		// ����
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	// ͼ��ĳ����С
//DEL 	CSize sizeImage		= pDib->GetDimensions();
//DEL 	int nWidth			= sizeImage.cx		;
//DEL 	int nHeight			= sizeImage.cy		;
//DEL 	
//DEL 	// ָ���ݶ����ݵ�ָ��
//DEL 	double * pdGrad;
//DEL 	
//DEL 	// ����ͼ��Ĵ�С�����ڴ�ռ䣬�洢�ݶȼ���Ľ��
//DEL 	pdGrad=new double[nHeight*nWidth];
//DEL 	
//DEL 	//ͼ�����ݵ�ָ��
//DEL 	LPBYTE  lpImage = pDib->m_lpImage;
//DEL 	
//DEL 	// ͼ���ڼ�����ڴ洢�е�ʵ�ʴ�С
//DEL 	CSize sizeImageSave	= pDib->GetDibSaveDim();
//DEL 	
//DEL 	// ͼ�����ڴ���ÿһ������ռ�õ�ʵ�ʿռ�
//DEL 	int nSaveWidth = sizeImageSave.cx;
//DEL 	
//DEL 	// Ӧ��Laplace�������ݶ�
//DEL 	LaplacianOperator(pDib, pdGrad);
//DEL 	
//DEL 	
//DEL 	for(y=0; y<nHeight ; y++ )
//DEL 		for(x=0 ; x<nWidth ; x++ )
//DEL 		{
//DEL 			if(*(pdGrad+y*nWidth+x)>50)
//DEL 				*(	lpImage+y*nSaveWidth+x	)=0;
//DEL 			else
//DEL 				*(	lpImage+y*nSaveWidth+x	)=255;			
//DEL 		}
//DEL 	
//DEL 	//�ͷ��ڴ�ռ�
//DEL 	delete []pdGrad;
//DEL 	pdGrad=NULL;
//DEL 	
//DEL 	// �ָ������״
//DEL 	EndWaitCursor(); 
//DEL 	
//DEL 	// ��������
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 	
//DEL 	// ������ͼ
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL 	
//DEL }

/*************************************************************************
*
* \�������ƣ�
*   OnEdgeCanny()
*
* \�������:
*   ��
*
* \����ֵ:
*   ��
*
* \˵��:
*   ʵ�ֲ��б߽�ָCanny����
*
************************************************************************
*/
void CImageProcessingView::OnEdgeCanny() 
{
	// TODO: Add your command handler code here

	//���Ĺ����״
	BeginWaitCursor(); 

	CImageProcessingDoc * pDoc = (CImageProcessingDoc *)this->GetDocument();
	CDib * pDib = pDoc->m_pDibInit;
	
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
	
	// �ж��Ƿ���8-bppλͼ
	if (lpBMIH->biBitCount != 8)
	{
		// ��ʾ�û�
		MessageBox("Ŀǰֻ֧��256ɫλͼ��ͼ��ָ", "ϵͳ��ʾ" ,
			MB_ICONINFORMATION | MB_OK);
		
		// ����
		return;
	}

	// ѭ�����Ʊ���
	int y; 
	int x;
	
	CSize sizeImage = pDib->GetDimensions();
	int nWidth = sizeImage.cx ;
	int nHeight= sizeImage.cy ;

	int nSaveWidth = pDib->GetDibSaveDim().cx;

	// �����ڴ棬�洢ͼ������
	unsigned char * pUnchImage = new unsigned char[nWidth*nHeight];

	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			pUnchImage[y*nWidth+x] = pDib->m_lpImage[y*nSaveWidth+x];
		}
	}
	
	// canny���Ӽ����Ľ��
	unsigned char * pUnchEdge = new unsigned char[nWidth*nHeight];
	
	// ����canny�������б߽���ȡ
	Canny(pUnchImage, nWidth, nHeight, 0.4, 0.4, 0.79, pUnchEdge) ;
	
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			pDib->m_lpImage[y*nWidth+x]=(unsigned char)(255-pUnchEdge[y*nWidth+x]);
		}
	}

	delete []pUnchImage;
	pUnchImage = NULL  ;
	delete []pUnchEdge ;
	pUnchEdge = NULL   ;
	
	// �ָ������״
	EndWaitCursor(); 
	
	// ��������
	pDoc->SetModifiedFlag(TRUE);
	
	// ������ͼ
	pDoc->UpdateAllViews(NULL);
	
}

/*************************************************************************
*
* \�������ƣ�
*   OnEdgeTrack()
*
* \�������:
*   ��
*
* \����ֵ:
*   ��
*
* \˵��:
*   ʵ�ֱ߽�����㷨
*
************************************************************************
*/
//DEL void CImageProcessingView::OnEdgeTrack() 
//DEL {
//DEL 	// TODO: Add your command handler code here
//DEL 
//DEL 	//���Ĺ����״
//DEL 	BeginWaitCursor(); 
//DEL 
//DEL 	// ���Doc���ָ
//DEL 	CImageProcessingDoc * pDoc = (CImageProcessingDoc *)this->GetDocument();
//DEL 	
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 
//DEL 	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
//DEL 	
//DEL 	// �ж��Ƿ���8-bppλͼ
//DEL 	if (lpBMIH->biBitCount != 8)
//DEL 	{
//DEL 		// ��ʾ�û�
//DEL 		MessageBox("Ŀǰֻ֧��256ɫλͼ��ͼ��ָ", "ϵͳ��ʾ" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 		
//DEL 		// ����
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	
//DEL 	// ͼ���ڼ�����ڴ洢�е�ʵ�ʴ�С
//DEL 	CSize sizeImageSave	= pDib->GetDibSaveDim();
//DEL 	
//DEL 	// ͼ�����ڴ���ÿһ������ռ�õ�ʵ�ʿռ�
//DEL 	int nSaveWidth = sizeImageSave.cx;
//DEL 	
//DEL 	// ����ͼ���������
//DEL 	int y;
//DEL 	
//DEL 	// ����ͼ��ĺ�����
//DEL 	int x;
//DEL 	
//DEL 	// ͼ��ĳ����С
//DEL 	CSize sizeImage		= pDib->GetDimensions();
//DEL 	int nWidth			= sizeImage.cx		;
//DEL 	int nHeight			= sizeImage.cy		;	
//DEL 	
//DEL 	
//DEL 	// ָ��ͼ�����ݵ�ָ��
//DEL 	LPBYTE lpImage ;
//DEL 	lpImage = pDib->m_lpImage ;
//DEL 	
//DEL 	// �߽���ٺ�Ľ������
//DEL 	unsigned char * pUnEdgeTrack ;
//DEL 	
//DEL 	pUnEdgeTrack = new unsigned char[nWidth * nHeight] ;
//DEL 	
//DEL 	EdgeTrack(pDib, pUnEdgeTrack);
//DEL 	
//DEL 	for(y=0; y<nHeight; y++)
//DEL 	{
//DEL 		for(x=0; x<nWidth; x++)
//DEL 		{	
//DEL 			lpImage[y*nSaveWidth + x] = (unsigned char) (255-pUnEdgeTrack[y*nWidth + x]);
//DEL 		}
//DEL 	}
//DEL 	
//DEL 	//�ͷ��ڴ�
//DEL 	delete pUnEdgeTrack;
//DEL 	pUnEdgeTrack = NULL;
//DEL 	
//DEL 	// �ָ������״
//DEL 	EndWaitCursor(); 
//DEL 	
//DEL 	// ��������
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 	
//DEL 	// ������ͼ
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL 	
//DEL }

/*************************************************************************
*
* \�������ƣ�
*   OnRegionGrow()
*
* \�������:
*   ��
*
* \����ֵ:
*   ��
*
* \˵��:
*   ʵ�����������㷨
*
************************************************************************
*/
//DEL void CImageProcessingView::OnRegionGrow() 
//DEL {
//DEL 	// TODO: Add your command handler code here
//DEL 
//DEL 	//���Ĺ����״
//DEL 	BeginWaitCursor(); 
//DEL 	
//DEL 	// ���Doc���ָ
//DEL 	CImageProcessingDoc * pDoc = (CImageProcessingDoc *)this->GetDocument();
//DEL 	
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 
//DEL 	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
//DEL 	
//DEL 	// �ж��Ƿ���8-bppλͼ
//DEL 	if (lpBMIH->biBitCount != 8)
//DEL 	{
//DEL 		// ��ʾ�û�
//DEL 		MessageBox("Ŀǰֻ֧��256ɫλͼ��ͼ��ָ", "ϵͳ��ʾ" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 		
//DEL 		// ����
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	// ����ͼ���������
//DEL 	int y;
//DEL 	
//DEL 	// ����ͼ��ĺ�����
//DEL 	int x;
//DEL 	
//DEL 	// ͼ���ڼ�����ڴ洢�е�ʵ�ʴ�С
//DEL 	CSize sizeImageSave	= pDib->GetDibSaveDim();
//DEL 	
//DEL 	// ͼ�����ڴ���ÿһ������ռ�õ�ʵ�ʿռ�
//DEL 	int nSaveWidth = sizeImageSave.cx;
//DEL 	
//DEL 	// ͼ��ĳ����С
//DEL 	CSize sizeImage		= pDib->GetDimensions();
//DEL 	int nWidth			= sizeImage.cx		;
//DEL 	int nHeight			= sizeImage.cy		;	
//DEL 	
//DEL 	// ָ��ͼ�����ݵ�ָ��
//DEL 	LPBYTE lpImage ;
//DEL 	lpImage = pDib->m_lpImage ;
//DEL 	
//DEL 	unsigned char * pUnchRgRst = new unsigned char[nWidth * nHeight];
//DEL 	// ��ʼ��
//DEL 	memset(pUnchRgRst, 0 , sizeof(unsigned char)*nWidth*nHeight );
//DEL 	
//DEL 	RegionGrow(pDib, pUnchRgRst);
//DEL 	
//DEL 	for(y=0; y<nHeight; y++)
//DEL 		for(x=0; x<nWidth; x++)
//DEL 		{
//DEL 			lpImage[y*nSaveWidth + x] =(unsigned char) (255-pUnchRgRst[y*nWidth+x] );
//DEL 		}
//DEL 		
//DEL 	// �ͷ��ڴ�
//DEL 	delete []pUnchRgRst;
//DEL 	pUnchRgRst = NULL  ;
//DEL 	
//DEL 	// �ָ������״
//DEL 	EndWaitCursor(); 
//DEL 	
//DEL 	// ��������
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 	
//DEL 	// ������ͼ
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL }

/*************************************************************************
 *
 * \�������ƣ�
 *   OnMotionBackground()
 *
 * \�������:
 *   ��
 * 
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   �ú��������˶�ͼ����ȡ���еľ�ֹ�����������˶�ͼ��Ҫ��Ϊbmp�ļ�������
 *˳�����С�
 *
 *************************************************************************
 */
//DEL void CImageProcessingView::OnMotionBackground() 
//DEL {
//DEL 	// ��ȡ�����ɹ���־
//DEL 	BOOL bFlag;
//DEL 	
//DEL 	// ��ȡ�ĵ�ָ��
//DEL 	CImageProcessingDoc* pDoc;
//DEL 	pDoc = GetDocument();
//DEL 
//DEL 	// ��õ�ǰ���ļ����ļ�·����
//DEL 	CString strPathName;
//DEL 	strPathName = pDoc->GetPathName();
//DEL 
//DEL 	// ���е���֡��
//DEL 	int nTotalFrameNum = 20;
//DEL 
//DEL 	// ͼ��Ŀ��
//DEL 	int nImageWidth;
//DEL 	nImageWidth = pDoc->m_pDibInit->m_lpBMIH->biWidth;	
//DEL 
//DEL 	// ͼ��ĸ߶�
//DEL 	int nImageHeight;
//DEL 	nImageHeight = pDoc->m_pDibInit->m_lpBMIH->biHeight;
//DEL 
//DEL 	// ͼ��ľ�ֹ����	
//DEL 	unsigned char* pUnchBackGround;
//DEL 	pUnchBackGround = new unsigned char[nImageWidth*nImageHeight];
//DEL 	
//DEL 	// ���Ĺ����״
//DEL 	BeginWaitCursor();
//DEL 	
//DEL 	// ����GetBackground������ȡ��ֹ����
//DEL 	bFlag = GetBackground(strPathName, nTotalFrameNum, nImageWidth,nImageHeight, pUnchBackGround);
//DEL 	if(bFlag == FALSE){
//DEL 		return;
//DEL 	}
//DEL 
//DEL 	// ����������Ϊ��ǰ��ʾͼ��
//DEL 	LPBYTE lpTemp;
//DEL 	lpTemp = pDoc->m_pDibInit->m_lpImage;
//DEL 
//DEL 	// �����ݿ�����ͼ����
//DEL 	memcpy(lpTemp, (LPBYTE)pUnchBackGround, nImageWidth*nImageHeight);
//DEL 	
//DEL 	// �ָ������״
//DEL 	EndWaitCursor(); 
//DEL 
//DEL 	// �ͷ��ѷ����ڴ�
//DEL 	delete[]pUnchBackGround;
//DEL 	pUnchBackGround = NULL;
//DEL 	
//DEL 	// ��������
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 	
//DEL 	// ������ͼ
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL }

/*************************************************************************
 *
 * \�������ƣ�
 *   OnRecogMatch()
 *
 * \�������:
 *   ��
 * 
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   ����ͼ��ģ�壬�ڴ�ƥ���ͼ�����ҵ�ƥ���λ��
 *
 *************************************************************************
 */
//DEL void CImageProcessingView::OnRecogMatch() 
//DEL {
//DEL 	CImageProcessingDoc* pDoc;
//DEL 	pDoc = GetDocument();
//DEL 
//DEL 	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ��ˮƽ���������Ŀ������ƣ�
//DEL 	if(pDoc->m_pDibInit->m_nColorTableEntries != 256)
//DEL 	{
//DEL 		// ��ʾ�û�
//DEL 		MessageBox("Ŀǰֻ֧��256ɫλͼ��ͼ����׼��", "ϵͳ��ʾ" , 
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 		
//DEL 		// ����
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	// ��ͼ��ʶ��Ի���
//DEL 	CDlgRecMatch* pDlg = new CDlgRecMatch(NULL, pDoc);
//DEL 	pDlg->DoModal();
//DEL 	
//DEL 	delete pDlg;
//DEL }

//DEL void CImageProcessingView::OnDegenerationInverse() 
//DEL {
//DEL 	// ͼ���ģ��
//DEL 		
//DEL 	// ���Ĺ����״
//DEL 	BeginWaitCursor();
//DEL 
//DEL 	// ��ȡ�ĵ�
//DEL 	CImageProcessingDoc* pDoc = GetDocument();
//DEL 
//DEL 	//  ���ͼ��CDib���ָ��
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 
//DEL 	// ���ͼ���ͷ�ļ���Ϣ
//DEL 	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
//DEL 
//DEL 	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ��ͼ��ģ����
//DEL 	if (lpBMIH->biBitCount != 8)
//DEL 	{
//DEL 		// ��ʾ�û�
//DEL 		MessageBox("Ŀǰֻ֧��256ɫλͼ��ͼ��ģ����", "ϵͳ��ʾ" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 						
//DEL 		// ����
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	::DIBNoRestriction(pDib);
//DEL 
//DEL 	// ��������
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 		
//DEL 	// ������ͼ
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL 
//DEL     // �ָ����
//DEL 	EndWaitCursor();
//DEL 	
//DEL }

//DEL void CImageProcessingView::OnRestoreInverse() 
//DEL {
//DEL 	// ͼ������˲�
//DEL 		
//DEL 	// ���Ĺ����״
//DEL 	BeginWaitCursor();
//DEL 
//DEL 	// ��ȡ�ĵ�
//DEL 	CImageProcessingDoc* pDoc = GetDocument();
//DEL 
//DEL 	//  ���ͼ��CDib���ָ��
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 
//DEL 	// ���ͼ���ͷ�ļ���Ϣ
//DEL 	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
//DEL 
//DEL 	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ�����˲���
//DEL 	if (lpBMIH->biBitCount != 8)
//DEL 	{
//DEL 		// ��ʾ�û�
//DEL 		MessageBox("Ŀǰֻ֧��256ɫλͼ�����˲���", "ϵͳ��ʾ" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 						
//DEL 		// ����
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	::DIBInverseFilter(pDib);
//DEL 
//DEL 	// ��������
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 		
//DEL 	// ������ͼ
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL 
//DEL     // �ָ����
//DEL 	EndWaitCursor();
//DEL 	
//DEL }

//DEL void CImageProcessingView::OnDegenerationMotion() 
//DEL {
//DEL 	// ͼ����˶�ģ��
//DEL 		
//DEL 	// ���Ĺ����״
//DEL 	BeginWaitCursor();
//DEL 
//DEL 	// ��ȡ�ĵ�
//DEL 	CImageProcessingDoc* pDoc = GetDocument();
//DEL 
//DEL 	//  ���ͼ��CDib���ָ��
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 
//DEL 	// ���ͼ���ͷ�ļ���Ϣ
//DEL 	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
//DEL 
//DEL 	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ���˶�ģ����
//DEL 	if (lpBMIH->biBitCount != 8)
//DEL 	{
//DEL 		// ��ʾ�û�
//DEL 		MessageBox("Ŀǰֻ֧��256ɫλͼ���˶�ģ����", "ϵͳ��ʾ" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 						
//DEL 		// ����
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	::DIBMotionDegeneration(pDib);
//DEL 
//DEL 	// ��������
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 		
//DEL 	// ������ͼ
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL 
//DEL     // �ָ����
//DEL 	EndWaitCursor();
//DEL 	
//DEL }

//DEL void CImageProcessingView::OnRestoreMotion() 
//DEL {
//DEL 	// �˶�ģ��ͼ��ԭ
//DEL 		
//DEL 	// ���Ĺ����״
//DEL 	BeginWaitCursor();
//DEL 
//DEL 	// ��ȡ�ĵ�
//DEL 	CImageProcessingDoc* pDoc = GetDocument();
//DEL 
//DEL 	//  ���ͼ��CDib���ָ��
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 
//DEL 	// ���ͼ���ͷ�ļ���Ϣ
//DEL 	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
//DEL 
//DEL 	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ���˶�ģ����ԭ��
//DEL 	if (lpBMIH->biBitCount != 8)
//DEL 	{
//DEL 		// ��ʾ�û�
//DEL 		MessageBox("Ŀǰֻ֧��256ɫλͼ���˶�ģ����ԭ��", "ϵͳ��ʾ" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 						
//DEL 		// ����
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	::DIBMotionRestore(pDib);
//DEL 
//DEL 	// ��������
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 		
//DEL 	// ������ͼ
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL 
//DEL     // �ָ����
//DEL 	EndWaitCursor();
//DEL 	
//DEL }

//DEL void CImageProcessingView::OnDEGENERATIONWinner() 
//DEL {	
//DEL 	// ͼ��ļ���ģ��
//DEL 		
//DEL 	// ���Ĺ����״
//DEL 	BeginWaitCursor();
//DEL 
//DEL 	// ��ȡ�ĵ�
//DEL 	CImageProcessingDoc* pDoc = GetDocument();
//DEL 
//DEL 	//  ���ͼ��CDib���ָ��
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 
//DEL 	// ���ͼ���ͷ�ļ���Ϣ
//DEL 	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
//DEL 
//DEL 	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ�ļ���ģ����
//DEL 	if (lpBMIH->biBitCount != 8)
//DEL 	{
//DEL 		// ��ʾ�û�
//DEL 		MessageBox("Ŀǰֻ֧��256ɫλͼ�ļ���ģ����", "ϵͳ��ʾ" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 						
//DEL 		// ����
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	::DIBNoiseDegeneration(pDib);
//DEL 
//DEL 	// ��������
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 		
//DEL 	// ������ͼ
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL 
//DEL     // �ָ����
//DEL 	EndWaitCursor();
//DEL 	
//DEL }


//DEL void CImageProcessingView::OnRestoreWinner() 
//DEL {
//DEL 	// ͼ���ά���˲�
//DEL 		
//DEL 	// ���Ĺ����״
//DEL 	BeginWaitCursor();
//DEL 
//DEL 	// ��ȡ�ĵ�
//DEL 	CImageProcessingDoc* pDoc = GetDocument();
//DEL 
//DEL 	//  ���ͼ��CDib���ָ��
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 
//DEL 	// ���ͼ���ͷ�ļ���Ϣ
//DEL 	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
//DEL 
//DEL 	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ��ά���˲���
//DEL 	if (lpBMIH->biBitCount != 8)
//DEL 	{
//DEL 		// ��ʾ�û�
//DEL 		MessageBox("Ŀǰֻ֧��256ɫλͼ��ά���˲���", "ϵͳ��ʾ" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 						
//DEL 		// ����
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	::DIBWinnerFilter(pDib);
//DEL 
//DEL 	// ��������
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 		
//DEL 	// ������ͼ
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL 
//DEL     // �ָ����
//DEL 	EndWaitCursor();
//DEL 	
//DEL }

//DEL void CImageProcessingView::OnMoment() 
//DEL {
//DEL 	// ͼ���pq������
//DEL 
//DEL 	// ���Ĺ����״
//DEL 	BeginWaitCursor();
//DEL 
//DEL 	// ��ȡ�ĵ�
//DEL 	CImageProcessingDoc* pDoc = GetDocument();
//DEL 
//DEL 	//  ���ͼ��CDib���ָ��
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 
//DEL 	// ���ͼ���ͷ�ļ���Ϣ
//DEL 	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
//DEL 
//DEL 	// �ж��Ƿ���8-bppλͼ
//DEL 	if (lpBMIH->biBitCount != 8)
//DEL 	{
//DEL 		// ��ʾ�û�
//DEL 		MessageBox("Ŀǰֻ֧��256ɫλͼ��", "ϵͳ��ʾ" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 						
//DEL 		// ����
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	::DIBMOMENT(pDib);
//DEL 	
//DEL     // �ָ����
//DEL 	EndWaitCursor();
//DEL 	
//DEL }

//DEL void CImageProcessingView::OnBarycentermoment() 
//DEL {
//DEL 	// ͼ������ľ�
//DEL 
//DEL 	// ���Ĺ����״
//DEL 	BeginWaitCursor();
//DEL 
//DEL 	// ��ȡ�ĵ�
//DEL 	CImageProcessingDoc* pDoc = GetDocument();
//DEL 
//DEL 	//  ���ͼ��CDib���ָ��
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 
//DEL 	// ���ͼ���ͷ�ļ���Ϣ
//DEL 	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
//DEL 
//DEL 	// �ж��Ƿ���8-bppλͼ
//DEL 	if (lpBMIH->biBitCount != 8)
//DEL 	{
//DEL 		// ��ʾ�û�
//DEL 		MessageBox("Ŀǰֻ֧��256ɫλͼ��", "ϵͳ��ʾ" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 						
//DEL 		// ����
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	::DIBBARYCENTERMOMENT(pDib);
//DEL 
//DEL 	// �ָ����
//DEL 	EndWaitCursor();
//DEL 	
//DEL }

//DEL void CImageProcessingView::OnAnalysisHolenum() 
//DEL {
//DEL 	// ��ȥ��ֵͼ����С����ֵ���������
//DEL 		
//DEL 	// ���Ĺ����״
//DEL 	BeginWaitCursor();
//DEL 
//DEL 	// ��ȡ�ĵ�
//DEL 	CImageProcessingDoc* pDoc = GetDocument();
//DEL 
//DEL 	//  ���ͼ��CDib���ָ��
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 
//DEL 	// ���ͼ���ͷ�ļ���Ϣ
//DEL 	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
//DEL 
//DEL 	// �ж��Ƿ���8-bppλͼ
//DEL 	if (lpBMIH->biBitCount != 8)
//DEL 	{
//DEL 		// ��ʾ�û�
//DEL 		MessageBox("Ŀǰֻ֧��256ɫλͼ��ͼ��", "ϵͳ��ʾ" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 						
//DEL 		// ����
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	::DIBHOLENUMBER(pDib);
//DEL 
//DEL 	// ��������
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 		
//DEL 	// ������ͼ
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL 
//DEL     // �ָ����
//DEL 	EndWaitCursor();
//DEL 	
//DEL }

//DEL void CImageProcessingView::OnStreetFramework() 
//DEL {
//DEL 	// ��������Ǽ���ȡ
//DEL 		
//DEL 	// ���Ĺ����״
//DEL 	BeginWaitCursor();
//DEL 
//DEL 	// ��ȡ�ĵ�
//DEL 	CImageProcessingDoc* pDoc = GetDocument();
//DEL 
//DEL 	//  ���ͼ��CDib���ָ��
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 
//DEL 	// ���ͼ���ͷ�ļ���Ϣ
//DEL 	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
//DEL 
//DEL 	// �ж��Ƿ���8-bppλͼ
//DEL 	if (lpBMIH->biBitCount != 8)
//DEL 	{
//DEL 		// ��ʾ�û�
//DEL 		MessageBox("Ŀǰֻ֧��256ɫλͼ��", "ϵͳ��ʾ" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 						
//DEL 		// ����
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	::DIBFREAMEWORK(pDib);
//DEL 
//DEL 	// ��������
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 		
//DEL 	// ������ͼ
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL 
//DEL     // �ָ����
//DEL 	EndWaitCursor();
//DEL 	
//DEL }

//DEL void CImageProcessingView::OnStreetTransform() 
//DEL {
//DEL 	// ��ֵͼ��Ľ�������任
//DEL 		
//DEL 	// ���Ĺ����״
//DEL 	BeginWaitCursor();
//DEL 
//DEL 	// ��ȡ�ĵ�
//DEL 	CImageProcessingDoc* pDoc = GetDocument();
//DEL 
//DEL 	//  ���ͼ��CDib���ָ��
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 
//DEL 	// ���ͼ���ͷ�ļ���Ϣ
//DEL 	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
//DEL 
//DEL 	// �ж��Ƿ���8-bppλͼ��
//DEL 	if (lpBMIH->biBitCount != 8)
//DEL 	{
//DEL 		// ��ʾ�û�
//DEL 		MessageBox("Ŀǰֻ֧��256ɫλͼ��", "ϵͳ��ʾ" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 						
//DEL 		// ����
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	::DIBSTREETDIS(pDib);
//DEL 
//DEL 	// ��������
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 		
//DEL 	// ������ͼ
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL 
//DEL     // �ָ����
//DEL 	EndWaitCursor();
//DEL 	
//DEL }

//DEL void CImageProcessingView::OnFrameRestore() 
//DEL {
//DEL 	// ��������Ǽܸ�ԭ
//DEL 		
//DEL 	// ���Ĺ����״
//DEL 	BeginWaitCursor();
//DEL 
//DEL 	// ��ȡ�ĵ�
//DEL 	CImageProcessingDoc* pDoc = GetDocument();
//DEL 
//DEL 	//  ���ͼ��CDib���ָ��
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 
//DEL 	// ���ͼ���ͷ�ļ���Ϣ
//DEL 	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
//DEL 
//DEL 	// �ж��Ƿ���8-bppλͼ
//DEL 	if (lpBMIH->biBitCount != 8)
//DEL 	{
//DEL 		// ��ʾ�û�
//DEL 		MessageBox("Ŀǰֻ֧��256ɫλͼ��", "ϵͳ��ʾ" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 						
//DEL 		// ����
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	::DIBCHESSBOARDDISRESTORE(pDib);
//DEL 
//DEL 	// ��������
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 		
//DEL 	// ������ͼ
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL 
//DEL     // �ָ����
//DEL 	EndWaitCursor();
//DEL 	
//DEL }


//DEL void CImageProcessingView::OnTrace() 
//DEL {
//DEL 	// ��ֵͼ��߽����
//DEL 
//DEL 	// ���Ĺ����״
//DEL 	BeginWaitCursor();
//DEL 
//DEL 	// ��ȡ�ĵ�
//DEL 	CImageProcessingDoc* pDoc = GetDocument();
//DEL 
//DEL 	//  ���ͼ��CDib���ָ��
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 
//DEL 	// ���ͼ���ͷ�ļ���Ϣ
//DEL 	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
//DEL 
//DEL 	// �ж��Ƿ���8-bppλͼ
//DEL 	if (lpBMIH->biBitCount != 8)
//DEL 	{
//DEL 		// ��ʾ�û�
//DEL 		MessageBox("Ŀǰֻ֧��256ɫλͼ��", "ϵͳ��ʾ" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 						
//DEL 		// ����
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	::DIBTrace(pDib);
//DEL 
//DEL 	// ��������
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 		
//DEL 	// ������ͼ
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL 
//DEL     // �ָ����
//DEL 	EndWaitCursor();
//DEL 	
//DEL 	
//DEL }

//DEL void CImageProcessingView::OnOutline() 
//DEL {
//DEL 	// ��ֵͼ��߽���ȡ
//DEL 
//DEL 	// ���Ĺ����״
//DEL 	BeginWaitCursor();
//DEL 
//DEL 	// ��ȡ�ĵ�
//DEL 	CImageProcessingDoc* pDoc = GetDocument();
//DEL 
//DEL 	//  ���ͼ��CDib���ָ��
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 
//DEL 	// ���ͼ���ͷ�ļ���Ϣ
//DEL 	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
//DEL 
//DEL 	// �ж��Ƿ���8-bppλͼ
//DEL 	if (lpBMIH->biBitCount != 8)
//DEL 	{
//DEL 		// ��ʾ�û�
//DEL 		MessageBox("Ŀǰֻ֧��256ɫλͼ��", "ϵͳ��ʾ" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 						
//DEL 		// ����
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	::DIBOUTLINE(pDib);
//DEL 
//DEL 	// ��������
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 		
//DEL 	// ������ͼ
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL 
//DEL     // �ָ����
//DEL 	EndWaitCursor();
//DEL 	
//DEL }

//DEL void CImageProcessingView::OnViewBayer() 
//DEL {
//DEL 	// Bayer��������ʾͼ��
//DEL 		
//DEL 	// ���Ĺ����״
//DEL 	BeginWaitCursor();
//DEL 
//DEL 	// ��ȡ�ĵ�
//DEL 	CImageProcessingDoc* pDoc = GetDocument();
//DEL 
//DEL 	//  ���ͼ��CDib���ָ��
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 
//DEL 	// ���ͼ���ͷ�ļ���Ϣ
//DEL 	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
//DEL 
//DEL 	// �ж��Ƿ���8-bppλͼ
//DEL 	if (lpBMIH->biBitCount != 8)
//DEL 	{
//DEL 		// ��ʾ�û�
//DEL 		MessageBox("Ŀǰֻ֧��256ɫλͼ��", "ϵͳ��ʾ" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 						
//DEL 		// ����
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	::LimbPatternBayer(pDib);
//DEL 
//DEL 	// ��������
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 		
//DEL 	// ������ͼ
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL 
//DEL     // �ָ����
//DEL 	EndWaitCursor();
//DEL 	
//DEL }

//DEL void CImageProcessingView::OnVIEWFloydSteinberg() 
//DEL {
//DEL 	// Floyd-Steinberg��������ʾͼ��
//DEL 		
//DEL 	// ���Ĺ����״
//DEL 	BeginWaitCursor();
//DEL 
//DEL 	// ��ȡ�ĵ�
//DEL 	CImageProcessingDoc* pDoc = GetDocument();
//DEL 
//DEL 	//  ���ͼ��CDib���ָ��
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 
//DEL 	// ���ͼ���ͷ�ļ���Ϣ
//DEL 	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
//DEL 
//DEL 	// �ж��Ƿ���8-bppλͼ
//DEL 	if (lpBMIH->biBitCount != 8)
//DEL 	{
//DEL 		// ��ʾ�û�
//DEL 		MessageBox("Ŀǰֻ֧��256ɫλͼ��", "ϵͳ��ʾ" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 						
//DEL 		// ����
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	::DitherFloydSteinberg(pDib);
//DEL 
//DEL 	// ��������
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 		
//DEL 	// ������ͼ
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL 
//DEL     // �ָ����
//DEL 	EndWaitCursor();
//DEL 	
//DEL }

//DEL void CImageProcessingView::OnCodingHuffman() 
//DEL {
//DEL 	// �����������
//DEL 	
//DEL 	// ��ȡ�ĵ�
//DEL 	CImageProcessingDoc * pDoc = GetDocument();
//DEL 	
//DEL 	// ָ��Դͼ�����ص�ָ��
//DEL 	unsigned char *	lpSrc;
//DEL 		
//DEL 	// ͼ��ĸ߶ȺͿ��
//DEL 	LONG	lHeight;
//DEL 	LONG	lWidth;
//DEL 	
//DEL 	// ͼ��ÿ�е��ֽ���
//DEL 	LONG	lLineBytes;
//DEL 	
//DEL 	// ͼ����������
//DEL 	LONG	lCountSum;
//DEL 	
//DEL 	// ѭ������
//DEL 	LONG	i;
//DEL 	LONG	j;
//DEL 	
//DEL 	// ����ָ��������������Ҷ�ֵ���ָ���
//DEL 	double * dProba;
//DEL 	
//DEL 	// ��ǰͼ����ɫ��Ŀ
//DEL 	int		nColorNum;
//DEL 
//DEL 	//  ���ͼ��CDib���ָ��
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 			
//DEL 	//ͼ�����ݵ�ָ��
//DEL 	LPBYTE  lpDIBBits = pDib->m_lpImage;
//DEL 
//DEL 	// ͷ�ļ���Ϣ
//DEL 	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
//DEL 
//DEL 	// �ж��Ƿ���8-bppλͼ��ֻ����8-bppλͼ�Ļ��������룩
//DEL 	if (lpBMIH->biBitCount != 8)
//DEL 	{
//DEL 		// ��ʾ�û�
//DEL 		MessageBox("Ŀǰֻ֧��256ɫλͼ�Ļ��������룡", "ϵͳ��ʾ" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 						
//DEL 		// ����
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	// ���Ĺ����״
//DEL 	BeginWaitCursor();
//DEL 	
//DEL 	/********************************************************************
//DEL 	   ��ʼ��������Ҷȼ����ֵĸ���	
//DEL 
//DEL 	   �����Ҫ��ָ�������н��й���������,
//DEL 	   ֻҪ����һ���ĳɸ������Ҷȼ����ʸ�ֵ����
//DEL 	**********************************************************************
//DEL 	*/
//DEL 	
//DEL 	//  ��ͷ�ļ���Ϣ�õ�ͼ��ı��������Ӷ��õ���ɫ��Ϣ
//DEL 	nColorNum = (int)pow(2,lpBMIH->biBitCount);
//DEL 
//DEL 	// �����ڴ�
//DEL 	dProba = new double[nColorNum];
//DEL 	
//DEL 	//�õ�ͼ��Ŀ�Ⱥ͸߶�
//DEL 	CSize   SizeDim;
//DEL 	SizeDim = pDib->GetDimensions();
//DEL 	lWidth = SizeDim.cx;
//DEL 	lHeight = SizeDim.cy;
//DEL 
//DEL 	// ����ͼ����������
//DEL 	lCountSum = lHeight * lWidth;
//DEL 	
//DEL 	//�õ�ʵ�ʵ�Dibͼ��洢��С
//DEL 	CSize   SizeRealDim;
//DEL 	SizeRealDim = pDib->GetDibSaveDim();
//DEL 
//DEL 	// ����ͼ��ÿ�е��ֽ���
//DEL 	lLineBytes = SizeRealDim.cx;
//DEL 	
//DEL 	// ����ֵ
//DEL 	for (i = 0; i < nColorNum; i ++)
//DEL 	{
//DEL 		dProba[i] = 0.0;
//DEL 	}
//DEL 	
//DEL 	// ��������Ҷ�ֵ�ļ���
//DEL 	for (i = 0; i < lHeight; i ++)
//DEL 	{
//DEL 		for (j = 0; j < lWidth; j ++)
//DEL 		{
//DEL 			// ָ��ͼ��ָ��
//DEL 			lpSrc = lpDIBBits + lLineBytes * i + j;
//DEL 			
//DEL 			// ������1
//DEL 			dProba[*(lpSrc)] = dProba[*(lpSrc)] + 1;
//DEL 		}
//DEL 	}
//DEL 		
//DEL 	
//DEL 	// ��������Ҷ�ֵ���ֵĸ���
//DEL 	for (i = 0; i < nColorNum; i ++)
//DEL 	{
//DEL 		dProba[i] = dProba[i] / (FLOAT)lCountSum;
//DEL 	}
//DEL 	
//DEL 	/***************************************************
//DEL 	 ������������������
//DEL 	 ���öԻ�����ʾ���������
//DEL 	****************************************************/
//DEL 	
//DEL 	// �����Ի���
//DEL 	CDlgHuffman dlgCoding;
//DEL 	
//DEL 	// ��ʼ������ֵ
//DEL 	dlgCoding.dProba = dProba;
//DEL 	dlgCoding.nColorNum = nColorNum;
//DEL 	
//DEL 	// ��ʾ�Ի���
//DEL 	dlgCoding.DoModal();
//DEL 		
//DEL 	// �ָ����
//DEL 	EndWaitCursor();
//DEL 	
//DEL }

//DEL void CImageProcessingView::OnCodingShanfino() 
//DEL {
//DEL 	// ��ũ����ŵ�����
//DEL 	
//DEL 	// ��ȡ�ĵ�
//DEL 	CImageProcessingDoc * pDoc = GetDocument();
//DEL 	
//DEL 	// ָ��Դͼ�����ص�ָ��
//DEL 	unsigned char *	lpSrc;
//DEL 		
//DEL 	// ͼ��ĸ߶�
//DEL 	LONG	lHeight;
//DEL 	LONG	lWidth;
//DEL 	
//DEL 	// ͼ��ÿ�е��ֽ���
//DEL 	LONG	lLineBytes;
//DEL 	
//DEL 	// ��ȡ��ǰDIB��ɫ��Ŀ
//DEL 	int		nColorNum;
//DEL 	
//DEL 	// ͼ����������
//DEL 	LONG	lCountSum;
//DEL 	
//DEL 	// ѭ������
//DEL 	LONG	i;
//DEL 	LONG	j;
//DEL 	
//DEL 	// ��������Ҷ�ֵ���ָ��ʵ�����ָ��
//DEL 	double * dProba;	
//DEL 
//DEL 	//  ���ͼ��CDib���ָ��
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 			
//DEL 	//ͼ�����ݵ�ָ��
//DEL 	LPBYTE  lpDIBBits = pDib->m_lpImage;
//DEL 
//DEL 	// ͷ�ļ���Ϣ
//DEL 	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
//DEL 
//DEL 	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ����ũ����ŵ���룩
//DEL 	if (lpBMIH->biBitCount != 8)
//DEL 	{
//DEL 		// ��ʾ�û�
//DEL 		MessageBox("Ŀǰֻ֧��256ɫλͼ����ũ����ŵ���룡", "ϵͳ��ʾ" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 						
//DEL 		// ����
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	// ���Ĺ����״
//DEL 	BeginWaitCursor();
//DEL 	
//DEL 	/******************************************************************************
//DEL 	// ��ʼ��������Ҷȼ����ֵĸ���	
//DEL 	//
//DEL 	// �����Ҫ��ָ�������н�����ũ����ŵ����,
//DEL 	//ֻҪ����һ���ĳɸ������Ҷȼ����ʸ�ֵ����
//DEL 	*****************************************************************************
//DEL 	*/
//DEL 	//  �Ҷ�ֵ�����ļ���
//DEL 	nColorNum = (int)pow(2,lpBMIH->biBitCount);
//DEL 
//DEL 	// �����ڴ�
//DEL 	dProba = new double[nColorNum];
//DEL 	
//DEL 	//�õ�ͼ��Ŀ�Ⱥ͸߶�
//DEL 	CSize   SizeDim;
//DEL 	SizeDim = pDib->GetDimensions();
//DEL 	lWidth = SizeDim.cx;
//DEL 	lHeight = SizeDim.cy;
//DEL 
//DEL 	// ����ͼ����������
//DEL 	lCountSum = lHeight * lWidth;
//DEL 	
//DEL 	//�õ�ʵ�ʵ�Dibͼ��洢��С
//DEL 	CSize   SizeRealDim;
//DEL 	SizeRealDim = pDib->GetDibSaveDim();
//DEL 
//DEL 	// ����ͼ��ÿ�е��ֽ���
//DEL 	lLineBytes = SizeRealDim.cx;
//DEL 
//DEL 	// ����ͼ����������
//DEL 	lCountSum = lHeight * lWidth;
//DEL 	
//DEL 	// ���ü���Ϊ0
//DEL 	for (i = 0; i < nColorNum; i ++)
//DEL 	{
//DEL 		dProba[i] = 0.0;
//DEL 	}
//DEL 	
//DEL 	// ��������Ҷ�ֵ�ļ��������ڷ�256ɫλͼ���˴�������dProba��ֵ��������ͬ��
//DEL 	for (i = 0; i < lHeight; i ++)
//DEL 	{
//DEL 		for (j = 0; j < lWidth; j ++)
//DEL 		{
//DEL 			// ָ��ͼ��ָ��
//DEL 			lpSrc = lpDIBBits + lLineBytes * i + j;
//DEL 			
//DEL 			// ������1
//DEL 			dProba[*(lpSrc)] = dProba[*(lpSrc)]+ 1;
//DEL 		}
//DEL 	}
//DEL 	
//DEL 	
//DEL 	// ��������Ҷ�ֵ���ֵĸ���
//DEL 	for (i = 0; i < nColorNum; i ++)
//DEL 	{
//DEL 		dProba[i] /= (double)lCountSum;
//DEL 	}
//DEL 	
//DEL 	/***************************************************
//DEL 	 ������ũ����ŵ��������
//DEL 	 ���öԻ�����ʾ��ũ����ŵ���
//DEL 	****************************************************/	
//DEL 	
//DEL 	// �����Ի���
//DEL 	CDlgShannon dlgPara;
//DEL 	
//DEL 	// ��ʼ������ֵ
//DEL 	dlgPara.m_dProba = dProba;
//DEL 	dlgPara.m_nColorNum = nColorNum;
//DEL 	
//DEL 	// ��ʾ�Ի���
//DEL 	dlgPara.DoModal();
//DEL 
//DEL 	//�ͷ��ڴ�
//DEL 	delete dProba;
//DEL 		
//DEL 	// �ָ����
//DEL 	EndWaitCursor();
//DEL }

//DEL void CImageProcessingView::OnCodingArith() 
//DEL {
//DEL 	CDlgArith dlgCoding;
//DEL 	
//DEL 	// ��ʾ�Ի���
//DEL 	dlgCoding.DoModal();
//DEL 	
//DEL }

//DEL void CImageProcessingView::OnCodingBitplane() 
//DEL {
//DEL 	// �����Ի���
//DEL 	CDlgBitPlane dlgCoding;	
//DEL 	
//DEL 	// ��ʾ�Ի���
//DEL 	dlgCoding.DoModal();	
//DEL 	
//DEL 	BYTE bBitNum = dlgCoding.m_BItNumber;
//DEL 
//DEL 	// ��ȡ�ĵ�
//DEL 	CImageProcessingDoc * pDoc = GetDocument();
//DEL 		
//DEL 	//  ���ͼ��CDib���ָ��
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 	
//DEL 	// ͷ�ļ���Ϣ
//DEL 	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
//DEL 
//DEL 	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ��λƽ��ֽ⣩
//DEL 	if (lpBMIH->biBitCount != 8)
//DEL 	{
//DEL 		// ��ʾ�û�
//DEL 		MessageBox("Ŀǰֻ֧��256ɫλͼ��λƽ��ֽ⣡", "ϵͳ��ʾ" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 						
//DEL 		// ����
//DEL 		return;
//DEL 	}
//DEL 	DIBBITPLANE(pDib,bBitNum);
//DEL 
//DEL 	// ��������
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 		
//DEL 	// ������ͼ
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL 	
//DEL }

//DEL void CImageProcessingView::OnCodingWriteimg() 
//DEL {
//DEL 	// �Ե�ǰͼ�����DPCM���루��ΪIMG��ʽ�ļ���
//DEL 	
//DEL 	// ��ȡ�ĵ�
//DEL 	CImageProcessingDoc * pDoc = GetDocument();
//DEL 		
//DEL 	//  ���ͼ��CDib���ָ��
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 			
//DEL 	//ͼ�����ݵ�ָ��
//DEL 	LPBYTE  lpDIBBits = pDib->m_lpImage;
//DEL 
//DEL 	// ͷ�ļ���Ϣ
//DEL 	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
//DEL 
//DEL 	// �ж��Ƿ���8-bppλͼ������8-bppλͼ��DPCM���룩
//DEL 	if (lpBMIH->biBitCount != 8)
//DEL 	{
//DEL 		// ��ʾ�û�
//DEL 		MessageBox("Ŀǰֻ֧��256ɫλͼ��DPCM���룡", "ϵͳ��ʾ" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 						
//DEL 		// ����
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	// ���Ĺ����״
//DEL 	BeginWaitCursor();
//DEL 	
//DEL 	// �ļ�����·��
//DEL 	CString strFilePath;
//DEL 	
//DEL 	// ��ȡԭʼ�ļ���
//DEL 	strFilePath = pDoc->GetPathName();
//DEL 	
//DEL 	// ���ĺ�׺ΪIMG
//DEL 	if (strFilePath.Right(4).CompareNoCase(".BMP") == 0)
//DEL 	{	
//DEL 		strFilePath = strFilePath.Left(strFilePath.GetLength()-3) + "IMG";
//DEL 	}
//DEL 	else
//DEL 	{
//DEL 		strFilePath += ".IMG";
//DEL 	}
//DEL 
//DEL 	// ����SaveAs�Ի���
//DEL 	CFileDialog dlg(FALSE, "IMG", strFilePath, 
//DEL 					OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
//DEL 		            "IMGͼ���ļ� (*.IMG) | *.IMG|�����ļ� (*.*) | *.*||", NULL);
//DEL 	
//DEL 	// ��ʾ�û�ѡ�񱣴��·��
//DEL 	if (dlg.DoModal() != IDOK)
//DEL 	{
//DEL 		// �ָ����
//DEL 		EndWaitCursor();
//DEL 	
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	// ��ȡ�û�ָ�����ļ�·��
//DEL 	strFilePath = dlg.GetPathName();
//DEL 	
//DEL 	// CFile��CFileException����
//DEL 	CFile file;
//DEL 	CFileException fe;
//DEL 	
//DEL 	// ���Դ���ָ����IMG�ļ�
//DEL 	if (!file.Open(strFilePath, CFile::modeCreate |
//DEL 	  CFile::modeReadWrite | CFile::shareExclusive, &fe))
//DEL 	{
//DEL 		MessageBox("��ָ��IMG�ļ�ʱʧ�ܣ�", "ϵͳ��ʾ" , 
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 	
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	// ����WRITE2IMG()��������ǰ��DIB����ΪIMG�ļ�
//DEL 	if (::WRITE2IMG(pDib, file))
//DEL 	{
//DEL 		MessageBox("�ɹ�����ΪIMG�ļ���", "ϵͳ��ʾ" , 
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 	}
//DEL 	else
//DEL 	{
//DEL 		MessageBox("����ΪIMG�ļ�ʧ�ܣ�", "ϵͳ��ʾ" , 
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 	}	
//DEL 		
//DEL 	// �ָ����
//DEL 	EndWaitCursor();
//DEL 	
//DEL }


//DEL void CImageProcessingView::OnCodingLoadimg() 
//DEL {
//DEL 	// ����IMG�ļ�
//DEL 	
//DEL 	// ��ȡ�ĵ�
//DEL 	CImageProcessingDoc * pDoc = GetDocument();
//DEL 		
//DEL 	//  ���ͼ��CDib���ָ��
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 
//DEL 	// �ļ�·��
//DEL 	CString strFilePath;
//DEL 	
//DEL 	// ����Open�Ի���
//DEL 	CFileDialog dlg(TRUE, "PCX", NULL,
//DEL 					OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
//DEL 					"IMGͼ���ļ� (*.PCX) | *.IMG|�����ļ� (*.*) | *.*||", NULL);
//DEL 	
//DEL 	// ��ʾ�û�ѡ�񱣴��·��
//DEL 	if (dlg.DoModal() != IDOK)
//DEL 	{
//DEL 		// ����
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	// ��ȡ�û�ָ�����ļ�·��
//DEL 	strFilePath = dlg.GetPathName();
//DEL 	
//DEL 	// CFile��CFileException����
//DEL 	CFile file;
//DEL 	CFileException fe;
//DEL 	
//DEL 	// ���Դ�ָ����PCX�ļ�
//DEL 	if (!file.Open(strFilePath, CFile::modeRead | CFile::shareDenyWrite, &fe))
//DEL 	{
//DEL 		// ��ʾ�û�
//DEL 		MessageBox("��ָ��PCX�ļ�ʱʧ�ܣ�", "ϵͳ��ʾ" , 
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 		
//DEL 		// ����
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	// ���Ĺ����״
//DEL 	BeginWaitCursor();
//DEL 
//DEL 	// ����LOADIMG()������ȡָ����IMG�ļ�
//DEL 	BOOL Succ = LOADIMG(pDib, file);
//DEL 
//DEL 	if (Succ == TRUE)
//DEL 	{
//DEL 		// ��ʾ�û�
//DEL 		MessageBox("�ɹ���ȡIMG�ļ���", "ϵͳ��ʾ" , 
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 		
//DEL 	}
//DEL 	else
//DEL 	{
//DEL 		// ��ʾ�û�
//DEL 		MessageBox("��ȡIMG�ļ�ʧ�ܣ�", "ϵͳ��ʾ" , 
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 	}
//DEL 
//DEL 	// ������ͼ
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL 	
//DEL 	// �ָ����
//DEL 	EndWaitCursor();
//DEL 	
//DEL }

