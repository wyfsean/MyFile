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

/****************************************************************
* 函数名称：
*      Convert256toGray()
*
* 参数：
*     HDIB hDIB     －图像的句柄
*
*  返回值：
*        无
*
*  功能：
*     将256色位图转化为灰度图
*
***************************************************************/

void Convert256toGray(HDIB hDIB)
{
	LPSTR	lpDIB;
	
	// 由DIB句柄得到DIB指针并锁定DIB
lpDIB = (LPSTR) ::GlobalLock((HGLOBAL)hDIB);
	
	// 指向DIB象素数据区的指针
	LPSTR   lpDIBBits;	

	// 指向DIB象素的指针
	BYTE *	lpSrc;	

	// 图像宽度
	LONG	lWidth;	
	// 图像高度
LONG  	lHeight;	

	// 图像每行的字节数
	LONG	lLineBytes;	

	// 指向BITMAPINFO结构的指针（Win3.0）
	LPBITMAPINFO lpbmi;	

	// 指向BITMAPCOREINFO结构的指针
	LPBITMAPCOREINFO lpbmc;
	
	// 获取指向BITMAPINFO结构的指针（Win3.0）
	lpbmi = (LPBITMAPINFO)lpDIB;	

	// 获取指向BITMAPCOREINFO结构的指针
	lpbmc = (LPBITMAPCOREINFO)lpDIB;	

	// 灰度映射表
	BYTE bMap[256];
	
	// 计算灰度映射表（保存各个颜色的灰度值），并更新DIB调色板
	int	i,j;
	for (i = 0; i < 256; i ++)
	{
		// 计算该颜色对应的灰度值
		bMap[i] = (BYTE)(0.299 * lpbmi->bmiColors[i].rgbRed +

					     0.587 * lpbmi->bmiColors[i].rgbGreen +

				         0.114 * lpbmi->bmiColors[i].rgbBlue + 0.5);			
		// 更新DIB调色板红色分量
		lpbmi->bmiColors[i].rgbRed = i;	
		
		// 更新DIB调色板绿色分量
		lpbmi->bmiColors[i].rgbGreen = i;	
		
		// 更新DIB调色板蓝色分量
		lpbmi->bmiColors[i].rgbBlue = i;
			
		// 更新DIB调色板保留位
		lpbmi->bmiColors[i].rgbReserved = 0;

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

//逐行扫描
for(i = 0; i < lHeight; i++)
{

  //逐列扫描
for(j = 0; j < lWidth; j++)
{
	// 指向DIB第i行，第j个象素的指针
	lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
			
	// 变换
	*lpSrc = bMap[*lpSrc];
}
}

//解除锁定
::GlobalUnlock ((HGLOBAL)hDIB);
}

/////////////////////////////////////////////////////////////////////////////
// CMyDIPView

IMPLEMENT_DYNCREATE(CMyDIPView, CScrollView)

BEGIN_MESSAGE_MAP(CMyDIPView, CScrollView)
	//{{AFX_MSG_MAP(CMyDIPView)
	ON_COMMAND(ID_MENUITEM32778, OnMenuitem32778)
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
	// 计算图像每行的字节数
	long lLineBytes = WIDTHBYTES(cxDIB * 8);
	// 每行
	for(i = 0; i < cyDIB; i++)
	{
		// 每列
		for(j = 0; j < cxDIB; j++)
		{
			// 指向DIB第i行，第j个象素的指针
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (cyDIB - 1 - i) + j;
			// 计算新的灰度值
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


//对比度拉伸
//DEL void CMyDIPView::OnMenuitem32777() 
//DEL {
//DEL 	
//DEL 	// 获取文档
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
//DEL 	long lLineBytes = WIDTHBYTES(cxDIB * 8);     // 计算图像每行的字节数
//DEL 	// 每行
//DEL 	for(i = 0; i < cyDIB; i++)
//DEL 	{
//DEL 		// 每列
//DEL 		for(j = 0; j < cxDIB; j++)
//DEL 		{
//DEL 			// 指向DIB第i行，第j个象素的指针
//DEL 			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (cyDIB - 1 - i) + j;
//DEL 			// 计算新的灰度值
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
	CMyDIPDoc* pDoc = GetDocument();
	Convert256toGray(pDoc->m_hDIB);	
	Invalidate(TRUE);
}
