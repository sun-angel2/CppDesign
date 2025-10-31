#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <cstdint>
#include <string>

class Executor {
private:
    int32_t x_;         // X坐标
    int32_t y_;         // Y坐标
    char heading_;      // 朝向(N/S/E/W)
    
    // 辅助方法：处理左转
    void turnLeft();
    // 辅助方法：处理右转
    void turnRight();
    // 辅助方法：处理前进
    void moveForward();

public:
    // 默认构造函数，初始化为(0,0,N)
    Executor();
    // 带参数的构造函数，自定义初始位置和朝向
    Executor(int32_t x, int32_t y, char heading);
    
    // 执行单个指令
    void executeCommand(char command);
    // 执行批量指令
    void executeCommands(const std::string& commands);
    
    // 获取当前X坐标
    int32_t getX() const;
    // 获取当前Y坐标
    int32_t getY() const;
    // 获取当前朝向
    char getHeading() const;
    // 一次性获取所有状态信息
    void getStatus(int32_t& x, int32_t& y, char& heading) const;
};

#endif // EXECUTOR_H