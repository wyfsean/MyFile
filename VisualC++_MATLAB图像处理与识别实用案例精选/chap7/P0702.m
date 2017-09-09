I=imread('cancer02.bmp');       % 注意必须保证二值图像中，细胞区域为白色区域或者像素点值为“1”
[x,y]=size(I);

BW = bwperim(I,8);              % 检测细胞的边缘跟踪，用于计算周长 

%检测垂直方向连读的周长像素点%
P1=0;
P2=0;
Ny=0;                           % 记录垂直方向连续周长像素点的个数
for i=1:x
    for j=1:y
        if (BW(i,j)>0)
            P2=j;
            if ((P2-P1)==1)    % 判断是否为垂直方向连续的周长像素点
                Ny=Ny+1;
            end
            P1=P2;
        end
    end
end    

%检测水平方向连读的周长像素点%
P1=0;
P2=0;
Nx=0;                           % 记录水平方向连续周长像素点的个数
for j=1:y
    for i=1:x
        if (BW(i,j)>0)
            P2=i;
            if ((P2-P1)==1)    % 判断是否为水平方向连续的周长像素点
                Nx=Nx+1;
            end
            P1=P2;
        end
    end
end 


SN=sum(sum(BW));            % 计算周长像素点的总数
Nd=SN-Nx-Ny;                % 计算奇数码的链码数目

H=max(sum(I));              % 计算细胞的高度 
W=max(sum(I'));             % 图象I经矩阵转置后，计算宽度
L=sqrt(2)*Nd+Nx+Ny;         % 计算周长

%====四个形态特征值计算===%
A=bwarea(I);                % 计算细胞的面积
C=4*pi*A/(L*L);             % 计算圆度
R=A/(H*W);                  % 计算矩形度
E=min(H,W)/max(H,W);        % 计算伸长度

%==设定相关阈值，识别癌细胞==%
Ath1=10000; Ath2=50000;
Cth=0.5;      Rth=0.5;   Eth=0.8;


if ((A>=Ath1)&&(A<Ath2))
    if ((C>=Cth)&&(R<=Rth)&&(E>Eth))
        Cancer_Right=1                     % 结论为癌细胞
    end    
else if (A>=Ath2)
        Cancer_Right=2                     % 结论为可疑小细胞癌细胞      
else
    Cancer_Right=0                         % 结论为正常细胞 
end
end

figure(1);
imshow(I);
figure(2);
imshow(BW);