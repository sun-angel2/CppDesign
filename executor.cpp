#include "executor.h"

// 默认初始化
Executor::Executor() : x_(0), y_(0), heading_('N') {}

// 自定义初始化
Executor::Executor(int32_t x, int32_t y, char heading) 
    : x_(x), y_(y), heading_(heading) {}

// 左转逻辑
void Executor::turnLeft() {
    switch (heading_) {
        case 'N': heading_ = 'W'; break;
        case 'W': heading_ = 'S'; break;
        case 'S': heading_ = 'E'; break;
        case 'E': heading_ = 'N'; break;
    }
}

// 右转逻辑
void Executor::turnRight() {
    switch (heading_) {
        case 'N': heading_ = 'E'; break;
        case 'E': heading_ = 'S'; break;
        case 'S': heading_ = 'W'; break;
        case 'W': heading_ = 'N'; break;
    }
}

// 前进逻辑
void Executor::moveForward() {
    switch (heading_) {
        case 'N': y_++; break;
        case 'S': y_--; break;
        case 'E': x_++; break;
        case 'W': x_--; break;
    }
}

// 执行单个指令
void Executor::executeCommand(char command) {
    switch (command) {
        case 'M': moveForward(); break;
        case 'L': turnLeft(); break;
        case 'R': turnRight(); break;
    }
}

// 执行批量指令
void Executor::executeCommands(const std::string& commands) {
    for (char c : commands) {
        executeCommand(c);
    }
}

// 获取X坐标
int32_t Executor::getX() const {
    return x_;
}

// 获取Y坐标
int32_t Executor::getY() const {
    return y_;
}

// 获取朝向
char Executor::getHeading() const {
    return heading_;
}

// 获取所有状态
void Executor::getStatus(int32_t& x, int32_t& y, char& heading) const {
    x = x_;
    y = y_;
    heading = heading_;
}