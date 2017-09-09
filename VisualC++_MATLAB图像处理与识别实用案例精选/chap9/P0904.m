I=imread('z12R.jpg');
I=im2bw(I,0.6);
SE1=strel('square', 3);
R=imerode(I,SE1);             % ����ʴ������
Edge_Road=I-R;                %  ��Ե���
[y,x]=size(I);
Px=zeros(y,1);
CenterR=zeros(y,x);
for j=1:y                     % ·�����߼��
    i=1;
    while((Edge_Road(j,i)~=1)&&(i<x))
            i=i+1;
    end
    if (i<x)
        P1=i;                 % ·�����Եλ��
    end
    
    i=x;
    while((Edge_Road(j,i)~=1)&&(i>1))
            i=i-1;
    end
    if (i>1)
        P2=i;                % ·���ұ�Եλ��
    end
   
    Px(j)=round((P1+P2)/2);  % ·������λ��
    CenterR(j,Px(j))=1;      
end
    
figure,imshow(R);
figure,imshow(Edge_Road);
figure,imshow(CenterR);
