I0=imread('8.jpg');% ����Ϊ��ֵͼ��
I=im2bw(I0,0.4);
[y0 x0]=size(I);
Range=sum((~I)');
Hy=0;
for j=1:y0
    if (Range(j)>=1)
        Hy=Hy+1;
    end
end
RangeX=sum((~I));
Wx=0;
for i=1:x0
    if (RangeX(i)>=1)
        Wx=Wx+1;
    end
end
Amp=24/Hy;             % ������ͼ���һ����24���ص�ĸ߶ȡ�
I=imresize(I,Amp);
[y x]=size(I);
%I=bwmorph(~I,'skel',Inf);
%I=~I;
tic
%====== �����ṹ =======%
% ��һ�ࣺ����V������б��L������б��R����ͻ�䣨P��
% �ڶ��ࣺ���Բ����C�����Ұ�Բ����Q��
% �����ࣺ�ṹ������T����
%=====================================%
Left=zeros(1,y);         % ����������
for j=1:y
        i=1;
    while ((i<=x)&&(I(j,i)==1))
        i=i+1;
    end
    if (i<=x)          
        Left(j)=i;
    end    
end
for j=1:y-1
    LeftD(j)=Left(j+1)-Left(j);
end
%========== �ṹ������ȡ =============%
j=1;
while ((Left(j)<1)&&(j<y))
       j=j+1;       
end 
Y1=j;

j=y;
while ((Left(j)<1)&&(j>1))
       j=j-1;
end 
Y2=j-1;      % ȥ������仯������

       
%============== �ұ� ==================%
Right=zeros(1,y);         % ����������
for j=1:y
        i=x;
    while ((i>=1)&&(I(j,i)==1))
        i=i-1;
    end
    if (i>=1)          
        Right(j)=i;
    end    
end
for j=1:y-1
    RightD(j)=Right(j+1)-Right(j);
end

%=====================================%
Top=zeros(1,x);         % �����������
for i=1:x
        j=1;
    while ((j<=y)&&(I(j,i)==1))
        j=j+1;
    end
    if (j<=y)          
        Top(i)=j;
    end    
end
for i=1:x-1
    TopD(i)=Top(i+1)-Top(i);
end
%==============================%
i=1;
while ((Top(i)<1)&&(i<x))
       i=i+1;       
end 
X1=i;

i=x;
while ((Top(i)<1)&&(i>1))
       i=i-1;
end 
X2=i-1;      % ȥ������仯������

%===================================%
Bottom=zeros(1,x);      % �ײ��������
for i=1:x
        j=y;
    while ((j>=1)&&(I(j,i)==1))
        j=j-1;
    end
    if (j>=1)
        Bottom(i)=j;
    end    
end
for i=1:x-1
    BottomD(i)=Bottom(i+1)-Bottom(i);
end

%========== ���� 1 �Ŀ������ =========%
Width=zeros(1,y);
for j=1:y
    Width(j)=Right(j)-Left(j);
end    
W=max(Width);

Po=0; % ���ڼ��ʻ�
Ne=0;
NS=0;%�ʻ���
for i=X1+4:X2-4
    for j=1:y-1
        if ((I(j+1,i)-I(j,i))>0) % �ɺڵ���
            Po=Po+1;
            if ((Po>=2)&&(j<=fix(0.7*y)))
                Po=3;
            end    
        else if  ((I(j+1,i)-I(j,i))<0) % �ɰ׵��� 
                  Ne=Ne+1;
                  if ((Ne>=2)&&(j<=fix(0.7*y)))
                       Ne=3;
                  end       
              end
          end                   
      end
      NS=[NS max(Po,Ne)];
      Po=0; 
      Ne=0;
  end
 Comp=max(NS); 
%========== �����ṹ������ȡ ==========%
if (min(W,Wx)>10)
    StrokeT=StrDetect01(TopD,X1,X2,3,6); % ���������ṹ���
    StrokeL=StrDetect01(LeftD,Y1,Y2,3,5); % ��߻����ṹ���
    StrokeR=StrDetect01(RightD,Y1,Y2,3,5); % �ұ߻����ṹ���
    StrokeB=StrDetect01(BottomD,X1,X2,3,6); % �ײ������ṹ���   
%========== ʶ�� ==========%
    Digit=Recognition(StrokeT,StrokeL,StrokeR,StrokeB,Comp)
else
    Digit='1'
end    

t=toc
%======= ��ʾ ======%
px=(1:x);
py=(1:y);

figure(1);
imshow(I);

figure(2);
plot(Left);grid
title('������');

figure(3);
plot(Top);grid
title('������');

figure(4);
plot(Right);grid
title('������');

figure(5);
plot(Width);grid
title('���');
