#include<iostream>

//Race for Grabage collection
using namespace std;
int main()
{
    for(;;)
    {
         int *a=new int;
         delete a;
    }
} 
