I=imread('cancer.bmp');
[y,x,z]=size(I);
myI=double(I);                  % ͼ�����������Ϊ�޷������ͣ�����ת��Ϊ˫����ʵ�Ͳ��ܼ���
%%%%%%%%%%% RGB to HSI  %%%%%%%%
H=zeros(y,x);
S=H;
HS_I=H;
for i=1:x
    for j=1:y
        HS_I(j,i)=((myI(j,i,1)+myI(j,i,2)+myI(j,i,3))/3);
        S(j,i)=1-3*min(myI(j,i,:))/(myI(j,i,1)+myI(j,i,2)+myI(j,i,3));
        if ((myI(j,i,1)==myI(j,i,2))&(myI(j,i,2)==myI(j,i,3)))        % ������ȣ�H����ʽ�з�ĸΪ�㣬�޷�����
            Hdegree=0;
        else    
            Hdegree=acos(0.5*(2*myI(j,i,1)-myI(j,i,2)-myI(j,i,3))/((myI(j,i,1)-myI(j,i,2))^2+(myI(j,i,1)-myI(j,i,3))*(myI(j,i,2)-myI(j,i,3)))^0.5);
        end    
        if (myI(j,i,2)>=myI(j,i,3))
            H(j,i)=Hdegree;                                     % G��B, H ��[0,��]��Χ��
        else
            H(j,i)=(2*pi-Hdegree);                              % G < B, H ��(��,2��]��Χ��
        end     
    end 
end

%=====ɫ��ʶ��====%          % ɫ��ʶ����Ҫ����ר��֪ʶ���˴�����һ��˵��

Hth1=0.9*2*pi; Hth2=0.1*2*pi;  % ����ɫ����ɫ��ɫ��ֵ��0��2�и���
Nred=0;                        % ��¼��ɫ��ӽ���ɫ�����ص����
for i=1:x
    for j=1:y
        if ((H(j,i)>=Hth1)||(H(j,i)<=Hth2))
            Nred=Nred+1;       
        end
    end
end

Ratio=Nred/(x*y);            % ��ɫ���ص���ռ����

if (Ratio>=0.6)              % �б��Ƿ�Ϊ��ɫ  
    Red=1
else
    Red=0
end    


HS_I=uint8(HS_I);                                                    % ʵ��ת��Ϊ���Ͳ�����Ϊͼ����ʾ

figure(1);
imshow(I);
figure(2);
imshow(HS_I);