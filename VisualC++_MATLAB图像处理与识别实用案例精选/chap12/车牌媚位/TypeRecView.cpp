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
	// 显示等待光标
	BeginWaitCursor();
	
	// 获取文档
	CTypeRecDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	// 获取DIB
	HDIB hDIB = pDoc->GetHDIB();
	
	// 判断DIB是否为空
	if (hDIB != NULL)
	{
		LPSTR lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);
		
		// 获取DIB宽度
		int cxDIB = (int) ::DIBWidth(lpDIB);
		
		// 获取DIB高度
		int cyDIB = (int) ::DIBHeight(lpDIB);
		
		::GlobalUnlock((HGLOBAL) hDIB);
		
		CRect rcDIB;
		rcDIB.top = rcDIB.left = 0;
		rcDIB.right = cxDIB;
		rcDIB.bottom = cyDIB;
		
		CRect rcDest;
		
		// 判断是否是打印
		if (pDC->IsPrinting())
		{
			// 是打印，计算输出图像的位置和大小，以便符合页面
			
			// 获取打印页面的水平宽度(象素)
			int cxPage = pDC->GetDeviceCaps(HORZRES);
			
			// 获取打印页面的垂直高度(象素)
			int cyPage = pDC->GetDeviceCaps(VERTRES);
			
			// 获取打印机每英寸象素数
			int cxInch = pDC->GetDeviceCaps(LOGPIXELSX);
			int cyInch = pDC->GetDeviceCaps(LOGPIXELSY);
			
			// 计算打印图像大小（缩放，根据页面宽度调整图像大小）
			rcDest.top = rcDest.left = 0;
			rcDest.bottom = (int)(((double)cyDIB * cxPage * cyInch)
				/ ((double)cxDIB * cxInch));
			rcDest.right = cxPage;
			
			// 计算打印图像位置（垂直居中）
			int temp = cyPage - (rcDest.bottom - rcDest.top);
			rcDest.bottom += temp/2;
			rcDest.top += temp/2;
			
		}
		else   
			// 非打印
		{
			// 不必缩放图像
			rcDest = rcDIB;
		}
		
		// 输出DIB
		::PaintDIB(pDC->m_hDC, &rcDest, pDoc->GetHDIB(),
			&rcDIB, pDoc->GetDocPalette());
	}
	
	// 恢复正常光标
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
	// 主要是为了设置子窗体默认的背景色
	// 背景色由文档成员变量m_refColorBKG指定
	
	// 获取文档
	CTypeRecDoc* pDoc = GetDocument();
	
	// 创建一个Brush
	CBrush brush(pDoc->m_refColorBKG);                                              
	
	// 保存以前的Brush
	CBrush* pOldBrush = pDC->SelectObject(&brush);
	
	// 获取重绘区域
	CRect rectClip;
	pDC->GetClipBox(&rectClip);
	
	// 重绘
	pDC->PatBlt(rectClip.left, rectClip.top, rectClip.Width(), rectClip.Height(), PATCOPY);
	
	// 恢复以前的Brush
	pDC->SelectObject(pOldBrush);                                                  
	
	// 返回
	return TRUE;
	
}

void CTypeRecView::OnFILE256ToGray() 
{
	
	// 将256色位图转换成灰度图
	
	// 获取文档
	CTypeRecDoc* pDoc = GetDocument();
	
	// 指向DIB的指针
	LPSTR	lpDIB;
	
	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());
	
	// 颜色表中的颜色数目
	WORD wNumColors;
	
	// 获取DIB中颜色表中的颜色数目
	wNumColors = ::DIBNumColors(lpDIB);
	
	// 判断是否是8-bpp位图
	if (wNumColors != 256)
	{
		// 提示用户
		MessageBox("非256色位图！", "系统提示" , MB_ICONINFORMATION | MB_OK);
		
		// 解除锁定
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// 返回
		return;
	}
	
	// 更改光标形状
	BeginWaitCursor();
	
	// 指向DIB象素指针
	LPSTR   lpDIBBits;
	
	// 指向DIB象素的指针
	BYTE *	lpSrc;
	
	// 循环变量
	LONG	i;
	LONG	j;
	
	// 图像宽度
	LONG	lWidth;
	
	// 图像高度
	LONG	lHeight;
	
	// 图像每行的字节数
	LONG	lLineBytes;
	
	// 指向BITMAPINFO结构的指针（Win3.0）
	LPBITMAPINFO lpbmi;
	
	// 指向BITMAPCOREINFO结构的指针
	LPBITMAPCOREINFO lpbmc;
	
	// 表明是否是Win3.0 DIB的标记
	BOOL bWinStyleDIB;
	
	// 获取指向BITMAPINFO结构的指针（Win3.0）
	lpbmi = (LPBITMAPINFO)lpDIB;
	
	// 获取指向BITMAPCOREINFO结构的指针
	lpbmc = (LPBITMAPCOREINFO)lpDIB;
	
	// 灰度映射表
	BYTE bMap[256];
	
	// 判断是否是WIN3.0的DIB
	bWinStyleDIB = IS_WIN30_DIB(lpDIB);
	
	// 计算灰度映射表（保存各个颜色的灰度值），并更新DIB调色板
	for (i = 0; i < 256; i ++)
	{
		if (bWinStyleDIB)
		{
			// 计算该颜色对应的灰度值
			bMap[i] = (BYTE)(0.299 * lpbmi->bmiColors[i].rgbRed +
				0.587 * lpbmi->bmiColors[i].rgbGreen +
				0.114 * lpbmi->bmiColors[i].rgbBlue + 0.5);
			
			// 更新DIB调色板红色分量
			lpbmi->bmiColors[i].rgbRed =(unsigned char) i;
			
			// 更新DIB调色板绿色分量
			lpbmi->bmiColors[i].rgbGreen = (unsigned char)i;
			
			// 更新DIB调色板蓝色分量
			lpbmi->bmiColors[i].rgbBlue = (unsigned char)i;
			
			// 更新DIB调色板保留位
			lpbmi->bmiColors[i].rgbReserved = 0;
		}
		else
		{
			// 计算该颜色对应的灰度值
			bMap[i] = (BYTE)(0.299 * lpbmc->bmciColors[i].rgbtRed +
				0.587 * lpbmc->bmciColors[i].rgbtGreen +
				0.114 * lpbmc->bmciColors[i].rgbtBlue + 0.5);
			
			// 更新DIB调色板红色分量
			lpbmc->bmciColors[i].rgbtRed =(unsigned char)i;
			
			// 更新DIB调色板绿色分量
			lpbmc->bmciColors[i].rgbtGreen =(unsigned char) i;
			
			// 更新DIB调色板蓝色分量
			lpbmc->bmciColors[i].rgbtBlue = (unsigned char)i;
		}
	}
	
	// 找到DIB图像象素起始位置
	lpDIBBits = ::FindDIBBits(lpDIB);	
	
	// 获取图像宽度
	lWidth = ::DIBWidth(lpDIB);
	
	// 获取图像高度
	lHeight = ::DIBHeight(lpDIB);
	
	// 计算图像每行的字节数
	lLineBytes = WIDTHBYTES(lWidth * 8);
	
	// 更换每个象素的颜色索引（即按照灰度映射表换成灰度值）
	// 每行
	for(i = 0; i < lHeight; i++)
	{
		// 每列
		for(j = 0; j < lWidth; j++)
		{
			// 指向DIB第i行，第j个象素的指针
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
			
			// 变换
			*lpSrc = bMap[*lpSrc];
		}
	}
	
	// 替换当前调色板为灰度调色板
	pDoc->GetDocPalette()->SetPaletteEntries(0, 256, (LPPALETTEENTRY) ColorsTable[0]);
	
	// 设置脏标记
	pDoc->SetModifiedFlag(TRUE);
	
	// 实现新的调色板
	OnDoRealize((WPARAM)m_hWnd,0);
	
	// 更新视图
	pDoc->UpdateAllViews(NULL);
	
	// 解除锁定
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
	
	// 恢复光标
	EndWaitCursor();		
	
}

LRESULT CTypeRecView::OnDoRealize(WPARAM wParam, LPARAM)
{
	ASSERT(wParam != NULL);
	
	// 获取文档
	CTypeRecDoc* pDoc = GetDocument();
	
	// 判断DIB是否为空
	if (pDoc->GetHDIB() == NULL)
	{
		// 直接返回
		return 0L;
	}
	
	// 获取Palette
	CPalette* pPal = pDoc->GetDocPalette();
	if (pPal != NULL)
	{
		// 获取MainFrame
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
			TRACE0("\tCCh1_1View::OnPaletteChanged中调用SelectPalette()失败！\n");
		}
	}
	
	return 0L;
	
}

//24位真彩色图转换成256级灰度图
//
void CTypeRecView::OnFILE24ToGray()    
{
	CTypeRecDoc* pDoc = GetDocument(); //获取文档
	LPSTR lpDIB;                       //指向DIB的指针
	
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());
	
	ConvertToGrayScale(lpDIB);
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
	pDoc->SetModifiedFlag(TRUE);
	pDoc->UpdateAllViews(NULL);				
}

//剪裁指定区域图像
//
void CTypeRecView::OnTempSubrect() 
{
	
	CTypeRecDoc* pDoc = GetDocument();
	HDIB hDIB,hNewDIB;
	hDIB=pDoc->GetHDIB();
	
	long lWidth;                    //图像宽度和高度
	long lHeight;
	
	// 指向DIB的指针
	LPSTR	lpDIB;
	
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());//获得当前位图
	// 找到DIB图像象素起始位置
	lWidth = ::DIBWidth(lpDIB);   //DIB 宽度
	lHeight = ::DIBHeight(lpDIB); //DIB 高度
	
    //假定的剪裁区域(车牌附近)
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
	// 复制当前图像
	
	// 获取文档
	CTypeRecDoc* pDoc = GetDocument();
	
	// 打开剪贴板
	if (OpenClipboard())
	{
		// 更改光标形状
		BeginWaitCursor();
		
		// 清空剪贴板
		EmptyClipboard();
		
		// 复制当前图像到剪贴板
		SetClipboardData (CF_DIB, CopyHandle((HANDLE) pDoc->GetHDIB()) );
		
		// 关闭剪贴板
		CloseClipboard();
		
		// 恢复光标
		EndWaitCursor();
	}
	
}

void CTypeRecView::OnTempPalette() 
{
	CDC *pDC=GetDC();
	//	if((!pDC->GetDeviceCaps(RASTERCAPS)) & RC_PALETTE)
	//	{
	//		AfxMessageBox("当前显示系统不支持调色板。");
	//		return;
	//	}
	
	CString str;
	int nColorNum=pDC->GetDeviceCaps(SIZEPALETTE);
	str.Format("当前系统调色板可同时显示的颜色数为%d.",nColorNum);
	AfxMessageBox(str);
	
	int nColorReserved=pDC->GetDeviceCaps(NUMRESERVED);
	str.Format("当前系统调色板保留的颜色数为%d。",nColorReserved);
	AfxMessageBox(str);
	
	ReleaseDC(pDC);
	
	
}

//灰度转换测试
//

void CTypeRecView::OnTempGray() 
{
	
	CTypeRecDoc* pDoc = GetDocument();
	HDIB hDIB=pDoc->GetHDIB();
	
	long lWidth;                    //图像宽度和高度
	long lHeight;
	LONG lLineBytes;
	
	// 指向DIB的指针
	LPSTR	lpDIB;
	unsigned char * lpSrc;          //指向原图像象素点的指针
	
	// 指向DIB象素指针
	LPSTR   lpDIBBits;
	
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());//获得当前位图
	
	// 更改光标形状
	BeginWaitCursor();
	// 找到DIB图像象素起始位置
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// 找到DIB图像象素起始位置
	lWidth = ::DIBWidth(lpDIB);   //DIB 宽度
	lHeight = ::DIBHeight(lpDIB); //DIB 高度
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
		// 获取文档
		CTypeRecDoc* pDoc = GetDocument();
		
		  // 指向DIB的指针
		  LPSTR	lpDIB;
		  
			// 指向DIB象素指针
			LPSTR   lpDIBBits;
			
			  unsigned char * lpSrc;          //指向原图像象素点的指针
			  
				// 锁定DIB
				lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());
				
				  // 更改光标形状
				  BeginWaitCursor();
				  
					// 找到DIB图像象素起始位置
					lpDIBBits = ::FindDIBBits(lpDIB);
					long lWidth;                    //图像宽度和高度
					long lHeight;
					LONG lLineBytes;
					
					  long i,j;           //循环变量
					  
						lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());//获得当前位图
						// 找到DIB图像象素起始位置
						lWidth = ::DIBWidth(lpDIB);   //DIB 宽度
						lHeight = ::DIBHeight(lpDIB); //DIB 高度
						
						  lLineBytes=WIDTHBYTES(lWidth*8);
						  
							for(i = 0; i < lHeight; i++)
							{
							// 每列
							for(j =  0; j < lWidth; j++)
							{
							lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
							*lpSrc=0;
							}
							}
							
							  // 更改光标形状
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
	
	long lWidth;                    //图像宽度和高度
	long lHeight;
	LONG lLineBytes;
	
	// 指向DIB的指针
	LPSTR	lpDIB;
	unsigned char * lpSrc;          //指向原图像象素点的指针
	
	// 指向DIB象素指针
	LPSTR   lpDIBBits;
	
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());//获得当前位图
	
	// 更改光标形状
	BeginWaitCursor();
	// 找到DIB图像象素起始位置
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// 找到DIB图像象素起始位置
	lWidth = ::DIBWidth(lpDIB);   //DIB 宽度
	lHeight = ::DIBHeight(lpDIB); //DIB 高度
	lLineBytes=WIDTHBYTES(lWidth*8);	
	
	int i,j,pixel;
	
	long lHistogram[256];
	for(i=0;i<256;i++)
		lHistogram[i]=0;
	
	//获得直方图
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
		//找最大灰度值点为背景灰度值
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

//用一初始阈值T对图像A进行二值化得到二值化图像B,初始阈值T的确定方法是:选择阈值
//T=Gmax-(Gmax-Gmin)/3,Gmax和Gmin分别是最高、最低灰度值。
//该阈值对不同牌照有一定的适应性,能够保证背景基本被置为0,以突出牌照区域
//
void CTypeRecView::OnTest11() 
{
	CTypeRecDoc* pDoc=GetDocument();   //获得文档
	LPSTR lpDIB;                       //指向DIB的指针
	LPSTR lpDIBBits;                   //指向DIB的象素的指针
	
    LONG lLineBytes;
	unsigned char * lpSrc;             //指向原图像象素点的指针
	
    long lWidth;                       //图像宽度和高度
	long lHeight;
	
	long i,j;           //循环变量
	
	OnEditCopy();
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());//获得当前位图
	// 找到DIB图像象素起始位置
	lpDIBBits = ::FindDIBBits(lpDIB);
	lWidth = ::DIBWidth(lpDIB);   //DIB 宽度
	lHeight = ::DIBHeight(lpDIB); //DIB 高度
	// 计算图像每行的字节数
	lLineBytes = WIDTHBYTES(lWidth * 8);
	
	long lCount[256];
	for(i=0;i<256;i++)
	{
		lCount[i]=0;  //清零
	}
	if(::DIBNumColors(lpDIB) != 256)  //256色位图不作任何处理
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
	
	//求窗口变换的上限和下限
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
	// 阈值
	INT	bThre;
	
	bThre=(INT)((2*bUp+bLow)/3);
	
	// 更改光标形状
	BeginWaitCursor();
	
	// 调用ThresholdTrans()函数进行阈值变换
	ThresholdTrans(lpDIBBits, ::DIBWidth(lpDIB), ::DIBHeight(lpDIB), bThre);
	
	pDoc->SetModifiedFlag(TRUE);
	pDoc->UpdateAllViews(NULL);
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
	// 恢复光标
	EndWaitCursor();
}

//(2)削弱背景干扰。对图像B作简单的相邻像素灰度值相减,得到新的图像G,
//即Gi,j=|Pi,j-Pi,j-1|i=0,1,…,439;j=0,1,…,639Gi,0=Pi,0,左边缘直接赋值,
//不会影响整体效果。
//
void CTypeRecView::OnTest12() 
{
	
	CTypeRecDoc* pDoc=GetDocument();   //获得文档
	LPSTR lpDIB;                       //指向DIB的指针
	
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());//获得当前位图
	
	//用自定义的模板消弱背景干扰
	myTemplate(lpDIB);
	
	pDoc->SetModifiedFlag(TRUE);
	pDoc->UpdateAllViews(NULL);
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
	
}

//区域灰度基本被赋值为0(图2(f))。考虑到文字是由许多短竖线组成,
//而背景噪声有一大部分是孤立噪声,用模板(1,1,1,1,1)T对G进行中值滤波,
//得到除掉了大部分干扰的图像C。
//
void CTypeRecView::OnTest13() 
{
	// 中值滤波
	
	// 获取文档
	CTypeRecDoc* pDoc = GetDocument();
	
	// 指向DIB的指针
	LPSTR	lpDIB;
	
	// 指向DIB象素指针
	LPSTR   lpDIBBits;
	
	// 滤波器的高度
	int iFilterH;
	
	// 滤波器的宽度
	int iFilterW;
	
	// 中心元素的X坐标
	int iFilterMX;
	
	// 中心元素的Y坐标
	int iFilterMY;
	
	// 锁定DIB
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());
	
	// 找到DIB图像象素起始位置
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// 判断是否是8-bpp位图（这里为了方便，只处理8-bpp位图的中值滤波，其它的可以类推）
	if (::DIBNumColors(lpDIB) != 256)
	{
		// 提示用户
		MessageBox("目前只支持256色位图的中值滤波！", "系统提示" ,
			MB_ICONINFORMATION | MB_OK);
		
		// 解除锁定
		::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
		
		// 返回
		return;
	}
	
	// 创建对话框
	
	// 初始化变量值
	iFilterH = 5;
	iFilterW = 1;
	iFilterMX = 0;
	iFilterMY = 2;
	
	
	// 更改光标形状
	BeginWaitCursor();
	
	// 调用MedianFilter()函数中值滤波
	if (myMedianFilter(lpDIBBits, ::DIBWidth(lpDIB), ::DIBHeight(lpDIB), 
		iFilterH, iFilterW, iFilterMX, iFilterMY))
	{
		
		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);
		
		// 更新视图
		pDoc->UpdateAllViews(NULL);
	}
	else
	{
		// 提示用户
		MessageBox("分配内存失败！", "系统提示" , MB_ICONINFORMATION | MB_OK);
	}
	
	// 解除锁定
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());
	
	// 恢复光标
	EndWaitCursor();
	
}

//利用水平投影法检测车牌水平位置
//
void CTypeRecView::OnTest14() 
{
	CTypeRecDoc* pDoc=GetDocument();   //获得文档
	LPSTR lpDIB;                       //指向DIB的指针
    long lWidth;                       //图像宽度和高度
	long lHeight;
	
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());//获得当前位图
	lWidth = ::DIBWidth(lpDIB);   //DIB 宽度
	lHeight = ::DIBHeight(lpDIB); //DIB 高度
	
	//水平投影，求取车牌子图像的上下边缘位置
	//
	myHprojectDIB(lpDIB, lWidth, lHeight,&m_ipzTop, &m_ipzBottom) ;
	m_ipzLeft=0;
	m_ipzRight=lWidth;
	pDoc->UpdateAllViews(NULL);
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());	
	
	//对含车牌图像进行剪裁，得到车牌高度，原图像宽度的图像
	OnTempSubrect();
}

//利用垂直投影法检测车牌垂直位置
//
void CTypeRecView::OnTest15() 
{
	CTypeRecDoc* pDoc=GetDocument();   //获得文档
	LPSTR lpDIB;                       //指向DIB的指针
	
    long lWidth;                       //图像宽度和高度
	long lHeight;
	
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());//获得当前位图
	lWidth = ::DIBWidth(lpDIB);   //DIB 宽度
	lHeight = ::DIBHeight(lpDIB); //DIB 高度
	
	myVprojectDIB(lpDIB, lWidth, lHeight,&m_ipzLeft, &m_ipzRight) ;
	pDoc->UpdateAllViews(NULL);
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());		
	OnTempSubrect();
}

void CTypeRecView::OnEditPaste() 
{
	// 粘贴图像
	
	// 创建新DIB
	HDIB hNewDIB = NULL;
	
	// 打开剪贴板
	if (OpenClipboard())
	{
		// 更改光标形状
		BeginWaitCursor();
		
		// 读取剪贴板中的图像
		hNewDIB = (HDIB) CopyHandle(::GetClipboardData(CF_DIB));
		
		// 关闭剪贴板
		CloseClipboard();
		
		// 判断是否读取成功
		if (hNewDIB != NULL)
		{
			// 获取文档
			CTypeRecDoc* pDoc = GetDocument();
			
			// 替换DIB，同时释放旧DIB对象
			pDoc->ReplaceHDIB(hNewDIB);
			
			// 更新DIB大小和调色板
			pDoc->InitDIBData();
			
			// 设置脏标记
			pDoc->SetModifiedFlag(TRUE);
			
			// 重新设置滚动视图大小
			//	SetScrollSizes(MM_TEXT, pDoc->GetDocSize());
			
			// 实现新的调色板
			OnDoRealize((WPARAM)m_hWnd,0);
			
			// 更新视图
			pDoc->UpdateAllViews(NULL);
		}
		// 恢复光标
		EndWaitCursor();
	}	
}

//综合第4，5步骤
//
void CTypeRecView::OnTest145() 
{
	CTypeRecDoc* pDoc=GetDocument();   //获得文档
	LPSTR lpDIB;                       //指向DIB的指针
    long lWidth;                       //图像宽度和高度
	long lHeight;
	
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());//获得当前位图
	lWidth = ::DIBWidth(lpDIB);   //DIB 宽度
	lHeight = ::DIBHeight(lpDIB); //DIB 高度
	
	//水平投影，求取车牌子图像的上下边缘位置
	//
	myHprojectDIB(lpDIB, lWidth, lHeight,&m_ipzTop, &m_ipzBottom) ;
	m_ipzLeft=0;
	m_ipzRight=lWidth;
	
	//对含车牌图像进行剪裁，得到车牌高度，原图像宽度的图像
	//	OnTempSubrect();
	
	HDIB hDIB;
	HDIB hNewDIB;
	hDIB=pDoc->GetHDIB();
	
    //假定的剪裁区域(车牌附近)
	//
	CRect rect(m_ipzLeft,m_ipzTop,m_ipzRight,m_ipzBottom);
	hNewDIB= myCropDIB(hDIB,rect);
	
	// 判断是否剪裁成功
	if (hNewDIB != NULL)
	{
		// 获取文档
		CTypeRecDoc* pDoc = GetDocument();
		
		// 替换DIB，同时释放旧DIB对象
		pDoc->ReplaceHDIB(hNewDIB);
		
		// 更新DIB大小和调色板
		pDoc->InitDIBData();
		
		// 设置脏标记
		pDoc->SetModifiedFlag(TRUE);
		
		// 实现新的调色板
		OnDoRealize((WPARAM)m_hWnd,0);
		
	}
	
	//5
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) pDoc->GetHDIB());//获得当前位图
	lWidth = ::DIBWidth(lpDIB);   //DIB 宽度
	lHeight =::DIBHeight(lpDIB); //DIB 高度
	
	myVprojectDIB(lpDIB, lWidth, lHeight,&m_ipzLeft, &m_ipzRight) ;
	
	pDoc->UpdateAllViews(NULL);
	::GlobalUnlock((HGLOBAL) pDoc->GetHDIB());	
}

//截取车牌子图像
//
void CTypeRecView::OnTest16() 
{
	OnEditPaste();
	OnTempSubrect();
	OnEditPaste();	
}
