function [Numeral]=Recognition(StrokeTop,StrokeLeft,StrokeRight,StrokeBottom,Comp)
% 采用四边的轮廓结构特征和笔划统计（仅针对 0 和 8）识别残缺数字
% Comp 是用于识别 0和8 的底部补充信息
StrT='T';
StrL='T';
StrR='T';
StrB='T';
RStr='T'; % 用于保存识别出的数字
[temp XT]=size(StrokeTop);
[temp XL]=size(StrokeLeft);
[temp XR]=size(StrokeRight);
%[temp XB]=size(StrokeBottom);

for Ti=2:XT
    if (StrokeTop(Ti)=='C')
        if ((XL==2)&&(XR==2))
             if ((Comp>=3)||((StrokeBottom(2)~='C')&&(StrokeLeft(2)=='C')&&(StrokeRight(2)=='C')))
                 RStr='8';
             else
                 RStr='0';
             end    
            
        else if ((StrokeLeft(XL)=='L')&&(StrokeLeft(XL-1)=='P')&&(StrokeLeft(2)~='C'))
                 RStr='2';
             else if ((StrokeLeft(2)=='C')&&(XL>=3)&&(StrokeLeft(3)=='P'))
                      RStr='9';
                  else if (XL>2)
                          for Li=2:XL
                              if (StrokeLeft(Li)=='P')
                                  RStr='3';
                              end
                          end
                      else if (XL==2)
                              for Ri=2:XR-1
                                  if (StrokeRight(Ri)=='P')
                                      RStr='6';
                                  end
                              end
                          end  
                      end  
                  end    
             end
         end     
    else if  (StrokeTop(Ti)=='V')  % Top
            if ((XR==2)&&(StrokeRight(2)=='C')) % 数字 3 右端只有一个结构
                RStr='3';       
            else if ((XR==2)&&((StrokeLeft(2)=='P')||(StrokeLeft(3)=='P')||(StrokeLeft(XL)=='V')))
                      RStr='7';
                  else if (XR>2)
                        for Ri=2:XR
                            if (StrokeRight(Ri)=='P')
                                RStr='5';
                            end
                        end
                        end
                  end
            end
        else if ((StrokeTop(Ti)=='L')) % Top
                 if ((StrokeRight(XR)=='V')&&(XR<=2))
                    RStr='4';
                else 
                    for Ri=2:XR
                        if ((StrokeRight(Ri)=='L')||(StrokeRight(Ri)=='C'))
                           RStr='6';
                        end
                    end
                end 
            end  
                
        end  %对应于'V'
    end % 对应于'C'
end % 对应于第一个 for 循环   
Numeral=RStr;                
            
