I1=imread('5-7(A).jpg'); % 图5-7(a)的欧拉数计算
BW1=im2bw(I1,0.7);  % 图像二值化。原始图像虽然看起来向二值图像，实质为RGB图像。
figure,imshow(~BW1); 
E1=bweuler(~BW1,8)  % 二值化后的图像，目标为0，背景为1，必须先求反后计算欧拉数。

I2=imread('5-7(B).jpg'); % 图5-7(b)的欧拉数计算
BW2=im2bw(I2,0.7);
figure,imshow(~BW2);
E2=bweuler(~BW2,8)
