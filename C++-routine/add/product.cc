#include<iostream>
int main()
{
    int a,sum=0;
    while(std::cin>>a)
        sum += a;
    std::cout<<"the sum form 1 to 10 is:"<<sum<<std::endl;
    return 0;
}