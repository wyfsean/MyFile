blood = imread('blood1.BMP');
[x,y]=size(blood);                % ���ͼ���С
b=double(blood);                  
N =sqrt(100) * randn(x,y);        %  ���ɷ���Ϊ10�İ�����
I=b+N;                            %  ��������ͼ��

for i=1:x                         % ʵ��ͼ��ĻҶ�Ϊ0��255
    for j=1:y
        if (I(i,j)>255)
            I(i,j)=255;
        end
        if (I(i,j)<0)
            I(i,j)=0;
        end
    end
end    


z0=max(max(I));                   % ���ͼ�������ĻҶ�
z1=min(min(I));                   % ��С�ĻҶ� 

T=(z0+z1)/2;                      
TT=0;
S0=0; n0=0;
S1=0; n1=0;
allow=0.5;                       % �¾���ֵ������ӽ��̶�
d=abs(T-TT);
count=0;                         % ��¼����ѭ��

while(d>=allow)                 % ���������ֵ�ָ��㷨
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
            Seg(i,j)=1;               % ��ֵ�ָ��ͼ��
        end
    end
end

SI=1-Seg;                            % ��ֵ�ָ���ͼ���󷴣������ø�ʴ�㷨���Ե
se1=strel('square',3);               % ���帯ʴ�㷨�Ľṹ
SI1=imerode(SI,se1);                 % ��ʴ�㷨
BW=SI-SI1;                           % ��Ե���
%=====��ͳ�ı�Ե��ⷽ��======%
I=uint8(I);
BW1=edge(I,'sobel', 0.11);
BW2=edge(I,'log', 0.015);
%===========ͼ����ʾ==========%
figure(1);
imshow(I);title('Original')                         % ��ʾ��ֵ�ָ��ͼ��
figure(2);
imshow(BW2);title('Soble')                         % ��ʾ���㷨�ı�Եͼ��
figure(3)
imshow(BW1);title('Gauss-Laplace') 
figure(4)
imshow(BW);title('New algorithm') 