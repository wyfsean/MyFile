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
	// 为小波变换设置的参数
	// 临时存放小波变换系数内存
	m_pDbImage = NULL;	
	
	// 设置当前层数
	m_nDWTCurDepth = 0;

	// 设置小波基紧支集长度
	m_nSupp = 1;
}

CImageProcessingView::~CImageProcessingView()
{
	// 释放已分配内存
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
 * \函数名称：
 *   OnFft2d()
 *
 * \输入参数:
 *   无
 * 
 * \返回值:
 *   无
 *
 * \说明:
 *   运行二维快速傅立叶变换
 *
 *************************************************************************
 */
//DEL void CImageProcessingView::OnFft2d() 
//DEL {
//DEL 	//图象FFT变换
//DEL 
//DEL 	// 更改光标形状
//DEL 	BeginWaitCursor();
//DEL 	
//DEL 	// 循环控制变量
//DEL 	int y;
//DEL 	int x;
//DEL 	
//DEL 	// 获得Doc类的指针
//DEL 	CImageProcessingDoc * pDoc = (CImageProcessingDoc *)this->GetDocument();
//DEL 
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 
//DEL 	// 获得图象的头文件信息
//DEL 	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
//DEL 
//DEL 	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的离散傅立叶变换）
//DEL 	if (lpBMIH->biBitCount != 8)
//DEL 	{
//DEL 		// 提示用户
//DEL 		MessageBox("目前只支持256色位图的离散傅立叶变换！", "系统提示" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 						
//DEL 		// 返回
//DEL 		return;
//DEL 	}
//DEL 
//DEL 	// 图象的宽长
//DEL 	CSize sizeImage ;
//DEL 	int nWidth ;
//DEL 	int nHeight;
//DEL 
//DEL 	// 获得图象的宽长
//DEL 	sizeImage = pDib->GetDimensions() ;
//DEL 		
//DEL 	nWidth = sizeImage.cx;
//DEL 	nHeight= sizeImage.cy;
//DEL 
//DEL 	// 临时变量
//DEL 	double	dTmpOne;
//DEL 	double  dTmpTwo;
//DEL 	
//DEL 	// 傅立叶变换竖直方向点数
//DEL 	int nTransHeight ;
//DEL 
//DEL 	// 傅立叶变换水平方向点数
//DEL 	int nTransWidth  ;	
//DEL 	
//DEL 	// 计算进行傅立叶变换的点数	（2的整数次幂）
//DEL 	dTmpOne = log(nWidth)/log(2);
//DEL 	dTmpTwo = ceil(dTmpOne)		   ;
//DEL 	dTmpTwo = pow(2,dTmpTwo)	   ;
//DEL 	nTransWidth = (int) dTmpTwo	   ;
//DEL 	
//DEL 	// 计算进行傅立叶变换的点数 （2的整数次幂）
//DEL 	dTmpOne = log(nHeight)/log(2);
//DEL 	dTmpTwo = ceil(dTmpOne)		   ;
//DEL 	dTmpTwo = pow(2,dTmpTwo)	   ;
//DEL 	nTransHeight = (int) dTmpTwo   ;
//DEL 
//DEL 	// 计算图象数据存储每行需要的字节数
//DEL 	// BMP文件的每行数据存储是DWORD对齐的
//DEL 	int		nSaveWidth;
//DEL 	nSaveWidth = ( (nWidth << 3) + 31)/32 * 4 ;
//DEL 
//DEL 	// 指向图象数据的指针
//DEL 	LPBYTE lpImage ;
//DEL 	lpImage = pDib->m_lpImage ;
//DEL 
//DEL 	// 图象象素值
//DEL 	unsigned char unchValue;
//DEL 
//DEL 	
//DEL 	// 指向时域数据的指针
//DEL 	complex<double> * pCTData ;
//DEL 
//DEL 	// 指向频域数据的指针
//DEL 	complex<double> * pCFData ;
//DEL 
//DEL 	// 分配内存
//DEL 	pCTData=new complex<double>[nTransWidth * nTransHeight];
//DEL 	pCFData=new complex<double>[nTransWidth * nTransHeight];
//DEL 
//DEL 	// 初始化
//DEL 	// 图象数据的宽和高不一定是2的整数次幂，所以pCTData
//DEL 	// 有一部分数据需要补0
//DEL 	for(y=0; y<nTransHeight; y++)
//DEL 	{
//DEL 		for(x=0; x<nTransWidth; x++)
//DEL 		{
//DEL 			pCTData[y*nTransWidth + x]=complex<double>(0,0);
//DEL 		}
//DEL 	}
//DEL 
//DEL 	// 把图象数据传给pCTData
//DEL 	for(y=0; y<nHeight; y++)
//DEL 	{
//DEL 		for(x=0; x<nWidth; x++)
//DEL 		{
//DEL 			unchValue = lpImage[y*nSaveWidth +x];
//DEL 			pCTData[y*nTransWidth + x]=complex<double>(unchValue,0);
//DEL 		}
//DEL 	}
//DEL 
//DEL 	// 傅立叶正变换
//DEL 	DIBFFT_2D(pCTData, nWidth, nHeight, pCFData) ;
//DEL 	
//DEL 	// 临时变量
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
//DEL 			// 为了显示，需要对幅度的大小进行伸缩
//DEL 			dTmp /= 100        ;
//DEL 
//DEL 			// 限制图象数据的大小
//DEL 			dTmp = min(dTmp, 255) ;
//DEL 
//DEL 			lpImage[y*nSaveWidth +x] = (unsigned char)(int)dTmp;
//DEL 		}
//DEL 	}
//DEL 
//DEL 	// 为了在屏幕上显示，我们把幅度值大的部分用黑色显示
//DEL 	for(y=0; y<nHeight; y++)
//DEL 	{
//DEL 		for(x=0; x<nWidth; x++)
//DEL 		{
//DEL 			lpImage[y*nSaveWidth +x] = 255 - lpImage[y*nSaveWidth +x];
//DEL 		}
//DEL 	}
//DEL 	
//DEL 	// 刷新屏幕
//DEL 	Invalidate();
//DEL 
//DEL 	// 释放内存
//DEL 	delete pCTData;
//DEL 	delete pCFData;
//DEL 	pCTData = NULL;
//DEL 	pCFData = NULL;
//DEL 
//DEL 	// 设置脏标记
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 		
//DEL 	// 更新视图
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL 
//DEL 	//  恢复光标形状
//DEL 	EndWaitCursor();
//DEL }


/*************************************************************************
 *
 * \函数名称：
 *   OnDft2d()
 *
 * \输入参数:
 *   无
 * 
 * \返回值:
 *   无
 *
 * \说明:
 *   运行二维傅立叶变换
 *
 *************************************************************************
 */
//DEL void CImageProcessingView::OnDft2d() 
//DEL {
//DEL 	//图象离散傅立叶变换
//DEL 	
//DEL 	//提示用户，直接进行离散傅立叶变换的时间很长
//DEL 	MessageBox("没有使用FFT，时间可能很长！", "作者提示" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 	
//DEL 	//更改光标形状
//DEL 	BeginWaitCursor(); 
//DEL 
//DEL 	// 循环控制变量
//DEL 	int y;
//DEL 	int x;
//DEL 	
//DEL 	CImageProcessingDoc * pDoc = (CImageProcessingDoc *)this->GetDocument();
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 
//DEL 	// 获得图象的头文件信息
//DEL 	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
//DEL 
//DEL 	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的离散傅立叶变换）
//DEL 	if (lpBMIH->biBitCount != 8)
//DEL 	{
//DEL 		// 提示用户
//DEL 		MessageBox("目前只支持256色位图的离散傅立叶变换！", "系统提示" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 						
//DEL 		// 返回
//DEL 		return;
//DEL 	}
//DEL 
//DEL 	//图象的长宽大小
//DEL 	CSize sizeImage		= pDib->GetDimensions();
//DEL 	int nWidth			= sizeImage.cx		;
//DEL 	int nHeight			= sizeImage.cy		;
//DEL 
//DEL 	// 计算图象数据存储每行需要的字节数
//DEL 	// BMP文件的每行数据存储是DWORD对齐的
//DEL 	int		nSaveWidth;
//DEL 	nSaveWidth = ( (nWidth << 3) + 31)/32 * 4 ;
//DEL 
//DEL 	// 指向图象数据的指针
//DEL 	LPBYTE lpImage ;
//DEL 	lpImage = pDib->m_lpImage ;
//DEL 
//DEL 	double * pTrRstRpart  = new double [nWidth*nHeight];
//DEL 	double * pTrRstIpart  = new double [nWidth*nHeight];	
//DEL 	
//DEL 	::DIBDFT_2D(pDib, pTrRstRpart,pTrRstIpart);
//DEL 
//DEL 	// 临时变量
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
//DEL 			// 为了显示，需要对幅度的大小进行伸缩
//DEL 			dTmp /= 100        ;
//DEL 
//DEL 			// 限制图象数据的大小
//DEL 			dTmp = min(dTmp, 255) ;
//DEL 
//DEL 			lpImage[y*nSaveWidth +x] = (unsigned char)(int)dTmp;
//DEL 		}
//DEL 	}
//DEL 
//DEL 	// 为了在屏幕上显示，我们把幅度值大的部分用黑色显示
//DEL 	for(y=0; y<nHeight; y++)
//DEL 	{
//DEL 		for(x=0; x<nWidth; x++)
//DEL 		{
//DEL 			lpImage[y*nSaveWidth +x] = 255 - lpImage[y*nSaveWidth +x];
//DEL 		}
//DEL 	}
//DEL 
//DEL 	// 释放内存
//DEL 	delete pTrRstRpart;
//DEL 	pTrRstRpart=NULL  ;
//DEL 	
//DEL 	delete pTrRstIpart;
//DEL 	pTrRstIpart=NULL  ;
//DEL 
//DEL 	// 设置脏标记
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 		
//DEL 	// 更新视图
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL 
//DEL 	// 恢复光标形状
//DEL 	EndWaitCursor(); 
//DEL 	
//DEL 	// 刷新屏幕
//DEL 	Invalidate();
//DEL }

//DEL void CImageProcessingView::OnFreqDct() 
//DEL {
//DEL 	// 图象的离散余弦变换
//DEL 		
//DEL 	// 更改光标形状
//DEL 	BeginWaitCursor();
//DEL 
//DEL 	// 获取文档
//DEL 	CImageProcessingDoc* pDoc = GetDocument();
//DEL 
//DEL 	//  获得图象CDib类的指针
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 
//DEL 	// 获得图象的头文件信息
//DEL 	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
//DEL 
//DEL 	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的离散余弦变换）
//DEL 	if (lpBMIH->biBitCount != 8)
//DEL 	{
//DEL 		// 提示用户
//DEL 		MessageBox("目前只支持256色位图的离散余弦变换！", "系统提示" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 						
//DEL 		// 返回
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	::DIBDct(pDib);
//DEL 	
//DEL 	// 设置脏标记
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 		
//DEL 	// 更新视图
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL 
//DEL     // 恢复光标
//DEL 	EndWaitCursor();
//DEL 	
//DEL }

//DEL void CImageProcessingView::OnFreqHotelling() 
//DEL {
//DEL 	// 图象霍特林变换
//DEL 	
//DEL 	// 更改光标形状
//DEL 	BeginWaitCursor();
//DEL 
//DEL 	// 获取文档
//DEL 	CImageProcessingDoc* pDoc = GetDocument();
//DEL 
//DEL 	//  获得图象CDib类的指针
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 
//DEL 	// 获得图象的头文件信息
//DEL 	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
//DEL 
//DEL 	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的离散霍特林变换）
//DEL 	if (lpBMIH->biBitCount != 8)
//DEL 	{
//DEL 		// 提示用户
//DEL 		MessageBox("目前只支持256色位图的离散霍特林变换！", "系统提示" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 						
//DEL 		// 返回
//DEL 		return;
//DEL 	}
//DEL 
//DEL 	//  图象的霍特林变换
//DEL 	DIBHOTELLING(pDib);
//DEL 	
//DEL 	// 设置脏标记
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 		
//DEL 	// 更新视图
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL 
//DEL     // 恢复光标
//DEL 	EndWaitCursor();
//DEL 	
//DEL }

//DEL void CImageProcessingView::OnFreqWalsh() 
//DEL {
//DEL 	// 图象的沃尔什－哈达玛变换
//DEL 	
//DEL 	// 更改光标形状
//DEL 	BeginWaitCursor();
//DEL 
//DEL 	// 获取文档
//DEL 	CImageProcessingDoc* pDoc = GetDocument();
//DEL 
//DEL 	//  获得图象CDib类的指针
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 
//DEL 	// 获得图象的头文件信息
//DEL 	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
//DEL 
//DEL 	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的沃尔什－哈达玛变换）
//DEL 	if (lpBMIH->biBitCount != 8)
//DEL 	{
//DEL 		// 提示用户
//DEL 		MessageBox("目前只支持256色位图的离散沃尔什变换！", "系统提示" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 						
//DEL 		// 返回
//DEL 		return;
//DEL 	}
//DEL 
//DEL 	//  进行沃尔什－哈达玛变换
//DEL 	DIBWalsh(pDib);
//DEL 	
//DEL 	// 设置脏标记
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 		
//DEL 	// 更新视图
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL 
//DEL     // 恢复光标
//DEL 	EndWaitCursor();
//DEL 	
//DEL }

/*************************************************************************
 *
 * \函数名称：
 *   OnViewHistogram()
 *
 * \输入参数:
 *   无
 * 
 * \返回值:
 *   无
 *
 * \说明:
 *   查看直方图，弹出直方图显示界面
 *
 *************************************************************************
 */
//DEL void CImageProcessingView::OnViewHistogram() 
//DEL {
//DEL 	// 获取文档
//DEL 	CImageProcessingDoc* pDoc = GetDocument();
//DEL 
//DEL 	// DIB的颜色数目
//DEL 	int nColorTableEntries;
//DEL 	nColorTableEntries = pDoc->m_pDibInit->m_nColorTableEntries;
//DEL 		
//DEL 	// 判断是否是8bpp位图（这里只处理8bpp位图）
//DEL 	if ( nColorTableEntries != 256)
//DEL 	{
//DEL 		// 提示用户，不再进行处理
//DEL 		MessageBox("目前只支持查看256色位图灰度直方图！", "系统提示" , MB_ICONINFORMATION | MB_OK);
//DEL 		
//DEL 		// 返回
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	// 更改光标形状
//DEL 	BeginWaitCursor();
//DEL 	
//DEL 	// 创建对话框
//DEL 	CDlgHistShow dlgHistShow;
//DEL 	
//DEL 	// 初始化变量值
//DEL 	dlgHistShow.m_pDib = pDoc->m_pDibInit;
//DEL 		
//DEL 	// 显示对话框
//DEL 	if (dlgHistShow.DoModal() != IDOK)
//DEL 	{
//DEL 		// 返回
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	// 恢复光标
//DEL 	EndWaitCursor();	
//DEL }

/*************************************************************************
 *
 * \函数名称：
 *   OnEnhanceSmooth()
 *
 * \输入参数:
 *   无
 * 
 * \返回值:
 *   无
 *
 * \说明:
 *   对图象进行平滑处理，并弹出平滑模板设置对话框
 *
 *************************************************************************
 */
//DEL void CImageProcessingView::OnEnhanceSmooth() 
//DEL {
//DEL 	// TODO: Add your command handler code here
//DEL 		// 图像平滑
//DEL 	
//DEL 	// 获取文档
//DEL 	CImageProcessingDoc* pDoc = GetDocument();
//DEL 	
//DEL 	// 模板高度
//DEL 	int		nTempHeight;
//DEL 	
//DEL 	// 模板宽度
//DEL 	int		nTempWidth;
//DEL 	
//DEL 	// 模板系数
//DEL 	double		dbTempCoef;
//DEL 	
//DEL 	// 模板中心元素X坐标
//DEL 	int		nTempCenX;
//DEL 	
//DEL 	// 模板中心元素Y坐标
//DEL 	int		nTempCenY;
//DEL 	
//DEL 	// 模板元素数组
//DEL 	double	pdbTemp[25];
//DEL 	
//DEL 	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的平滑，其它的可以类推）
//DEL 	if(pDoc->m_pDibInit->m_nColorTableEntries != 256)
//DEL 	//if (::DIBNumColors(lpDIB) != 256)
//DEL 	{
//DEL 		// 提示用户
//DEL 		MessageBox("目前只支持256色位图的平滑！", "系统提示" , 
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 		
//DEL 		// 返回
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	// 创建对话框
//DEL 	CDlgSmooth dlgSmth;
//DEL 	
//DEL 	// 给模板数组赋初值（为平均模板）
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
//DEL 	// 初始化对话框变量值
//DEL 	dlgSmth.m_nTemType  = 0;
//DEL 	dlgSmth.m_nSmthTemHeight  = 3;
//DEL 	dlgSmth.m_nSmthTemWidth  = 3;
//DEL 	dlgSmth.m_nSmthTemCenX = 1;
//DEL 	dlgSmth.m_nSmthTemCenY = 1;
//DEL 	dlgSmth.m_dbSmthTemCoef  = (double) (1.0 / 9.0);
//DEL 	dlgSmth.m_pdbTemp = pdbTemp;
//DEL 	
//DEL 	// 显示对话框，提示用户设定平移量
//DEL 	if (dlgSmth.DoModal() != IDOK)
//DEL 	{
//DEL 		// 返回
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	// 获取用户设定的平移量
//DEL 	nTempHeight   = dlgSmth.m_nSmthTemHeight;
//DEL 	nTempWidth  = dlgSmth.m_nSmthTemWidth;
//DEL 	nTempCenX = dlgSmth.m_nSmthTemCenX;
//DEL 	nTempCenY = dlgSmth.m_nSmthTemCenY;
//DEL 	dbTempCoef  = dlgSmth.m_dbSmthTemCoef;
//DEL 	
//DEL 	// 删除对话框
//DEL 	delete dlgSmth;	
//DEL 	
//DEL 	// 更改光标形状
//DEL 	BeginWaitCursor();
//DEL 	
//DEL 	// 调用Template()函数平滑DIB
//DEL 	if (GeneralTemplate(pDoc->m_pDibInit, nTempWidth, nTempHeight, 
//DEL 		nTempCenX, nTempCenY, pdbTemp, dbTempCoef))
//DEL 	{		
//DEL 		// 设置脏标记
//DEL 		pDoc->SetModifiedFlag(TRUE);
//DEL 
//DEL 		// 更新视图
//DEL 		pDoc->UpdateAllViews(NULL);
//DEL 	}
//DEL 	else
//DEL 	{
//DEL 		// 提示用户
//DEL 		MessageBox("分配内存失败！", "系统提示" , MB_ICONINFORMATION | MB_OK);
//DEL 	}
//DEL 	
//DEL 	// 恢复光标
//DEL 	EndWaitCursor();	
//DEL 	
//DEL }

/*************************************************************************
 *
 * \函数名称：
 *   OnEnhanceMedian()
 *
 * \输入参数:
 *   无
 * 
 * \返回值:
 *   无
 *
 * \说明:
 *   对图象进行中值滤波，并弹出平滑模板设置对话框
 *
 *************************************************************************
 */
//DEL void CImageProcessingView::OnEnhanceMedian() 
//DEL {
//DEL 	// 中值滤波
//DEL 	
//DEL 	// 获取文档
//DEL 	CImageProcessingDoc* pDoc = GetDocument();
//DEL 	
//DEL 	// 滤波器的高度
//DEL 	int nFilterHeight;
//DEL 	
//DEL 	// 滤波器的宽度
//DEL 	int nFilterWidth;
//DEL 	
//DEL 	// 中心元素的X坐标
//DEL 	int nFilterCenX;
//DEL 	
//DEL 	// 中心元素的Y坐标
//DEL 	int nFilterCenY;
//DEL 	
//DEL 	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的平滑，其它的可以类推）
//DEL 	if(pDoc->m_pDibInit->m_nColorTableEntries != 256)
//DEL 	{
//DEL 		// 提示用户
//DEL 		MessageBox("目前只支持256色位图的平滑！", "系统提示" , 
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 		
//DEL 		// 返回
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	
//DEL 	// 创建对话框
//DEL 	CDlgMedian dlgMedian;
//DEL 	
//DEL 	// 初始化变量值
//DEL 	dlgMedian.m_nFilterType = 0;
//DEL 	dlgMedian.m_nFilterHeight = 3;
//DEL 	dlgMedian.m_nFilterWidth = 1;
//DEL 	dlgMedian.m_nFilterCenX = 0;
//DEL 	dlgMedian.m_nFilterCenY = 1;
//DEL 	
//DEL 	// 显示对话框，提示用户设定平移量
//DEL 	if (dlgMedian.DoModal() != IDOK)
//DEL 	{
//DEL 		// 返回
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	// 获取用户的设定
//DEL 	nFilterHeight = dlgMedian.m_nFilterHeight;
//DEL 	nFilterWidth = dlgMedian.m_nFilterWidth;
//DEL 	nFilterCenX = dlgMedian.m_nFilterCenX;
//DEL 	nFilterCenY = dlgMedian.m_nFilterCenY;
//DEL 	
//DEL 	// 删除对话框
//DEL 	delete dlgMedian;	
//DEL 	
//DEL 	// 更改光标形状
//DEL 	BeginWaitCursor();
//DEL 	
//DEL 	// 调用MedianFilter()函数中值滤波
//DEL 	if (MedianFilter(pDoc->m_pDibInit, nFilterWidth,
//DEL 		nFilterHeight, nFilterCenX, nFilterCenY ))
//DEL 	
//DEL 	{		
//DEL 		// 设置脏标记
//DEL 		pDoc->SetModifiedFlag(TRUE);
//DEL 
//DEL 		// 更新视图
//DEL 		pDoc->UpdateAllViews(NULL);
//DEL 	}
//DEL 	else
//DEL 	{
//DEL 		// 提示用户
//DEL 		MessageBox("分配内存失败！", "系统提示" , MB_ICONINFORMATION | MB_OK);
//DEL 	}
//DEL 	
//DEL 	// 恢复光标
//DEL 	EndWaitCursor();
//DEL 
//DEL 	// 设置脏标记
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 	
//DEL 	// 更新视图
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL 	
//DEL }


//DEL void CImageProcessingView::OnEnhancePseudcolor() 
//DEL {
//DEL 	// 伪彩色编码
//DEL 	
//DEL 	// 获取文档
//DEL 	CImageProcessingDoc* pDoc = GetDocument();
//DEL 	
//DEL 	// 保存用户选择的伪彩色编码表索引
//DEL 	int		nColor;
//DEL 	
//DEL 	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的平滑，其它的可以类推）
//DEL 	if(pDoc->m_pDibInit->m_nColorTableEntries != 256)
//DEL 	{
//DEL 		// 提示用户
//DEL 		MessageBox("目前只支持256色位图的伪彩色变换！", "系统提示" , 
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 
//DEL 		// 返回
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	// 参数对话框
//DEL 	CDlgEnhColor dlgPara;
//DEL 	
//DEL 	// 初始化变量值
//DEL 	if (pDoc->m_nColorIndex >= 0)
//DEL 	{
//DEL 		// 初始选中当前的伪彩色
//DEL 		dlgPara.m_nColor = pDoc->m_nColorIndex;
//DEL 	}
//DEL 	else
//DEL 	{
//DEL 		// 初始选中灰度伪彩色编码表
//DEL 		dlgPara.m_nColor = 0;
//DEL 	}
//DEL 	
//DEL 	// 指向名称数组的指针
//DEL 	dlgPara.m_lpColorName = (LPSTR) ColorScaleName;
//DEL 	
//DEL 	// 伪彩色编码数目
//DEL 	dlgPara.m_nColorCount = COLOR_SCALE_COUNT;
//DEL 	
//DEL 	// 名称字符串长度
//DEL 	dlgPara.m_nNameLen = sizeof(ColorScaleName) / COLOR_SCALE_COUNT;
//DEL 	
//DEL 	// 显示对话框，提示用户设定平移量
//DEL 	if (dlgPara.DoModal() != IDOK)
//DEL 	{
//DEL 		// 返回
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	// 获取用户的设定
//DEL 	nColor = dlgPara.m_nColor;
//DEL 	
//DEL 	// 删除对话框
//DEL 	delete dlgPara;	
//DEL 	
//DEL 	// 更改光标形状
//DEL 	BeginWaitCursor();
//DEL 	
//DEL 	// 判断伪彩色编码是否改动
//DEL 	if (pDoc->m_nColorIndex != nColor)
//DEL 	{
//DEL 		// 调用ReplaceColorPal()函数变换调色板
//DEL 		ReplaceDIBColorTable(pDoc->m_pDibInit, (LPBYTE)ColorsTable[nColor]);
//DEL 		
//DEL 		// 更新调色板
//DEL 		pDoc->m_pDibInit->MakePalette();
//DEL 		
//DEL 		// 更新类成员变量
//DEL 		pDoc->m_nColorIndex = nColor;
//DEL 		
//DEL 		// 设置脏标记
//DEL 		pDoc->SetModifiedFlag(TRUE);
//DEL 				
//DEL 		// 更新视图
//DEL 		pDoc->UpdateAllViews(NULL);
//DEL 	}	
//DEL 	
//DEL 
//DEL 	// 恢复光标
//DEL 	EndWaitCursor();
//DEL }

//DEL void CImageProcessingView::OnTransDwt() 
//DEL {
//DEL 	// 获得文档类指针
//DEL 	CImageProcessingDoc * pDoc = (CImageProcessingDoc *)this->GetDocument();
//DEL 
//DEL 	// 指向图象的指针
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 
//DEL 	// 更改光标形状
//DEL 	BeginWaitCursor();
//DEL 
//DEL 	// 进行小波变换
//DEL 	int rsl = DIBDWTStep(pDib,0);
//DEL 
//DEL 	// 恢复光标形状
//DEL 	EndWaitCursor();
//DEL 
//DEL 	// 如果小波变换不成功，则直接返回
//DEL 	if (!rsl)			
//DEL 		return;
//DEL 
//DEL 	// 设置脏标志
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 
//DEL 	// 更新显示
//DEL 	pDoc->UpdateAllViews(FALSE);
//DEL }

//DEL void CImageProcessingView::OnTransIdwt() 
//DEL {
//DEL 	// 获得文档类指针	
//DEL 	CImageProcessingDoc * pDoc = (CImageProcessingDoc *)this->GetDocument();
//DEL 
//DEL 	// 指向图象的指针
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 
//DEL 	// 更改光标形状
//DEL 	BeginWaitCursor();
//DEL 
//DEL 	// 进行小波反变换
//DEL 	int rsl = DIBDWTStep(pDib,1);
//DEL 
//DEL 	// 恢复光标形状
//DEL 	EndWaitCursor();
//DEL 
//DEL 	// 如果小波变换不成功，则直接返回
//DEL 	if (!rsl)			
//DEL 		return;
//DEL 	pDoc->UpdateAllViews(FALSE);
//DEL 
//DEL 	// 设置脏标记
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 	
//DEL 	// 更新视图
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL }

//DEL BOOL CImageProcessingView::DIBDWTStep(CDib* pDib, int nInv)
//DEL {
//DEL 	// 循环变量
//DEL 	int i, j;
//DEL 
//DEL 	// 获取图象的长度和宽度
//DEL 	int nWidth  = pDib->m_lpBMIH->biWidth;
//DEL 	int nHeight = pDib->m_lpBMIH->biHeight;
//DEL 		
//DEL 	// 获取变换的最大层数
//DEL 	int nMaxWLevel = Log2(nWidth);
//DEL 	int nMaxHLevel = Log2(nHeight);
//DEL 	int nMaxLevel;
//DEL 	if (nWidth == 1<<nMaxWLevel && nHeight == 1<<nMaxHLevel)
//DEL 		nMaxLevel = min(nMaxWLevel, nMaxHLevel);
//DEL 
//DEL 	// 获取图象的存储尺寸
//DEL 	CSize sizeImageSave = pDib->GetDibSaveDim();
//DEL 
//DEL 	// 临时变量
//DEL 	double	*pDbTemp;
//DEL 	BYTE	*pBits;
//DEL 
//DEL 	// 如果小波变换的存储内存还没有分配，则分配此内存
//DEL 	if(!m_pDbImage){			
//DEL 		m_pDbImage = new double[nWidth*nHeight];
//DEL 		if (!m_pDbImage)	return FALSE;
//DEL 
//DEL 		// 将图象数据放入m_pDbImage中 
//DEL 		for (j=0; j<nHeight; j++)
//DEL 		{
//DEL 			pDbTemp = m_pDbImage + j*sizeImageSave.cx;
//DEL 			pBits = pDib->m_lpImage + (nHeight-1-j)*sizeImageSave.cx;		
//DEL 			for (i=0; i<nWidth; i++)
//DEL 				pDbTemp[i] = pBits[i];
//DEL 		}
//DEL 	}
//DEL 	
//DEL 	// 进行小波变换（或反变换）
//DEL 	if (!DWTStep_2D(m_pDbImage, nMaxWLevel-m_nDWTCurDepth, nMaxHLevel-m_nDWTCurDepth,
//DEL 						nMaxWLevel, nMaxHLevel, nInv, 1, m_nSupp))
//DEL 		return FALSE;
//DEL 
//DEL 	// 如果是反变换，则当前层数减1
//DEL 	if (nInv)
//DEL 		m_nDWTCurDepth --;
//DEL 	// 否则加1
//DEL 	else
//DEL 		m_nDWTCurDepth ++;
//DEL 
//DEL 	// 然后，将数据拷贝回原CDib中，并进行相应的数据转换
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
//DEL 	// 返回
//DEL 	return TRUE;
//DEL }
//DEL void CImageProcessingView::OnEnhanceLintrans() 
//DEL {
//DEL 	// 获取文档
//DEL 	CImageProcessingDoc* pDoc = GetDocument();
//DEL 
//DEL 	// 创建对话框
//DEL 	CDlgEhnLinTrans dlgPara;
//DEL 	
//DEL 	// 点1坐标
//DEL 	int	nX1;
//DEL 	int	nY1;
//DEL 	
//DEL 	// 点2坐标
//DEL 	int	nX2;
//DEL 	int	nY2;
//DEL 
//DEL 	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的灰度拉伸，其它的可以类推）
//DEL 	if(pDoc->m_pDibInit->m_nColorTableEntries != 256)
//DEL 	{
//DEL 		// 提示用户
//DEL 		MessageBox("目前只支持256色位图的伪彩色变换！", "系统提示" , 
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 
//DEL 		// 返回
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	// 初始化变量值
//DEL 	dlgPara.m_nX1 = 50;
//DEL 	dlgPara.m_nY1 = 30;
//DEL 	dlgPara.m_nX2 = 200;
//DEL 	dlgPara.m_nY2 = 220;
//DEL 	
//DEL 	// 显示对话框，提示用户设定拉伸位置
//DEL 	if (dlgPara.DoModal() != IDOK)
//DEL 	{
//DEL 		// 返回
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	// 获取用户的设定
//DEL 	nX1 = dlgPara.m_nX1;
//DEL 	nY1 = dlgPara.m_nY1;
//DEL 	nX2 = dlgPara.m_nX2;
//DEL 	nY2 = dlgPara.m_nY2;
//DEL 	
//DEL 	// 删除对话框
//DEL 	delete dlgPara;	
//DEL 	
//DEL 	// 更改光标形状
//DEL 	BeginWaitCursor();
//DEL 	
//DEL 	// 调用GrayStretch()函数进行灰度拉伸
//DEL 	GraySegLinTrans(pDoc->m_pDibInit, nX1, nY1, nX2, nY2);
//DEL 	
//DEL 	// 设置脏标记
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 	
//DEL 	// 更新视图
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL 	
//DEL 	// 恢复光标
//DEL 	EndWaitCursor();
//DEL }

//DEL void CImageProcessingView::OnEnhanceHistequ() 
//DEL {
//DEL 	// 直方图均衡
//DEL 
//DEL 	// 获取文档
//DEL 	CImageProcessingDoc* pDoc = GetDocument();
//DEL 		
//DEL 	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的直方图均衡，其它的可以类推）
//DEL 	if(pDoc->m_pDibInit->m_nColorTableEntries != 256)
//DEL 	{
//DEL 		// 提示用户
//DEL 		MessageBox("目前只支持256色位图的伪彩色变换！", "系统提示" , 
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 
//DEL 		// 返回
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	// 更改光标形状
//DEL 	BeginWaitCursor();
//DEL 	
//DEL 	// 调用HistogramEqualize()函数进行直方图均衡
//DEL 	HistogramEqualize(pDoc->m_pDibInit);
//DEL 	
//DEL 	// 设置脏标记
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 	
//DEL 	// 更新视图
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL 		
//DEL 	// 恢复光标
//DEL 	EndWaitCursor();	
//DEL }

/*************************************************************************
 *
 * \函数名称：
 *   OnRegReg()
 *
 * \输入参数:
 *   无
 * 
 * \返回值:
 *   无
 *
 * \说明:
 *   该函数实现图象的配准
 *
 *************************************************************************
 */
//DEL void CImageProcessingView::OnRegReg() 
//DEL {	
//DEL 	// 获得文档类句柄
//DEL 	CImageProcessingDoc* pDoc;
//DEL 	pDoc = GetDocument();
//DEL 
//DEL 	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的水平镜像，其它的可以类推）
//DEL 	if(pDoc->m_pDibInit->m_nColorTableEntries != 256)
//DEL 	{
//DEL 		// 提示用户
//DEL 		MessageBox("目前只支持256色位图的图象配准！", "系统提示" , 
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 		
//DEL 		// 返回
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	// 打开图象配准对话框
//DEL 	CDlgReg* pDlg=new CDlgReg(NULL,pDoc);
//DEL 
//DEL 	pDlg->DoModal();
//DEL 
//DEL 	delete pDlg;
//DEL }

/*************************************************************************
 *
 * \函数名称：
 *   OnEnhanceSharp()
 *
 * \输入参数:
 *   无
 * 
 * \返回值:
 *   无
 *
 * \说明:
 *   该函数利用Laplacian算子实现图象的锐化
 *
 *************************************************************************
 */
//DEL void CImageProcessingView::OnEnhanceSharp() 
//DEL {
//DEL 	//更改光标形状
//DEL 	BeginWaitCursor(); 
//DEL 	
//DEL 	CImageProcessingDoc * pDoc = (CImageProcessingDoc *)this->GetDocument();
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 	
//DEL 	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
//DEL 	
//DEL 	// 判断是否是8-bpp位图
//DEL 	if (lpBMIH->biBitCount != 8)
//DEL 	{
//DEL 		// 提示用户
//DEL 		MessageBox("目前只支持256色位图的图象分割！", "系统提示" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 		
//DEL 		// 返回
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	// 循环控制变量
//DEL 	int y;
//DEL 	int x;
//DEL 	
//DEL 	CSize sizeImage = pDib->GetDimensions();
//DEL 	int nWidth = sizeImage.cx ;
//DEL 	int nHeight= sizeImage.cy ;
//DEL 	
//DEL 	int nSaveWidth = pDib->GetDibSaveDim().cx;
//DEL 	
//DEL 	// 开辟内存，存储图象数据,该数据的存储不是DWORD对齐的
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
//DEL 	// 调用LinearSharpen函数进行图象锐化增强
//DEL 	LinearSharpen(pUnchImage, nWidth, nHeight) ;
//DEL 	
//DEL 	// 增强以后的图象拷贝到pDib中，进行显示
//DEL 	for(y=0; y<nHeight; y++)
//DEL 	{
//DEL 		for(x=0; x<nWidth; x++)
//DEL 		{
//DEL 			pDib->m_lpImage[y*nSaveWidth+x] = pUnchImage[y*nWidth+x];
//DEL 		}
//DEL 	}
//DEL 	
//DEL 	// 释放内存
//DEL 	delete []pUnchImage;
//DEL 	pUnchImage = NULL  ;
//DEL 	
//DEL 	// 恢复光标形状
//DEL 	EndWaitCursor(); 
//DEL 	
//DEL 	// 设置脏标记
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 	
//DEL 	// 更新视图
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL 	
//DEL }

/*************************************************************************
*
* \函数名称：
*   OnEnhanceSmoothFr()
*
* \输入参数:
*   无
*
* \返回值:
*   无
*
* \说明:
*   该函数利用低通滤波实现图象平滑
*
************************************************************************
*/
//DEL void CImageProcessingView::OnEnhanceSmoothFr() 
//DEL {
//DEL 	// TODO: Add your command handler code here
//DEL 	//更改光标形状
//DEL 	BeginWaitCursor(); 
//DEL 	
//DEL 	CImageProcessingDoc * pDoc = (CImageProcessingDoc *)this->GetDocument();
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 	
//DEL 	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
//DEL 	
//DEL 	// 判断是否是8-bpp位图
//DEL 	if (lpBMIH->biBitCount != 8)
//DEL 	{
//DEL 		// 提示用户
//DEL 		MessageBox("目前只支持256色位图的图象分割！", "系统提示" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 		
//DEL 		// 返回
//DEL 		return;
//DEL 	}
//DEL 
//DEL 	// 循环控制变量
//DEL 	int y;
//DEL 	int x;
//DEL 	
//DEL 	CSize sizeImage = pDib->GetDimensions();
//DEL 	int nWidth = sizeImage.cx ;
//DEL 	int nHeight= sizeImage.cy ;
//DEL 
//DEL 	int nSaveWidth = pDib->GetDibSaveDim().cx;
//DEL 	
//DEL 	// 开辟内存，存储图象数据,该数据的存储不是DWORD对齐的
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
//DEL 	// 调用低通滤波函数进行图象增强
//DEL 	LowPassFilterEnhance(pUnchImage, nWidth, nHeight, nWidth/16) ;
//DEL 	
//DEL 	// 增强以后的图象拷贝到pDib中，进行显示
//DEL 	for(y=0; y<nHeight; y++)
//DEL 	{
//DEL 		for(x=0; x<nWidth; x++)
//DEL 		{
//DEL 			pDib->m_lpImage[y*nSaveWidth+x] = pUnchImage[y*nWidth+x];
//DEL 		}
//DEL 	}
//DEL 	
//DEL 	// 释放内存
//DEL 	delete []pUnchImage;
//DEL 	pUnchImage = NULL  ;
//DEL 	
//DEL 	// 恢复光标形状
//DEL 	EndWaitCursor(); 
//DEL 	
//DEL 	// 设置脏标记
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 	
//DEL 	// 更新视图
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL }

/*************************************************************************
*
* \函数名称：
*   OnEnhanceButtLow()
*
* \输入参数:
*   无
*
* \返回值:
*   无
*
* \说明:
*   该函数利用Butterworth低通滤波实现图象平滑
*
************************************************************************
*/
//DEL void CImageProcessingView::OnEnhanceButtLow() 
//DEL {
//DEL 	// TODO: Add your command handler code here
//DEL 	//更改光标形状
//DEL 	BeginWaitCursor(); 
//DEL 	
//DEL 	CImageProcessingDoc * pDoc = (CImageProcessingDoc *)this->GetDocument();
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 	
//DEL 	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
//DEL 	
//DEL 	// 判断是否是8-bpp位图
//DEL 	if (lpBMIH->biBitCount != 8)
//DEL 	{
//DEL 		// 提示用户
//DEL 		MessageBox("目前只支持256色位图的图象分割！", "系统提示" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 		
//DEL 		// 返回
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	// 循环控制变量
//DEL 	int y;
//DEL 	int x;
//DEL 	
//DEL 	CSize sizeImage = pDib->GetDimensions();
//DEL 	int nWidth = sizeImage.cx ;
//DEL 	int nHeight= sizeImage.cy ;
//DEL 	
//DEL 	int nSaveWidth = pDib->GetDibSaveDim().cx;
//DEL 	
//DEL 	// 开辟内存，存储图象数据,该数据的存储不是DWORD对齐的
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
//DEL 	// 调用ButterWorth低通滤波函数进行图象增强
//DEL 	ButterWorthLowPass(pUnchImage, nWidth, nHeight, nWidth/2) ;
//DEL 	
//DEL 	// 增强以后的图象拷贝到pDib中，进行显示
//DEL 	for(y=0; y<nHeight; y++)
//DEL 	{
//DEL 		for(x=0; x<nWidth; x++)
//DEL 		{
//DEL 			pDib->m_lpImage[y*nSaveWidth+x] = pUnchImage[y*nWidth+x];
//DEL 		}
//DEL 	}
//DEL 	
//DEL 	// 释放内存
//DEL 	delete []pUnchImage;
//DEL 	pUnchImage = NULL  ;
//DEL 	
//DEL 	// 恢复光标形状
//DEL 	EndWaitCursor(); 
//DEL 	
//DEL 	// 设置脏标记
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 	
//DEL 	// 更新视图
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL }

/*************************************************************************
*
* \函数名称：
*   OnEnhanceSharpFreq()
*
* \输入参数:
*   无
*
* \返回值:
*   无
*
* \说明:
*   该函数利用高通滤波实现图象增强
*
************************************************************************
*/
//DEL void CImageProcessingView::OnEnhanceSharpFreq() 
//DEL {
//DEL 	// TODO: Add your command handler code here
//DEL 	//更改光标形状
//DEL 	BeginWaitCursor(); 
//DEL 	
//DEL 	CImageProcessingDoc * pDoc = (CImageProcessingDoc *)this->GetDocument();
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 	
//DEL 	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
//DEL 	
//DEL 	// 判断是否是8-bpp位图
//DEL 	if (lpBMIH->biBitCount != 8)
//DEL 	{
//DEL 		// 提示用户
//DEL 		MessageBox("目前只支持256色位图的图象分割！", "系统提示" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 		
//DEL 		// 返回
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	// 循环控制变量
//DEL 	int y;
//DEL 	int x;
//DEL 	
//DEL 	CSize sizeImage = pDib->GetDimensions();
//DEL 	int nWidth = sizeImage.cx ;
//DEL 	int nHeight= sizeImage.cy ;
//DEL 	
//DEL 	int nSaveWidth = pDib->GetDibSaveDim().cx;
//DEL 	
//DEL 	// 开辟内存，存储图象数据,该数据的存储不是DWORD对齐的
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
//DEL 	// 调用高通滤波函数进行图象增强
//DEL 	HighPassFilterEnhance(pUnchImage, nWidth, nHeight, 50) ;
//DEL 	
//DEL 	// 增强以后的图象拷贝到pDib中，进行显示
//DEL 	for(y=0; y<nHeight; y++)
//DEL 	{
//DEL 		for(x=0; x<nWidth; x++)
//DEL 		{
//DEL 			pDib->m_lpImage[y*nSaveWidth+x] = pUnchImage[y*nWidth+x];
//DEL 		}
//DEL 	}
//DEL 	
//DEL 	// 释放内存
//DEL 	delete []pUnchImage;
//DEL 	pUnchImage = NULL  ;
//DEL 	
//DEL 	// 恢复光标形状
//DEL 	EndWaitCursor(); 
//DEL 	
//DEL 	// 设置脏标记
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 	
//DEL 	// 更新视图
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL }

/*************************************************************************
*
* \函数名称：
*   OnEnhanceButtHight()
*
* \输入参数:
*   无
*
* \返回值:
*   无
*
* \说明:
*   该函数利用Butterworth高通滤波实现图象平滑
*
************************************************************************
*/
//DEL void CImageProcessingView::OnEnhanceButtHight() 
//DEL {	
//DEL 	//更改光标形状
//DEL 	BeginWaitCursor(); 
//DEL 	
//DEL 	CImageProcessingDoc * pDoc = (CImageProcessingDoc *)this->GetDocument();
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 	
//DEL 	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
//DEL 	
//DEL 	// 判断是否是8-bpp位图
//DEL 	if (lpBMIH->biBitCount != 8)
//DEL 	{
//DEL 		// 提示用户
//DEL 		MessageBox("目前只支持256色位图的图象分割！", "系统提示" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 		
//DEL 		// 返回
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	// 循环控制变量
//DEL 	int y;
//DEL 	int x;
//DEL 	
//DEL 	CSize sizeImage = pDib->GetDimensions();
//DEL 	int nWidth = sizeImage.cx ;
//DEL 	int nHeight= sizeImage.cy ;
//DEL 	
//DEL 	int nSaveWidth = pDib->GetDibSaveDim().cx;
//DEL 	
//DEL 	// 开辟内存，存储图象数据,该数据的存储不是DWORD对齐的
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
//DEL 	// 调用ButterWorth高通滤波函数进行图象增强
//DEL 	ButterWorthHighPass(pUnchImage, nWidth, nHeight, nWidth/2) ;
//DEL 	
//DEL 	// 增强以后的图象拷贝到pDib中，进行显示
//DEL 	for(y=0; y<nHeight; y++)
//DEL 	{
//DEL 		for(x=0; x<nWidth; x++)
//DEL 		{
//DEL 			pDib->m_lpImage[y*nSaveWidth+x] = pUnchImage[y*nWidth+x];
//DEL 		}
//DEL 	}
//DEL 	
//DEL 	// 释放内存
//DEL 	delete []pUnchImage;
//DEL 	pUnchImage = NULL  ;
//DEL 	
//DEL 	// 恢复光标形状
//DEL 	EndWaitCursor(); 
//DEL 	
//DEL 	// 设置脏标记
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 	
//DEL 	// 更新视图
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL 	
//DEL }

/*************************************************************************
*
* \函数名称：
*   OnRegionsegFix()
*
* \输入参数:
*   无
*
* \返回值:
*   无
*
* \说明:
*   实现并行区域分割
*
************************************************************************
*/
//DEL void CImageProcessingView::OnRegionsegFix() 
//DEL {
//DEL 	// TODO: Add your command handler code here
//DEL 
//DEL 	//更改光标形状
//DEL 	BeginWaitCursor(); 
//DEL 
//DEL 	CImageProcessingDoc * pDoc = (CImageProcessingDoc *)this->GetDocument();
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 
//DEL 	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
//DEL 	
//DEL 	// 判断是否是8-bpp位图
//DEL 	if (lpBMIH->biBitCount != 8)
//DEL 	{
//DEL 		// 提示用户
//DEL 		MessageBox("目前只支持256色位图的图象分割！", "系统提示" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 					
//DEL 		// 返回
//DEL 		return;
//DEL 	}
//DEL 
//DEL 	// 调用函数实现固定阈值分割
//DEL 	RegionSegFixThreshold(pDib,200);
//DEL 
//DEL 	// 恢复光标形状
//DEL 	EndWaitCursor(); 
//DEL 	
//DEL 	// 设置脏标记
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 	
//DEL 	// 更新视图
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL }

/*************************************************************************
*
* \函数名称：
*   OnAdaRegionSeg()
*
* \输入参数:
*   无
*
* \返回值:
*   无
*
* \说明:
*   实现自适应区域分割算法
*
************************************************************************
*/
//DEL void CImageProcessingView::OnAdaRegionSeg() 
//DEL {
//DEL 	// TODO: Add your command handler code here
//DEL 	
//DEL 	//更改光标形状
//DEL 	BeginWaitCursor(); 
//DEL 	
//DEL 	CImageProcessingDoc * pDoc = (CImageProcessingDoc *)this->GetDocument();
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 	
//DEL 	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
//DEL 	
//DEL 	// 判断是否是8-bpp位图
//DEL 	if (lpBMIH->biBitCount != 8)
//DEL 	{
//DEL 		// 提示用户
//DEL 		MessageBox("目前只支持256色位图的图象分割！", "系统提示" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 		
//DEL 		// 返回
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	// 自适应区域分割
//DEL 	RegionSegAdaptive(pDib);
//DEL 	
//DEL 	// 恢复光标形状
//DEL 	EndWaitCursor(); 
//DEL 	
//DEL 		// 设置脏标记
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 	
//DEL 	// 更新视图
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL 	
//DEL }
/*************************************************************************
*
* \函数名称：
*   OnEdgeRoberts()
*
* \输入参数:
*   无
*
* \返回值:
*   无
*
* \说明:
*   实现并行边界分割－Roberts算子
*
************************************************************************
*/
//DEL void CImageProcessingView::OnEdgeRoberts() 
//DEL {
//DEL 	// TODO: Add your command handler code here
//DEL 
//DEL 	//更改光标形状
//DEL 	BeginWaitCursor(); 
//DEL 
//DEL 	// 循环控制变量
//DEL 	int y;
//DEL 	int x;
//DEL 	
//DEL 	CImageProcessingDoc * pDoc = (CImageProcessingDoc *)this->GetDocument();
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 
//DEL 	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
//DEL 	
//DEL 	// 判断是否是8-bpp位图
//DEL 	if (lpBMIH->biBitCount != 8)
//DEL 	{
//DEL 		// 提示用户
//DEL 		MessageBox("目前只支持256色位图的图象分割！", "系统提示" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 		
//DEL 		// 返回
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	// 图象的长宽大小
//DEL 	CSize sizeImage		= pDib->GetDimensions();
//DEL 	int nWidth			= sizeImage.cx		;
//DEL 	int nHeight			= sizeImage.cy		;
//DEL 	
//DEL 	// 指向梯度数据的指针
//DEL 	double * pdGrad;
//DEL 	
//DEL 	// 按照图像的大小开辟内存空间，存储梯度计算的结果
//DEL 	pdGrad=new double[nHeight*nWidth];
//DEL 	
//DEL 	//图像数据的指针
//DEL 	LPBYTE  pImageData = pDib->m_lpImage;
//DEL 	
//DEL 	// 图像在计算机在存储中的实际大小
//DEL 	CSize sizeImageSave	= pDib->GetDibSaveDim();
//DEL 	
//DEL 	// 图像在内存中每一行象素占用的实际空间
//DEL 	int nSaveWidth = sizeImageSave.cx;
//DEL 	
//DEL 	// 应用Roberts算子求梯度
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
//DEL   //释放梯度结果使用的内存空间
//DEL 	delete pdGrad;
//DEL 	pdGrad=NULL;
//DEL 
//DEL 	// 恢复光标形状
//DEL 	EndWaitCursor(); 
//DEL 	
//DEL 	// 设置脏标记
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 	
//DEL 	// 更新视图
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL }


/*************************************************************************
*
* \函数名称：
*   OnEdgeSobel()
*
* \输入参数:
*   无
*
* \返回值:
*   无
*
* \说明:
*   实现并行边界分割－Sobel算子
*
************************************************************************
*/
//DEL void CImageProcessingView::OnEdgeSobel() 
//DEL {
//DEL 	// TODO: Add your command handler code here
//DEL 	
//DEL 	//更改光标形状
//DEL 	BeginWaitCursor(); 
//DEL 
//DEL 	// 循环控制变量
//DEL 	int y;
//DEL 	int x;
//DEL 	
//DEL 	CImageProcessingDoc * pDoc = (CImageProcessingDoc *)this->GetDocument();
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 
//DEL 	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
//DEL 	
//DEL 	// 判断是否是8-bpp位图
//DEL 	if (lpBMIH->biBitCount != 8)
//DEL 	{
//DEL 		// 提示用户
//DEL 		MessageBox("目前只支持256色位图的图象分割！", "系统提示" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 		
//DEL 		// 返回
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	// 图象的长宽大小
//DEL 	CSize sizeImage		= pDib->GetDimensions();
//DEL 	int nWidth			= sizeImage.cx		;
//DEL 	int nHeight			= sizeImage.cy		;
//DEL 	
//DEL 	// 指向梯度数据的指针
//DEL 	double * pdGrad;
//DEL 	
//DEL 	// 按照图像的大小开辟内存空间，存储梯度计算的结果
//DEL 	pdGrad=new double[nHeight*nWidth];
//DEL 	
//DEL 	//图像数据的指针
//DEL 	LPBYTE  lpImage = pDib->m_lpImage;
//DEL 	
//DEL 	// 图像在计算机在存储中的实际大小
//DEL 	CSize sizeImageSave	= pDib->GetDibSaveDim();
//DEL 	
//DEL 	// 图像在内存中每一行象素占用的实际空间
//DEL 	int nSaveWidth = sizeImageSave.cx;
//DEL 	
//DEL 	// 应用Sobel算子求梯度
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
//DEL   //释放内存空间
//DEL 	delete []pdGrad;
//DEL 	pdGrad=NULL;
//DEL 	
//DEL 	// 恢复光标形状
//DEL 	EndWaitCursor(); 
//DEL 	
//DEL 	// 设置脏标记
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 	
//DEL 	// 更新视图
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL }
/*************************************************************************
*
* \函数名称：
*   OnEdgePrewitt()
*
* \输入参数:
*   无
*
* \返回值:
*   无
*
* \说明:
*   实现并行边界分割－Prewitt算子
*
************************************************************************
*/
//DEL void CImageProcessingView::OnEdgePrewitt() 
//DEL {
//DEL 	// TODO: Add your command handler code here
//DEL 	
//DEL 	//更改光标形状
//DEL 	BeginWaitCursor(); 
//DEL 
//DEL 	// 循环控制变量
//DEL 	int y;
//DEL 	int x;
//DEL 	
//DEL 	CImageProcessingDoc * pDoc = (CImageProcessingDoc *)this->GetDocument();
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 
//DEL 	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
//DEL 	
//DEL 	// 判断是否是8-bpp位图
//DEL 	if (lpBMIH->biBitCount != 8)
//DEL 	{
//DEL 		// 提示用户
//DEL 		MessageBox("目前只支持256色位图的图象分割！", "系统提示" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 		
//DEL 		// 返回
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	// 图象的长宽大小
//DEL 	CSize sizeImage		= pDib->GetDimensions();
//DEL 	int nWidth			= sizeImage.cx		;
//DEL 	int nHeight			= sizeImage.cy		;
//DEL 	
//DEL 	// 指向梯度数据的指针
//DEL 	double * pdGrad;
//DEL 	
//DEL 	// 按照图像的大小开辟内存空间，存储梯度计算的结果
//DEL 	pdGrad=new double[nHeight*nWidth];
//DEL 	
//DEL 	//图像数据的指针
//DEL 	LPBYTE  lpImage = pDib->m_lpImage;
//DEL 	
//DEL 	// 图像在计算机在存储中的实际大小
//DEL 	CSize sizeImageSave	= pDib->GetDibSaveDim();
//DEL 	
//DEL 	// 图像在内存中每一行象素占用的实际空间
//DEL 	int nSaveWidth = sizeImageSave.cx;
//DEL 	
//DEL 	// 应用Prewitt算子求梯度
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
//DEL   //释放内存空间
//DEL 	delete []pdGrad;
//DEL 	pdGrad=NULL;
//DEL 	
//DEL 	// 恢复光标形状
//DEL 	EndWaitCursor(); 
//DEL 	
//DEL 	// 设置脏标记
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 	
//DEL 	// 更新视图
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL 		
//DEL }

/*************************************************************************
*
* \函数名称：
*   OnEdgeLaplace()
*
* \输入参数:
*   无
*
* \返回值:
*   无
*
* \说明:
*   实现并行边界分割－拉普拉斯算子
*
************************************************************************
*/
//DEL void CImageProcessingView::OnEdgeLaplace() 
//DEL {
//DEL 	// TODO: Add your command handler code here
//DEL 	
//DEL 	//更改光标形状
//DEL 	BeginWaitCursor(); 
//DEL 	
//DEL 	// 循环控制变量
//DEL 	int y;
//DEL 	int x;
//DEL 	
//DEL 	CImageProcessingDoc * pDoc = (CImageProcessingDoc *)this->GetDocument();
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 	
//DEL 	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
//DEL 	
//DEL 	// 判断是否是8-bpp位图
//DEL 	if (lpBMIH->biBitCount != 8)
//DEL 	{
//DEL 		// 提示用户
//DEL 		MessageBox("目前只支持256色位图的图象分割！", "系统提示" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 		
//DEL 		// 返回
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	// 图象的长宽大小
//DEL 	CSize sizeImage		= pDib->GetDimensions();
//DEL 	int nWidth			= sizeImage.cx		;
//DEL 	int nHeight			= sizeImage.cy		;
//DEL 	
//DEL 	// 指向梯度数据的指针
//DEL 	double * pdGrad;
//DEL 	
//DEL 	// 按照图像的大小开辟内存空间，存储梯度计算的结果
//DEL 	pdGrad=new double[nHeight*nWidth];
//DEL 	
//DEL 	//图像数据的指针
//DEL 	LPBYTE  lpImage = pDib->m_lpImage;
//DEL 	
//DEL 	// 图像在计算机在存储中的实际大小
//DEL 	CSize sizeImageSave	= pDib->GetDibSaveDim();
//DEL 	
//DEL 	// 图像在内存中每一行象素占用的实际空间
//DEL 	int nSaveWidth = sizeImageSave.cx;
//DEL 	
//DEL 	// 应用Laplace算子求梯度
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
//DEL 	//释放内存空间
//DEL 	delete []pdGrad;
//DEL 	pdGrad=NULL;
//DEL 	
//DEL 	// 恢复光标形状
//DEL 	EndWaitCursor(); 
//DEL 	
//DEL 	// 设置脏标记
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 	
//DEL 	// 更新视图
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL 	
//DEL }

/*************************************************************************
*
* \函数名称：
*   OnEdgeCanny()
*
* \输入参数:
*   无
*
* \返回值:
*   无
*
* \说明:
*   实现并行边界分割－Canny算子
*
************************************************************************
*/
void CImageProcessingView::OnEdgeCanny() 
{
	// TODO: Add your command handler code here

	//更改光标形状
	BeginWaitCursor(); 

	CImageProcessingDoc * pDoc = (CImageProcessingDoc *)this->GetDocument();
	CDib * pDib = pDoc->m_pDibInit;
	
	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
	
	// 判断是否是8-bpp位图
	if (lpBMIH->biBitCount != 8)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的图象分割！", "系统提示" ,
			MB_ICONINFORMATION | MB_OK);
		
		// 返回
		return;
	}

	// 循环控制变量
	int y; 
	int x;
	
	CSize sizeImage = pDib->GetDimensions();
	int nWidth = sizeImage.cx ;
	int nHeight= sizeImage.cy ;

	int nSaveWidth = pDib->GetDibSaveDim().cx;

	// 开辟内存，存储图象数据
	unsigned char * pUnchImage = new unsigned char[nWidth*nHeight];

	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			pUnchImage[y*nWidth+x] = pDib->m_lpImage[y*nSaveWidth+x];
		}
	}
	
	// canny算子计算后的结果
	unsigned char * pUnchEdge = new unsigned char[nWidth*nHeight];
	
	// 调用canny函数进行边界提取
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
	
	// 恢复光标形状
	EndWaitCursor(); 
	
	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);
	
	// 更新视图
	pDoc->UpdateAllViews(NULL);
	
}

/*************************************************************************
*
* \函数名称：
*   OnEdgeTrack()
*
* \输入参数:
*   无
*
* \返回值:
*   无
*
* \说明:
*   实现边界跟踪算法
*
************************************************************************
*/
//DEL void CImageProcessingView::OnEdgeTrack() 
//DEL {
//DEL 	// TODO: Add your command handler code here
//DEL 
//DEL 	//更改光标形状
//DEL 	BeginWaitCursor(); 
//DEL 
//DEL 	// 获得Doc类的指
//DEL 	CImageProcessingDoc * pDoc = (CImageProcessingDoc *)this->GetDocument();
//DEL 	
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 
//DEL 	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
//DEL 	
//DEL 	// 判断是否是8-bpp位图
//DEL 	if (lpBMIH->biBitCount != 8)
//DEL 	{
//DEL 		// 提示用户
//DEL 		MessageBox("目前只支持256色位图的图象分割！", "系统提示" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 		
//DEL 		// 返回
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	
//DEL 	// 图像在计算机在存储中的实际大小
//DEL 	CSize sizeImageSave	= pDib->GetDibSaveDim();
//DEL 	
//DEL 	// 图像在内存中每一行象素占用的实际空间
//DEL 	int nSaveWidth = sizeImageSave.cx;
//DEL 	
//DEL 	// 遍历图象的纵坐标
//DEL 	int y;
//DEL 	
//DEL 	// 遍历图象的横坐标
//DEL 	int x;
//DEL 	
//DEL 	// 图象的长宽大小
//DEL 	CSize sizeImage		= pDib->GetDimensions();
//DEL 	int nWidth			= sizeImage.cx		;
//DEL 	int nHeight			= sizeImage.cy		;	
//DEL 	
//DEL 	
//DEL 	// 指向图像数据的指针
//DEL 	LPBYTE lpImage ;
//DEL 	lpImage = pDib->m_lpImage ;
//DEL 	
//DEL 	// 边界跟踪后的结果区域
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
//DEL 	//释放内存
//DEL 	delete pUnEdgeTrack;
//DEL 	pUnEdgeTrack = NULL;
//DEL 	
//DEL 	// 恢复光标形状
//DEL 	EndWaitCursor(); 
//DEL 	
//DEL 	// 设置脏标记
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 	
//DEL 	// 更新视图
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL 	
//DEL }

/*************************************************************************
*
* \函数名称：
*   OnRegionGrow()
*
* \输入参数:
*   无
*
* \返回值:
*   无
*
* \说明:
*   实现区域生长算法
*
************************************************************************
*/
//DEL void CImageProcessingView::OnRegionGrow() 
//DEL {
//DEL 	// TODO: Add your command handler code here
//DEL 
//DEL 	//更改光标形状
//DEL 	BeginWaitCursor(); 
//DEL 	
//DEL 	// 获得Doc类的指
//DEL 	CImageProcessingDoc * pDoc = (CImageProcessingDoc *)this->GetDocument();
//DEL 	
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 
//DEL 	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
//DEL 	
//DEL 	// 判断是否是8-bpp位图
//DEL 	if (lpBMIH->biBitCount != 8)
//DEL 	{
//DEL 		// 提示用户
//DEL 		MessageBox("目前只支持256色位图的图象分割！", "系统提示" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 		
//DEL 		// 返回
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	// 遍历图象的纵坐标
//DEL 	int y;
//DEL 	
//DEL 	// 遍历图象的横坐标
//DEL 	int x;
//DEL 	
//DEL 	// 图像在计算机在存储中的实际大小
//DEL 	CSize sizeImageSave	= pDib->GetDibSaveDim();
//DEL 	
//DEL 	// 图像在内存中每一行象素占用的实际空间
//DEL 	int nSaveWidth = sizeImageSave.cx;
//DEL 	
//DEL 	// 图象的长宽大小
//DEL 	CSize sizeImage		= pDib->GetDimensions();
//DEL 	int nWidth			= sizeImage.cx		;
//DEL 	int nHeight			= sizeImage.cy		;	
//DEL 	
//DEL 	// 指向图像数据的指针
//DEL 	LPBYTE lpImage ;
//DEL 	lpImage = pDib->m_lpImage ;
//DEL 	
//DEL 	unsigned char * pUnchRgRst = new unsigned char[nWidth * nHeight];
//DEL 	// 初始化
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
//DEL 	// 释放内存
//DEL 	delete []pUnchRgRst;
//DEL 	pUnchRgRst = NULL  ;
//DEL 	
//DEL 	// 恢复光标形状
//DEL 	EndWaitCursor(); 
//DEL 	
//DEL 	// 设置脏标记
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 	
//DEL 	// 更新视图
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL }

/*************************************************************************
 *
 * \函数名称：
 *   OnMotionBackground()
 *
 * \输入参数:
 *   无
 * 
 * \返回值:
 *   无
 *
 * \说明:
 *   该函数根据运动图象提取其中的静止背景。其中运动图象要求为bmp文件，并按
 *顺序排列。
 *
 *************************************************************************
 */
//DEL void CImageProcessingView::OnMotionBackground() 
//DEL {
//DEL 	// 提取背景成功标志
//DEL 	BOOL bFlag;
//DEL 	
//DEL 	// 获取文档指针
//DEL 	CImageProcessingDoc* pDoc;
//DEL 	pDoc = GetDocument();
//DEL 
//DEL 	// 获得当前打开文件的文件路径名
//DEL 	CString strPathName;
//DEL 	strPathName = pDoc->GetPathName();
//DEL 
//DEL 	// 序列的总帧数
//DEL 	int nTotalFrameNum = 20;
//DEL 
//DEL 	// 图象的宽度
//DEL 	int nImageWidth;
//DEL 	nImageWidth = pDoc->m_pDibInit->m_lpBMIH->biWidth;	
//DEL 
//DEL 	// 图象的高度
//DEL 	int nImageHeight;
//DEL 	nImageHeight = pDoc->m_pDibInit->m_lpBMIH->biHeight;
//DEL 
//DEL 	// 图象的静止背景	
//DEL 	unsigned char* pUnchBackGround;
//DEL 	pUnchBackGround = new unsigned char[nImageWidth*nImageHeight];
//DEL 	
//DEL 	// 更改光标形状
//DEL 	BeginWaitCursor();
//DEL 	
//DEL 	// 调用GetBackground函数提取静止背景
//DEL 	bFlag = GetBackground(strPathName, nTotalFrameNum, nImageWidth,nImageHeight, pUnchBackGround);
//DEL 	if(bFlag == FALSE){
//DEL 		return;
//DEL 	}
//DEL 
//DEL 	// 将背景设置为当前显示图象
//DEL 	LPBYTE lpTemp;
//DEL 	lpTemp = pDoc->m_pDibInit->m_lpImage;
//DEL 
//DEL 	// 将数据拷贝到图象中
//DEL 	memcpy(lpTemp, (LPBYTE)pUnchBackGround, nImageWidth*nImageHeight);
//DEL 	
//DEL 	// 恢复光标形状
//DEL 	EndWaitCursor(); 
//DEL 
//DEL 	// 释放已分配内存
//DEL 	delete[]pUnchBackGround;
//DEL 	pUnchBackGround = NULL;
//DEL 	
//DEL 	// 设置脏标记
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 	
//DEL 	// 更新视图
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL }

/*************************************************************************
 *
 * \函数名称：
 *   OnRecogMatch()
 *
 * \输入参数:
 *   无
 * 
 * \返回值:
 *   无
 *
 * \说明:
 *   根据图象模板，在待匹配的图象中找到匹配的位置
 *
 *************************************************************************
 */
//DEL void CImageProcessingView::OnRecogMatch() 
//DEL {
//DEL 	CImageProcessingDoc* pDoc;
//DEL 	pDoc = GetDocument();
//DEL 
//DEL 	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的水平镜像，其它的可以类推）
//DEL 	if(pDoc->m_pDibInit->m_nColorTableEntries != 256)
//DEL 	{
//DEL 		// 提示用户
//DEL 		MessageBox("目前只支持256色位图的图象配准！", "系统提示" , 
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 		
//DEL 		// 返回
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	// 打开图象识别对话框
//DEL 	CDlgRecMatch* pDlg = new CDlgRecMatch(NULL, pDoc);
//DEL 	pDlg->DoModal();
//DEL 	
//DEL 	delete pDlg;
//DEL }

//DEL void CImageProcessingView::OnDegenerationInverse() 
//DEL {
//DEL 	// 图象的模糊
//DEL 		
//DEL 	// 更改光标形状
//DEL 	BeginWaitCursor();
//DEL 
//DEL 	// 获取文档
//DEL 	CImageProcessingDoc* pDoc = GetDocument();
//DEL 
//DEL 	//  获得图象CDib类的指针
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 
//DEL 	// 获得图象的头文件信息
//DEL 	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
//DEL 
//DEL 	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的图象模糊）
//DEL 	if (lpBMIH->biBitCount != 8)
//DEL 	{
//DEL 		// 提示用户
//DEL 		MessageBox("目前只支持256色位图的图象模糊！", "系统提示" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 						
//DEL 		// 返回
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	::DIBNoRestriction(pDib);
//DEL 
//DEL 	// 设置脏标记
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 		
//DEL 	// 更新视图
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL 
//DEL     // 恢复光标
//DEL 	EndWaitCursor();
//DEL 	
//DEL }

//DEL void CImageProcessingView::OnRestoreInverse() 
//DEL {
//DEL 	// 图象的逆滤波
//DEL 		
//DEL 	// 更改光标形状
//DEL 	BeginWaitCursor();
//DEL 
//DEL 	// 获取文档
//DEL 	CImageProcessingDoc* pDoc = GetDocument();
//DEL 
//DEL 	//  获得图象CDib类的指针
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 
//DEL 	// 获得图象的头文件信息
//DEL 	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
//DEL 
//DEL 	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的逆滤波）
//DEL 	if (lpBMIH->biBitCount != 8)
//DEL 	{
//DEL 		// 提示用户
//DEL 		MessageBox("目前只支持256色位图的逆滤波！", "系统提示" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 						
//DEL 		// 返回
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	::DIBInverseFilter(pDib);
//DEL 
//DEL 	// 设置脏标记
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 		
//DEL 	// 更新视图
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL 
//DEL     // 恢复光标
//DEL 	EndWaitCursor();
//DEL 	
//DEL }

//DEL void CImageProcessingView::OnDegenerationMotion() 
//DEL {
//DEL 	// 图象的运动模糊
//DEL 		
//DEL 	// 更改光标形状
//DEL 	BeginWaitCursor();
//DEL 
//DEL 	// 获取文档
//DEL 	CImageProcessingDoc* pDoc = GetDocument();
//DEL 
//DEL 	//  获得图象CDib类的指针
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 
//DEL 	// 获得图象的头文件信息
//DEL 	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
//DEL 
//DEL 	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的运动模糊）
//DEL 	if (lpBMIH->biBitCount != 8)
//DEL 	{
//DEL 		// 提示用户
//DEL 		MessageBox("目前只支持256色位图的运动模糊！", "系统提示" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 						
//DEL 		// 返回
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	::DIBMotionDegeneration(pDib);
//DEL 
//DEL 	// 设置脏标记
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 		
//DEL 	// 更新视图
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL 
//DEL     // 恢复光标
//DEL 	EndWaitCursor();
//DEL 	
//DEL }

//DEL void CImageProcessingView::OnRestoreMotion() 
//DEL {
//DEL 	// 运动模糊图象复原
//DEL 		
//DEL 	// 更改光标形状
//DEL 	BeginWaitCursor();
//DEL 
//DEL 	// 获取文档
//DEL 	CImageProcessingDoc* pDoc = GetDocument();
//DEL 
//DEL 	//  获得图象CDib类的指针
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 
//DEL 	// 获得图象的头文件信息
//DEL 	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
//DEL 
//DEL 	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的运动模糊复原）
//DEL 	if (lpBMIH->biBitCount != 8)
//DEL 	{
//DEL 		// 提示用户
//DEL 		MessageBox("目前只支持256色位图的运动模糊复原！", "系统提示" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 						
//DEL 		// 返回
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	::DIBMotionRestore(pDib);
//DEL 
//DEL 	// 设置脏标记
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 		
//DEL 	// 更新视图
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL 
//DEL     // 恢复光标
//DEL 	EndWaitCursor();
//DEL 	
//DEL }

//DEL void CImageProcessingView::OnDEGENERATIONWinner() 
//DEL {	
//DEL 	// 图象的加噪模糊
//DEL 		
//DEL 	// 更改光标形状
//DEL 	BeginWaitCursor();
//DEL 
//DEL 	// 获取文档
//DEL 	CImageProcessingDoc* pDoc = GetDocument();
//DEL 
//DEL 	//  获得图象CDib类的指针
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 
//DEL 	// 获得图象的头文件信息
//DEL 	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
//DEL 
//DEL 	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的加噪模糊）
//DEL 	if (lpBMIH->biBitCount != 8)
//DEL 	{
//DEL 		// 提示用户
//DEL 		MessageBox("目前只支持256色位图的加噪模糊！", "系统提示" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 						
//DEL 		// 返回
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	::DIBNoiseDegeneration(pDib);
//DEL 
//DEL 	// 设置脏标记
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 		
//DEL 	// 更新视图
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL 
//DEL     // 恢复光标
//DEL 	EndWaitCursor();
//DEL 	
//DEL }


//DEL void CImageProcessingView::OnRestoreWinner() 
//DEL {
//DEL 	// 图象的维纳滤波
//DEL 		
//DEL 	// 更改光标形状
//DEL 	BeginWaitCursor();
//DEL 
//DEL 	// 获取文档
//DEL 	CImageProcessingDoc* pDoc = GetDocument();
//DEL 
//DEL 	//  获得图象CDib类的指针
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 
//DEL 	// 获得图象的头文件信息
//DEL 	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
//DEL 
//DEL 	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的维纳滤波）
//DEL 	if (lpBMIH->biBitCount != 8)
//DEL 	{
//DEL 		// 提示用户
//DEL 		MessageBox("目前只支持256色位图的维纳滤波！", "系统提示" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 						
//DEL 		// 返回
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	::DIBWinnerFilter(pDib);
//DEL 
//DEL 	// 设置脏标记
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 		
//DEL 	// 更新视图
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL 
//DEL     // 恢复光标
//DEL 	EndWaitCursor();
//DEL 	
//DEL }

//DEL void CImageProcessingView::OnMoment() 
//DEL {
//DEL 	// 图象的pq阶力矩
//DEL 
//DEL 	// 更改光标形状
//DEL 	BeginWaitCursor();
//DEL 
//DEL 	// 获取文档
//DEL 	CImageProcessingDoc* pDoc = GetDocument();
//DEL 
//DEL 	//  获得图象CDib类的指针
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 
//DEL 	// 获得图象的头文件信息
//DEL 	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
//DEL 
//DEL 	// 判断是否是8-bpp位图
//DEL 	if (lpBMIH->biBitCount != 8)
//DEL 	{
//DEL 		// 提示用户
//DEL 		MessageBox("目前只支持256色位图！", "系统提示" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 						
//DEL 		// 返回
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	::DIBMOMENT(pDib);
//DEL 	
//DEL     // 恢复光标
//DEL 	EndWaitCursor();
//DEL 	
//DEL }

//DEL void CImageProcessingView::OnBarycentermoment() 
//DEL {
//DEL 	// 图象的重心矩
//DEL 
//DEL 	// 更改光标形状
//DEL 	BeginWaitCursor();
//DEL 
//DEL 	// 获取文档
//DEL 	CImageProcessingDoc* pDoc = GetDocument();
//DEL 
//DEL 	//  获得图象CDib类的指针
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 
//DEL 	// 获得图象的头文件信息
//DEL 	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
//DEL 
//DEL 	// 判断是否是8-bpp位图
//DEL 	if (lpBMIH->biBitCount != 8)
//DEL 	{
//DEL 		// 提示用户
//DEL 		MessageBox("目前只支持256色位图！", "系统提示" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 						
//DEL 		// 返回
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	::DIBBARYCENTERMOMENT(pDib);
//DEL 
//DEL 	// 恢复光标
//DEL 	EndWaitCursor();
//DEL 	
//DEL }

//DEL void CImageProcessingView::OnAnalysisHolenum() 
//DEL {
//DEL 	// 消去二值图象中小于阈值面积的区域
//DEL 		
//DEL 	// 更改光标形状
//DEL 	BeginWaitCursor();
//DEL 
//DEL 	// 获取文档
//DEL 	CImageProcessingDoc* pDoc = GetDocument();
//DEL 
//DEL 	//  获得图象CDib类的指针
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 
//DEL 	// 获得图象的头文件信息
//DEL 	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
//DEL 
//DEL 	// 判断是否是8-bpp位图
//DEL 	if (lpBMIH->biBitCount != 8)
//DEL 	{
//DEL 		// 提示用户
//DEL 		MessageBox("目前只支持256色位图的图象！", "系统提示" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 						
//DEL 		// 返回
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	::DIBHOLENUMBER(pDib);
//DEL 
//DEL 	// 设置脏标记
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 		
//DEL 	// 更新视图
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL 
//DEL     // 恢复光标
//DEL 	EndWaitCursor();
//DEL 	
//DEL }

//DEL void CImageProcessingView::OnStreetFramework() 
//DEL {
//DEL 	// 街区距离骨架提取
//DEL 		
//DEL 	// 更改光标形状
//DEL 	BeginWaitCursor();
//DEL 
//DEL 	// 获取文档
//DEL 	CImageProcessingDoc* pDoc = GetDocument();
//DEL 
//DEL 	//  获得图象CDib类的指针
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 
//DEL 	// 获得图象的头文件信息
//DEL 	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
//DEL 
//DEL 	// 判断是否是8-bpp位图
//DEL 	if (lpBMIH->biBitCount != 8)
//DEL 	{
//DEL 		// 提示用户
//DEL 		MessageBox("目前只支持256色位图！", "系统提示" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 						
//DEL 		// 返回
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	::DIBFREAMEWORK(pDib);
//DEL 
//DEL 	// 设置脏标记
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 		
//DEL 	// 更新视图
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL 
//DEL     // 恢复光标
//DEL 	EndWaitCursor();
//DEL 	
//DEL }

//DEL void CImageProcessingView::OnStreetTransform() 
//DEL {
//DEL 	// 二值图象的街区距离变换
//DEL 		
//DEL 	// 更改光标形状
//DEL 	BeginWaitCursor();
//DEL 
//DEL 	// 获取文档
//DEL 	CImageProcessingDoc* pDoc = GetDocument();
//DEL 
//DEL 	//  获得图象CDib类的指针
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 
//DEL 	// 获得图象的头文件信息
//DEL 	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
//DEL 
//DEL 	// 判断是否是8-bpp位图（
//DEL 	if (lpBMIH->biBitCount != 8)
//DEL 	{
//DEL 		// 提示用户
//DEL 		MessageBox("目前只支持256色位图！", "系统提示" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 						
//DEL 		// 返回
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	::DIBSTREETDIS(pDib);
//DEL 
//DEL 	// 设置脏标记
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 		
//DEL 	// 更新视图
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL 
//DEL     // 恢复光标
//DEL 	EndWaitCursor();
//DEL 	
//DEL }

//DEL void CImageProcessingView::OnFrameRestore() 
//DEL {
//DEL 	// 街区距离骨架复原
//DEL 		
//DEL 	// 更改光标形状
//DEL 	BeginWaitCursor();
//DEL 
//DEL 	// 获取文档
//DEL 	CImageProcessingDoc* pDoc = GetDocument();
//DEL 
//DEL 	//  获得图象CDib类的指针
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 
//DEL 	// 获得图象的头文件信息
//DEL 	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
//DEL 
//DEL 	// 判断是否是8-bpp位图
//DEL 	if (lpBMIH->biBitCount != 8)
//DEL 	{
//DEL 		// 提示用户
//DEL 		MessageBox("目前只支持256色位图！", "系统提示" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 						
//DEL 		// 返回
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	::DIBCHESSBOARDDISRESTORE(pDib);
//DEL 
//DEL 	// 设置脏标记
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 		
//DEL 	// 更新视图
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL 
//DEL     // 恢复光标
//DEL 	EndWaitCursor();
//DEL 	
//DEL }


//DEL void CImageProcessingView::OnTrace() 
//DEL {
//DEL 	// 二值图象边界跟踪
//DEL 
//DEL 	// 更改光标形状
//DEL 	BeginWaitCursor();
//DEL 
//DEL 	// 获取文档
//DEL 	CImageProcessingDoc* pDoc = GetDocument();
//DEL 
//DEL 	//  获得图象CDib类的指针
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 
//DEL 	// 获得图象的头文件信息
//DEL 	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
//DEL 
//DEL 	// 判断是否是8-bpp位图
//DEL 	if (lpBMIH->biBitCount != 8)
//DEL 	{
//DEL 		// 提示用户
//DEL 		MessageBox("目前只支持256色位图！", "系统提示" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 						
//DEL 		// 返回
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	::DIBTrace(pDib);
//DEL 
//DEL 	// 设置脏标记
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 		
//DEL 	// 更新视图
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL 
//DEL     // 恢复光标
//DEL 	EndWaitCursor();
//DEL 	
//DEL 	
//DEL }

//DEL void CImageProcessingView::OnOutline() 
//DEL {
//DEL 	// 二值图象边界提取
//DEL 
//DEL 	// 更改光标形状
//DEL 	BeginWaitCursor();
//DEL 
//DEL 	// 获取文档
//DEL 	CImageProcessingDoc* pDoc = GetDocument();
//DEL 
//DEL 	//  获得图象CDib类的指针
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 
//DEL 	// 获得图象的头文件信息
//DEL 	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
//DEL 
//DEL 	// 判断是否是8-bpp位图
//DEL 	if (lpBMIH->biBitCount != 8)
//DEL 	{
//DEL 		// 提示用户
//DEL 		MessageBox("目前只支持256色位图！", "系统提示" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 						
//DEL 		// 返回
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	::DIBOUTLINE(pDib);
//DEL 
//DEL 	// 设置脏标记
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 		
//DEL 	// 更新视图
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL 
//DEL     // 恢复光标
//DEL 	EndWaitCursor();
//DEL 	
//DEL }

//DEL void CImageProcessingView::OnViewBayer() 
//DEL {
//DEL 	// Bayer抖动法显示图象
//DEL 		
//DEL 	// 更改光标形状
//DEL 	BeginWaitCursor();
//DEL 
//DEL 	// 获取文档
//DEL 	CImageProcessingDoc* pDoc = GetDocument();
//DEL 
//DEL 	//  获得图象CDib类的指针
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 
//DEL 	// 获得图象的头文件信息
//DEL 	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
//DEL 
//DEL 	// 判断是否是8-bpp位图
//DEL 	if (lpBMIH->biBitCount != 8)
//DEL 	{
//DEL 		// 提示用户
//DEL 		MessageBox("目前只支持256色位图！", "系统提示" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 						
//DEL 		// 返回
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	::LimbPatternBayer(pDib);
//DEL 
//DEL 	// 设置脏标记
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 		
//DEL 	// 更新视图
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL 
//DEL     // 恢复光标
//DEL 	EndWaitCursor();
//DEL 	
//DEL }

//DEL void CImageProcessingView::OnVIEWFloydSteinberg() 
//DEL {
//DEL 	// Floyd-Steinberg抖动法显示图象
//DEL 		
//DEL 	// 更改光标形状
//DEL 	BeginWaitCursor();
//DEL 
//DEL 	// 获取文档
//DEL 	CImageProcessingDoc* pDoc = GetDocument();
//DEL 
//DEL 	//  获得图象CDib类的指针
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 
//DEL 	// 获得图象的头文件信息
//DEL 	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
//DEL 
//DEL 	// 判断是否是8-bpp位图
//DEL 	if (lpBMIH->biBitCount != 8)
//DEL 	{
//DEL 		// 提示用户
//DEL 		MessageBox("目前只支持256色位图！", "系统提示" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 						
//DEL 		// 返回
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	::DitherFloydSteinberg(pDib);
//DEL 
//DEL 	// 设置脏标记
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 		
//DEL 	// 更新视图
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL 
//DEL     // 恢复光标
//DEL 	EndWaitCursor();
//DEL 	
//DEL }

//DEL void CImageProcessingView::OnCodingHuffman() 
//DEL {
//DEL 	// 哈夫曼编码表
//DEL 	
//DEL 	// 获取文档
//DEL 	CImageProcessingDoc * pDoc = GetDocument();
//DEL 	
//DEL 	// 指向源图象象素的指针
//DEL 	unsigned char *	lpSrc;
//DEL 		
//DEL 	// 图象的高度和宽度
//DEL 	LONG	lHeight;
//DEL 	LONG	lWidth;
//DEL 	
//DEL 	// 图象每行的字节数
//DEL 	LONG	lLineBytes;
//DEL 	
//DEL 	// 图象象素总数
//DEL 	LONG	lCountSum;
//DEL 	
//DEL 	// 循环变量
//DEL 	LONG	i;
//DEL 	LONG	j;
//DEL 	
//DEL 	// 数组指针用来保存各个灰度值出现概率
//DEL 	double * dProba;
//DEL 	
//DEL 	// 当前图象颜色数目
//DEL 	int		nColorNum;
//DEL 
//DEL 	//  获得图象CDib类的指针
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 			
//DEL 	//图象数据的指针
//DEL 	LPBYTE  lpDIBBits = pDib->m_lpImage;
//DEL 
//DEL 	// 头文件信息
//DEL 	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
//DEL 
//DEL 	// 判断是否是8-bpp位图（只处理8-bpp位图的霍夫曼编码）
//DEL 	if (lpBMIH->biBitCount != 8)
//DEL 	{
//DEL 		// 提示用户
//DEL 		MessageBox("目前只支持256色位图的霍夫曼编码！", "系统提示" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 						
//DEL 		// 返回
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	// 更改光标形状
//DEL 	BeginWaitCursor();
//DEL 	
//DEL 	/********************************************************************
//DEL 	   开始计算各个灰度级出现的概率	
//DEL 
//DEL 	   如果需要对指定的序列进行哈夫曼编码,
//DEL 	   只要将这一步改成给各个灰度级概率赋值即可
//DEL 	**********************************************************************
//DEL 	*/
//DEL 	
//DEL 	//  由头文件信息得到图象的比特数，从而得到颜色信息
//DEL 	nColorNum = (int)pow(2,lpBMIH->biBitCount);
//DEL 
//DEL 	// 分配内存
//DEL 	dProba = new double[nColorNum];
//DEL 	
//DEL 	//得到图象的宽度和高度
//DEL 	CSize   SizeDim;
//DEL 	SizeDim = pDib->GetDimensions();
//DEL 	lWidth = SizeDim.cx;
//DEL 	lHeight = SizeDim.cy;
//DEL 
//DEL 	// 计算图象象素总数
//DEL 	lCountSum = lHeight * lWidth;
//DEL 	
//DEL 	//得到实际的Dib图象存储大小
//DEL 	CSize   SizeRealDim;
//DEL 	SizeRealDim = pDib->GetDibSaveDim();
//DEL 
//DEL 	// 计算图象每行的字节数
//DEL 	lLineBytes = SizeRealDim.cx;
//DEL 	
//DEL 	// 赋零值
//DEL 	for (i = 0; i < nColorNum; i ++)
//DEL 	{
//DEL 		dProba[i] = 0.0;
//DEL 	}
//DEL 	
//DEL 	// 计算各个灰度值的计数
//DEL 	for (i = 0; i < lHeight; i ++)
//DEL 	{
//DEL 		for (j = 0; j < lWidth; j ++)
//DEL 		{
//DEL 			// 指向图象指针
//DEL 			lpSrc = lpDIBBits + lLineBytes * i + j;
//DEL 			
//DEL 			// 计数加1
//DEL 			dProba[*(lpSrc)] = dProba[*(lpSrc)] + 1;
//DEL 		}
//DEL 	}
//DEL 		
//DEL 	
//DEL 	// 计算各个灰度值出现的概率
//DEL 	for (i = 0; i < nColorNum; i ++)
//DEL 	{
//DEL 		dProba[i] = dProba[i] / (FLOAT)lCountSum;
//DEL 	}
//DEL 	
//DEL 	/***************************************************
//DEL 	 构建霍夫曼编码的码表
//DEL 	 并用对话框显示霍夫曼码表
//DEL 	****************************************************/
//DEL 	
//DEL 	// 创建对话框
//DEL 	CDlgHuffman dlgCoding;
//DEL 	
//DEL 	// 初始化变量值
//DEL 	dlgCoding.dProba = dProba;
//DEL 	dlgCoding.nColorNum = nColorNum;
//DEL 	
//DEL 	// 显示对话框
//DEL 	dlgCoding.DoModal();
//DEL 		
//DEL 	// 恢复光标
//DEL 	EndWaitCursor();
//DEL 	
//DEL }

//DEL void CImageProcessingView::OnCodingShanfino() 
//DEL {
//DEL 	// 香农－弗诺编码表
//DEL 	
//DEL 	// 获取文档
//DEL 	CImageProcessingDoc * pDoc = GetDocument();
//DEL 	
//DEL 	// 指向源图象象素的指针
//DEL 	unsigned char *	lpSrc;
//DEL 		
//DEL 	// 图象的高度
//DEL 	LONG	lHeight;
//DEL 	LONG	lWidth;
//DEL 	
//DEL 	// 图象每行的字节数
//DEL 	LONG	lLineBytes;
//DEL 	
//DEL 	// 获取当前DIB颜色数目
//DEL 	int		nColorNum;
//DEL 	
//DEL 	// 图象象素总数
//DEL 	LONG	lCountSum;
//DEL 	
//DEL 	// 循环变量
//DEL 	LONG	i;
//DEL 	LONG	j;
//DEL 	
//DEL 	// 保存各个灰度值出现概率的数组指针
//DEL 	double * dProba;	
//DEL 
//DEL 	//  获得图象CDib类的指针
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 			
//DEL 	//图象数据的指针
//DEL 	LPBYTE  lpDIBBits = pDib->m_lpImage;
//DEL 
//DEL 	// 头文件信息
//DEL 	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
//DEL 
//DEL 	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的香农－费诺编码）
//DEL 	if (lpBMIH->biBitCount != 8)
//DEL 	{
//DEL 		// 提示用户
//DEL 		MessageBox("目前只支持256色位图的香农－费诺编码！", "系统提示" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 						
//DEL 		// 返回
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	// 更改光标形状
//DEL 	BeginWaitCursor();
//DEL 	
//DEL 	/******************************************************************************
//DEL 	// 开始计算各个灰度级出现的概率	
//DEL 	//
//DEL 	// 如果需要对指定的序列进行香农－弗诺编码,
//DEL 	//只要将这一步改成给各个灰度级概率赋值即可
//DEL 	*****************************************************************************
//DEL 	*/
//DEL 	//  灰度值总数的计算
//DEL 	nColorNum = (int)pow(2,lpBMIH->biBitCount);
//DEL 
//DEL 	// 分配内存
//DEL 	dProba = new double[nColorNum];
//DEL 	
//DEL 	//得到图象的宽度和高度
//DEL 	CSize   SizeDim;
//DEL 	SizeDim = pDib->GetDimensions();
//DEL 	lWidth = SizeDim.cx;
//DEL 	lHeight = SizeDim.cy;
//DEL 
//DEL 	// 计算图象象素总数
//DEL 	lCountSum = lHeight * lWidth;
//DEL 	
//DEL 	//得到实际的Dib图象存储大小
//DEL 	CSize   SizeRealDim;
//DEL 	SizeRealDim = pDib->GetDibSaveDim();
//DEL 
//DEL 	// 计算图象每行的字节数
//DEL 	lLineBytes = SizeRealDim.cx;
//DEL 
//DEL 	// 计算图象象素总数
//DEL 	lCountSum = lHeight * lWidth;
//DEL 	
//DEL 	// 重置计数为0
//DEL 	for (i = 0; i < nColorNum; i ++)
//DEL 	{
//DEL 		dProba[i] = 0.0;
//DEL 	}
//DEL 	
//DEL 	// 计算各个灰度值的计数（对于非256色位图，此处给数组dProba赋值方法将不同）
//DEL 	for (i = 0; i < lHeight; i ++)
//DEL 	{
//DEL 		for (j = 0; j < lWidth; j ++)
//DEL 		{
//DEL 			// 指向图象指针
//DEL 			lpSrc = lpDIBBits + lLineBytes * i + j;
//DEL 			
//DEL 			// 计数加1
//DEL 			dProba[*(lpSrc)] = dProba[*(lpSrc)]+ 1;
//DEL 		}
//DEL 	}
//DEL 	
//DEL 	
//DEL 	// 计算各个灰度值出现的概率
//DEL 	for (i = 0; i < nColorNum; i ++)
//DEL 	{
//DEL 		dProba[i] /= (double)lCountSum;
//DEL 	}
//DEL 	
//DEL 	/***************************************************
//DEL 	 构建香农－费诺编码的码表
//DEL 	 并用对话框显示香农－费诺码表
//DEL 	****************************************************/	
//DEL 	
//DEL 	// 创建对话框
//DEL 	CDlgShannon dlgPara;
//DEL 	
//DEL 	// 初始化变量值
//DEL 	dlgPara.m_dProba = dProba;
//DEL 	dlgPara.m_nColorNum = nColorNum;
//DEL 	
//DEL 	// 显示对话框
//DEL 	dlgPara.DoModal();
//DEL 
//DEL 	//释放内存
//DEL 	delete dProba;
//DEL 		
//DEL 	// 恢复光标
//DEL 	EndWaitCursor();
//DEL }

//DEL void CImageProcessingView::OnCodingArith() 
//DEL {
//DEL 	CDlgArith dlgCoding;
//DEL 	
//DEL 	// 显示对话框
//DEL 	dlgCoding.DoModal();
//DEL 	
//DEL }

//DEL void CImageProcessingView::OnCodingBitplane() 
//DEL {
//DEL 	// 创建对话框
//DEL 	CDlgBitPlane dlgCoding;	
//DEL 	
//DEL 	// 显示对话框
//DEL 	dlgCoding.DoModal();	
//DEL 	
//DEL 	BYTE bBitNum = dlgCoding.m_BItNumber;
//DEL 
//DEL 	// 获取文档
//DEL 	CImageProcessingDoc * pDoc = GetDocument();
//DEL 		
//DEL 	//  获得图象CDib类的指针
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 	
//DEL 	// 头文件信息
//DEL 	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
//DEL 
//DEL 	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的位平面分解）
//DEL 	if (lpBMIH->biBitCount != 8)
//DEL 	{
//DEL 		// 提示用户
//DEL 		MessageBox("目前只支持256色位图的位平面分解！", "系统提示" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 						
//DEL 		// 返回
//DEL 		return;
//DEL 	}
//DEL 	DIBBITPLANE(pDib,bBitNum);
//DEL 
//DEL 	// 设置脏标记
//DEL 	pDoc->SetModifiedFlag(TRUE);
//DEL 		
//DEL 	// 更新视图
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL 	
//DEL }

//DEL void CImageProcessingView::OnCodingWriteimg() 
//DEL {
//DEL 	// 对当前图象进行DPCM编码（存为IMG格式文件）
//DEL 	
//DEL 	// 获取文档
//DEL 	CImageProcessingDoc * pDoc = GetDocument();
//DEL 		
//DEL 	//  获得图象CDib类的指针
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 			
//DEL 	//图象数据的指针
//DEL 	LPBYTE  lpDIBBits = pDib->m_lpImage;
//DEL 
//DEL 	// 头文件信息
//DEL 	LPBITMAPINFOHEADER lpBMIH=pDib->m_lpBMIH;
//DEL 
//DEL 	// 判断是否是8-bpp位图（处理8-bpp位图的DPCM编码）
//DEL 	if (lpBMIH->biBitCount != 8)
//DEL 	{
//DEL 		// 提示用户
//DEL 		MessageBox("目前只支持256色位图的DPCM编码！", "系统提示" ,
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 						
//DEL 		// 返回
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	// 更改光标形状
//DEL 	BeginWaitCursor();
//DEL 	
//DEL 	// 文件保存路径
//DEL 	CString strFilePath;
//DEL 	
//DEL 	// 获取原始文件名
//DEL 	strFilePath = pDoc->GetPathName();
//DEL 	
//DEL 	// 更改后缀为IMG
//DEL 	if (strFilePath.Right(4).CompareNoCase(".BMP") == 0)
//DEL 	{	
//DEL 		strFilePath = strFilePath.Left(strFilePath.GetLength()-3) + "IMG";
//DEL 	}
//DEL 	else
//DEL 	{
//DEL 		strFilePath += ".IMG";
//DEL 	}
//DEL 
//DEL 	// 创建SaveAs对话框
//DEL 	CFileDialog dlg(FALSE, "IMG", strFilePath, 
//DEL 					OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
//DEL 		            "IMG图象文件 (*.IMG) | *.IMG|所有文件 (*.*) | *.*||", NULL);
//DEL 	
//DEL 	// 提示用户选择保存的路径
//DEL 	if (dlg.DoModal() != IDOK)
//DEL 	{
//DEL 		// 恢复光标
//DEL 		EndWaitCursor();
//DEL 	
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	// 获取用户指定的文件路径
//DEL 	strFilePath = dlg.GetPathName();
//DEL 	
//DEL 	// CFile和CFileException对象
//DEL 	CFile file;
//DEL 	CFileException fe;
//DEL 	
//DEL 	// 尝试创建指定的IMG文件
//DEL 	if (!file.Open(strFilePath, CFile::modeCreate |
//DEL 	  CFile::modeReadWrite | CFile::shareExclusive, &fe))
//DEL 	{
//DEL 		MessageBox("打开指定IMG文件时失败！", "系统提示" , 
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 	
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	// 调用WRITE2IMG()函数将当前的DIB保存为IMG文件
//DEL 	if (::WRITE2IMG(pDib, file))
//DEL 	{
//DEL 		MessageBox("成功保存为IMG文件！", "系统提示" , 
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 	}
//DEL 	else
//DEL 	{
//DEL 		MessageBox("保存为IMG文件失败！", "系统提示" , 
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 	}	
//DEL 		
//DEL 	// 恢复光标
//DEL 	EndWaitCursor();
//DEL 	
//DEL }


//DEL void CImageProcessingView::OnCodingLoadimg() 
//DEL {
//DEL 	// 读入IMG文件
//DEL 	
//DEL 	// 获取文档
//DEL 	CImageProcessingDoc * pDoc = GetDocument();
//DEL 		
//DEL 	//  获得图象CDib类的指针
//DEL 	CDib * pDib = pDoc->m_pDibInit;
//DEL 
//DEL 	// 文件路径
//DEL 	CString strFilePath;
//DEL 	
//DEL 	// 创建Open对话框
//DEL 	CFileDialog dlg(TRUE, "PCX", NULL,
//DEL 					OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
//DEL 					"IMG图象文件 (*.PCX) | *.IMG|所有文件 (*.*) | *.*||", NULL);
//DEL 	
//DEL 	// 提示用户选择保存的路径
//DEL 	if (dlg.DoModal() != IDOK)
//DEL 	{
//DEL 		// 返回
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	// 获取用户指定的文件路径
//DEL 	strFilePath = dlg.GetPathName();
//DEL 	
//DEL 	// CFile和CFileException对象
//DEL 	CFile file;
//DEL 	CFileException fe;
//DEL 	
//DEL 	// 尝试打开指定的PCX文件
//DEL 	if (!file.Open(strFilePath, CFile::modeRead | CFile::shareDenyWrite, &fe))
//DEL 	{
//DEL 		// 提示用户
//DEL 		MessageBox("打开指定PCX文件时失败！", "系统提示" , 
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 		
//DEL 		// 返回
//DEL 		return;
//DEL 	}
//DEL 	
//DEL 	// 更改光标形状
//DEL 	BeginWaitCursor();
//DEL 
//DEL 	// 调用LOADIMG()函数读取指定的IMG文件
//DEL 	BOOL Succ = LOADIMG(pDib, file);
//DEL 
//DEL 	if (Succ == TRUE)
//DEL 	{
//DEL 		// 提示用户
//DEL 		MessageBox("成功读取IMG文件！", "系统提示" , 
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 		
//DEL 	}
//DEL 	else
//DEL 	{
//DEL 		// 提示用户
//DEL 		MessageBox("读取IMG文件失败！", "系统提示" , 
//DEL 			MB_ICONINFORMATION | MB_OK);
//DEL 	}
//DEL 
//DEL 	// 更新视图
//DEL 	pDoc->UpdateAllViews(NULL);
//DEL 	
//DEL 	// 恢复光标
//DEL 	EndWaitCursor();
//DEL 	
//DEL }

