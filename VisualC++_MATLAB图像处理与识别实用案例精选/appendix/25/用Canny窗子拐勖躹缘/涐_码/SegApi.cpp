#include "GlobalApi.h"
#include "stdafx.h"
#include "cdib.h"
#include "math.h"
#include <direct.h>
#include <complex>
using namespace std;

/*************************************************************************
 *
 * \�������ƣ�
 *   RegionSegFixThreshold()
 *
 * \�������:
 *   CDib * pDib					- ָ��CDib���ָ�룬����ԭʼͼ����Ϣ
 *   int nThreshold					- ����ָ����ֵ
 *
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   1���߼�����ʾ��Ӧ����Ϊǰ������0��ʾ����
 *   ��ֵ�ָ�Ĺؼ�����������ֵ��ѡȡ����ֵ��ѡȡһ��Ӧ����ʵ�ʵ�Ӧ�ö�
 *   ����趨��
 *
 *************************************************************************
 */
void RegionSegFixThreshold(CDib * pDib, int nThreshold)
{
	//����ͼ���������
	int y;

	//����ͼ��ĺ�����
	int x;

	//ͼ��ĳ����С
	CSize sizeImage		= pDib->GetDimensions();
	int nWidth			= sizeImage.cx		;
	int nHeight			= sizeImage.cy		;

	//ͼ���ڼ�����ڴ洢�е�ʵ�ʴ�С
	CSize sizeImageSave	= pDib->GetDibSaveDim();

	//ͼ�����ڴ���ÿһ������ռ�õ�ʵ�ʿռ�
	int nSaveWidth = sizeImageSave.cx;

	
	//ͼ�����ݵ�ָ��
	LPBYTE  pImageData = pDib->m_lpImage;

	for(y=0; y<nHeight ; y++ )
		for(x=0; x<nWidth ; x++ )
		{
			if( *(pImageData+y*nSaveWidth+x) < nThreshold)
				*(pImageData+y*nSaveWidth+x) = 0;
		}
}

/*************************************************************************
 *
 * \�������ƣ�
 *   RegionSegAdaptive()
 *
 * \�������:
 *   CDib * pDib					- ָ��CDib���ָ�룬����ԭʼͼ����Ϣ
 *
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   1���߼�����ʾ��Ӧ����Ϊǰ������0��ʾ����
 *   ��ֵ�ָ�Ĺؼ�����������ֵ��ѡȡ����ֵ��ѡȡһ��Ӧ����ʵ�ʵ�Ӧ�ö�
 *   ����趨���������У���ֵ���ǹ̶��ģ����Ǹ���ͼ�����ص�ʵ�����ʶ��趨�ġ�
 *   ���������ͼ��ֳ��ĸ���ͼ��Ȼ�����ÿ����ͼ��ľ�ֵ�����ݾ�ֵ������ֵ
 *   ��ֵֻ��Ӧ���ڶ�Ӧ����ͼ��
 *
 *************************************************************************
 */
void RegionSegAdaptive(CDib * pDib)
{
	//����ͼ���������
	int y;

	//����ͼ��ĺ�����
	int x;

	//ͼ��ĳ����С
	CSize sizeImage		= pDib->GetDimensions();
	int nWidth			= sizeImage.cx		;
	int nHeight			= sizeImage.cy		;

	//ͼ���ڼ�����ڴ洢�е�ʵ�ʴ�С
	CSize sizeImageSave	= pDib->GetDibSaveDim();

	//ͼ�����ڴ���ÿһ������ռ�õ�ʵ�ʿռ�
	int nSaveWidth = sizeImageSave.cx;

	//ͼ�����ݵ�ָ��
	LPBYTE  lpImage = pDib->m_lpImage;

	// �ֲ���ֵ
	int nThd[2][2] ;

	// ��ͼ���ƽ��ֵ
	int nLocAvg ;
	
	// ������ͼ�����ɨ�裺
	nLocAvg = 0 ;
	// y����
	for(y=0; y<nHeight/2 ; y++ )
	{
		// x����
		for(x=0; x<nWidth/2 ; x++ )
		{
			nLocAvg += lpImage[y*nSaveWidth + x];
		}
	}
	// �����ֵ
	nLocAvg /= ( (nHeight/2) * (nWidth/2) ) ;

	// ������ֵΪ��ͼ���ƽ��ֵ
	nThd[0][0] = nLocAvg ;

	// ������ͼ�����ɨ����зָ
	// y����
	for(y=0; y<nHeight/2 ; y++ )
	{
		// x����
		for(x=0; x<nWidth/2 ; x++ )
		{
			if(lpImage[y*nSaveWidth + x]<nThd[0][0])
				lpImage[y*nSaveWidth + x] = 255 ;
			else
			{
				lpImage[y*nSaveWidth + x] = 0 ;
			}
			
		}
	}

	// =============================================
	// ������ͼ�����ɨ�裺
	nLocAvg = 0 ;
	// y����
	for(y=nHeight/2; y<nHeight ; y++ )
	{
		// x����
		for(x=0; x<nWidth/2 ; x++ )
		{
			nLocAvg += lpImage[y*nSaveWidth + x];
		}
	}
	// �����ֵ
	nLocAvg /= ( (nHeight - nHeight/2) * (nWidth/2) ) ;

	// ������ֵΪ��ͼ���ƽ��ֵ
	nThd[1][0] = nLocAvg ;

	// ������ͼ�����ɨ����зָ
	// y����
	for(y=nHeight/2; y<nHeight ; y++ )
	{
		// x����
		for(x=0; x<nWidth/2 ; x++ )
		{
			if(lpImage[y*nSaveWidth + x]<nThd[1][0])
				lpImage[y*nSaveWidth + x] = 255 ;
			else
			{
				lpImage[y*nSaveWidth + x] = 0 ;
			}
			
		}
	}

	// =============================================
	// ������ͼ�����ɨ�裺
	nLocAvg = 0 ;
	// y����
	for(y=0; y<nHeight/2 ; y++ )
	{
		// x����
		for(x=nWidth/2; x<nWidth ; x++ )
		{
			nLocAvg += lpImage[y*nSaveWidth + x];
		}
	}
	// �����ֵ
	nLocAvg /= ( (nHeight/2) * (nWidth - nWidth/2) ) ;
	
	// ������ֵΪ��ͼ���ƽ��ֵ
	nThd[0][1] = nLocAvg ;

	// ������ͼ�����ɨ����зָ
	// y����
	for(y=0; y<nHeight/2 ; y++ )
	{
		// x����
		for(x=nWidth/2; x<nWidth ; x++ )
		{
			if(lpImage[y*nSaveWidth + x]<nThd[0][1])
				lpImage[y*nSaveWidth + x] = 255 ;
			else
			{
				lpImage[y*nSaveWidth + x] = 0 ;
			}
			
		}
	}

	// =============================================
	// ������ͼ�����ɨ�裺
	nLocAvg = 0 ;
	// y����
	for(y=nHeight/2; y<nHeight ; y++ )
	{
		// x����
		for(x=nWidth/2; x<nWidth ; x++ )
		{
			nLocAvg += lpImage[y*nSaveWidth + x];
		}
	}
	// �����ֵ
	nLocAvg /= ( (nHeight - nHeight/2) * (nWidth - nWidth/2) ) ;

	// ������ֵΪ��ͼ���ƽ��ֵ
	nThd[1][1] = nLocAvg ;

	// ������ͼ�����ɨ����зָ
	// y����
	for(y=nHeight/2; y<nHeight ; y++ )
	{
		// x����
		for(x=nWidth/2; x<nWidth ; x++ )
		{
			if(lpImage[y*nSaveWidth + x]<nThd[1][1])
				lpImage[y*nSaveWidth + x] = 255 ;
			else
			{
				lpImage[y*nSaveWidth + x] = 0 ;
			}
		}
	}
	
	// Ϊ����ʾ������ʾ���߼�1�ú�ɫ��ʾ���߼�0�ð�ɫ��ʾ
	for(y=0; y<nHeight ; y++ )
	{
		// x����
		for(x=0; x<nWidth ; x++ )
		{
			lpImage[y*nSaveWidth + x] = 255 - lpImage[y*nSaveWidth + x] ;
		}
	}
}

/*************************************************************************
 *
 * \�������ƣ�
 *   RobertsOperator()
 *
 * \�������:
 *   CDib * pDib		- ָ��CDib���ָ�룬����ԭʼͼ����Ϣ
 *   double * pdGrad	- ָ���ݶ����ݵ�ָ�룬����ͼ����ݶ���Ϣ
 *
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   Roberts����
 *
 *************************************************************************
 */
void RobertsOperator(CDib * pDib, double * pdGrad)
{
	// ����ͼ���������
	int y;

	// ����ͼ��ĺ�����
	int x;

	// ͼ��ĳ����С
	CSize sizeImage		= pDib->GetDimensions();
	int nWidth			= sizeImage.cx		;
	int nHeight			= sizeImage.cy		;

	// ͼ���ڼ�����ڴ洢�е�ʵ�ʴ�С
	CSize sizeImageSave	= pDib->GetDibSaveDim();

	// ͼ�����ڴ���ÿһ������ռ�õ�ʵ�ʿռ�
	int nSaveWidth = sizeImageSave.cx;

	// ͼ�����ݵ�ָ��
	LPBYTE  pImageData = pDib->m_lpImage;

	// ��ʼ��
	for(y=0; y<nHeight ; y++ )
		for(x=0 ; x<nWidth ; x++ )
		{
			*(pdGrad+y*nWidth+x)=0;
		}


	// ���濪ʼ����Roberts���ӽ��м��㣬Ϊ�˱�֤��������Ҫ��
	// ������λ��ͼ�����ݵ��ڲ������������ѭ����������
	// y<nHeight-1 ������y<nHeight����Ӧ��x����Ҳ��x<nWidth-1
	// ������x<nWidth

	//����������������ʾRoberts���ӵ�һ��ģ�����������ֵ
	int nUpLeft;
	int nDownRight;

	// ����������������ʾRoberts���ӵڶ���ģ�����������ֵ
	int nUpRight;
	int nDownLeft;

	// ����������������ʾRoberts���Ӽ���Ľ��
	int nValueOne;
	int nValueTwo;

	// ��ʱ����
	double dGrad;

	for(y=0; y<nHeight-1 ; y++ )
		for(x=0 ; x<nWidth-1 ; x++ )
		{
			// Roberts���ӵ�һ��ģ����Ҫ������ֵ
			nUpLeft		=*(pImageData+y*nSaveWidth+x) ; 
			nDownRight	=*(	pImageData+(y+1)*nSaveWidth+x+1	);
			nDownRight	*=-1;

			//Roberts���ӵĵ�һ��ģ�������
			nValueOne	=nUpLeft+nDownRight	;

			// Roberts���ӵڶ���ģ����Ҫ������ֵ
			nUpRight	=*(	pImageData+y*nSaveWidth+x+1	)		;
			nDownLeft	=*(	pImageData+(y+1)*nSaveWidth+x	);
			nDownLeft	*=-1;

			// Roberts���ӵĵڶ���ģ�������
			nValueTwo	=nUpRight+nDownLeft;

			// ��������ƫ������ƽ����
			dGrad=nValueOne*nValueOne + nValueTwo*nValueTwo;

			// ����
			dGrad=pow(dGrad,0.5);

			// ��������ŷʽ����
			*(pdGrad+y*nWidth+x)=dGrad;
		}
}
/*************************************************************************
 *
 * \�������ƣ�
 *   LaplacianOperator()
 *
 * \�������:
 *   CDib * pDib		- ָ��CDib���ָ�룬����ԭʼͼ����Ϣ
 *   double * pdGrad	- ָ���ݶ����ݵ�ָ�룬����ͼ����ݶ���Ϣ
 *
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   LaplacianOperator����,�Ƕ������ӣ�����Roberts����������Ҫ����ģ�����
 *   �ݶȣ�LaplacianOperator����ֻҪһ�����ӾͿ��Լ����ݶȡ�������Ϊ������
 *   ������Ϣ���������Ƚ�����
 *
 *************************************************************************
 */
void LaplacianOperator(CDib * pDib, double * pdGrad)
{
		// ����ͼ���������
	int y;

	// ����ͼ��ĺ�����
	int x;

	// ͼ��ĳ����С
	CSize sizeImage		= pDib->GetDimensions();
	int nWidth			= sizeImage.cx		;
	int nHeight			= sizeImage.cy		;

	// ͼ���ڼ�����ڴ洢�е�ʵ�ʴ�С
	CSize sizeImageSave	= pDib->GetDibSaveDim();

	// ͼ�����ڴ���ÿһ������ռ�õ�ʵ�ʿռ�
	int nSaveWidth = sizeImageSave.cx;

	// ͼ�����ݵ�ָ��
	LPBYTE  lpImage = pDib->m_lpImage;

	// ��ʼ��
	for(y=0; y<nHeight ; y++ )
		for(x=0 ; x<nWidth ; x++ )
		{
			*(pdGrad+y*nWidth+x)=0;
		}

	// ����ģ��ϵ��
	static int nWeight[3][3] ;
	nWeight[0][0] = -1 ;   
	nWeight[0][1] = -1 ;   
	nWeight[0][2] = -1 ;   
	nWeight[1][0] = -1 ;   
	nWeight[1][1] =  8 ;   
	nWeight[1][2] = -1 ;   
	nWeight[2][0] = -1 ;   
	nWeight[2][1] = -1 ;   
	nWeight[2][2] = -1 ;   



	//�������������ʾLaplacian��������ֵ
	int nTmp[3][3];
	
	// ��ʱ����
	double dGrad;

	// ģ��ѭ�����Ʊ���
	int yy ;
	int xx ;

	
	// ���濪ʼ����Laplacian���ӽ��м��㣬Ϊ�˱�֤��������Ҫ��
	// ������λ��ͼ�����ݵ��ڲ������������ѭ����������
	// y<nHeight-2 ������y<nHeight����Ӧ��x����Ҳ��x<nWidth-2
	// ������x<nWidth
	for(y=1; y<nHeight-2 ; y++ )
		for(x=1 ; x<nWidth-2 ; x++ )
		{
			dGrad = 0 ; 
			// Laplacian������Ҫ�ĸ�������ֵ

			// ģ���һ��
			nTmp[0][0] = lpImage[(y-1)*nSaveWidth + x - 1 ] ; 
			nTmp[0][1] = lpImage[(y-1)*nSaveWidth + x     ] ; 
			nTmp[0][2] = lpImage[(y-1)*nSaveWidth + x + 1 ] ; 

			// ģ��ڶ���
			nTmp[1][0] = lpImage[y*nSaveWidth + x - 1 ] ; 
			nTmp[1][1] = lpImage[y*nSaveWidth + x     ] ; 
			nTmp[1][2] = lpImage[y*nSaveWidth + x + 1 ] ; 

			// ģ�������
			nTmp[2][0] = lpImage[(y+1)*nSaveWidth + x - 1 ] ; 
			nTmp[2][1] = lpImage[(y+1)*nSaveWidth + x     ] ; 
			nTmp[2][2] = lpImage[(y+1)*nSaveWidth + x + 1 ] ; 
			
			// �����ݶ�
			for(yy=0; yy<3; yy++)
				for(xx=0; xx<3; xx++)
				{
					dGrad += nTmp[yy][xx] * nWeight[yy][xx] ;
				}
			
			// �ݶ�ֵд���ڴ�
			*(pdGrad+y*nWidth+x)=dGrad;
		}

}

/*************************************************************************
 *
 * \�������ƣ�
 *   RegionGrow()
 *
 * \�������:
 *   CDib * pDib					- ָ��CDib���ָ�룬����ԭʼͼ����Ϣ
 *   unsigned char * pUnRegion		- ָ���������������ָ��
 *
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   pUnRegionָ��ָ����������洢�����������Ľ��������1���߼�����ʾ
 *	 ��Ӧ����Ϊ��������0��ʾΪ����������
 *   ��������һ����������Ƚ���Ҫ�����⣺
 *		1. ���ӵ��ѡȡ
 *		2. ����׼��
 *		3. ��ֹ����
 *	 ������Ϊ��������������Ҫ�������������ÿ���������ĺû�ֱ�ӹ�ϵ��
 *	 ���������Ľ����
 *	 �����������ӵ�ѡȡΪͼ������ģ�����׼�����������ص�����ֵС��
 *	 nThreshold, ��ֹ������һֱ���е���û����������׼����Ҫ������ʱΪֹ
 *
 *************************************************************************
 */
void RegionGrow(CDib * pDib, unsigned char * pUnRegion, int nThreshold)
{
	static int nDx[]={-1,0,1,0};
	static int nDy[]={0,1,0,-1};

	// ����ͼ���������
//	int y;

	// ����ͼ��ĺ�����
//	int x;

	// ͼ��ĳ����С
	CSize sizeImage		= pDib->GetDimensions();
	int nWidth			= sizeImage.cx		;
	int nHeight			= sizeImage.cy		;

	// ͼ���ڼ�����ڴ洢�е�ʵ�ʴ�С
	CSize sizeImageSave	= pDib->GetDibSaveDim();

	// ͼ�����ڴ���ÿһ������ռ�õ�ʵ�ʿռ�
	int nSaveWidth = sizeImageSave.cx;

	// ��ʼ��
	memset(pUnRegion,0,sizeof(unsigned char)*nWidth*nHeight);

	// ���ӵ�
	int nSeedX, nSeedY;

	// �������ӵ�Ϊͼ�������
	nSeedX = nWidth /2 ;
	nSeedY = nHeight/2 ;

	// �����ջ���洢����
	int * pnGrowQueX ;
	int * pnGrowQueY ;
	
	// ����ռ�
	pnGrowQueX = new int [nWidth*nHeight];
	pnGrowQueY = new int [nWidth*nHeight];

	// ͼ�����ݵ�ָ��
	unsigned char *  pUnchInput =(unsigned char * )pDib->m_lpImage;
	
	// �����ջ�������յ�
	// ��nStart=nEnd, ��ʾ��ջ��ֻ��һ����
	int nStart ;
	int nEnd   ;

	//��ʼ��
	nStart = 0 ;
	nEnd   = 0 ;

	// �����ӵ������ѹ��ջ
	pnGrowQueX[nEnd] = nSeedX;
	pnGrowQueY[nEnd] = nSeedY;

	// ��ǰ���ڴ��������
	int nCurrX ;
	int nCurrY ;

	// ѭ�����Ʊ���
	int k ;

	// ͼ��ĺ�������,�����Ե�ǰ���ص�4������б���
	int xx;
	int yy;

	while (nStart<=nEnd)
	{
		// ��ǰ���ӵ������
		nCurrX = pnGrowQueX[nStart];
		nCurrY = pnGrowQueY[nStart];					

		// �Ե�ǰ���4������б���
		for (k=0; k<4; k++)	
		{	
			// 4�������ص�����
			xx = nCurrX+nDx[k];
			yy = nCurrY+nDy[k];
			
			// �ж�����(xx��yy) �Ƿ���ͼ���ڲ�
			// �ж�����(xx��yy) �Ƿ��Ѿ������
			// pUnRegion[yy*nWidth+xx]==0 ��ʾ��û�д���

			// �����������ж�����(xx��yy)�͵�ǰ����(nCurrX,nCurrY) ����ֵ��ľ���ֵ
			if (	(xx < nWidth) && (xx>=0) && (yy<nHeight) && (yy>=0) 
				    && (pUnRegion[yy*nWidth+xx]==0) 
					&& abs(pUnchInput[yy*nSaveWidth+xx] - pUnchInput[nCurrY*nSaveWidth+nCurrX])<nThreshold )
			{
				// ��ջ��β��ָ�����һλ
				nEnd++;

				// ����(xx��yy) ѹ��ջ
				pnGrowQueX[nEnd] = xx;
				pnGrowQueY[nEnd] = yy;

				// ������(xx��yy)���ó��߼�1��255��
				// ͬʱҲ���������ش����
				pUnRegion[yy*nWidth+xx] = 255 ;
			}
		}
		nStart++;
	}

	// �ͷ��ڴ�
	delete []pnGrowQueX;
	delete []pnGrowQueY;
    pnGrowQueX = NULL ;
	pnGrowQueY = NULL ;
}

void DFT_2D(CDib * pDib,double * pTrRstRpart, double * pTrRstIpart)
{
	double PI = 3.14159;
	//����ͼ���������
	int y;

	//����ͼ��ĺ�����
	int x;

	//Ƶ��ĺ�����
	int m;

	//Ƶ���������
	int n; 

	//ͼ��ĳ����С
	CSize sizeImage		= pDib->GetDimensions();
	int nWidth			= sizeImage.cx		;
	int nHeight			= sizeImage.cy		;

	//ͼ���ڼ�����ڴ洢�е�ʵ�ʴ�С
	CSize sizeImageSave	= pDib->GetDibSaveDim();

	int nSaveWidth = sizeImageSave.cx;

	//ͼ�����ݵ�ָ��
	LPBYTE  pImageData = pDib->m_lpImage;

	//��ʼ���������
	for(n=0; n<nHeight ; n++ )
		for(m=0 ; m<nWidth ; m++ )
		{
			*(	pTrRstRpart + n*nWidth + m	) =0;
			*(	pTrRstIpart + n*nWidth + m	) =0;
		}
	double fCosTable;
	double fSinTable;
	int	  nPxValue;

	fCosTable=0 ;
	nPxValue =0;

	double fTmpRstR;
	double fTmpRstI;
	for(n=0; n<nHeight ; n++ )
		for(m=0 ; m<nWidth ; m++ )
		{
			fTmpRstR=0;
			fTmpRstI=0;
			for(y=0; y<nHeight ; y++ )
				for(x=0 ; x<nWidth ; x++ )
				{
					fCosTable= cos(	2*PI*( ((double)m*x)/nWidth + ((double)n*y)/nHeight) ) ;
					fSinTable= sin(	-2*PI*( ((double)m*x)/nWidth + ((double)n*y)/nHeight) ) ;
					nPxValue = *(pImageData+ y*nSaveWidth + x )			;

					fTmpRstR+=fCosTable* nPxValue						;
					fTmpRstI+=fSinTable* nPxValue						;
				}
			*( pTrRstRpart + nWidth * n + m ) = fTmpRstR;
			*( pTrRstIpart + nWidth * n + m ) = fTmpRstI;
		}
}


void IDFT_2D(CDib * pDib,double * pTrRstRpart, double * pTrRstIpart)
{
	double PI = 3.14159;
	//����ͼ���������
	int y;

	//����ͼ��ĺ�����
	int x;

	//Ƶ��ĺ�����
	int m;

	//Ƶ���������
	int n; 

	//ͼ��ĳ����С
	CSize sizeImage		= pDib->GetDimensions();
	int nWidth			= sizeImage.cx		;
	int nHeight			= sizeImage.cy		;

	//ͼ���ڼ�����ڴ洢�е�ʵ�ʴ�С
	CSize sizeImageSave	= pDib->GetDibSaveDim();

	int nSaveWidth = sizeImageSave.cx;

	//ͼ�����ݵ�ָ��
	LPBYTE  pImageData = pDib->m_lpImage;

	double fCosTable;
	double fSinTable;
	fCosTable=0 ;
	fSinTable=0 ;

	double fTmpPxValue;
	double fRpartValue;
	double fIpartValue;
	fTmpPxValue=0;
	fRpartValue=0;
	fIpartValue=0;

	for(y=0; y<nHeight ; y++ )
		for(x=0 ; x<nWidth ; x++ )
		{
			fTmpPxValue=0;
			for(n=0; n<nHeight ; n++ )
				for(m=0 ; m<nWidth ; m++ )
				{
					fCosTable= cos(	2*PI*( ((double)m*x)/nWidth + ((double)n*y)/nHeight) ) ;
					fSinTable= sin(	2*PI*( ((double)m*x)/nWidth + ((double)n*y)/nHeight) ) ;
					fRpartValue=*(pTrRstRpart+ n*nHeight + m ) ;
					fIpartValue=*(pTrRstIpart+ n*nHeight + m ) ;
					
					fTmpPxValue+=fCosTable* fRpartValue-fSinTable*fIpartValue;
				}
			fTmpPxValue=fTmpPxValue/(nHeight*nWidth);
			*( pImageData + nSaveWidth * y + x) = (unsigned char) fTmpPxValue ;
		}
}

/*************************************************************************
 *
 * \�������ƣ�
 *   SobelOperator()
 *
 * \�������:
 *   CDib * pDib		  - ָ��CDib���ָ�룬����ԭʼͼ����Ϣ
 *   double * pdGrad	- ָ���ݶ����ݵ�ָ�룬����ͼ����ݶ���Ϣ
 *
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   Sobe����
 *
 *   ���б߽�ָ�
 *
 *************************************************************************
 */
void SobelOperator(CDib * pDib, double * pdGrad)
{
		// ����ͼ���������
	int y;
	
	// ����ͼ��ĺ�����
	int x;
	
	// ͼ��ĳ����С
	CSize sizeImage		= pDib->GetDimensions();
	int nWidth			= sizeImage.cx		;
	int nHeight			= sizeImage.cy		;
	
	// ͼ���ڼ�����ڴ洢�е�ʵ�ʴ�С
	CSize sizeImageSave	= pDib->GetDibSaveDim();
	
	// ͼ�����ڴ���ÿһ������ռ�õ�ʵ�ʿռ�
	int nSaveWidth = sizeImageSave.cx;
	
	// ͼ�����ݵ�ָ��
	LPBYTE  lpImage = pDib->m_lpImage;
	
	// ��ʼ��
	for(y=0; y<nHeight ; y++ )
		for(x=0 ; x<nWidth ; x++ )
		{
			*(pdGrad+y*nWidth+x)=0;
		}
		
		// ����ģ��ϵ��
		static int nWeight[2][3][3] ;
		nWeight[0][0][0] = -1 ;   
		nWeight[0][0][1] =  0 ;   
		nWeight[0][0][2] =  1 ;   
		nWeight[0][1][0] = -2 ;   
		nWeight[0][1][1] =  0 ;   
		nWeight[0][1][2] =  2 ;   
		nWeight[0][2][0] = -1 ;   
		nWeight[0][2][1] =  0 ;   
		nWeight[0][2][2] =  1 ;   
		
		nWeight[1][0][0] =  1 ;   
		nWeight[1][0][1] =  2 ;   
		nWeight[1][0][2] =  1 ;   
		nWeight[1][1][0] =  0 ;   
		nWeight[1][1][1] =  0 ;   
		nWeight[1][1][2] =  0 ;   
		nWeight[1][2][0] = -1 ;   
		nWeight[1][2][1] = -2 ;   
		nWeight[1][2][2] = -1 ;   
		
		
		
		//�������������ʾLaplacian��������ֵ
		int nTmp[3][3];
		
		// ��ʱ����
		double dGrad   ;
		double dGradOne;
		double dGradTwo;
		
		// ģ��ѭ�����Ʊ���
		int yy ;
		int xx ;
		
		
		// ���濪ʼ����Prewitt���ӽ��м��㣬Ϊ�˱�֤��������Ҫ��
		// ������λ��ͼ�����ݵ��ڲ������������ѭ����������
		// y<nHeight-1 ������y<nHeight����Ӧ��x����Ҳ��x<nWidth-1
		// ������x<nWidth
		for(y=1; y<nHeight-1 ; y++ )
			for(x=1 ; x<nWidth-1 ; x++ )
			{
				dGrad    = 0 ; 
				dGradOne = 0 ;
				dGradTwo = 0 ;
				// Laplacian������Ҫ�ĸ�������ֵ
				
				// ģ���һ��
				nTmp[0][0] = lpImage[(y-1)*nSaveWidth + x - 1 ] ; 
				nTmp[0][1] = lpImage[(y-1)*nSaveWidth + x     ] ; 
				nTmp[0][2] = lpImage[(y-1)*nSaveWidth + x + 1 ] ; 
				
				// ģ��ڶ���
				nTmp[1][0] = lpImage[y*nSaveWidth + x - 1 ] ; 
				nTmp[1][1] = lpImage[y*nSaveWidth + x     ] ; 
				nTmp[1][2] = lpImage[y*nSaveWidth + x + 1 ] ; 
				
				// ģ�������
				nTmp[2][0] = lpImage[(y+1)*nSaveWidth + x - 1 ] ; 
				nTmp[2][1] = lpImage[(y+1)*nSaveWidth + x     ] ; 
				nTmp[2][2] = lpImage[(y+1)*nSaveWidth + x + 1 ] ; 
				
				// �����ݶ�
				for(yy=0; yy<3; yy++)
					for(xx=0; xx<3; xx++)
					{
						dGradOne += nTmp[yy][xx] * nWeight[0][yy][xx] ;
						dGradTwo += nTmp[yy][xx] * nWeight[1][yy][xx] ;
					}
					dGrad = dGradOne*dGradOne + dGradTwo*dGradTwo  ;
					dGrad = sqrt(dGrad) ;
					// �ݶ�ֵд���ڴ�
					*(pdGrad+y*nWidth+x)=dGrad;
			}
}

/*************************************************************************
 *
 * \�������ƣ�
 *   PrewittOperator()
 *
 * \�������:
 *   CDib * pDib		  - ָ��CDib���ָ�룬����ԭʼͼ����Ϣ
 *   double * pdGrad	- ָ���ݶ����ݵ�ָ�룬����ͼ����ݶ���Ϣ
 *
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   Prewitt����
 *
 *   ���б߽�ָ�
 *
 *************************************************************************
 */
void PrewittOperator(CDib * pDib, double * pdGrad)
{
	// ����ͼ���������
	int y;
	
	// ����ͼ��ĺ�����
	int x;
	
	// ͼ��ĳ����С
	CSize sizeImage		= pDib->GetDimensions();
	int nWidth			= sizeImage.cx		;
	int nHeight			= sizeImage.cy		;
	
	// ͼ���ڼ�����ڴ洢�е�ʵ�ʴ�С
	CSize sizeImageSave	= pDib->GetDibSaveDim();
	
	// ͼ�����ڴ���ÿһ������ռ�õ�ʵ�ʿռ�
	int nSaveWidth = sizeImageSave.cx;
	
	// ͼ�����ݵ�ָ��
	LPBYTE  lpImage = pDib->m_lpImage;
	
	// ��ʼ��
	for(y=0; y<nHeight ; y++ )
		for(x=0 ; x<nWidth ; x++ )
		{
			*(pdGrad+y*nWidth+x)=0;
		}
		
		// ����ģ��ϵ��
		static int nWeight[2][3][3] ;
		nWeight[0][0][0] = -1 ;   
		nWeight[0][0][1] =  0 ;   
		nWeight[0][0][2] =  1 ;   
		nWeight[0][1][0] = -1 ;   
		nWeight[0][1][1] =  0 ;   
		nWeight[0][1][2] =  1 ;   
		nWeight[0][2][0] = -1 ;   
		nWeight[0][2][1] =  0 ;   
		nWeight[0][2][2] =  1 ;   
		
		nWeight[1][0][0] =  1 ;   
		nWeight[1][0][1] =  1 ;   
		nWeight[1][0][2] =  1 ;   
		nWeight[1][1][0] =  0 ;   
		nWeight[1][1][1] =  0 ;   
		nWeight[1][1][2] =  0 ;   
		nWeight[1][2][0] = -1 ;   
		nWeight[1][2][1] = -1 ;   
		nWeight[1][2][2] = -1 ;   
		
		
		
		//�������������ʾLaplacian��������ֵ
		int nTmp[3][3];
		
		// ��ʱ����
		double dGrad   ;
		double dGradOne;
		double dGradTwo;
		
		// ģ��ѭ�����Ʊ���
		int yy ;
		int xx ;
		
		
		// ���濪ʼ����Prewitt���ӽ��м��㣬Ϊ�˱�֤��������Ҫ��
		// ������λ��ͼ�����ݵ��ڲ������������ѭ����������
		// y<nHeight-1 ������y<nHeight����Ӧ��x����Ҳ��x<nWidth-1
		// ������x<nWidth
		for(y=1; y<nHeight-1 ; y++ )
			for(x=1 ; x<nWidth-1 ; x++ )
			{
				dGrad    = 0 ; 
				dGradOne = 0 ;
				dGradTwo = 0 ;
				// Laplacian������Ҫ�ĸ�������ֵ
				
				// ģ���һ��
				nTmp[0][0] = lpImage[(y-1)*nSaveWidth + x - 1 ] ; 
				nTmp[0][1] = lpImage[(y-1)*nSaveWidth + x     ] ; 
				nTmp[0][2] = lpImage[(y-1)*nSaveWidth + x + 1 ] ; 
				
				// ģ��ڶ���
				nTmp[1][0] = lpImage[y*nSaveWidth + x - 1 ] ; 
				nTmp[1][1] = lpImage[y*nSaveWidth + x     ] ; 
				nTmp[1][2] = lpImage[y*nSaveWidth + x + 1 ] ; 
				
				// ģ�������
				nTmp[2][0] = lpImage[(y+1)*nSaveWidth + x - 1 ] ; 
				nTmp[2][1] = lpImage[(y+1)*nSaveWidth + x     ] ; 
				nTmp[2][2] = lpImage[(y+1)*nSaveWidth + x + 1 ] ; 
				
				// �����ݶ�
				for(yy=0; yy<3; yy++)
					for(xx=0; xx<3; xx++)
					{
						dGradOne += nTmp[yy][xx] * nWeight[0][yy][xx] ;
						dGradTwo += nTmp[yy][xx] * nWeight[1][yy][xx] ;
					}
					dGrad = dGradOne*dGradOne + dGradTwo*dGradTwo  ;
					dGrad = sqrt(dGrad) ;
					// �ݶ�ֵд���ڴ�
					*(pdGrad+y*nWidth+x)=dGrad;
			}
}

/*************************************************************************
 *
 * \�������ƣ�
 *   EdgeTrack()
 *
 * \�������:
 *   CDib * pDib					- ָ��CDib���ָ�룬����ԭʼͼ����Ϣ
 *   unsigned char * pUnEdgeTrack	- ָ��߽���ٽ����ָ��
 *
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   pUnEdgeTrackָ��ָ����������洢�˱߽���ٵĽ��������1���߼�����ʾ
 *	 ��Ӧ����Ϊ�߽�㣬0��ʾΪ�Ǳ߽��
 *
 *   ���б߽�ָ�
 *
 *************************************************************************
 */
void EdgeTrack(CDib * pDib, unsigned char * pUnEdgeTrack)
{
	static int nDx[8]={-1,-1,-1, 0, 0, 1, 1, 1};
	static int nDy[8]={-1, 0, 1,-1, 1,-1, 0, 1};

	// ����ͼ���������
	int y;

	// ����ͼ��ĺ�����
	int x;

	// ͼ��ĳ����С
	CSize sizeImage		= pDib->GetDimensions();
	int nWidth			= sizeImage.cx		;
	int nHeight			= sizeImage.cy		;
	
	// ָ���ݶ����ݵ�ָ��
	double * pdGrad;

	// ����ͼ��Ĵ�С�����ڴ�ռ䣬�洢�ݶȼ���Ľ��
	pdGrad=new double[nHeight*nWidth];

    // ����Roberts�������ݶ�
	RobertsOperator(pDib, pdGrad);

	// ���嵱ǰ�����ݶ�ֵ
	double dCurrGrad = 0;
	
	// ��������ݶ�ֵ
	double dMaxGrad;

	// ���ó�ֵ
	dMaxGrad = 0;

	// ����ݶ�ֵ��Ӧ�����ص�����
	int nPx;
	int nPy;

	nPx = 0;
	nPy = 0;

	// ���ݶ����ֵ���ڵ����ص�����
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			dCurrGrad = pdGrad[y*nWidth + x] ;
			if( dMaxGrad< dCurrGrad )
			{
				dMaxGrad = dCurrGrad;
				nPx = x ;
				nPy = y ; 
			}
		}
	}

	// ��ʼ��
	memset(pUnEdgeTrack,0,sizeof(unsigned char)*nWidth*nHeight);
	dCurrGrad = pdGrad[nPy*nWidth + nPx] ;

	// �ӣ�nPx��nPy���㿪ʼ���б߽����
	pUnEdgeTrack[nPy*nWidth + nPx] = 255 ;

	// ѭ��������������ǰ���ص�8����
	int i ;

	int yy;
	int xx;
	
	int nDetX;
	int nDetY;

	while(dCurrGrad>10)
	{
		// ���õ�ǰ��Ϊ�߽��
		pUnEdgeTrack[nPy*nWidth + nPx] = 255 ;

		dMaxGrad = 0 ;
		for(i=0; i<8; i++)
		{
			nDetX=nDx[i];
			nDetY=nDy[i];
			y = nPy + nDetY;
			x = nPx + nDetX;

			// �ж��Ƿ���ͼ���ڲ�
			if(x>=0 && x<nWidth && y>=0 && y<nHeight)
			{
				if( ( pdGrad[y*nWidth + x] > dMaxGrad)  && ( pUnEdgeTrack[y*nWidth + x] == 0) )
				{
					dMaxGrad = pdGrad[y*nWidth + x] ;
					yy = y;
					xx = x;
				}
			}
		}
		// ��һ���߽����ݶȣ���������
		dCurrGrad = dMaxGrad ;
		nPy = yy;
		nPx = xx;
	}

	//�ͷ��ڴ�
	delete pdGrad;
	pdGrad = NULL;

}

/*************************************************************************
 *
 * \�������ƣ�
 *   MakeGauss()
 *
 * \�������:
 *   double sigma									        - ��˹�����ı�׼��
 *   double **pdKernel										- ָ���˹���������ָ��
 *   int *pnWindowSize										- ���ݵĳ���
 *
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   ���������������һ��һά�ĸ�˹�������������ݣ������ϸ�˹���ݵĳ���Ӧ
 *   �������޳��ģ�����Ϊ�˼���ļ򵥺��ٶȣ�ʵ�ʵĸ�˹����ֻ�������޳���
 *   pnWindowSize�������ݳ���
 *   
 *************************************************************************
 */
void MakeGauss(double sigma, double **pdKernel, int *pnWindowSize)
{
	// ѭ�����Ʊ���
	int i   ;
	
	// ��������ĵ�
	int nCenter;

	// �����ĳһ�㵽���ĵ�ľ���
	double  dDis  ; 

	double PI = 3.14159;
	// �м����
	double  dValue; 
	double  dSum  ;
	dSum = 0 ; 
	
	// ���鳤�ȣ����ݸ����۵�֪ʶ��ѡȡ[-3*sigma, 3*sigma]���ڵ����ݡ�
	// ��Щ���ݻḲ�Ǿ��󲿷ֵ��˲�ϵ��
	*pnWindowSize = 1 + 2 * ceil(3 * sigma);
	
	// ����
	nCenter = (*pnWindowSize) / 2;
	
	// �����ڴ�
	*pdKernel = new double[*pnWindowSize] ;
	
	for(i=0; i< (*pnWindowSize); i++)
	{
		dDis = (double)(i - nCenter);
		dValue = exp(-(1/2)*dDis*dDis/(sigma*sigma)) / (sqrt(2 * PI) * sigma );
		(*pdKernel)[i] = dValue ;
		dSum += dValue;
	}
	
	// ��һ��
	for(i=0; i<(*pnWindowSize) ; i++)
	{
		(*pdKernel)[i] /= dSum;
	}
}

/*************************************************************************
 *
 * \�������ƣ�
 *   GaussianSmooth()
 *
 * \�������:
 *   unsigned char * pUnchImg				- ָ��ͼ�����ݵ�ָ��
 *   int nWidth											- ͼ�����ݿ��
 *   int nHeight										- ͼ�����ݸ߶�
 *   double dSigma									- ��˹�����ı�׼��
 *   unsigned char * pUnchSmthdImg	- ָ�򾭹�ƽ��֮���ͼ������
 *
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   Ϊ����ֹ���������ø�˹�˲���ͼ������˲����˲��ȶ�x������У�Ȼ���
 *   y������С�
 *   
 *************************************************************************
 */
void GaussianSmooth(unsigned char *pUnchImg, int nWidth, int nHeight, 
										double sigma, unsigned char * pUnchSmthdImg)
{
	// ѭ�����Ʊ���
  int y;
	int x;
	
	int i;
	
	// ��˹�˲��������鳤��
	
	int nWindowSize;
	
	//  ���ڳ��ȵ�1/2
	int	nHalfLen;   
	
	// һά��˹�����˲���
	double *pdKernel ;
	
	// ��˹ϵ����ͼ�����ݵĵ��
	double  dDotMul     ;
	
	// ��˹�˲�ϵ�����ܺ�
	double  dWeightSum     ;          
  
	// �м����
	double * pdTmp ;
	
	// �����ڴ�
	pdTmp = new double[nWidth*nHeight];
	
	// ����һά��˹�����˲���
	// MakeGauss(sigma, &dKernel, &nWindowSize);
	MakeGauss(sigma, &pdKernel, &nWindowSize) ;
	
	// MakeGauss���ش��ڵĳ��ȣ����ô˱������㴰�ڵİ볤
	nHalfLen = nWindowSize / 2;
	
  // x��������˲�
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			dDotMul		= 0;
			dWeightSum = 0;
			for(i=(-nHalfLen); i<=nHalfLen; i++)
			{
				// �ж��Ƿ���ͼ���ڲ�
				if( (i+x) >= 0  && (i+x) < nWidth )
				{
					dDotMul += (double)pUnchImg[y*nWidth + (i+x)] * pdKernel[nHalfLen+i];
					dWeightSum += pdKernel[nHalfLen+i];
				}
			}
			pdTmp[y*nWidth + x] = dDotMul/dWeightSum ;
		}
	}
	
	// y��������˲�
	for(x=0; x<nWidth; x++)
	{
		for(y=0; y<nHeight; y++)
		{
			dDotMul		= 0;
			dWeightSum = 0;
			for(i=(-nHalfLen); i<=nHalfLen; i++)
			{
				// �ж��Ƿ���ͼ���ڲ�
				if( (i+y) >= 0  && (i+y) < nHeight )
				{
					dDotMul += (double)pdTmp[(y+i)*nWidth + x] * pdKernel[nHalfLen+i];
					dWeightSum += pdKernel[nHalfLen+i];
				}
			}
			pUnchSmthdImg[y*nWidth + x] = (unsigned char)(int)dDotMul/dWeightSum ;
		}
	}

	// �ͷ��ڴ�
	delete []pdKernel;
	pdKernel = NULL ;
	
	delete []pdTmp;
	pdTmp = NULL;
}


/*************************************************************************
 *
 * \�������ƣ�
 *   DirGrad()
 *
 * \�������:
 *   unsigned char *pUnchSmthdImg					- ������˹�˲����ͼ��
 *   int nWidht														- ͼ����
 *   int nHeight      										- ͼ��߶�
 *   int *pnGradX                         - x����ķ�����
 *   int *pnGradY                         - y����ķ�����
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   ����������㷽���������õ�΢��������(-1 0 1) �� (-1 0 1)'(ת��)
 *   �����ʱ��Ա߽����ز��������⴦��
 *   
 *   
 *************************************************************************
 */
void DirGrad(unsigned char *pUnchSmthdImg, int nWidth, int nHeight,
										 int *pnGradX , int *pnGradY)
{
	// ѭ�����Ʊ���
	int y ;
	int x ;
	
	// ����x����ķ��������ڱ߽�������˴�����ֹҪ���ʵ����س���
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			pnGradX[y*nWidth+x] = (int) ( pUnchSmthdImg[y*nWidth+min(nWidth-1,x+1)]  
													          - pUnchSmthdImg[y*nWidth+max(0,x-1)]     );
		}
	}

	// ����y����ķ��������ڱ߽�������˴�����ֹҪ���ʵ����س���
	for(x=0; x<nWidth; x++)
	{
		for(y=0; y<nHeight; y++)
		{
			pnGradY[y*nWidth+x] = (int) ( pUnchSmthdImg[min(nHeight-1,y+1)*nWidth + x]  
																		- pUnchSmthdImg[max(0,y-1)*nWidth+ x ]     );
		}
	}
}
/*************************************************************************
 *
 * \�������ƣ�
 *   GradMagnitude()
 *
 * \�������:
 *   int *pnGradX                         - x����ķ�����
 *   int *pnGradY                         - y����ķ�����
 *   int nWidht														- ͼ����
 *   int nHeight      										- ͼ��߶�
 *   int *pnMag                           - �ݶȷ���   
 *
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   ����������÷����������ݶȷ��ȣ���������DirGrad��������Ľ��
 *   
 *************************************************************************
 */
void GradMagnitude(int *pnGradX, int *pnGradY, int nWidth, int nHeight, int *pnMag)
{
	
	// ѭ�����Ʊ���
	int y ;
	int x ;

	// �м����
	double dSqtOne;
	double dSqtTwo;
	
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			dSqtOne = pnGradX[y*nWidth + x] * pnGradX[y*nWidth + x];
			dSqtTwo = pnGradY[y*nWidth + x] * pnGradY[y*nWidth + x];
			pnMag[y*nWidth + x] = (int)(sqrt(dSqtOne + dSqtTwo) + 0.5);
		}
	}
}
/*************************************************************************
 *
 * \�������ƣ�
 *   NonmaxSuppress()
 *
 * \�������:
 *   int *pnMag                - �ݶ�ͼ
 *   int *pnGradX							 - x����ķ�����	
 *   int *pnGradY              - y����ķ�����
 *   int nWidth                - ͼ�����ݿ��
 *   int nHeight               - ͼ�����ݸ߶�
 *   unsigned char *pUnchRst   - ����NonmaxSuppress�����Ľ��
 *
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   ��ֹ�ݶ�ͼ�зǾֲ���ֵ������ء�
 *   
 *************************************************************************
 */
void NonmaxSuppress(int *pnMag, int *pnGradX, int *pnGradY, int nWidth, 
										int nHeight,	unsigned char *pUnchRst)
{
	// ѭ�����Ʊ���
	int y ;
	int x ;
	int nPos;

	// x�����ݶȷ���
	int gx  ;
	int gy  ;

	// ��ʱ����
	int g1, g2, g3, g4 ;
	double weight  ;
	double dTmp1   ;
	double dTmp2   ;
	double dTmp    ;
	
	// ����ͼ���Ե����Ϊ�����ܵı߽��
	for(x=0; x<nWidth; x++)		
	{
		pUnchRst[x] = 0 ;
		pUnchRst[nHeight-1+x] = 0;
	}
	for(y=0; y<nHeight; y++)		
	{
		pUnchRst[y*nWidth] = 0 ;
		pUnchRst[y*nWidth + nWidth-1] = 0;
	}

	for(y=1; y<nHeight-1; y++)
	{
		for(x=1; x<nWidth-1; x++)
		{
			nPos = y*nWidth + x ;
			
			// �����ǰ���ص��ݶȷ���Ϊ0�����Ǳ߽��
			if(pnMag[nPos] == 0 )
			{
				pUnchRst[nPos] = 0 ;
			}
			else
			{
				// ��ǰ���ص��ݶȷ���
				dTmp = pnMag[nPos] ;
				
				// x��y������
				gx = pnGradX[nPos]  ;
				gy = pnGradY[nPos]  ;

				// ���������y������x������˵�������ķ�����ӡ�������y������
				if (abs(gy) > abs(gx)) 
				{
					// �����ֵ�ı���
					weight = fabs(gx)/fabs(gy); 

					g2 = pnMag[nPos-nWidth] ; 
					g4 = pnMag[nPos+nWidth] ;
					
					// ���x��y��������ķ������ķ�����ͬ
					// C�ǵ�ǰ���أ���g1-g4��λ�ù�ϵΪ��
					//	g1 g2 
					//		 C         
					//		 g4 g3 
					if (gx*gy > 0) 
					{ 					
						g1 = pnMag[nPos-nWidth-1] ;
						g3 = pnMag[nPos+nWidth+1] ;
					} 

					// ���x��y��������ķ������ķ����෴
					// C�ǵ�ǰ���أ���g1-g4��λ�ù�ϵΪ��
					//	   g2 g1
					//		 C         
					//	g3 g4  
					else 
					{ 
						g1 = pnMag[nPos-nWidth+1] ;
						g3 = pnMag[nPos+nWidth-1] ;
					} 
				}
				
				// ���������x������y������˵�������ķ�����ӡ�������x����
				// ����ж���������x������y������ȵ����
				else
				{
					// �����ֵ�ı���
					weight = fabs(gy)/fabs(gx); 
					
					g2 = pnMag[nPos+1] ; 
					g4 = pnMag[nPos-1] ;
					
					// ���x��y��������ķ������ķ�����ͬ
					// C�ǵ�ǰ���أ���g1-g4��λ�ù�ϵΪ��
					//	g3   
					//	g4 C g2       
					//       g1
					if (gx*gy > 0) 
					{				
						g1 = pnMag[nPos+nWidth+1] ;
						g3 = pnMag[nPos-nWidth-1] ;
					} 
					// ���x��y��������ķ������ķ����෴
					// C�ǵ�ǰ���أ���g1-g4��λ�ù�ϵΪ��
					//	     g1
					//	g4 C g2       
					//  g3     
					else 
					{ 
						g1 = pnMag[nPos-nWidth+1] ;
						g3 = pnMag[nPos+nWidth-1] ;
					}
				}

				// ��������g1-g4���ݶȽ��в�ֵ
				{
					dTmp1 = weight*g1 + (1-weight)*g2 ;
					dTmp2 = weight*g3 + (1-weight)*g4 ;
					
					// ��ǰ���ص��ݶ��Ǿֲ������ֵ
					// �õ�����Ǹ��߽��
					if(dTmp>=dTmp1 && dTmp>=dTmp2)
					{
						pUnchRst[nPos] = 128 ;
					}
					else
					{
						// �������Ǳ߽��
						pUnchRst[nPos] = 0 ;
					}
				}
			} //else
		} // for

	}
} 

/*************************************************************************
 *
 * \�������ƣ�
 *   TraceEdge()
 *
 * \�������:
 *   int    x									- ��������x���� 
 *   int    y									- ��������y����
 *   int nLowThd							- �ж�һ�����Ƿ�Ϊ�߽��ĵ���ֵ
 *   unsigned char *pUnchEdge - ��¼�߽��Ļ�����
 *   int *pnMag               - �ݶȷ���ͼ
 *   int nWidth               - ͼ�����ݿ��
 *
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   �ݹ����  
 *   ��(x,y)������������б߽��ĸ��٣�����ֻ����pUnchEdge��û�д�����
 *   �����Ǳ߽�����Щ����(=128),����ֵΪ0�����õ㲻�����Ǳ߽�㣬����ֵ
 *   Ϊ255�����õ��Ѿ�������Ϊ�߽�㣬�����ٿ���
 *   
 *   
 *************************************************************************
 */
void TraceEdge (int y, int x, int nLowThd, unsigned char *pUnchEdge, int *pnMag, int nWidth) 
{ 
	// ��8�������ؽ��в�ѯ
	int xNb[8] = {1, 1, 0,-1,-1,-1, 0, 1} ;
	int yNb[8] = {0, 1, 1, 1,0 ,-1,-1,-1} ;

	int yy ;
	int xx ;
	
	int k  ;
	
	for(k=0; k<8; k++)
	{
		yy = y + yNb[k] ;
		xx = x + xNb[k] ;
		// ���������Ϊ���ܵı߽�㣬��û�д����
		// �����ݶȴ�����ֵ
		if(pUnchEdge[yy*nWidth+xx] == 128  && pnMag[yy*nWidth+xx]>=nLowThd)
		{
			// �Ѹõ����ó�Ϊ�߽��
			pUnchEdge[yy*nWidth+xx] = 255 ;

			// �Ըõ�Ϊ���Ľ��и���
			TraceEdge(yy, xx, nLowThd, pUnchEdge, pnMag, nWidth);
		}
	}
} 

/*************************************************************************
 *
 * \�������ƣ�
 *   EstimateThreshold()
 *
 * \�������:
 *   int *pnMag               - �ݶȷ���ͼ
 *	 int nWidth               - ͼ�����ݿ��
 *	 int nHeight              - ͼ�����ݸ߶�
 *   int *pnThdHigh           - ����ֵ
 *   int *pnThdLow            - ����ֵ
 *	 double dRatioLow         - ����ֵ�͸���ֵ֮��ı���
 *	 double dRatioHigh        - ����ֵռͼ�����������ı���
 *   unsigned char *pUnchEdge - ����non-maximum����������
 *
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   ����non-maximum����������pUnchEdge��ͳ��pnMag��ֱ��ͼ��ȷ����ֵ��
 *   ��������ֻ��ͳ��pUnchEdge�п���Ϊ�߽�����Щ���ء�Ȼ������ֱ��ͼ��
 *   ����dRatioHigh���ø���ֵ���洢��pnThdHigh������dRationLow�͸���ֵ��
 *   ���õ���ֵ���洢��*pnThdLow��dRatioHigh��һ�ֱ����������ݶ�С��
 *   *pnThdHigh��������Ŀռ��������Ŀ�ı�����dRationLow����*pnThdHigh
 *   ��*pnThdLow�ı��������������canny�㷨��ԭ������߸�����һ�����䡣
 *
 *************************************************************************
 */
void EstimateThreshold(int *pnMag, int nWidth, int nHeight, int *pnThdHigh,int *pnThdLow, 
											 unsigned char * pUnchEdge, double dRatioHigh, double dRationLow) 
{ 
	// ѭ�����Ʊ���
	int y;
	int x;
	int k;
	
	// ������Ĵ�С���ݶ�ֵ�ķ�Χ�йأ�������ñ�������㷨����ô�ݶȵķ�Χ���ᳬ��pow(2,10)
	int nHist[1024] ;

	// ���ܵı߽���Ŀ
	int nEdgeNb     ;

	// ����ݶ�ֵ
	int nMaxMag     ;

	int nHighCount  ;

	nMaxMag = 0     ; 
	
	// ��ʼ��
	for(k=0; k<1024; k++) 
	{
		nHist[k] = 0; 
	}

	// ͳ��ֱ��ͼ��Ȼ������ֱ��ͼ������ֵ
	for(y=0; y<nHeight; y++)
	{
		for(x=0; x<nWidth; x++)
		{
			// ֻ��ͳ����Щ�����Ǳ߽�㣬���һ�û�д����������
			if(pUnchEdge[y*nWidth+x]==128)
			{
				nHist[ pnMag[y*nWidth+x] ]++;
			}
		}
	}

	nEdgeNb = nHist[0]  ;
	nMaxMag = 0         ;
	// ͳ�ƾ����������ֵ��ֹ(non-maximum suppression)�����ж�������
	for(k=1; k<1024; k++)
	{
		if(nHist[k] != 0)
		{
			// ����ݶ�ֵ
			nMaxMag = k;
		}
		
		// �ݶ�Ϊ0�ĵ��ǲ�����Ϊ�߽���
		// ����non-maximum suppression���ж�������
		nEdgeNb += nHist[k];
	}

	// �ݶȱȸ���ֵ*pnThdHighС�����ص�����Ŀ
	nHighCount = (int)(dRatioHigh * nEdgeNb +0.5);
	
	k = 1;
	nEdgeNb = nHist[1];
	
	// �������ֵ
	while( (k<(nMaxMag-1)) && (nEdgeNb < nHighCount) )
	{
		k++;
		nEdgeNb += nHist[k];
	}

	// ���ø���ֵ
	*pnThdHigh = k ;

	// ���õ���ֵ
	*pnThdLow  = (int)((*pnThdHigh) * dRationLow+ 0.5);
}

/*************************************************************************
 *
 * \�������ƣ�
 *   Hysteresis()
 *
 * \�������:
 *   int *pnMag               - �ݶȷ���ͼ
 *	 int nWidth               - ͼ�����ݿ��
 *	 int nHeight              - ͼ�����ݸ߶�
 *	 double dRatioLow         - ����ֵ�͸���ֵ֮��ı���
 *	 double dRatioHigh        - ����ֵռͼ�����������ı���
 *   unsigned char *pUnchEdge - ��¼�߽��Ļ�����
 *
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   ������ʵ�����ơ��������󡱵�һ�����ܣ�Ҳ���ǣ��ȵ���EstimateThreshold
 *   �����Ծ���non-maximum����������pUnchSpr����һ������ֵ��Ȼ���ж�
 *   pUnchSpr�п��ܵı߽�����(=128)���ݶ��ǲ��Ǵ��ڸ���ֵnThdHigh�������
 *   ����ֵ�󣬸õ㽫��Ϊһ���߽����㣬����TraceEdge�������Ѷ�Ӧ�ñ߽�
 *   �����������ҳ�������󣬵������������ʱ�������������û�б���־��
 *   �߽�㣬��ô��һ�����Ǳ߽�㡣
 *   
 *************************************************************************
 */
void Hysteresis(int *pnMag, int nWidth, int nHeight, double dRatioLow, 
								double dRatioHigh, unsigned char *pUnchEdge)
{
	// ѭ�����Ʊ���
	int y;
	int x;

	int nThdHigh ;
	int nThdLow  ;

	int nPos;

	// ����TraceEdge��Ҫ�ĵ���ֵ���Լ�Hysteresis����ʹ�õĸ���ֵ
	EstimateThreshold(pnMag, nWidth, nHeight, &nThdHigh, 
		               &nThdLow, pUnchEdge,dRatioHigh, dRatioLow);

  // ���ѭ������Ѱ�Ҵ���nThdHigh�ĵ㣬��Щ�㱻���������߽�㣬Ȼ����
	// TraceEdge���������ٸõ��Ӧ�ı߽�
   for(y=0; y<nHeight; y++)
	 {
      for(x=0; x<nWidth; x++)
			{
				nPos = y*nWidth + x ; 

				// ����������ǿ��ܵı߽�㣬�����ݶȴ��ڸ���ֵ����������Ϊ
				// һ���߽�����
				if((pUnchEdge[nPos] == 128) && (pnMag[nPos] >= nThdHigh))
				{
					// ���øõ�Ϊ�߽��
					pUnchEdge[nPos] = 255;
					TraceEdge(y, x, nThdLow, pUnchEdge, pnMag, nWidth);
				}
      }
   }

	 // ��Щ��û�б�����Ϊ�߽��������Ѿ������ܳ�Ϊ�߽��
   for(y=0; y<nHeight; y++)
	 {
		 for(x=0; x<nWidth; x++)
		 {
			 nPos = y*nWidth + x ; 
			 if(pUnchEdge[nPos] != 255)
			 {
				 // ����Ϊ�Ǳ߽��
				 pUnchEdge[nPos] = 0 ;
			 }
		 }
	 }
}


/*************************************************************************
 *
 * \�������ƣ�
 *   Canny()
 *
 * \�������:
 *   unsigned char *pUnchImage- ͼ������
 *	 int nWidth               - ͼ�����ݿ��
 *	 int nHeight              - ͼ�����ݸ߶�
 *   double sigma             - ��˹�˲��ı�׼����
 *	 double dRatioLow         - ����ֵ�͸���ֵ֮��ı���
 *	 double dRatioHigh        - ����ֵռͼ�����������ı���
 *   unsigned char *pUnchEdge - canny���Ӽ����ķָ�ͼ
 *
 * \����ֵ:
 *   ��
 *
 * \˵��:
 *   canny�ָ����ӣ�����Ľ��������pUnchEdge�У��߼�1(255)��ʾ�õ�Ϊ
 *   �߽�㣬�߼�0(0)��ʾ�õ�Ϊ�Ǳ߽�㡣�ú����Ĳ���sigma��dRatioLow
 *   dRatioHigh������Ҫָ���ġ���Щ������Ӱ��ָ��߽����Ŀ�Ķ���
 *************************************************************************
 */
void Canny(unsigned char *pUnchImage, int nWidth, int nHeight, double sigma,
					 double dRatioLow, double dRatioHigh, unsigned char *pUnchEdge)
{
	// ������˹�˲����ͼ������
	unsigned char * pUnchSmooth ;
  
	// ָ��x��������ָ��
	int * pnGradX ; 

	// ָ��y��������ָ��
	int * pnGradY ;

	// �ݶȵķ���
	int * pnGradMag ;

	pUnchSmooth  = new unsigned char[nWidth*nHeight] ;
	pnGradX      = new int [nWidth*nHeight]          ;
	pnGradY      = new int [nWidth*nHeight]          ;
	pnGradMag    = new int [nWidth*nHeight]          ;

	// ��ԭͼ������˲�
	GaussianSmooth(pUnchImage, nWidth, nHeight, sigma, pUnchSmooth) ;

	// ���㷽����
	DirGrad(pUnchSmooth, nWidth, nHeight, pnGradX, pnGradY) ;

	// �����ݶȵķ���
	GradMagnitude(pnGradX, pnGradY, nWidth, nHeight, pnGradMag) ;

	// Ӧ��non-maximum ����
	NonmaxSuppress(pnGradMag, pnGradX, pnGradY, nWidth, nHeight, pUnchEdge) ;

	// Ӧ��Hysteresis���ҵ����еı߽�
	Hysteresis(pnGradMag, nWidth, nHeight, dRatioLow, dRatioHigh, pUnchEdge);


	// �ͷ��ڴ�
	delete pnGradX      ;
	pnGradX      = NULL ;
	delete pnGradY      ;
	pnGradY      = NULL ;
	delete pnGradMag    ;
	pnGradMag    = NULL ;
	delete pUnchSmooth ;
	pUnchSmooth  = NULL ;
}
					