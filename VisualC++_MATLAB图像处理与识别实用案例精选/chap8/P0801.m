%I=imread('24-1.jpg'); 该图像的试验结果读者自行试验。
I=imread('22-2.jpg');    
tic 
[y,x,z]=size(I);
myI=double(I);
%%%%%%%%%%% RGB to HSI  %%%%%%%%

HS_I=(myI(:,:,1)+myI(:,:,2)+myI(:,:,3))/3;
t1=toc
tic
%%%% 边缘点数量统计 与 S分量的纹理分割%%%%%%%%%
S=zeros(y,x);

BW= edge(HS_I,'canny',0.2);%边缘检测，对边缘像素点作统计分析。
t2=toc
tic
ES=0;  
   for i=1:x
       for j=1:y
           if (BW(j,i)==1)               
              S(j,i)=1-min(myI(j,i,:))/HS_I(j,i);
              SR=round(S(j,i)*100)/100;
              ES=[ES SR];
           end 
       end
  end
ESx=(0:0.01:1);


BW1=zeros(y,x);
 for i=1:x
       for j=1:y 
           % 根据上述的统计直方图设定 S 的阈值
           if ((BW(j,i)==1)&(S(j,i)<=0.06)&(S(j,i)>=0.01)) 
              BW1(j,i)=1;
           end 
       end
  end
t3=toc
tic
%%%%%%%%%%%%% 边缘像素点数量统计 %%%%%%%%%%%%%%%%%%
 EdgePointsX=sum(BW1); % X方向
 AveragePointsX=mean(EdgePointsX);
 EPx=(1:x);
 
 
%%%%%%%%%%%%% 文字图像区域分割 %%%%%%%%%%%%%%%%%%%
              % X Direction
ColumnX=zeros(1,x);
for i=1:x
    if (EdgePointsX(i)>=AveragePointsX)
        ColumnX(i)=1;
    end
end
Nx=sum(ColumnX); 

Posx=zeros(1,Nx); % 挑出边缘点数量大于平均值的位置
k=1;
for i=1:x
    if (EdgePointsX(i)>=AveragePointsX)
        Posx(k)=i;
        k=k+1;
    end
end

gapx=12;             % 间隔小于10列的，区域合并
Partx0=zeros(1,Nx);
Partx1=zeros(1,Nx);
k=1;
Partx0(1)=Posx(1);
for i=2:Nx
    d=Posx(i)-Posx(i-1);
    if (d>gapx)
       Partx1(k)= Posx(i-1);
       k=k+1;
       Partx0(k)=Posx(i);
   end
end  
Partx1(k)= Posx(Nx);   %  最后一列大于平均值的位置作为，最后一个区域的切分位置

Spanx=zeros(1,k); % 共有k个区域
for i=1:k
    Spanx(i)=sum(ColumnX(Partx0(i):Partx1(i)));
end
[mytemp nPartx]=max(Spanx);
word_Xseg0=Partx0(nPartx);
word_Xseg1=Partx1(nPartx); 

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

 BWSeg=BW1(:,word_Xseg0:word_Xseg1);  % Y方向边缘点统计
 BWT=BWSeg';
 EdgePointsY=sum(BWT);           
 AveragePointsY=mean(EdgePointsY);
 EPy=(1:y);
            
RowY=zeros(1,y);
for i=1:y
    if (EdgePointsY(i)>=AveragePointsY)
        RowY(i)=1;  
    end
end
Ny=sum(RowY);

Posy=zeros(1,Ny); % 挑出边缘点数量大于平均值的位置
k=1;
for i=1:y
    if (EdgePointsY(i)>=AveragePointsY)
        Posy(k)=i;
        k=k+1;
    end
end

gapy=20;             % 间隔小于30列的，区域合并
Party0=zeros(1,Ny);
Party1=zeros(1,Ny);
k=1;
Party0(1)=Posy(1);
for i=2:Ny
    d=Posy(i)-Posy(i-1);
    if (d>gapy)
       Party1(k)= Posy(i-1);
       k=k+1;
       Party0(k)=Posy(i);
   end
end  
Party1(k)= Posy(Ny);   %  最后一列大于平均值的位置作为，最后一个区域的切分位置

Spany=zeros(1,k); % 共有k个区域
for i=1:k
    Spany(i)=sum(RowY(Party0(i):Party1(i)));
end
[mytemp nParty]=max(Spany);
word_Yseg0=Party0(nParty);
word_Yseg1=Party1(nParty); 

word=I(word_Yseg0:word_Yseg1,word_Xseg0:word_Xseg1,:);
t4= toc

HS_I=uint8(HS_I);

%%%%%%%%%%%%%%%%%% Display %%%%%%%%%%%%%%%%%%%%
figure(1);
imshow(I);
figure(2);
imshow(HS_I);
figure(3);
imshow(BW);
figure(4);
hist(ES,ESx);grid
title('S Histogram of the Edge Pixels');
figure(5);
imshow(BW1);
title('The most is the Character Edge pixels in the Histogram');
figure(6);
plot(EPx,EdgePointsX);hold on
plot(EPx,AveragePointsX,'r');grid
title('X Direction Edge Pixels Statistics');
figure(7);
plot(EPy,EdgePointsY);hold on
plot(EPy,AveragePointsY,'r');grid
title('Y Direction Edge Pixels Statistics');
figure(8);
imshow(word);
title('Extraction Character');
