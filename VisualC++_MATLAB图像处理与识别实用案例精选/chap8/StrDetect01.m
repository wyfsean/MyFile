function [Stroke]= StrDetect01(LeftD,Y1,Y2,ST,PT)
% STΪ�ṹ��ֵ��Ϊ��ָ���߶ȺͿ�Ƚṹ�仯�Ĳ�ͬ
SL=0;
SR=0;
SV=0;
Count=0;


%PT=5;    % ͻ�����ֵ
Str='T'; % T��ʾ�ṹδ����Str���ڱ��浱ǰ�Ļ����ṹ
Stroke='T'; % ���ڱ�������ṹ
Range=Y2-Y1+1; % �ַ��Ŀ�Ȼ��߸߶�

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
    else       % ��⵽ͻ��ľ���
        if ((Count>=fix(Range/4)+1))   % �趨�ַ��������ܷ�����ͻ�䷶Χ
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

%========= ��ȡ�ṹ ===============%
if (Count>=fix(Range/4)+1) % ����ͻ���ʣ�ಿ�ֿ����޷��γ��ַ��ṹ
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
   
