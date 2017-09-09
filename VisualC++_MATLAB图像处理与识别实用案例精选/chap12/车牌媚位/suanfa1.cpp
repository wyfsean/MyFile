
/**************************************************************************
*  文件名：suanfa1.cpp
*
*  车牌定位函数库：
*	
*  myMedianFilter()		- 图像中值滤波。
*  myGetMedianNum()	    - 获取中值。被函数MedianFilter()调用来求中值。
*  ReplaceColorPal()	    - 更换伪彩色编码表。
*  ConvertToGrayScale();   - 24位真彩色图转换成灰度图
*  ThresholdTrans();       - 阀值变换
*  myTemplate();           - 图像模板变换，通过改变模板，可以用它实现
*						      图像的平滑、锐化、边缘识别等操作。
*  myHprojectDIB();        - 水平投影
*  myVprojectDIB() ;       - 垂直投影 
*  myCropDIB();            - 区域剪裁
*************************************************************************/

#include "stdafx.h"
#include "suanfa1.h"

#include <math.h>
#include <direct.h>

/*************************************************************************
*
* 函数名称：
*   ConvertToGrayScale()
*
* 参数:
*   LPSTR lpDIB			- 指向源DIB图像指针
*   
* 返回值:
*   BOOL					- 成功返回TRUE，否则返回FALSE。
*
* 说明:
*   该函数将24位真彩色图转换成256级灰度图
*
************************************************************************/
BOOL WINAPI ConvertToGrayScale(LPSTR lpDIB)   
{
	
	LPSTR lpDIBBits;                //指向DIB的象素的指针
	LPSTR lpNewDIBBits;             //指向DIB灰度图图像(新图像)开始处象素的指针
    LONG lLineBytes;
	unsigned char * lpSrc;          //指向原图像象素点的指针
	unsigned char * lpdest;         //指向目标图像象素点的指针
	
    unsigned  char *ired,*igreen,*iblue;
	
    long lWidth;                    //图像宽度和高度
	long lHeight;
	
	long i,j;           //循环变量
	
	lWidth = ::DIBWidth(lpDIB);   //DIB 宽度
	lHeight = ::DIBHeight(lpDIB); //DIB 高度
	RGBQUAD  *lpRGBquad;
	lpRGBquad = (RGBQUAD *)&lpDIB[sizeof(BITMAPINFOHEADER)]; //INFOHEADER后为调色板		
    if(::DIBNumColors(lpDIB) == 256)  //256色位图不作任何处理
	{
		return TRUE;
    }
	
	if(::DIBNumColors(lpDIB) != 256)  //非256色位图将它灰度化
	{
		lLineBytes = WIDTHBYTES(lWidth*8*3);
		lpdest=  new  BYTE[lHeight*lWidth];
		lpDIBBits = (LPSTR)lpDIB + sizeof(BITMAPINFOHEADER);//指向DIB象素
		for(i = 0;i < lHeight; i++)
			for(j = 0;j < lWidth*3; j+=3)
			{
				ired   =  (unsigned  char*)lpDIBBits + lLineBytes * i + j + 2;
				igreen =  (unsigned  char*)lpDIBBits + lLineBytes * i + j + 1;
				iblue  =  (unsigned  char*)lpDIBBits + lLineBytes * i +	j ;
				lpdest[i*lWidth + j/3]  = (unsigned  char)((*ired)*0.299 + (*igreen)*0.588 + (*iblue)*0.114);
			}
			
			//需要做三件事情：1、修改INFOHEADER 2、增加调色板 3、修改原图像灰度值
			LPBITMAPINFOHEADER  lpBI;
			lpBI  =  (LPBITMAPINFOHEADER)lpDIB;
			lpBI->biBitCount = 8;
			
			//设置灰度调色板
			for(i = 0;i < 256;i++)
			{
				lpRGBquad[i].rgbRed   = (unsigned char)i;
				lpRGBquad[i].rgbGreen = (unsigned char)i;
				lpRGBquad[i].rgbBlue  = (unsigned char)i;
				lpRGBquad[i].rgbReserved = 0;
			}
			
			lpNewDIBBits= ::FindDIBBits(lpDIB);  //找到DIB图像象素起始位置
			lLineBytes=WIDTHBYTES(lWidth  *  8);
			//修改灰度值
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
* 函数名称：
*   ThresholdTrans()
*
* 参数:
*   LPSTR lpDIBBits    - 指向源DIB图像指针
*   LONG  lWidth       - 源图像宽度（象素数）
*   LONG  lHeight      - 源图像高度（象素数）
*   BYTE  bThre	    - 阈值
*
* 返回值:
*   BOOL               - 成功返回TRUE，否则返回FALSE。
*
* 说明:
*   该函数用来对图像进行阈值变换。对于灰度值小于阈值的象素直接设置
* 灰度值为0；灰度值大于阈值的象素直接设置为255。
*
************************************************************************/
BOOL WINAPI ThresholdTrans(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, BYTE bThre)
{
	
	// 指向源图像的指针
	unsigned char*	lpSrc;
	
	// 循环变量
	LONG	i;
	LONG	j;
	
	// 图像每行的字节数
	LONG	lLineBytes;
	
	// 计算图像每行的字节数
	lLineBytes = WIDTHBYTES(lWidth * 8);
	
	// 每行
	for(i = 0; i < lHeight; i++)
	{
		// 每列
		for(j = 0; j < lWidth; j++)
		{
			// 指向DIB第i行，第j个象素的指针
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
			
			// 判断是否小于阈值
			if ((*lpSrc) < bThre)
			{
				// 直接赋值为0
				*lpSrc = 0;
			}
			else
			{
				// 直接赋值为255
				*lpSrc = 255;
			}
		}
	}
	
	// 返回
	return TRUE;
}

/*************************************************************************
*
* 函数名称：
*   Template()
*
* 参数:
*   LPSTR lpDIBBits    - 指向源DIB图像指针
*   LONG  lWidth       - 源图像宽度（象素数）
*   LONG  lHeight      - 源图像高度（象素数）
*   int   iTempH		- 模板的高度
*   int   iTempW		- 模板的宽度
*   int   iTempMX		- 模板的中心元素X坐标 ( < iTempW - 1)
*   int   iTempMY		- 模板的中心元素Y坐标 ( < iTempH - 1)
*	 FLOAT * fpArray	- 指向模板数组的指针
*	 FLOAT fCoef		- 模板系数
*
* 返回值:
*   BOOL               - 成功返回TRUE，否则返回FALSE。
*
* 说明:
*   该函数用指定的模板（任意大小）来对图像进行操作，参数iTempH指定模板
* 的高度，参数iTempW指定模板的宽度，参数iTempMX和iTempMY指定模板的中心
* 元素坐标，参数fpArray指定模板元素，fCoef指定系数。
*
************************************************************************/

BOOL WINAPI myTemplate(LPSTR lpDIB)			
{
	// 指向复制图像的指针
	LPSTR	lpNewDIBBits;
	HLOCAL	hNewDIBBits;
	
	// 指向要复制区域的指针
	unsigned char*	lpDst;
	
	LPSTR lpDIBBits;                   //指向DIB的象素的指针
	
    LONG lLineBytes;                   // 图像每行的字节数
	unsigned char * lpSrc;             //指向原图像象素点的指针
	
    long lWidth;                       //图像宽度和高度
	long lHeight;
	
	// 计算结果
	INT	fResult;
	
	// 找到DIB图像象素起始位置
	lpDIBBits = ::FindDIBBits(lpDIB);
	lWidth = ::DIBWidth(lpDIB);   //DIB 宽度
	lHeight = ::DIBHeight(lpDIB); //DIB 高度
	
	// 计算图像每行的字节数
	lLineBytes = WIDTHBYTES(lWidth * 8);
	
	// 暂时分配内存，以保存新图像
	hNewDIBBits = LocalAlloc(LHND, lLineBytes * lHeight);
	
	// 判断是否内存分配失败
	if (hNewDIBBits == NULL)
	{
		// 分配内存失败
		return false;
	}
	
	// 锁定内存
	lpNewDIBBits = (char * )LocalLock(hNewDIBBits);
	
	// 初始化图像为原始图像
	memcpy(lpNewDIBBits, lpDIBBits, lLineBytes * lHeight);
	
	
	long i,j;           //循环变量
	
	//
	//
	for(i = 0; i < lHeight-1; i++)
	{
		// 每列
		for(j = 0; j < lWidth-1; j++)
		{
			// 指向新DIB第i行，第j个象素的指针
			lpDst = (unsigned char*)lpNewDIBBits + lLineBytes * (lHeight - 1 - i) + j;
			
			// 指向DIB第i行，第j个象素的指针
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
			
			fResult=(*lpSrc)-(*(lpSrc+1));
			
			// 取绝对值
			if(fResult<0) fResult=-fResult;
			
			// 判断是否超过255
			if(fResult > 255)
			{
				// 直接赋值为255
				* lpDst = 255;
			}
			else
			{
				// 赋值
				* lpDst = (unsigned char) (fResult + 0.5);
			}
			
		}
	}
	
	// 复制变换后的图像
	memcpy(lpDIBBits, lpNewDIBBits, lLineBytes * lHeight);
	
	// 释放内存
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);
	
	// 返回
	return TRUE;
	
}

/*************************************************************************
*
* 函数名称：
*   myHprojectDIB()
*
* 参数:
*   LPSTR lpDIBBits    - 指向源DIB图像指针
*   LONG  lWidth       - 源图像宽度（象素数）
*   LONG  lHeight      - 源图像高度（象素数）
*   INT*  iTop         - 车牌上边缘
*   INT*  iBottom      - 车牌下边缘
* 返回值:
*   BOOL               - 成功返回TRUE，否则返回FALSE。
*
* 说明:
*   该函数用于对含车牌图像进行水平投影运算，求取车牌子图像的上下边缘位置
*
************************************************************************/
BOOL WINAPI myHprojectDIB(LPSTR lpDIB, LONG lWidth, LONG lHeight, 
						  int* iTop, int* iBottom) 
{
	LPSTR lpDIBBits;                   //指向DIB的象素的指针
	
    LONG lLineBytes;                   // 图像每行的字节数
	unsigned char * lpSrc;             //指向原图像象素点的指针
	
	unsigned char pixel;				//像素值
	
	int i,j;
	
	// 计算结果
	INT* iResult;
	
	INT pzBottom,pzTop;
	
	bool findPZ=false;
	
	// 找到DIB图像象素起始位置
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// 计算图像每行的字节数
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
	pzBottom=pzBottom-20;   //微量调整
	*iBottom=lHeight-pzBottom;
	*iTop=lHeight-pzTop;
	return true;
}

/*************************************************************************
*
* 函数名称：
*   myVprojectDIB()
*
* 参数:
*   LPSTR lpDIBBits    - 指向源DIB图像指针
*   LONG  lWidth       - 源图像宽度（象素数）
*   LONG  lHeight      - 源图像高度（象素数）
*   INT*  iLeft        - 车牌左边缘
*   INT*  iRight       - 车牌右边缘
* 返回值:
*   BOOL               - 成功返回TRUE，否则返回FALSE。
*
* 说明:
*   该函数用于对含车牌图像进行垂直投影运算，求取车牌子图像的左右边缘位置
*
************************************************************************/
BOOL WINAPI myVprojectDIB(LPSTR lpDIB, LONG lWidth, LONG lHeight, 
						  int* iLeft, int* iRight) 
{
	LPSTR lpDIBBits;                   //指向DIB的象素的指针
	
    LONG lLineBytes;                   // 图像每行的字节数
	unsigned char * lpSrc;             //指向原图像象素点的指针
	
	unsigned char pixel;				//像素值
	
	int i,j;
	
	// 计算结果
	INT* iResult;
	
	INT pzLeft,pzRight;
	
	bool findPZ=false;
	
	// 找到DIB图像象素起始位置
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	// 计算图像每行的字节数
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
* 函数名称：
*   MedianFilter()
*
* 参数:
*   LPSTR lpDIBBits		- 指向源DIB图像指针
*   LONG  lWidth			- 源图像宽度（象素数）
*   LONG  lHeight			- 源图像高度（象素数）
*   int   iFilterH			- 滤波器的高度
*   int   iFilterW			- 滤波器的宽度
*   int   iFilterMX		- 滤波器的中心元素X坐标
*   int   iFilterMY		- 滤波器的中心元素Y坐标
*
* 返回值:
*   BOOL					- 成功返回TRUE，否则返回FALSE。
*
* 说明:
*   该函数对DIB图像进行中值滤波。
*
************************************************************************/

BOOL WINAPI myMedianFilter(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, 
						   int iFilterH, int iFilterW, 
						   int iFilterMX, int iFilterMY)
{
	
	// 指向源图像的指针
	unsigned char*	lpSrc;
	
	// 指向要复制区域的指针
	unsigned char*	lpDst;
	
	// 指向复制图像的指针
	LPSTR			lpNewDIBBits;
	HLOCAL			hNewDIBBits;
	
	// 指向滤波器数组的指针
	unsigned char	* aValue;
	HLOCAL			hArray;
	
	// 循环变量
	LONG			i;
	LONG			j;
	LONG			k;
	LONG			l;
	
	// 图像每行的字节数
	LONG			lLineBytes;
	
	// 计算图像每行的字节数
	lLineBytes = WIDTHBYTES(lWidth * 8);
	
	// 暂时分配内存，以保存新图像
	hNewDIBBits = LocalAlloc(LHND, lLineBytes * lHeight);
	
	// 判断是否内存分配失败
	if (hNewDIBBits == NULL)
	{
		// 分配内存失败
		return FALSE;
	}
	
	// 锁定内存
	lpNewDIBBits = (char * )LocalLock(hNewDIBBits);
	
	// 初始化图像为原始图像
	memcpy(lpNewDIBBits, lpDIBBits, lLineBytes * lHeight);
	
	// 暂时分配内存，以保存滤波器数组
	hArray = LocalAlloc(LHND, iFilterH * iFilterW);
	
	// 判断是否内存分配失败
	if (hArray == NULL)
	{
		// 释放内存
		LocalUnlock(hNewDIBBits);
		LocalFree(hNewDIBBits);
		
		// 分配内存失败
		return FALSE;
	}
	
	// 锁定内存
	aValue = (unsigned char * )LocalLock(hArray);
	
	// 开始中值滤波
	// 行(除去边缘几行)
	for(i = iFilterMY; i < lHeight - iFilterH + iFilterMY + 1; i++)
	{
		// 列(除去边缘几列)
		for(j = iFilterMX; j < lWidth - iFilterW + iFilterMX + 1; j++)
		{
			// 指向新DIB第i行，第j个象素的指针
			lpDst = (unsigned char*)lpNewDIBBits + lLineBytes * (lHeight - 1 - i) + j;
			
			// 读取滤波器数组
			for (k = 0; k < iFilterH; k++)
			{
				for (l = 0; l < iFilterW; l++)
				{
					// 指向DIB第i - iFilterMY + k行，第j - iFilterMX + l个象素的指针
					lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i + iFilterMY - k) + j - iFilterMX + l;
					
					// 保存象素值
					aValue[k * iFilterW + l] = *lpSrc;
				}
			}
			
			// 获取中值
			* lpDst = myGetMedianNum(aValue, iFilterH * iFilterW);
		}
	}
	
	// 复制变换后的图像
	memcpy(lpDIBBits, lpNewDIBBits, lLineBytes * lHeight);
	
	// 释放内存
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);
	LocalUnlock(hArray);
	LocalFree(hArray);
	
	// 返回
	return TRUE;
}

/*************************************************************************
*
* 函数名称：
*   GetMedianNum()
*
* 参数:
*   unsigned char * bpArray	- 指向要获取中值的数组指针
*   int   iFilterLen			- 数组长度
*
* 返回值:
*   unsigned char      - 返回指定数组的中值。
*
* 说明:
*   该函数用冒泡法对一维数组进行排序，并返回数组元素的中值。
*
************************************************************************/

unsigned char WINAPI myGetMedianNum(unsigned char * bArray, int iFilterLen)
{
	// 循环变量
	int		i;
	int		j;
	
	// 中间变量
	unsigned char bTemp;
	
	// 用冒泡法对数组进行排序
	for (j = 0; j < iFilterLen - 1; j ++)
	{
		for (i = 0; i < iFilterLen - j - 1; i ++)
		{
			if (bArray[i] > bArray[i + 1])
			{
				// 互换
				bTemp = bArray[i];
				bArray[i] = bArray[i + 1];
				bArray[i + 1] = bTemp;
			}
		}
	}
	
	// 计算中值
	if ((iFilterLen & 1) > 0)
	{
		// 数组有奇数个元素，返回中间一个元素
		bTemp = bArray[(iFilterLen + 1) / 2];
	}
	else
	{
		// 数组有偶数个元素，返回中间两个元素平均值
		bTemp = (bArray[iFilterLen / 2] + bArray[iFilterLen / 2 + 1]) / 2;
	}
	
	// 返回中值
	return bTemp;
}

/*************************************************************************
*
* 函数名称：
*   InteEqualize()
*
* 参数:
*   LPSTR lpDIBBits    - 指向源DIB图像指针
*   LONG  lWidth       - 源图像宽度（象素数）
*   LONG  lHeight      - 源图像高度（象素数）
*
* 返回值:
*   BOOL               - 成功返回TRUE，否则返回FALSE。
*
* 说明:
*   该函数用来对图像进行直方图均衡。
*
************************************************************************/
BOOL WINAPI InteEqualize(LPSTR lpDIBBits, LONG lWidth, LONG lHeight)
{
	
	// 指向源图像的指针
	unsigned char*	lpSrc;
	
	// 临时变量
	LONG	lTemp;
	
	// 循环变量
	LONG	i;
	LONG	j;
	
	// 灰度映射表
	BYTE	bMap[256];
	
	// 灰度映射表
	LONG	lCount[256];
	
	// 图像每行的字节数
	LONG	lLineBytes;
	
	// 计算图像每行的字节数
	lLineBytes = WIDTHBYTES(lWidth * 8);
	
	// 重置计数为0
	for (i = 0; i < 256; i ++)
	{
		// 清零
		lCount[i] = 0;
	}
	
	// 计算各个灰度值的计数
	for (i = 0; i < lHeight; i ++)
	{
		for (j = 0; j < lWidth; j ++)
		{
			lpSrc = (unsigned char *)lpDIBBits + lLineBytes * i + j;
			
			// 计数加1
			lCount[*(lpSrc)]++;
		}
	}
	
	// 计算灰度映射表
	for (i = 0; i < 256; i++)
	{
		// 初始为0
		lTemp = 0;
		
		for (j = 0; j <= i ; j++)
		{
			lTemp += lCount[j];
		}
		
		// 计算对应的新灰度值
		bMap[i] = (BYTE) (lTemp * 255 / lHeight / lWidth);
	}
	
	// 每行
	for(i = 0; i < lHeight; i++)
	{
		// 每列
		for(j = 0; j < lWidth; j++)
		{
			// 指向DIB第i行，第j个象素的指针
			lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
			
			// 计算新的灰度值
			*lpSrc = bMap[*lpSrc];
		}
	}
	
	// 返回
	return TRUE;
}

/*************************************************************************
*
* 函数名称：
*   AddMinusDIB()
*
* 参数:
*   LPSTR lpDIBBits    - 指向源DIB图像指针
*   LPSTR lpDIBBitsBK  - 指向背景DIB图像指针
*   LONG  lWidth       - 源图像宽度（象素数）
*   LONG  lHeight      - 源图像高度（象素数）
*	 bool  bAddMinus	- 为true时执行加运算，否则执行减运算。
*
* 返回值:
*   BOOL               - 运算成功返回TRUE，否则返回FALSE。
*
* 说明:
* 该函数用于对两幅图像进行加减运算。
* 
* 要求目标图像为255个灰度值的灰度图像。
************************************************************************/

BOOL WINAPI AddMinusDIB(LPSTR lpDIBBits, LPSTR lpDIBBitsBK, LONG lWidth, LONG lHeight ,bool bAddMinus)
{
	
	// 指向源图像的指针
	LPSTR	lpSrc,lpSrcBK;
	
	// 指向缓存图像的指针
	LPSTR	lpDst;
	
	// 指向缓存DIB图像的指针
	LPSTR	lpNewDIBBits;
	HLOCAL	hNewDIBBits;
	
	//循环变量
	long i;
	long j;
	
	//像素值
	unsigned char pixel,pixelBK;
	
	// 图像每行的字节数
	LONG lLineBytes;
	
	// 暂时分配内存，以保存新图像
	hNewDIBBits = LocalAlloc(LHND, lWidth * lHeight);
	
	if (hNewDIBBits == NULL)
	{
		// 分配内存失败
		return FALSE;
	}
	
	// 锁定内存
	lpNewDIBBits = (char * )LocalLock(hNewDIBBits);
	
	// 初始化新分配的内存，设定初始值为255
	lpDst = (char *)lpNewDIBBits;
	memset(lpDst, (BYTE)255, lWidth * lHeight);
	
	// 计算图像每行的字节数
	lLineBytes = WIDTHBYTES(lWidth * 8);
	
	for (j = 0;j < lHeight ;j++)
	{
		for(i = 0;i < lWidth ;i++)
		{
			// 指向源图像倒数第j行，第i个象素的指针			
			lpSrc = (char *)lpDIBBits + lLineBytes * j + i;
			lpSrcBK = (char *)lpDIBBitsBK + lLineBytes * j + i;
			
			// 指向目标图像倒数第j行，第i个象素的指针			
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
	
	
	// 复制腐蚀后的图像
	memcpy(lpDIBBits, lpNewDIBBits, lWidth * lHeight);
	
	// 释放内存
	LocalUnlock(hNewDIBBits);
	LocalFree(hNewDIBBits);
	
	// 返回
	return TRUE;
}

/*************************************************************************
*
* 函数名称：
*   myCropDIB()
*
* 参数:
*   HDIB hDIB          - 指向源DIB图像句柄
*   LPRECT lpRect      - 指向要剪裁的矩形区域
*
* 返回值:
*   HDIB               - 返回裁剪后的矩形区域图像句柄。
*
* 说明:
* 该函数用于对指定图像进行指定区域裁剪操作。
* 
* 要求目标图像为255个灰度值的灰度图像。
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
