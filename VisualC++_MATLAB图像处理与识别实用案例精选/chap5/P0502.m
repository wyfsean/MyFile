I1=imread('5-7(A).jpg'); % ͼ5-7(a)��ŷ��������
BW1=im2bw(I1,0.7);  % ͼ���ֵ����ԭʼͼ����Ȼ���������ֵͼ��ʵ��ΪRGBͼ��
figure,imshow(~BW1); 
E1=bweuler(~BW1,8)  % ��ֵ�����ͼ��Ŀ��Ϊ0������Ϊ1���������󷴺����ŷ������

I2=imread('5-7(B).jpg'); % ͼ5-7(b)��ŷ��������
BW2=im2bw(I2,0.7);
figure,imshow(~BW2);
E2=bweuler(~BW2,8)
