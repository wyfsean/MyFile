I=imread('19-3-02.jpg');
%I=imread('49-3-BW01.jpg');
I=im2bw(I,0.43);
[y x]=size(I);


Top=zeros(1,x);         % 顶端轮廓检测
for i=1:x
        j=1;
    while ((I(j,i)==1)&&(j<y))
        j=j+1;
    end
    Top(i)=y-j;
end

Bottom=zeros(1,x);      % 底部轮廓检测
for i=1:x
        j=y;
    while ((I(j,i)==1)&&(j>1))
        j=j-1;
    end
    Bottom(i)=y-j;
end

Height=Top-Bottom;
WordHeight=max(Height);    % 文字高度

%=== 轮廓线的凹检测 ===%
TopD=zeros(1,x-1);
Concave=1;                            % 记录凹轮廓处的位置, 1表示默认起始列为第一个Concave
Deep=0;                               % 下降值
DeepH=0;                              %  上升值
DeepT=5;                              % 凹轮廓的深度阈值
Sign=0;
for i=1:x-1
    TopD(i)=Top(i+1)-Top(i);
end
for i=1:x-2
    if (TopD(i)<0)  % 判断是否为凹轮廓
        Sign=1;     % 置标志位
        DeepH=0;
        Deep=Deep+TopD(i);
        tempX=i+1;  % 下一列可能为切分的Concave， 最接近于左端
    end
    if ((Sign==1)&&(TopD(i)>0))
        if (abs(Deep)>=DeepT)
            DeepH=DeepH+TopD(i);
            if (abs(DeepH)>=DeepT)
               Concave=[Concave tempX];
               Sign=0;    % 确认为凹后，复位标志位
               DeepH=0;
           end
        else
           Sign=0;    % 确认为凹后，复位标志位
           Deep=0;           
        end      
    end
end

%=== 轮廓线的凸检测 ===%
BottomD=zeros(1,x-1);
Convex=1;
Asend=0;                     % 上升值
Desend=0;                    % 下降值
ConvexT=3;                   %  凸程度阈值
Sign=0;
for i=1:x-1
    BottomD(i)=Bottom(i+1)-Bottom(i);
end
for i=1:x-2
    if (BottomD(i)>0)
        Sign=1;
        Desend=0;
        Asend=Asend+BottomD(i);        
        tempX=i+1;         % 最接近于左端
    end
    if((Sign==1)&&(BottomD(i)<0))
        if (abs(Asend)>=ConvexT)
            Desend=Desend+BottomD(i);
            if (abs(Desend)>=ConvexT)
                Convex=[Convex tempX];
                Sign=0;             % 复位                                    
                Desend=0;
            end
        else
        Sign=0;             % 复位
        Asend=0;     
        end  
    end
end

%=== 切分 ===%

[mytemp n]=size(Concave);   % 注意 Concave 的第一个数值无效
StrokeT=5;                  % 笔划宽度阈值
GapT=8;
W=zeros(1,n);

for i=1:n-1
    W(i)=Concave(i+1)-Concave(i);
end
W(n)=x-Concave(n);
Width=median(W);           % 近似的字符宽度

    PXR1=1;                 % 记录第一次切分位置
    PXR2=1;                 % 记录第二次切分位置
       
    Mark=0;         % 记录黑白转换的次数
    %CrossSign=0;    % 交错粘连的标志
    
    Black=zeros(1,x);        % 统计笔划像素点
    BP=zeros(1,x);
    SegSoke=zeros(3,x);      % 切分点处的笔划宽度
    RH=zeros(1,x);           % 切分后的高度比
    RW=zeros(1,x);           % 切分后的宽度比    
    Score=zeros(1,x);        % 特征值的总得分
    
    XGood=1;                 %  X切分位置    
    SegY=1;         % 记录第一次切分的Y深度

for k=2:n
    WordH=max(Height(Concave(k-1):Concave(k)));
    WordW=Concave(k)-Concave(k-1);
    if ((WordW>=0.5*Width)&&(WordW<=1.5*Width))
             
        % 选定切分的区域
        PX1=Concave(k);
        PX2=PX1;              
        while ((TopD(PX2)==0)&&(PX2<x))
               PX2=PX2+1;                % 凹右边的列位置
        end
            
       i=fix((PX1+PX2)/2);
   
       if (Top(i)==1)                  % 无粘连
           PXR1=[PXR1 i];
           PXR2=[PXR2 i];
       else                            
            j=y+1-Top(i);                % PY为实际的y坐标值，此处已为黑色像素点
            Mark=0;        
            while((j<y)&&(Mark<2)) 
                if (I(j,i)==0)
                    Black(i)=Black(i)+1; % 记录黑色象素点数            
                    Si=i;
                    while ((Si>1)&&(I(j,Si)==0))   % 左笔划宽度
                            Si=Si-1;
                            SegSoke(1,i)=SegSoke(1,i)+1;
                    end   
                        
                    Si=i;
                    while ((Si<x)&&(I(j,Si)==0))   % 右笔划宽度
                           Si=Si+1;
                           SegSoke(2,i)=SegSoke(2,i)+1;
                     end
               end
                 
                Mark=Mark+abs(I(j+1,i)-I(j,i));  % 检测是否通过笔划
                j=j+1;
            end
            SegY=[SegY j-1];                           % 第一次切分截止处
            if (j==38)
                PXR1=[PXR1 i];
                PXR2=[PXR2 i];
            else                                       % 单点粘连  
                SLi=i;
                while ((SLi>1)&&(I(j-1,SLi)==1))        % 选定区域左边界
                       SLi=SLi-1;
                end
        
                SRi=i;
                while ((SRi<x)&&(I(j-1,SRi)==1))        % 选定区域右边界
                       SRi=SRi+1;
                end
                [Mytemp PX2]=max(Bottom(SLi:SRi));
                PXR2=[PXR2 PX2+SLi-1];
                PXR1=[PXR1 i];
            end
      end    
         
    else if (WordW>1.5*Width)
             PX=fix((Concave(k)+Concave(k-1))/2);  % 避免水平“横”的粘连
             k=k-1;
         end
                      %  如果宽度过小，则不切分       
   end            
               
end

%==== Segment ===%
for i=2:n
    WI=I(:,fix((PXR1(i-1)+PXR2(i-1))/2):fix((PXR1(i)+PXR2(i))/2));
    figure(10+i);imshow(WI);
end
WI=I(:,fix((PXR1(i)+PXR2(i))/2):x);
figure(10+i+1);imshow(WI);


%=== 图像显示 ===%

px=(1:x);                % X轴坐标

figure(1);
imshow(I);

figure(2);
plot(Top);hold on
plot(px,y,'red');grid
title('上轮廓');

figure(3);
plot(Bottom);hold on
plot(px,y,'red');grid
title('下轮廓');

figure(4);
plot(Height);grid
