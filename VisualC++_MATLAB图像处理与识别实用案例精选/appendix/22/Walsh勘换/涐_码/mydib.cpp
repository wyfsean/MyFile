#include "stdafx.h"
#include "mydib.h"

/*************************************************************************
					      ��������ӳ���
*************************************************************************/
void rectcoding(HDIB hDIB,int wRect,int hRect)
{
	// ָ��Դͼ�����ص�ָ��
	unsigned char *	lpSrc;
	
	// ָ��DIB��ָ��
	LPSTR	lpDIB;

	// ָ��DIB����ָ��
	LPSTR   lpDIBBits;
	
	//���������λͼ����ĵ�ַ
	lpDIB = (LPSTR) ::GlobalLock((HGLOBAL) hDIB);
	
	// �ж��Ƿ���8-bppλͼ������Ϊ�˷��㣬ֻ����8-bppλͼ��ģ�������������Ŀ������ƣ�
	if (::DIBNumColors(lpDIB) != 256)
	{
		// ��ʾ�û�
		::AfxMessageBox ("Ŀǰֻ֧��256ɫλͼ�����㣡");
		
		// ����
		return;
	}

	// �ҵ�DIBͼ��������ʼλ��
	lpDIBBits = ::FindDIBBits(lpDIB);
	
	//ͼ�������

	//�������

	//���ͼ��Ŀ�Ⱥ͸߶�
	LONG lWidth=::DIBWidth ((char*)lpDIB);
	LONG lHeight=::DIBHeight ((char*)lpDIB);

	// ����ͼ��ÿ�е��ֽ���
	LONG lLineBytes = WIDTHBYTES(lWidth * 8);

	if(lWidth!=lHeight)
	{
		// ��ʾ�û�
		::AfxMessageBox("ֻ�ܶԿ�Ⱥ͸߶���ȵ�ͼ����з�����룡");

		// ����
		return;
	}

	//�������

	//ѭ������
	LONG i,j,k;
	int ii,jj;
	double* Xt=new double [lWidth*lHeight/(hRect*wRect)];
	int Xi;
	int* a0=new int [lWidth*lHeight/(hRect*wRect)];
	int* a1=new int [lWidth*lHeight/(hRect*wRect)];
	int* N0=new int [lWidth*lHeight/(hRect*wRect)];
	int* N1=new int [lWidth*lHeight/(hRect*wRect)];

	//������ֵ
	k=0;
	for (i = 0; i < lHeight; i =i+hRect)
	{
		for (j = 0; j < lWidth; j =j+wRect)
		{
			Xt[k]=0;
			//����Xt
			for(ii=0;ii<hRect;ii++)
				for(jj=0;jj<wRect;jj++)
				{   // ָ��ͼ��ָ��
					lpSrc = (unsigned char *)lpDIBBits + lLineBytes * (i+ii) + (j+jj);
					Xi=*lpSrc;
					Xt[k]+=Xi;
				}	
			Xt[k]/=hRect*wRect;
			k++;	
		}		
	}

	//����N0��N1
	k=0;
	for (i = 0; i < lHeight; i =i+hRect)
	{
		for (j = 0; j < lWidth; j =j+wRect)
		{
			N0[k]=0;
			N1[k]=0;
			//����N0��N1
			for(ii=0;ii<hRect;ii++)
				for(jj=0;jj<wRect;jj++)
				{   // ָ��ͼ��ָ��
					lpSrc = (unsigned char *)lpDIBBits + lLineBytes * (i+ii) + (j+jj);
					Xi=*lpSrc;
					if(Xi<Xt[k]) N0[k]++;
					else N1[k]++;
				}	
			k++;	
		}	
	}

	//����a0��a1
	k=0;
	for (i = 0; i < lHeight; i =i+hRect)
	{
		for (j = 0; j < lWidth; j =j+wRect)
		{
			a0[k]=0;
			a1[k]=0;
			//����N0��N1
			for(ii=0;ii<hRect;ii++)
				for(jj=0;jj<wRect;jj++)
				{   // ָ��ͼ��ָ��
					lpSrc = (unsigned char *)lpDIBBits + lLineBytes * (i+ii) + (j+jj);
					Xi=*lpSrc;
					if(Xi<Xt[k]) a0[k]+=Xi;
					else a1[k]+=Xi;
				}	
			if(N0[k]!=0&&N1[k]!=0)
			{	a0[k]/=N0[k];
				a1[k]/=N1[k];
			}	
			k++;	
		}		
	}

	//��ֵ���������
	k=0;
	for (i = 0; i < lHeight; i =i+hRect)
	{
		for (j = 0; j < lWidth; j =j+wRect)
		{
			for(ii=0;ii<hRect;ii++)
				for(jj=0;jj<wRect;jj++)
				{   // ָ��ͼ��ָ��
					lpSrc = (unsigned char *)lpDIBBits + lLineBytes * (i+ii) + (j+jj);
					Xi=*lpSrc;
					if(Xi<Xt[k]) Xi=a0[k];
					else Xi=a1[k];
					//���µ�����ֵ������
					*lpSrc=Xi;
				}		
			k++;	
		}		
	}
	::GlobalUnlock((HGLOBAL) hDIB);
}