/*************************************************************************
	> File Name: string.cpp
	> Author: jetming
	> Mail: jetmingx@qq.com
	> Created Time: Fri 20 May 2016 03:16:40 PM CST
 ************************************************************************/

#include<iostream>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

using namespace std;

class myString{
public:
    myString(){};
    ~myString(){};

    char* strcpy(char* des,char* source);
};

char* myString::strcpy(char* des,char* source){
    assert((des!=NULL&&source!=NULL));
    char* des_strat=des;
    
    while((*des++=*source++)!='\0');

    return des_strat;
}


int main()
{
    myString mystr;

    char *a=(char*)malloc(sizeof(char*)*6);
    char b[16]="hello";
    
    char *c=(char*)malloc(sizeof(char*));
    //*(c+2)='h';

    //cout<<"++a:"<<*(a+10)<<"c:"<<*(c+2)<<endl;

    strcpy(c,b);// c库函数 
    cout<<"c:"<<c<<" strlen:"<<strlen(b)<<endl;
    free(c);
    if(mystr.strcpy(a,b)==NULL)
        cout<<"cpy erro.";

    cout<<"a:"<<a<<endl;
    
}
