/*************************************************************************
	> File Name: test_threadpool.cpp
	> Author: jetming
	> Mail: jetmingx@qq.com
	> Created Time: Sat 25 Jun 2016 08:30:40 PM CST
 ************************************************************************/

#include<iostream>

#include "threadpool.h"

using namespace std;

class worker{
public:
    worker(int a,int b):m_a(a),m_b(b){}
    ~worker(){}
    void process(){
        cout<<"process:"<<m_a+m_b<<endl;
    }

private:
    int m_a;
    int m_b;

};

int main()
{
    //worker wk(1,2);
    threadpool<worker> pool;

    int i=0;
    while(i<8){
        worker* wk=new worker(i++,i+1);
        pool.append(wk);
    }

    while(1);
    return 0;
}

