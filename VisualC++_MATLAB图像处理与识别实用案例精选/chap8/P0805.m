I=imread('brand02.jpg');
HS_I=double(I);
tic
G=gradient(HS_I);  % 计算梯度值
[y,x]=size(I);
T=60;
%%%%%%% 商标文字分割 %%%%%%%%%%
n=30;
GY=zeros(y,1);                 
for j=1:y   
    for i=1:x-n
         Max=max(G(j,i:i+n));
         Min=min(G(j,i:i+n));
         MGD(j,i)=Max-Min;       % 计算最大梯度差
         if (MGD(j,i)>T)         
             GY(j,1)=GY(j,1)+1;  % MGD大于阈值的次数统计
         end    
    end
end    
[temp MaxY]=max(GY);
PY1=MaxY;
while ((GY(PY1,1)>=n)&&(PY1>1))  % 次数阈值检测
       PY1=PY1-1;
end    
PY2=MaxY;
while ((GY(PY2,1)>=n)&&(PY2<y))
       PY2=PY2+1;
end
%%%%%%%%%% X 方向的分割 %%%%%%%%%%%
n=15;
GX=zeros(1,x);
for i=1:x
    for j=1:y-n
         Max=max(G(j:j+n,i));
         Min=min(G(j:j+n,i));
         MGD(j,i)=Max-Min;
         if (MGD(j,i)>T)
             GX(1,i)=GX(1,i)+1;
         end
     end
 end
 PX1=1;
while (((GX(1,PX1)<=n)||(GX(1,PX1+1)<=n))&&(PX1<x)) % 列方向保证同时相邻的两列满足次数要求
       PX1=PX1+1;
end    
PX2=x;
while (((GX(1,PX2)<=n)||(GX(1,PX2-1)<=n))&&(PX2>PX1))
       PX2=PX2-1;
end    

HS_I=uint8(HS_I);
IY=HS_I(PY1:PY2,PX1:PX2);
t=toc
%%%%%%%%%%%%%%%%%%%%%%%%%%
figure,imshow(HS_I);
figure,plot(MGD(40,:));grid
figure,plot(MGD(140,:));grid
figure,imshow(IY);
