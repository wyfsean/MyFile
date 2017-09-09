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

/**************************************************
* �������ƣ�
*     ThinnerRosenfeld
*
* ������
*   void*     image             ����ֵ��ͼ�����ǰ��ɫΪ1����ɫΪ0
*   unsigned  longlx             ��ͼ��Ŀ��
*   unsigned  longly             ��ͼ��ĸ߶�
*
* ����ֵ
*       ��
*
*�������ܣ�
*       �������ͼ�����ϸ�������ϸ�����ͼ��
***********************************************************/
void ThinnerRosenfeld(void *image, unsigned long lx, unsigned long ly)
{
    char *f, *g;
    char n[10];
    char a[5] = {0, -1, 1, 0, 0};
    char b[5] = {0, 0, 0, 1, -1};
    char nrnd, cond, n48, n26, n24, n46, n68, n82, n123, n345, n567, n781;
    short k, shori;
    unsigned long i, j;
    long ii, jj, kk, kk1, kk2, kk3, size;
    size = (long)lx * (long)ly;

    g = (char *)malloc(size);
    if(g==NULL)
    {
        printf("error in alocating mmeory!\n");
        return;
    }

    f = (char *)image;
    for(kk=0l; kk<size; kk++)
    {
        g[kk] = f[kk];
    }

    do
    {
        shori = 0;
        for(k=1; k<=4; k++)
        {
            for(i=1; i<lx-1; i++)
            {
                ii = i + a[k];

                for(j=1; j<ly-1; j++)
                {
                    kk = i*ly + j;

                    if(!f[kk])
                        continue;

                    jj = j + b[k];
                    kk1 = ii*ly + jj;

                    if(f[kk1])
                        continue;

                    kk1 = kk - ly -1;
                    kk2 = kk1 + 1;
                    kk3 = kk2 + 1;
                    n[3] = f[kk1];
                    n[2] = f[kk2];
                    n[1] = f[kk3];
                    kk1 = kk - 1;
                    kk3 = kk + 1;
                    n[4] = f[kk1];
                    n[8] = f[kk3];
                    kk1 = kk + ly - 1;
                    kk2 = kk1 + 1;
                    kk3 = kk2 + 1;
                    n[5] = f[kk1];
                    n[6] = f[kk2];
                    n[7] = f[kk3];

                    nrnd = n[1] + n[2] + n[3] + n[4]
                        +n[5] + n[6] + n[7] + n[8];
                    if(nrnd<=1)
                        continue;

                    cond = 0;
                    n48 = n[4] + n[8];
                    n26 = n[2] + n[6];
                    n24 = n[2] + n[4];
                    n46 = n[4] + n[6];
                    n68 = n[6] + n[8];
                    n82 = n[8] + n[2];
                    n123 = n[1] + n[2] + n[3];
                    n345 = n[3] + n[4] + n[5];
                    n567 = n[5] + n[6] + n[7];
                    n781 = n[7] + n[8] + n[1];

                    if(n[2]==1 && n48==0 && n567>0)
                    {
                        if(!cond)
                            continue;
                        g[kk] = 0;
                        shori = 1;
                        continue;
                    }

                    if(n[6]==1 && n48==0 && n123>0)
                    {
                        if(!cond)
                            continue;
                        g[kk] = 0;
                        shori = 1;
                        continue;
                    }

                    if(n[8]==1 && n26==0 && n345>0)
                    {
                        if(!cond)
                            continue;
                        g[kk] = 0;
                        shori = 1;
                        continue;
                    }

                    if(n[4]==1 && n26==0 && n781>0)
                    {
                        if(!cond)
                            continue;
                        g[kk] = 0;
                        shori = 1;
                        continue;
                    }

                    if(n[5]==1 && n46==0)
                    {
                        if(!cond)
                            continue;
                        g[kk] = 0;
                        shori = 1;
                        continue;
                    }

                    if(n[7]==1 && n68==0)
                    {
                        if(!cond)
                            continue;
                        g[kk] = 0;
                        shori = 1;
                        continue;
                    }

                    if(n[1]==1 && n82==0)
                    {
                        if(!cond)
                            continue;
                        g[kk] = 0;
                        shori = 1;
                        continue;
                    }

                    if(n[3]==1 && n24==0)
                    {
                        if(!cond)
                            continue;
                        g[kk] = 0;
                        shori = 1;
                        continue;
                    }

                    cond = 1;
                    if(!cond)
                        continue;
                    g[kk] = 0;
                    shori = 1;
                }
            }

            for(i=0; i<lx; i++)
            {
                for(j=0; j<ly; j++)
                {
                    kk = i*ly + j;
                    f[kk] = g[kk];
                }
            }
        }
    }while(shori);

    free(g);
}              


void Thinning(HDIB hDIB)
{
	// ָ��DIB��ָ��
	LPSTR lpDIB=(LPSTR) ::GlobalLock((HGLOBAL)hDIB);
	
	// ָ��DIB����ָ��
	LPSTR    lpDIBBits;	

	// �ҵ�DIBͼ��������ʼλ��
	lpDIBBits = ::FindDIBBits(lpDIB);	
	
	// ָ��Դͼ���ָ��
	LPSTR	lpSrc;

	//ͼ��ĸ߶ȺͿ��
	LONG lWidth;
	LONG lHeight;

	//��ȡͼ��Ŀ��
	lWidth=::DIBWidth ((char*)lpDIB);

	//��ȡͼ��ĸ߶�
	lHeight=::DIBHeight ((char*)lpDIB);

    //����ÿ�е��ֽ���
    LONG  lLineBytes = (lWidth+3)*4/4;

	//ѭ������
    int i,j;

    //�����洢�����2ֵ����
	BYTE *image = (BYTE*)malloc(lWidth*lHeight*sizeof(BYTE));

	//��2ֵ����ֵ

	for(i=0;i<lHeight;i++)
	{
	    for(j=0;j<lWidth;j++)
		{
	      lpSrc=lpDIBBits+i*lLineBytes+j;
	    
	      if(*lpSrc==0)
	      image[i*lWidth+j]=1;

		  else
         image[i*lWidth+j]=0;
		}
	}

   //���ú�������ϸ��,���ֺ�������ѡ��

   ThinnerRosenfeld(image,lHeight,lWidth);

   //ThinnerHilditch(image,lHeight,lWidth);



   //�������ֵ��ԭͼ����

   for(i=0;i<lHeight;i++)
   {
     for(j=0;j<lWidth;j++)
	 {
	   lpSrc=lpDIBBits+i*lLineBytes+j;

       if(image[i*lWidth+j]==1)
	   *lpSrc=(BYTE)0;

	   else
       *lpSrc=(BYTE)255;
	 }
   
   }


 //����ڴ�
   free(image);
   ::GlobalUnlock ((HGLOBAL)hDIB);

	return;
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
	CMyDIPDoc* pDoc = GetDocument();
	Thinning(pDoc->m_hDIB);	
	Invalidate(TRUE);
}
