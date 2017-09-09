// MyDIPView.cpp : implementation of the CMyDIPView class
//

#include "stdafx.h"
#include "MyDIP.h"

#include "MyDIPDoc.h"
#include "MyDIPView.h"

#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/*****************************************************
* 
*   �������ƣ�
*      Template��
*
*    ������
*	   HDIB    hDIB         ��ͼ��ľ��
*      double  *tem         ��ָ��ģ���ָ��
*      int  tem_w           ��ģ��Ŀ��
*      int  tem_h           ��ģ��ĸ߶�
*      double xishu         ��ģ���ϵ��
*         
*    ���ܣ�
*	   ��ͼ�����ģ�����
*
*    ˵����
*	   Ϊ�����������ģ��Ŀ�Ⱥ͸߶ȶ�ӦΪ����
*******************************************************/
 HDIB Template(HDIB hDIB,double * tem ,int tem_w,int tem_h,double xishu)
{

    //ͳ���м�ֵ
    double sum;

    //ָ��ͼ����ʼλ�õ�ָ��
    BYTE *lpDIB=(BYTE*)::GlobalLock((HGLOBAL) hDIB);

	//ָ��������ʼλ�õ�ָ��
	BYTE *pScrBuff =(BYTE*)::FindDIBBits((char*)lpDIB);
   
	//��ȡͼ�����ɫ��Ϣ
    int numColors=(int) ::DIBNumColors((char *)lpDIB);

    //���ͼ����256ɫ����
     if (numColors!=256) 
	 {   
        //�������
	  	::GlobalUnlock((HGLOBAL) hDIB);

		//����
		return(hDIB);
	 }
    
    //��ָ��ͼ��������ʼλ�õ�ָ�룬��ֵ��ָ�����
    BYTE* oldbuf = pScrBuff;

    //ѭ������
    int i,j,m,n;

	int w, h, dw;

	//��ȡͼ��Ŀ��
	w = (int) ::DIBWidth((char *)lpDIB);
	
	//��ȡͼ��ĸ߶�
	h = (int) ::DIBHeight((char *)lpDIB);
	
	//����ͼ��ÿ�е��ֽ���
	dw = (w+3)/4*4;      
	
	//����һ����ԭͼ���С��ͬ��25ɫ�Ҷ�λͼ
    HDIB newhDIB=NewDIB(w,h,8);  
    
	//ָ���µ�λͼ��ָ��
	BYTE *newlpDIB=(BYTE*)::GlobalLock((HGLOBAL) newhDIB);

	//ָ���µ�λͼ��������ʼλ�õ�ָ�� 
    BYTE *destBuf = (BYTE*)FindDIBBits((char *)newlpDIB);
    
   //��ָ����ͼ��������ʼλ�õ�ָ�룬��ֵ��ָ�����
    BYTE *newbuf=destBuf; 
	
	//��ͼ�����ɨ��
   
	//�� 
    for(i=0;i<h;i++)
    {  
		//��
	   for(j=0;j<w;j++)
	   {   

		   //Ϊͳ�Ʊ�������ʼֵ
	       sum=0;

         //����ͼ���4���߿�����ر���ԭ�ҶȲ���
	     if( j<((tem_w-1)/2) || j>(w-(tem_w+1)/2) || i<((tem_h-1)/2) || i>(h-(tem_h+1)/2) )
	      *(newbuf+i*dw+j)=*(oldbuf+i*dw+j);
		 
         //�������������ؽ���ģ�����
		 else 
         { 

          //���㣨i,j������Ϊģ�������
          for(m=i-((tem_h-1)/2);m<=i+((tem_h-1)/2);m++)
          {
		     for(n=j-((tem_w-1)/2);n<=j+((tem_w-1)/2);n++)
		    
             //���Ե㣨i��j��Ϊ���ģ���ģ���С��ͬ�ķ�Χ�ڵ�������ģ�����λ�õ�ϵ��
			 //������˲����ε���
		     sum+=*(oldbuf+m*dw+n)* tem[(m-i+((tem_h-1)/2))*tem_w+n-j+((tem_w-1)/2)];
		  
		  }
          
		  //����������ܵ�ģ��ϵ��
          sum=(int)sum*xishu;

		  //�������ֵ
		  sum = fabs(sum);

		  //���С��0��ǿ�Ƹ�ֵΪ0
          if(sum<0)     
          sum=0;

		  //�������255��ǿ�Ƹ�ֵΪ255
          if(sum>255)
		  sum=255;

		  //������Ľ���ŵ��µ�λͼ����Ӧλ��
	      *(newbuf+i*dw+j)=sum;
		 }
	   }
	} 
   
	//�������
	::GlobalUnlock((HGLOBAL)hDIB);
    
	//�����µ�λͼ�ľ��
    return(newhDIB);
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
	// TODO: Add your command handler code here
   
	CMyDIPDoc* pDoc = GetDocument();

	//�趨ģ�����
	double tem[9]={1,2,1,
		           2,4,2,
				   1,2,1};

    //�趨ģ��ϵ��
    double  xishu = 0.0625;   

    //����ģ�����
	pDoc->m_hDIB =Template(pDoc->m_hDIB,tem ,3,3, xishu);

	Invalidate(TRUE);
}