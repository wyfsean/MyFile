J= imread('RoadG1.jpg');
[x,y]=size(J);
I=double(J);
z0=max(max(I));                   % 求出图象中最大的灰度
z1=min(min(I));                   % 最小的灰度 

T=(z0+z1)/2;                      
TT=0;
S0=0; n0=0;
S1=0; n1=0;
allow=0.5;                       % 新旧阈值的允许接近程度
d=abs(T-TT);
count=0;                         % 记录几次循环

while(d>=allow)                 % 迭代最佳阈值分割算法
    count=count+1;
    for i=1:x
        for j=1:y
            if (I(i,j)>=T)
                S0=S0+I(i,j);
                n0=n0+1;
            end
            if (I(i,j)<T)
                S1=S1+I(i,j);
                n1=n1+1;
            end
        end
    end 
    T0=S0/n0;
    T1=S1/n1;
    TT=(T0+T1)/2;
    d=abs(T-TT);
    T=TT;
end

Seg=zeros(x,y);
for i=1:x
    for j=1:y
        if(I(i,j)>=T)
            Seg(i,j)=1;               % 阈值分割的图象
        end
    end
end

J0=Seg;
SE=strel('rectangle', [6 4]);         % 结构定义
IM=imopen(J0,SE);              %  “开”运算

figure,imshow(J);    % 图9-7(a)
figure,imhist(J);     % 图9-7(b)
figure,imshow(Seg);  % 图9-7(c)
figure,imshow(IM);  % 图9-7(d)
