I=imread('z12R.jpg');
I=im2bw(I,0.6);
SE1=strel('square', 3);
R=imerode(I,SE1);             % “腐蚀”运算
Edge_Road=I-R;                %  边缘检测
[y,x]=size(I);
Px=zeros(y,1);
CenterR=zeros(y,x);
for j=1:y                     % 路径中线检测
    i=1;
    while((Edge_Road(j,i)~=1)&&(i<x))
            i=i+1;
    end
    if (i<x)
        P1=i;                 % 路径左边缘位置
    end
    
    i=x;
    while((Edge_Road(j,i)~=1)&&(i>1))
            i=i-1;
    end
    if (i>1)
        P2=i;                % 路径右边缘位置
    end
   
    Px(j)=round((P1+P2)/2);  % 路径中线位置
    CenterR(j,Px(j))=1;      
end
    
figure,imshow(R);
figure,imshow(Edge_Road);
figure,imshow(CenterR);
