#include <iostream>
#include"algorithm.h"
#include "queue.h" 
using namespace std;
using namespace afe;

int main() {
    {
    // 1. 测试默认构造和 empty
    queue<int> q;
    cout << "1. 初始是否为空: " << (q.empty() ? "是" : "否") << endl;
    cout << "1. 初始大小: " << q.size() << endl;

    // 2. 测试 push 和 front/back
    q.push(10);
    q.push(20);
    q.push(30);
    cout << "2. 入队 10, 20, 30 后:" << endl;
    cout << "   队头 (front): " << q.front() << endl; // 应该是 10
    cout << "   队尾 (back):  " << q.back() << endl;  // 应该是 30
    cout << "   当前大小: " << q.size() << endl;

    // 3. 测试 pop (先进先出)
    q.pop();
    cout << "3. 出队一次后:" << endl;
    cout << "   队头 (front): " << q.front() << endl; // 应该是 20
    cout << "   当前大小: " << q.size() << endl;

    // 4. 测试比较运算符 (对应你在 queue 里写的 friend 函数)
    queue<int> q2;
    q2.push(20);
    q2.push(30);
    
    if (q == q2) {
        cout << "4. q 和 q2 相等" << endl;
    } else {
        cout << "4. q 和 q2 不相等" << endl;
    }

    // 5. 连续出队直到清空，测试边界条件
    q.pop();
    q.pop();
    cout << "5. 全部出队后，是否为空: " << (q.empty() ? "是" : "否") << endl;
    }

    {
        int ia[9]={0,1,2,3,4,8,9,3,5};
        priority_queue<int> ipq(ia,ia+9);
        cout<<"size="<<ipq.size()<<endl;

        for(int i=0;i<ipq.size();i++)
            cout<<ipq.top()<<' ';
        cout<<endl;

        while(!ipq.empty()){
            cout<<ipq.top()<<" ";
            ipq.pop();
        }
        cout<<endl;
    }

    return 0;
}
