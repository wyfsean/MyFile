I=imread('z12.JPG');
[y,x,z]=size(I);
myI=double(I);
tic
%%%%%%%%%%% RGB to HSI  %%%%%%%%
H=zeros(y,x);
S=H;
HS_I=H;
for i=1:x
    for j=1:y
        HS_I(j,i)=((myI(j,i,1)+myI(j,i,2)+myI(j,i,3))/3);    % I 分量；读者实际应用时可以不作！
        S(j,i)=1-3*min(myI(j,i,:))/(myI(j,i,1)+myI(j,i,2)+myI(j,i,3));
        if  ((myI(j,i,1)==myI(j,i,2))&(myI(j,i,2)==myI(j,i,3)))   % 三者相等，式(10)分母为0
            Hdegree=0;
        else   
            Hdegree=acos(0.5*(2*myI(j,i,1)-myI(j,i,2)-myI(j,i,3))/((myI(j,i,1)-myI(j,i,2))^2+(myI(j,i,1)-myI(j,i,3))*(myI(j            ,i,2)-myI(j,i,3)))^0.5);
        end    
        if (myI(j,i,2)>=myI(j,i,3))
            H(j,i)=Hdegree/(2*pi);          % G>=B,H在[0 π]范围内
        else
            H(j,i)=(2*pi-Hdegree)/(2*pi);    % G<B,H在[0 2π]范围内
        end 
        if (H(j,i)>=0.9)
            H(j,i)=0;                      % H在0和2π附近应同属于一种颜色分量
        end    
    end 
end

t=toc
%%%%%%%%%%%%%%% Slect the White and Black %%%%%%%%%%%%%%%%%%%%%%
HS_I=uint8(HS_I);
H=255*H;
H=uint8(H);
S=255*S;
S=uint8(S);

I0=zeros(y,x);
for i=1:x
    for j=1:y
               
        if (((H(j,i)<=35)&(S(j,i)>=80)))
             I0(j,i)=1;
         else
             I0(j,i)=0;
         end 
    end 
end

SE=strel('rectangle', [12 8]);
IM=imclose(I0,SE);
figure(1);
imshow(I);
figure(3);
imshow(H);
figure,
imshow(S);
figure,
imshow(HS_I);
figure,imshow(I0);
figure,imshow(IM);
