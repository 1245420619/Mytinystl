#include <iostream>
#include "queue.h" 

int main() {
    // 1. 测试默认构造和 empty
    afe::queue<int> q;
    std::cout << "1. 初始是否为空: " << (q.empty() ? "是" : "否") << std::endl;
    std::cout << "1. 初始大小: " << q.size() << std::endl;

    // 2. 测试 push 和 front/back
    q.push(10);
    q.push(20);
    q.push(30);
    std::cout << "2. 入队 10, 20, 30 后:" << std::endl;
    std::cout << "   队头 (front): " << q.front() << std::endl; // 应该是 10
    std::cout << "   队尾 (back):  " << q.back() << std::endl;  // 应该是 30
    std::cout << "   当前大小: " << q.size() << std::endl;

    // 3. 测试 pop (先进先出)
    q.pop();
    std::cout << "3. 出队一次后:" << std::endl;
    std::cout << "   队头 (front): " << q.front() << std::endl; // 应该是 20
    std::cout << "   当前大小: " << q.size() << std::endl;

    // 4. 测试比较运算符 (对应你在 queue 里写的 friend 函数)
    afe::queue<int> q2;
    q2.push(20);
    q2.push(30);
    
    if (q == q2) {
        std::cout << "4. q 和 q2 相等" << std::endl;
    } else {
        std::cout << "4. q 和 q2 不相等" << std::endl;
    }

    // 5. 连续出队直到清空，测试边界条件
    q.pop();
    q.pop();
    std::cout << "5. 全部出队后，是否为空: " << (q.empty() ? "是" : "否") << std::endl;

    return 0;
}
