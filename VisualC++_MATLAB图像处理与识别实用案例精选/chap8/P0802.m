I=imread('19-3-02.jpg');
%I=imread('49-3-BW01.jpg');
I=im2bw(I,0.43);
[y x]=size(I);


Top=zeros(1,x);         % �����������
for i=1:x
        j=1;
    while ((I(j,i)==1)&&(j<y))
        j=j+1;
    end
    Top(i)=y-j;
end

Bottom=zeros(1,x);      % �ײ��������
for i=1:x
        j=y;
    while ((I(j,i)==1)&&(j>1))
        j=j-1;
    end
    Bottom(i)=y-j;
end

Height=Top-Bottom;
WordHeight=max(Height);    % ���ָ߶�

%=== �����ߵİ���� ===%
TopD=zeros(1,x-1);
Concave=1;                            % ��¼����������λ��, 1��ʾĬ����ʼ��Ϊ��һ��Concave
Deep=0;                               % �½�ֵ
DeepH=0;                              %  ����ֵ
DeepT=5;                              % �������������ֵ
Sign=0;
for i=1:x-1
    TopD(i)=Top(i+1)-Top(i);
end
for i=1:x-2
    if (TopD(i)<0)  % �ж��Ƿ�Ϊ������
        Sign=1;     % �ñ�־λ
        DeepH=0;
        Deep=Deep+TopD(i);
        tempX=i+1;  % ��һ�п���Ϊ�зֵ�Concave�� ��ӽ������
    end
    if ((Sign==1)&&(TopD(i)>0))
        if (abs(Deep)>=DeepT)
            DeepH=DeepH+TopD(i);
            if (abs(DeepH)>=DeepT)
               Concave=[Concave tempX];
               Sign=0;    % ȷ��Ϊ���󣬸�λ��־λ
               DeepH=0;
           end
        else
           Sign=0;    % ȷ��Ϊ���󣬸�λ��־λ
           Deep=0;           
        end      
    end
end

%=== �����ߵ�͹��� ===%
BottomD=zeros(1,x-1);
Convex=1;
Asend=0;                     % ����ֵ
Desend=0;                    % �½�ֵ
ConvexT=3;                   %  ͹�̶���ֵ
Sign=0;
for i=1:x-1
    BottomD(i)=Bottom(i+1)-Bottom(i);
end
for i=1:x-2
    if (BottomD(i)>0)
        Sign=1;
        Desend=0;
        Asend=Asend+BottomD(i);        
        tempX=i+1;         % ��ӽ������
    end
    if((Sign==1)&&(BottomD(i)<0))
        if (abs(Asend)>=ConvexT)
            Desend=Desend+BottomD(i);
            if (abs(Desend)>=ConvexT)
                Convex=[Convex tempX];
                Sign=0;             % ��λ                                    
                Desend=0;
            end
        else
        Sign=0;             % ��λ
        Asend=0;     
        end  
    end
end

%=== �з� ===%

[mytemp n]=size(Concave);   % ע�� Concave �ĵ�һ����ֵ��Ч
StrokeT=5;                  % �ʻ������ֵ
GapT=8;
W=zeros(1,n);

for i=1:n-1
    W(i)=Concave(i+1)-Concave(i);
end
W(n)=x-Concave(n);
Width=median(W);           % ���Ƶ��ַ����

    PXR1=1;                 % ��¼��һ���з�λ��
    PXR2=1;                 % ��¼�ڶ����з�λ��
       
    Mark=0;         % ��¼�ڰ�ת���Ĵ���
    %CrossSign=0;    % ����ճ���ı�־
    
    Black=zeros(1,x);        % ͳ�Ʊʻ����ص�
    BP=zeros(1,x);
    SegSoke=zeros(3,x);      % �зֵ㴦�ıʻ����
    RH=zeros(1,x);           % �зֺ�ĸ߶ȱ�
    RW=zeros(1,x);           % �зֺ�Ŀ�ȱ�    
    Score=zeros(1,x);        % ����ֵ���ܵ÷�
    
    XGood=1;                 %  X�з�λ��    
    SegY=1;         % ��¼��һ���зֵ�Y���

for k=2:n
    WordH=max(Height(Concave(k-1):Concave(k)));
    WordW=Concave(k)-Concave(k-1);
    if ((WordW>=0.5*Width)&&(WordW<=1.5*Width))
             
        % ѡ���зֵ�����
        PX1=Concave(k);
        PX2=PX1;              
        while ((TopD(PX2)==0)&&(PX2<x))
               PX2=PX2+1;                % ���ұߵ���λ��
        end
            
       i=fix((PX1+PX2)/2);
   
       if (Top(i)==1)                  % ��ճ��
           PXR1=[PXR1 i];
           PXR2=[PXR2 i];
       else                            
            j=y+1-Top(i);                % PYΪʵ�ʵ�y����ֵ���˴���Ϊ��ɫ���ص�
            Mark=0;        
            while((j<y)&&(Mark<2)) 
                if (I(j,i)==0)
                    Black(i)=Black(i)+1; % ��¼��ɫ���ص���            
                    Si=i;
                    while ((Si>1)&&(I(j,Si)==0))   % ��ʻ����
                            Si=Si-1;
                            SegSoke(1,i)=SegSoke(1,i)+1;
                    end   
                        
                    Si=i;
                    while ((Si<x)&&(I(j,Si)==0))   % �ұʻ����
                           Si=Si+1;
                           SegSoke(2,i)=SegSoke(2,i)+1;
                     end
               end
                 
                Mark=Mark+abs(I(j+1,i)-I(j,i));  % ����Ƿ�ͨ���ʻ�
                j=j+1;
            end
            SegY=[SegY j-1];                           % ��һ���зֽ�ֹ��
            if (j==38)
                PXR1=[PXR1 i];
                PXR2=[PXR2 i];
            else                                       % ����ճ��  
                SLi=i;
                while ((SLi>1)&&(I(j-1,SLi)==1))        % ѡ��������߽�
                       SLi=SLi-1;
                end
        
                SRi=i;
                while ((SRi<x)&&(I(j-1,SRi)==1))        % ѡ�������ұ߽�
                       SRi=SRi+1;
                end
                [Mytemp PX2]=max(Bottom(SLi:SRi));
                PXR2=[PXR2 PX2+SLi-1];
                PXR1=[PXR1 i];
            end
      end    
         
    else if (WordW>1.5*Width)
             PX=fix((Concave(k)+Concave(k-1))/2);  % ����ˮƽ���ᡱ��ճ��
             k=k-1;
         end
                      %  �����ȹ�С�����з�       
   end            
               
end

%==== Segment ===%
for i=2:n
    WI=I(:,fix((PXR1(i-1)+PXR2(i-1))/2):fix((PXR1(i)+PXR2(i))/2));
    figure(10+i);imshow(WI);
end
WI=I(:,fix((PXR1(i)+PXR2(i))/2):x);
figure(10+i+1);imshow(WI);


%=== ͼ����ʾ ===%

px=(1:x);                % X������

figure(1);
imshow(I);

figure(2);
plot(Top);hold on
plot(px,y,'red');grid
title('������');

figure(3);
plot(Bottom);hold on
plot(px,y,'red');grid
title('������');

figure(4);
plot(Height);grid
