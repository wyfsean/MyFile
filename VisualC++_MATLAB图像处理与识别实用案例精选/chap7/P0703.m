I=imread('cancer.bmp');
[y,x,z]=size(I);
myI=double(I);                  % 图象的数据类型为无符号整型，必须转换为双精度实型才能计算
%%%%%%%%%%% RGB to HSI  %%%%%%%%
H=zeros(y,x);
S=H;
HS_I=H;
for i=1:x
    for j=1:y
        HS_I(j,i)=((myI(j,i,1)+myI(j,i,2)+myI(j,i,3))/3);
        S(j,i)=1-3*min(myI(j,i,:))/(myI(j,i,1)+myI(j,i,2)+myI(j,i,3));
        if ((myI(j,i,1)==myI(j,i,2))&(myI(j,i,2)==myI(j,i,3)))        % 三者相等，H计算式中分母为零，无法计算
            Hdegree=0;
        else    
            Hdegree=acos(0.5*(2*myI(j,i,1)-myI(j,i,2)-myI(j,i,3))/((myI(j,i,1)-myI(j,i,2))^2+(myI(j,i,1)-myI(j,i,3))*(myI(j,i,2)-myI(j,i,3)))^0.5);
        end    
        if (myI(j,i,2)>=myI(j,i,3))
            H(j,i)=Hdegree;                                     % G≥B, H 在[0,π]范围内
        else
            H(j,i)=(2*pi-Hdegree);                              % G < B, H 在(π,2π]范围内
        end     
    end 
end

%=====色度识别====%          % 色度识别需要依靠专家知识，此处仅举一例说明

Hth1=0.9*2*pi; Hth2=0.1*2*pi;  % 检测红色，红色的色度值在0或2π附近
Nred=0;                        % 记录红色或接近红色的像素点个数
for i=1:x
    for j=1:y
        if ((H(j,i)>=Hth1)||(H(j,i)<=Hth2))
            Nred=Nred+1;       
        end
    end
end

Ratio=Nred/(x*y);            % 红色像素点所占比例

if (Ratio>=0.6)              % 判别是否为红色  
    Red=1
else
    Red=0
end    


HS_I=uint8(HS_I);                                                    % 实型转换为整型才能作为图象显示

figure(1);
imshow(I);
figure(2);
imshow(HS_I);