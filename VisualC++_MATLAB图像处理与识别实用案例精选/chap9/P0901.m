J= imread('RoadG1.jpg');
[x,y]=size(J);
I=double(J);
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

J0=Seg;
SE=strel('rectangle', [6 4]);         % �ṹ����
IM=imopen(J0,SE);              %  ����������

figure,imshow(J);    % ͼ9-7(a)
figure,imhist(J);     % ͼ9-7(b)
figure,imshow(Seg);  % ͼ9-7(c)
figure,imshow(IM);  % ͼ9-7(d)
