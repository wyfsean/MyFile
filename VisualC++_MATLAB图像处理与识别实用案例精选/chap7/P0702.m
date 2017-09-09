I=imread('cancer02.bmp');       % ע����뱣֤��ֵͼ���У�ϸ������Ϊ��ɫ����������ص�ֵΪ��1��
[x,y]=size(I);

BW = bwperim(I,8);              % ���ϸ���ı�Ե���٣����ڼ����ܳ� 

%��ⴹֱ�����������ܳ����ص�%
P1=0;
P2=0;
Ny=0;                           % ��¼��ֱ���������ܳ����ص�ĸ���
for i=1:x
    for j=1:y
        if (BW(i,j)>0)
            P2=j;
            if ((P2-P1)==1)    % �ж��Ƿ�Ϊ��ֱ�����������ܳ����ص�
                Ny=Ny+1;
            end
            P1=P2;
        end
    end
end    

%���ˮƽ�����������ܳ����ص�%
P1=0;
P2=0;
Nx=0;                           % ��¼ˮƽ���������ܳ����ص�ĸ���
for j=1:y
    for i=1:x
        if (BW(i,j)>0)
            P2=i;
            if ((P2-P1)==1)    % �ж��Ƿ�Ϊˮƽ�����������ܳ����ص�
                Nx=Nx+1;
            end
            P1=P2;
        end
    end
end 


SN=sum(sum(BW));            % �����ܳ����ص������
Nd=SN-Nx-Ny;                % �����������������Ŀ

H=max(sum(I));              % ����ϸ���ĸ߶� 
W=max(sum(I'));             % ͼ��I������ת�ú󣬼�����
L=sqrt(2)*Nd+Nx+Ny;         % �����ܳ�

%====�ĸ���̬����ֵ����===%
A=bwarea(I);                % ����ϸ�������
C=4*pi*A/(L*L);             % ����Բ��
R=A/(H*W);                  % ������ζ�
E=min(H,W)/max(H,W);        % �����쳤��

%==�趨�����ֵ��ʶ��ϸ��==%
Ath1=10000; Ath2=50000;
Cth=0.5;      Rth=0.5;   Eth=0.8;


if ((A>=Ath1)&&(A<Ath2))
    if ((C>=Cth)&&(R<=Rth)&&(E>Eth))
        Cancer_Right=1                     % ����Ϊ��ϸ��
    end    
else if (A>=Ath2)
        Cancer_Right=2                     % ����Ϊ����Сϸ����ϸ��      
else
    Cancer_Right=0                         % ����Ϊ����ϸ�� 
end
end

figure(1);
imshow(I);
figure(2);
imshow(BW);