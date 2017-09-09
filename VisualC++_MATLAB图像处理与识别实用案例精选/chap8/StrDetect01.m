function [Stroke]= StrDetect01(LeftD,Y1,Y2,ST,PT)
% ST为结构阈值，为了指定高度和宽度结构变化的不同
SL=0;
SR=0;
SV=0;
Count=0;


%PT=5;    % 突变的阈值
Str='T'; % T表示结构未定，Str用于保存当前的基本结构
Stroke='T'; % 用于保存基本结构
Range=Y2-Y1+1; % 字符的宽度或者高度

for j=Y1:Y2
    Count=Count+1;
    
    if (abs(LeftD(j))<PT)
           if (LeftD(j)<0)
                   SL=SL+1;
               else if (LeftD(j)>0)
                   SR=SR+1;
               else
                   SV=SV+1;
               end
           end 
    else       % 检测到突变的决策
        if ((Count>=fix(Range/4)+1))   % 设定字符轮廓可能发生的突变范围
            if ((SL>=3)&&(SR>=3))
                  Str='C';
            else if ((SV>=2*(SL+SR))&&((max(SL,SR)<3)||(min(SL,SR)<2)))
                  Str='V';
               else if ((SL>SR)&&((SL>=0.5*SV)&&((SR<=1)||(SL>(SR+SV)))))
                       Str='L';
                     else if ((SR>SL)&&((SR>=0.5*SV)&&((SL<=1)||(SL>(SR+SV)))))
                              Str='R';
                              else if (max(SL,SR)>=3)&&(min(SL,SR)>=2) 
                                  Str='C';
                              end
                          end
                     end
               end
           end
         Stroke=[Stroke Str];  
     end
         if ((j>=2+Y1)&&((j<=Y2-2)))
              Stroke=[Stroke 'P'];
         end     
        SL=0;
        SR=0;
        SV=0;
        Count=0;
        Str='T';
    end
end 

%========= 提取结构 ===============%
if (Count>=fix(Range/4)+1) % 发生突变后，剩余部分可能无法形成字符结构
if ((SL>=ST)&&(SR>=ST))
    Str='C';
else if ((SV>=2*(SL+SR))&&((max(SL,SR)<3)||(min(SL,SR)<2)))
         Str='V';
     else if ((SL>SR)&&((SL>=0.5*SV)&&((SR<=2)||(SL>=(SR+SV)))))
              Str='L';
          else if ((SR>SL)&&((SR>=0.5*SV)&&((SL<=2)||(SL>=(SR+SV)))))
                   Str='R';
               else if (max(SL,SR)>=3)&&(min(SL,SR)>=2) 
                        Str='C';
                    end     
               end
           end
       end
   end
Stroke=[Stroke Str];   
end  
   
