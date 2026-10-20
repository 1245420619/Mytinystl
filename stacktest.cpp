#include <iostream>
#include "stack.h"

int main() {
    // 1. 测试基本构造和 empty
    afe::stack<int> s;
    std::cout << "1. 初始是否为空: " << (s.empty() ? "是" : "否") << std::endl;
    std::cout << "1. 初始大小: " << s.size() << std::endl;

    // 2. 测试 push 和 top
    s.push(10);
    s.push(20);
    s.push(30);
    std::cout << "2. 压入 10, 20, 30 后，栈顶: " << s.top() << std::endl;
    std::cout << "2. 当前大小: " << s.size() << std::endl;

    // 3. 测试 pop
    s.pop();
    std::cout << "3. 弹出一次后，栈顶: " << s.top() << std::endl;
    std::cout << "3. 当前大小: " << s.size() << std::endl;

    // 4. 测试比较运算符 (对应你刚写的 friend 函数)
    afe::stack<int> s2;
    s2.push(10);
    s2.push(20);
    
    if (s == s2) {
        std::cout << "4. s 和 s2 相等" << std::endl;
    } else {
        std::cout << "4. s 和 s2 不相等" << std::endl;
    }

    // 5. 连续 pop 直到清空，测试边界条件
    s.pop();
    s.pop(); // 此时 s 应该为空
    std::cout << "5. 全部弹出后，是否为空: " << (s.empty() ? "是" : "否") << std::endl;

    return 0;
}
#endif
