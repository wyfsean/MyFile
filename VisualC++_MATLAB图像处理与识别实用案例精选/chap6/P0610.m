time=1:0.01:2.5;
X=sin(sin(time).*time*10);
P=con2seq(X);
T=con2seq(2*[0 X(1:(end-1))]+X);  %创建一个信号T
plot(time, cat(2,T{:}),'-');
Net=newlin([-3 3],1,[0 1],0.1); %生成一个神经网络；[-3 3]是预期的输入范围；第二个
                                      %参数是每一层的神经元数目。[0 1]指定一个无延迟输入
                                      %和一个有延迟输入。最后一个参数表示学习速率。
[Net,Y,E,Pf]=adapt(Net,P,T);  %调用adapt函数来学习这个信号
figure,plot(time,cat(2,Y{:}),time,cat(2,T{:}),'-*',time,cat(2,E{:}),'-',[1 2.5], [0 0], 'k');grid % 实线表示原始信号T，*表示学习过程Y，点划线表示学习误差，
                    % 如图6-25所示
