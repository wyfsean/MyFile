// suanfa1.h
#include "DIBAPI.h"

#ifndef _INC_SUANFA1
#define _INC_SUANFA1

// º¯ÊýÔ­ÐÍ
HDIB WINAPI myCropDIB(HDIB hDIB, LPRECT lpRect);
unsigned char WINAPI myGetMedianNum(unsigned char * bArray, int iFilterLen);
BOOL WINAPI myMedianFilter(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, 
						 int iFilterH, int iFilterW, 
						 int iFilterMX, int iFilterMY);
BOOL WINAPI ConvertToGrayScale(LPSTR lpDIB);
BOOL WINAPI ThresholdTrans(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, BYTE bThre);
BOOL WINAPI myTemplate(LPSTR lpDIBBits);
BOOL WINAPI myHprojectDIB(LPSTR lpDIBBits, LONG lWidth, LONG lHeight, 
						 int* iTop, int* iBottom); 
BOOL WINAPI myVprojectDIB(LPSTR lpDIB, LONG lWidth, LONG lHeight, 
						 int* iLeft, int* iRight) ;

BOOL WINAPI InteEqualize(LPSTR lpDIBBits, LONG lWidth, LONG lHeight);
BOOL WINAPI AddMinusDIB(LPSTR lpDIBBits, LPSTR lpDIBBitsBK, LONG lWidth, LONG lHeight ,bool bAddMinus);

//HDIB WINAPI ConvertToGrayScale(LPSTR lpDIB) ;
#endif //!_INC_SUANFA1