
/**************************************************************************
*  �ļ�����suanfa1.cpp
*
*  ���ƶ�λ�����⣺
*	
*  myMedianFilter()		- ͼ����ֵ�˲���
*  myGetMedianNum()	    - ��ȡ��ֵ��������MedianFilter()����������ֵ��
*  ReplaceColorPal()	    - ����α��ɫ�����
*  ConvertToGrayScale();   - 24λ���ɫͼת���ɻҶ�ͼ
*  ThresholdTrans();       - ��ֵ�任
*  myTemplate();           - ͼ��ģ��任��ͨ���ı�ģ�壬��������ʵ��
*						      ͼ���ƽ�����񻯡���Եʶ��Ȳ�����
*  myHprojectDIB();        - ˮƽͶӰ
*  myVprojectDIB() ;       - ��ֱͶӰ 
*  myCropDIB();            - �������
*************************************************************************/

#include "stdafx.h"
#include "suanfa1.h"

#include <math.h>
#include <direct.h>

/*************************************************************************
*
* �������ƣ�
*   ConvertToGrayScale()
*
* ����:
*   LPSTR lpDIB			- ָ��ԴDIBͼ��ָ��
*   
* ����ֵ:
*   BOOL					- �ɹ�����TRUE�����򷵻�FALSE��
*
* ˵��:
*   �ú�����24λ���ɫͼת����256���Ҷ�ͼ
*
************************************************************************/
BOOL WINAPI ConvertToGrayScale(LPSTR lpDIB)   
{
	
	LPSTR lpDIBBits;                //ָ��DIB�����ص�ָ��
	LPSTR lpNewDIBBits;             //ָ��DIB�Ҷ�ͼͼ��(��ͼ��)��ʼ�����ص�ָ��
    LONG lLineBytes;
	unsigned char * lpSrc;          //ָ��ԭͼ�����ص��ָ��
	unsigned char * lpdest;         //ָ��Ŀ��ͼ�����ص��ָ��
	
    unsigned  char *ired,*igreen,*iblue;
	
    long lWidth;                    //ͼ���Ⱥ͸߶�
	long lHeight;
	
	long i,j;           //ѭ������
	
	lWidth = ::DIBWidth(lpDIB);   //DIB ���
	lHeight = ::DIBHeight(lpDIB); //DIB �߶�
	RGBQUAD  *lpRGBquad;
	lpRGBquad = (RGBQUAD *)&lpDIB[sizeof(BITMAPINFOHEADER)]; //INFOHEADER��Ϊ��ɫ��		
    if(::DIBNumColors(lpDIB) == 256)  //256ɫλͼ�����κδ���
	{
		return TRUE;
    }
	
	if(::DIBNumColors(lpDIB) != 256)  //��256ɫλͼ�����ҶȻ�
	{
		lLineBytes = WIDTHBYTES(lWidth*8*3);
		lpdest=  new  BYTE[lHeight*lWidth];
		lpDIBBits = (LPSTR)lpDIB + sizeof(BITMAPINFOHEADER);//ָ��DIB����
		for(i = 0;i < lHeight; i++)
			for(j = 0;j < lWidth*3; j+=3)
			{
				ired   =  (unsigned  char*)lpDIBBits + lLineBytes * i + j + 2;
				igreen =  (unsigned  char*)lpDIBBits + lLineBytes * i + j + 1;
				iblue  =  (unsigned  char*)lpDIBBits + lLineBytes * i +	j ;
				lpdest[i*lWidth + j/3]  = (unsigned  char)((*ired)*0.299 + (*igreen)*0.588 + (*iblue)*0.114);
			}
			
			//��Ҫ���������飺1���޸�INFOHEADER 2�����ӵ�ɫ�� 3���޸�ԭͼ��Ҷ�ֵ
			LPBITMAPINFOHEADER  lpBI;
			lpBI  =  (LPBITMAPINFOHEADER)lpDIB;
			lpBI->biBitCount = 8;
			
			//���ûҶȵ�ɫ��
			for(i = 0;i < 256;i++)
			{
				lpRGBquad[i].rgbRed   = (unsigned char)i;
				lpRGBquad[i].rgbGreen = (unsigned char)i;
				lpRGBquad[i].rgbBlue  = (unsigned char)i;
				lpRGBquad[i].rgbReserved = 0;
			}
			
			lpNewDIBBits= ::FindDIBBits(lpDIB);  //�ҵ�DIBͼ��������ʼλ��
			lLineBytes=WIDTHBYTES(lWidth  *  8);
			//�޸ĻҶ�ֵ
			for(i = 0;i < lHeight; i++)
				for(j = 0;j < lWidth; j++)
				{
					lpSrc = (unsigned  char*)lpNewDIBBits + lLineBytes * i+ j ;
					*lpSrc=lpdest[i*lWidth+j];
				}
				delete  lpdest;
	}	
	return true;
}

/*************************************************************************
*
* �������ƣ�
*   ThresholdTrans()
*
* ����:
*   LPSTR lpDIBBits    - ָ��ԴDIBͼ��ָ��
*   LONG  lWidth       - Դͼ���ȣ���������
*   LONG  lHeight      - Դͼ��߶ȣ���������
*   BYTE  bThre	    - ��ֵ
*
* ����ֵ:
*   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
*
* ˵��:
*   �ú���������ͼ�������ֵ�任�����ڻҶ�ֵС����ֵ������ֱ������
* �Ҷ�ֵΪ0���Ҷ�ֵ������ֵ������ֱ������Ϊ255��
*
************************************************************************/
BOOL WINAPI ThresholdTrans(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, BYTE bThre)
{
	
	// ָ��Դͼ���ָ��
	unsigned char*	lpSrc;
	
	// ѭ������
	LONG	i;
	LONG	j;
	
	// ͼ��ÿ�е��ֽ���
	LONG	lLineBytes;
	
	// ����ͼ��ÿ�е��ֽ���
	lLineBytes = WIDTHBYTES(lWidth * 8);
	
	// ÿ��
	for(i = 0; i < lHeight; i++)
	{
		// ÿ��
		for(j = 0; j < lWidth; j++)
		{
			// ָ��DIB��i�У���j�����ص�ָ��
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
			
			// �ж��Ƿ�С����ֵ
			if ((*lpSrc) < bThre)
			{
				// ֱ�Ӹ�ֵΪ0
				*lpSrc = 0;
			}
			else
			{
				// ֱ�Ӹ�ֵΪ255
				*lpSrc = 255;
			}
		}
	}
	
	// ����
	return TRUE;
}

/*************************************************************************
*
* �������ƣ�
*   Template()
*
* ����:
*   LPSTR lpDIBBits    - ָ��ԴDIBͼ��ָ��
*   LONG  lWidth       - Դͼ���ȣ���������
*   LONG  lHeight      - Դͼ��߶ȣ���������
*   int   iTempH		- ģ��ĸ߶�
*   int   iTempW		- ģ��Ŀ��
*   int   iTempMX		- ģ�������Ԫ��X���� ( < iTempW - 1)
*   int   iTempMY		- ģ�������Ԫ��Y���� ( < iTempH - 1)
*	 FLOAT * fpArray	- ָ��ģ�������ָ��
*	 FLOAT fCoef		- ģ��ϵ��
*
* ����ֵ:
*   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
*
* ˵��:
*   �ú�����ָ����ģ�壨�����С������ͼ����в���������iTempHָ��ģ��
* �ĸ߶ȣ�����iTempWָ��ģ��Ŀ�ȣ�����iTempMX��iTempMYָ��ģ�������
* Ԫ�����꣬����fpArrayָ��ģ��Ԫ�أ�fCoefָ��ϵ����
*
************************************************************************/

BOOL WINAPI myTemplate(LPSTR lpDIB)			
{
	// ָ����ͼ���ָ��
	LPSTR	lpNewDIBBits;
	HLOCAL	hNewDIBBits;
	
	// ָ��Ҫ���������ָ��
	unsigned char*	lpDst;
	
	LPSTR lpDIBBits;                   //ָ��DIB�����ص�ָ��
	
    LONG lLineBytes;                   // ͼ��ÿ�е��ֽ���
	unsigned char * lpSrc;             //ָ��ԭͼ�����ص��ָ��
	
    long lWidth;                       //ͼ���Ⱥ͸߶�
	long lHeight;
	
	// ������
	INT	fResult;
	
	// �ҵ�DIBͼ��������ʼλ��
	lpDIBBits = ::FindDIBBits(lpDIB);
	lWidth = ::DIBWidth(lpDIB);   //DIB ���
	lHeight = ::DIBHeight(lpDIB); //DIB �߶�
	
	// ����ͼ��ÿ�е��ֽ���
	lLineBytes = WIDTHBYTES(lWidth * 8);
	
	// ��ʱ�����ڴ棬�Ա�����ͼ��
	hNewDIBBits = LocalAlloc(LHND, lLineBytes * lHeight);
	
	// �ж��Ƿ��ڴ����ʧ��
	if (hNewDIBBits == NULL)
	{
		// �����ڴ�ʧ��
		return false;
	}
	
	// �����ڴ�
	lpNewDIBBits = (char * )LocalLock(hNewDIBBits);
	
	// ��ʼ��ͼ��Ϊԭʼͼ��
	memcpy(lpNewDIBBits, lpDIBBits, lLineBytes * lHeight);
	
	
	long i,j;           //ѭ������
	
	//
	//
	for(i = 0; i < lHeight-1; i++)
	{
		// ÿ��
		for(j = 0; j < lWidth-1; j++)
		{
			// ָ����DIB��i�У���j�����ص�ָ��
			lpDst = (unsigned char*)lpNewDIBBits + lLineBytes * (lHeight - 1 - i) + j;
			
			// ָ��DIB��i�У���j�����ص�ָ��
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
			
			fResult=(*lpSrc)-(*(lpSrc+1));
			
			// ȡ����ֵ
			if(fResult<0) fResult=-fResult;
			
			// �ж��Ƿ񳬹�255
			if(fResult > 255)
			{
				// ֱ�Ӹ�ֵΪ255
				* lpDst = 255;
			}
			else
			{
				// ��ֵ
				* lpDst = (unsigned char) (fResult + 0.5);
			}
			
		}
	}
	
	// ���Ʊ任���ͼ��
	memcpy(lpDIBBits, lpNewDIBBits, lLineBytes * lHeight);
	
	// �ͷ��ڴ�
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);
	
	// ����
	return TRUE;
	
}

/*************************************************************************
*
* �������ƣ�
*   myHprojectDIB()
*
* ����:
*   LPSTR lpDIBBits    - ָ��ԴDIBͼ��ָ��
*   LONG  lWidth       - Դͼ���ȣ���������
*   LONG  lHeight      - Դͼ��߶ȣ���������
*   INT*  iTop         - �����ϱ�Ե
*   INT*  iBottom      - �����±�Ե
* ����ֵ:
*   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
*
* ˵��:
*   �ú������ڶԺ�����ͼ�����ˮƽͶӰ���㣬��ȡ������ͼ������±�Եλ��
*
************************************************************************/
BOOL WINAPI myHprojectDIB(LPSTR lpDIB, LONG lWidth, LONG lHeight, 
						  int* iTop, int* iBottom) 
{
	LPSTR lpDIBBits;                   //ָ��DIB�����ص�ָ��
	
    LONG lLineBytes;                   // ͼ��ÿ�е��ֽ���
	unsigned char * lpSrc;             //ָ��ԭͼ�����ص��ָ��
	
	unsigned char pixel;				//����ֵ
	
	int i,j;
	
	// ������
	INT* iResult;
	
	INT pzBottom,pzTop;
	
	bool findPZ=false;
	
	// �ҵ�DIBͼ��������ʼλ��
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// ����ͼ��ÿ�е��ֽ���
	lLineBytes = WIDTHBYTES(lWidth * 8);
	
	iResult=new INT[lHeight];
	
	for(i=0;i<lHeight;i++)
		iResult[i]=0;
	
	for(j=lHeight/5;j<lHeight*0.8;j++)
	{
		iResult[j]=0;
		for(i=0;i<lWidth;i++)
		{
			lpSrc=(unsigned char*)lpDIBBits+lLineBytes*j+i;
			pixel=(unsigned char)(*lpSrc);
			if(pixel==255)
			{
				iResult[j]++;
			}
		}
		if((!findPZ)&&iResult[j]>12)
		{
			pzBottom=j;
			findPZ=true;
		}
		if(findPZ&&iResult[j]<12)
		{
			pzTop=j;
			break;
		}
		
	}
	pzTop=pzBottom+55;
	pzBottom=pzBottom-20;   //΢������
	*iBottom=lHeight-pzBottom;
	*iTop=lHeight-pzTop;
	return true;
}

/*************************************************************************
*
* �������ƣ�
*   myVprojectDIB()
*
* ����:
*   LPSTR lpDIBBits    - ָ��ԴDIBͼ��ָ��
*   LONG  lWidth       - Դͼ���ȣ���������
*   LONG  lHeight      - Դͼ��߶ȣ���������
*   INT*  iLeft        - �������Ե
*   INT*  iRight       - �����ұ�Ե
* ����ֵ:
*   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
*
* ˵��:
*   �ú������ڶԺ�����ͼ����д�ֱͶӰ���㣬��ȡ������ͼ������ұ�Եλ��
*
************************************************************************/
BOOL WINAPI myVprojectDIB(LPSTR lpDIB, LONG lWidth, LONG lHeight, 
						  int* iLeft, int* iRight) 
{
	LPSTR lpDIBBits;                   //ָ��DIB�����ص�ָ��
	
    LONG lLineBytes;                   // ͼ��ÿ�е��ֽ���
	unsigned char * lpSrc;             //ָ��ԭͼ�����ص��ָ��
	
	unsigned char pixel;				//����ֵ
	
	int i,j;
	
	// ������
	INT* iResult;
	
	INT pzLeft,pzRight;
	
	bool findPZ=false;
	
	// �ҵ�DIBͼ��������ʼλ��
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// ����ͼ��ÿ�е��ֽ���
	lLineBytes = WIDTHBYTES(lWidth * 8);
	
	iResult=new INT[lWidth];
	
	for(i=0;i<lWidth;i++)
		iResult[i]=0;
	
	for(i=lWidth/5;i<lWidth*0.8;i++)
	{
		iResult[i]=0;
		for(j=0;j<lHeight;j++)
		{
			lpSrc=(unsigned char*)lpDIBBits+lLineBytes*j+i;
			pixel=(unsigned char)(*lpSrc);
			if(pixel==255)
			{
				iResult[i]++;
			}
		}
		if((!findPZ)&&iResult[i]>10)
		{
			pzLeft=i;
			findPZ=true;
		}
		pzRight=pzLeft+150;
	}
	
	*iLeft=pzLeft-10;
	*iRight=pzRight+10;
	return true;
}

/*************************************************************************
*
* �������ƣ�
*   MedianFilter()
*
* ����:
*   LPSTR lpDIBBits		- ָ��ԴDIBͼ��ָ��
*   LONG  lWidth			- Դͼ���ȣ���������
*   LONG  lHeight			- Դͼ��߶ȣ���������
*   int   iFilterH			- �˲����ĸ߶�
*   int   iFilterW			- �˲����Ŀ��
*   int   iFilterMX		- �˲���������Ԫ��X����
*   int   iFilterMY		- �˲���������Ԫ��Y����
*
* ����ֵ:
*   BOOL					- �ɹ�����TRUE�����򷵻�FALSE��
*
* ˵��:
*   �ú�����DIBͼ�������ֵ�˲���
*
************************************************************************/

BOOL WINAPI myMedianFilter(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, 
						   int iFilterH, int iFilterW, 
						   int iFilterMX, int iFilterMY)
{
	
	// ָ��Դͼ���ָ��
	unsigned char*	lpSrc;
	
	// ָ��Ҫ���������ָ��
	unsigned char*	lpDst;
	
	// ָ����ͼ���ָ��
	LPSTR			lpNewDIBBits;
	HLOCAL			hNewDIBBits;
	
	// ָ���˲��������ָ��
	unsigned char	* aValue;
	HLOCAL			hArray;
	
	// ѭ������
	LONG			i;
	LONG			j;
	LONG			k;
	LONG			l;
	
	// ͼ��ÿ�е��ֽ���
	LONG			lLineBytes;
	
	// ����ͼ��ÿ�е��ֽ���
	lLineBytes = WIDTHBYTES(lWidth * 8);
	
	// ��ʱ�����ڴ棬�Ա�����ͼ��
	hNewDIBBits = LocalAlloc(LHND, lLineBytes * lHeight);
	
	// �ж��Ƿ��ڴ����ʧ��
	if (hNewDIBBits == NULL)
	{
		// �����ڴ�ʧ��
		return FALSE;
	}
	
	// �����ڴ�
	lpNewDIBBits = (char * )LocalLock(hNewDIBBits);
	
	// ��ʼ��ͼ��Ϊԭʼͼ��
	memcpy(lpNewDIBBits, lpDIBBits, lLineBytes * lHeight);
	
	// ��ʱ�����ڴ棬�Ա����˲�������
	hArray = LocalAlloc(LHND, iFilterH * iFilterW);
	
	// �ж��Ƿ��ڴ����ʧ��
	if (hArray == NULL)
	{
		// �ͷ��ڴ�
		LocalUnlock(hNewDIBBits);
		LocalFree(hNewDIBBits);
		
		// �����ڴ�ʧ��
		return FALSE;
	}
	
	// �����ڴ�
	aValue = (unsigned char * )LocalLock(hArray);
	
	// ��ʼ��ֵ�˲�
	// ��(��ȥ��Ե����)
	for(i = iFilterMY; i < lHeight - iFilterH + iFilterMY + 1; i++)
	{
		// ��(��ȥ��Ե����)
		for(j = iFilterMX; j < lWidth - iFilterW + iFilterMX + 1; j++)
		{
			// ָ����DIB��i�У���j�����ص�ָ��
			lpDst = (unsigned char*)lpNewDIBBits + lLineBytes * (lHeight - 1 - i) + j;
			
			// ��ȡ�˲�������
			for (k = 0; k < iFilterH; k++)
			{
				for (l = 0; l < iFilterW; l++)
				{
					// ָ��DIB��i - iFilterMY + k�У���j - iFilterMX + l�����ص�ָ��
					lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i + iFilterMY - k) + j - iFilterMX + l;
					
					// ��������ֵ
					aValue[k * iFilterW + l] = *lpSrc;
				}
			}
			
			// ��ȡ��ֵ
			* lpDst = myGetMedianNum(aValue, iFilterH * iFilterW);
		}
	}
	
	// ���Ʊ任���ͼ��
	memcpy(lpDIBBits, lpNewDIBBits, lLineBytes * lHeight);
	
	// �ͷ��ڴ�
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);
	LocalUnlock(hArray);
	LocalFree(hArray);
	
	// ����
	return TRUE;
}

/*************************************************************************
*
* �������ƣ�
*   GetMedianNum()
*
* ����:
*   unsigned char * bpArray	- ָ��Ҫ��ȡ��ֵ������ָ��
*   int   iFilterLen			- ���鳤��
*
* ����ֵ:
*   unsigned char      - ����ָ���������ֵ��
*
* ˵��:
*   �ú�����ð�ݷ���һά����������򣬲���������Ԫ�ص���ֵ��
*
************************************************************************/

unsigned char WINAPI myGetMedianNum(unsigned char * bArray, int iFilterLen)
{
	// ѭ������
	int		i;
	int		j;
	
	// �м����
	unsigned char bTemp;
	
	// ��ð�ݷ��������������
	for (j = 0; j < iFilterLen - 1; j ++)
	{
		for (i = 0; i < iFilterLen - j - 1; i ++)
		{
			if (bArray[i] > bArray[i + 1])
			{
				// ����
				bTemp = bArray[i];
				bArray[i] = bArray[i + 1];
				bArray[i + 1] = bTemp;
			}
		}
	}
	
	// ������ֵ
	if ((iFilterLen & 1) > 0)
	{
		// ������������Ԫ�أ������м�һ��Ԫ��
		bTemp = bArray[(iFilterLen + 1) / 2];
	}
	else
	{
		// ������ż����Ԫ�أ������м�����Ԫ��ƽ��ֵ
		bTemp = (bArray[iFilterLen / 2] + bArray[iFilterLen / 2 + 1]) / 2;
	}
	
	// ������ֵ
	return bTemp;
}

/*************************************************************************
*
* �������ƣ�
*   InteEqualize()
*
* ����:
*   LPSTR lpDIBBits    - ָ��ԴDIBͼ��ָ��
*   LONG  lWidth       - Դͼ���ȣ���������
*   LONG  lHeight      - Դͼ��߶ȣ���������
*
* ����ֵ:
*   BOOL               - �ɹ�����TRUE�����򷵻�FALSE��
*
* ˵��:
*   �ú���������ͼ�����ֱ��ͼ���⡣
*
************************************************************************/
BOOL WINAPI InteEqualize(LPSTR lpDIBBits, LONG lWidth, LONG lHeight)
{
	
	// ָ��Դͼ���ָ��
	unsigned char*	lpSrc;
	
	// ��ʱ����
	LONG	lTemp;
	
	// ѭ������
	LONG	i;
	LONG	j;
	
	// �Ҷ�ӳ���
	BYTE	bMap[256];
	
	// �Ҷ�ӳ���
	LONG	lCount[256];
	
	// ͼ��ÿ�е��ֽ���
	LONG	lLineBytes;
	
	// ����ͼ��ÿ�е��ֽ���
	lLineBytes = WIDTHBYTES(lWidth * 8);
	
	// ���ü���Ϊ0
	for (i = 0; i < 256; i ++)
	{
		// ����
		lCount[i] = 0;
	}
	
	// ��������Ҷ�ֵ�ļ���
	for (i = 0; i < lHeight; i ++)
	{
		for (j = 0; j < lWidth; j ++)
		{
			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * i + j;
			
			// ������1
			lCount[*(lpSrc)]++;
		}
	}
	
	// ����Ҷ�ӳ���
	for (i = 0; i < 256; i++)
	{
		// ��ʼΪ0
		lTemp = 0;
		
		for (j = 0; j <= i ; j++)
		{
			lTemp += lCount[j];
		}
		
		// �����Ӧ���»Ҷ�ֵ
		bMap[i] = (BYTE) (lTemp * 255 / lHeight / lWidth);
	}
	
	// ÿ��
	for(i = 0; i < lHeight; i++)
	{
		// ÿ��
		for(j = 0; j < lWidth; j++)
		{
			// ָ��DIB��i�У���j�����ص�ָ��
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
			
			// �����µĻҶ�ֵ
			*lpSrc = bMap[*lpSrc];
		}
	}
	
	// ����
	return TRUE;
}

/*************************************************************************
*
* �������ƣ�
*   AddMinusDIB()
*
* ����:
*   LPSTR lpDIBBits    - ָ��ԴDIBͼ��ָ��
*   LPSTR lpDIBBitsBK  - ָ�򱳾�DIBͼ��ָ��
*   LONG  lWidth       - Դͼ���ȣ���������
*   LONG  lHeight      - Դͼ��߶ȣ���������
*	 bool  bAddMinus	- Ϊtrueʱִ�м����㣬����ִ�м����㡣
*
* ����ֵ:
*   BOOL               - ����ɹ�����TRUE�����򷵻�FALSE��
*
* ˵��:
* �ú������ڶ�����ͼ����мӼ����㡣
* 
* Ҫ��Ŀ��ͼ��Ϊ255���Ҷ�ֵ�ĻҶ�ͼ��
************************************************************************/

BOOL WINAPI AddMinusDIB(LPSTR lpDIBBits, LPSTR lpDIBBitsBK, LONG lWidth, LONG lHeight ,bool bAddMinus)
{
	
	// ָ��Դͼ���ָ��
	LPSTR	lpSrc,lpSrcBK;
	
	// ָ�򻺴�ͼ���ָ��
	LPSTR	lpDst;
	
	// ָ�򻺴�DIBͼ���ָ��
	LPSTR	lpNewDIBBits;
	HLOCAL	hNewDIBBits;
	
	//ѭ������
	long i;
	long j;
	
	//����ֵ
	unsigned char pixel,pixelBK;
	
	// ͼ��ÿ�е��ֽ���
	LONG lLineBytes;
	
	// ��ʱ�����ڴ棬�Ա�����ͼ��
	hNewDIBBits = LocalAlloc(LHND, lWidth * lHeight);
	
	if (hNewDIBBits == NULL)
	{
		// �����ڴ�ʧ��
		return FALSE;
	}
	
	// �����ڴ�
	lpNewDIBBits = (char * )LocalLock(hNewDIBBits);
	
	// ��ʼ���·�����ڴ棬�趨��ʼֵΪ255
	lpDst = (char *)lpNewDIBBits;
	memset(lpDst, (BYTE)255, lWidth * lHeight);
	
	// ����ͼ��ÿ�е��ֽ���
	lLineBytes = WIDTHBYTES(lWidth * 8);
	
	for (j = 0;j < lHeight ;j++)
	{
		for(i = 0;i < lWidth ;i++)
		{
			// ָ��Դͼ������j�У���i�����ص�ָ��			
			lpSrc = (char *)lpDIBBits + lLineBytes * j + i;
			lpSrcBK = (char *)lpDIBBitsBK + lLineBytes * j + i;
			
			// ָ��Ŀ��ͼ������j�У���i�����ص�ָ��			
			lpDst = (char *)lpNewDIBBits + lLineBytes * j + i;
			
			pixel = (unsigned char)*lpSrc;
			pixelBK = (unsigned char)*lpSrcBK;
			if(bAddMinus)
				*lpDst = pixel + pixelBK > 255 ? 255 : pixel + pixelBK;
			else
				//	*lpDst = pixel - pixelBK < 0 ? 0 : pixel - pixelBK;
				if(abs(pixel - pixelBK)<10)
					*lpDst=0;
				else
					*lpDst=pixel;
		}
	}
	
	
	// ���Ƹ�ʴ���ͼ��
	memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);
	
	// �ͷ��ڴ�
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);
	
	// ����
	return TRUE;
}

/*************************************************************************
*
* �������ƣ�
*   myCropDIB()
*
* ����:
*   HDIB hDIB          - ָ��ԴDIBͼ����
*   LPRECT lpRect      - ָ��Ҫ���õľ�������
*
* ����ֵ:
*   HDIB               - ���زü���ľ�������ͼ������
*
* ˵��:
* �ú������ڶ�ָ��ͼ�����ָ������ü�������
* 
* Ҫ��Ŀ��ͼ��Ϊ255���Ҷ�ֵ�ĻҶ�ͼ��
************************************************************************/

HDIB WINAPI myCropDIB(HDIB hDIB, LPRECT lpRect)
{
	
	LPBITMAPINFO lpbmi = NULL; 
    LPBYTE       lpSourceBits, lpTargetBits, lpResult; 
    HDC			 hDC = NULL, hSourceDC, hTargetDC; 
    HBITMAP      hSourceBitmap, hTargetBitmap, hOldTargetBitmap, hOldSourceBitmap; 
    DWORD        dwSourceBitsSize, dwTargetBitsSize, dwTargetHeaderSize; 
	int			 nWidth, nHeight;
	HDIB		 hNewDIB;
	DWORD		 dwSize;
	
	// Get DIB pointer
	if (! hDIB)
	{
		return NULL;
	}
	LPBITMAPINFO lpSrcDIB = (LPBITMAPINFO)GlobalLock(hDIB);
	if (! lpSrcDIB)
	{
		return NULL;
	}
	
    // Allocate and fill out a BITMAPINFO struct for the new DIB 
    dwTargetHeaderSize = sizeof( BITMAPINFOHEADER ) + PaletteSize((LPSTR)lpSrcDIB); 
    lpbmi = (LPBITMAPINFO)malloc( dwTargetHeaderSize ); 
	memcpy(lpbmi, lpSrcDIB, dwTargetHeaderSize);
	nWidth = RECTWIDTH(lpRect);
	nHeight = RECTHEIGHT(lpRect);
    lpbmi->bmiHeader.biWidth = nWidth; 
    lpbmi->bmiHeader.biHeight = nHeight; 
	
    // Gonna use DIBSections and BitBlt() to do the conversion, so make 'em 
	hDC = ::GetDC( NULL ); 
    hTargetBitmap = CreateDIBSection( hDC, lpbmi, DIB_RGB_COLORS, (VOID **)&lpTargetBits, NULL, 0 ); 
    hSourceBitmap = CreateDIBSection( hDC, lpSrcDIB, DIB_RGB_COLORS, (VOID **)&lpSourceBits, NULL, 0 ); 
    hSourceDC = CreateCompatibleDC( hDC ); 
    hTargetDC = CreateCompatibleDC( hDC ); 
	
    // Flip the bits on the source DIBSection to match the source DIB 
    dwSourceBitsSize = lpSrcDIB->bmiHeader.biHeight * BytesPerLine((LPBYTE)&(lpSrcDIB->bmiHeader)); 
    dwTargetBitsSize = lpbmi->bmiHeader.biHeight * BytesPerLine((LPBYTE)&(lpbmi->bmiHeader)); 
    memcpy( lpSourceBits, FindDIBBits((LPSTR)lpSrcDIB), dwSourceBitsSize ); 
    lpbmi->bmiHeader.biSizeImage = dwTargetBitsSize; 
	
    // Select DIBSections into DCs 
    hOldSourceBitmap = (HBITMAP)SelectObject( hSourceDC, hSourceBitmap ); 
    hOldTargetBitmap = (HBITMAP)SelectObject( hTargetDC, hTargetBitmap ); 
	
	// put old bitmap in new bitmap 
    BitBlt(hTargetDC, 0, 0, nWidth, nHeight, hSourceDC, lpRect->left, lpRect->top, SRCCOPY); 
	
    // Clean up and delete the DCs 
    SelectObject( hSourceDC, hOldSourceBitmap ); 
    SelectObject( hTargetDC, hOldTargetBitmap ); 
    DeleteDC( hSourceDC ); 
    DeleteDC( hTargetDC ); 
    ::ReleaseDC( NULL, hDC ); 
	
    // Flush the GDI batch, so we can play with the bits 
    GdiFlush(); 
	
    // Allocate enough memory for the new CF_DIB, and copy bits 
	dwSize = dwTargetHeaderSize + dwTargetBitsSize;
	hNewDIB =(HDIB)GlobalAlloc(GHND, dwSize);
    lpResult = (LPBYTE)GlobalLock(hNewDIB);//malloc( dwTargetHeaderSize + dwTargetBitsSize ); 
    memcpy( lpResult, lpbmi, dwTargetHeaderSize ); 
    memcpy( FindDIBBits( (LPSTR)lpResult ), lpTargetBits, dwTargetBitsSize ); 
	
    // final cleanup 
    DeleteObject( hTargetBitmap ); 
    DeleteObject( hSourceBitmap ); 
    free( lpbmi ); 
	GlobalUnlock(hDIB);
	GlobalUnlock(hNewDIB);
	
    return hNewDIB;
}
