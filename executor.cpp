#include "executor.h"
#include "vehicle_behavior.h"

// 默认初始化
Executor::Executor() : x_(0), y_(0), heading_('N'), is_accelerating_(false), is_reversing_(false), behavior_(nullptr) {}

// 自定义初始化
Executor::Executor(int32_t x, int32_t y, char heading) 
    : x_(x), y_(y), heading_(heading), is_accelerating_(false), is_reversing_(false), behavior_(nullptr) {}

void Executor::setBehavior(IVehicleBehavior* behavior) {
    behavior_ = behavior;
}

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

// 后退逻辑
void Executor::moveBackward() {
    switch (heading_) {
        case 'N': y_--; break;
        case 'S': y_++; break;
        case 'E': x_--; break;
        case 'W': x_++; break;
    }
}

// 掉头逻辑
void Executor::executeTurnRound() {
    if (behavior_) {
        behavior_->executeTurnRound(this);
        return;
    }

    if (is_accelerating_) {
        moveForward();
        turnLeft();
        moveForward();
        turnLeft();
    } else {
        turnLeft();
        moveForward();
        turnLeft();
    }
}

// 执行单个指令
void Executor::executeCommand(char command) {
    if (behavior_) {
        switch (command) {
            case 'M': behavior_->moveForward(this); break;
            case 'L': behavior_->turnLeft(this); break;
            case 'R': behavior_->turnRight(this); break;
            case 'F': is_accelerating_ = !is_accelerating_; break;
            case 'B': is_reversing_ = !is_reversing_; break;
        }
        return;
    }

    switch (command) {
        case 'F':
            is_accelerating_ = !is_accelerating_;
            break;
        case 'B':
            is_reversing_ = !is_reversing_;
            break;
        case 'M':
            if (is_accelerating_ && is_reversing_) {
                moveBackward();
                moveBackward();
            } else if (is_reversing_) {
                moveBackward();
            } else if (is_accelerating_) {
                moveForward();
                moveForward();
            } else {
                moveForward();
            }
            break;
        case 'L':
            if (is_accelerating_ && is_reversing_) {
                moveBackward();
                turnRight();
            } else if (is_reversing_) {
                turnRight();
            } else if (is_accelerating_) {
                moveForward();
                turnLeft();
            } else {
                turnLeft();
            }
            break;
        case 'R':
            if (is_accelerating_ && is_reversing_) {
                moveBackward();
                turnLeft();
            } else if (is_reversing_) {
                turnLeft();
            } else if (is_accelerating_) {
                moveForward();
                turnRight();
            } else {
                turnRight();
            }
            break;
    }
}

// 执行批量指令
void Executor::executeCommands(const std::string& commands) {
    size_t i = 0;
    while (i < commands.length()) {
        if (i + 1 < commands.length() && commands.substr(i, 2) == "TR") {
            executeTurnRound();
            i += 2;
        } else {
            executeCommand(commands[i]);
            i++;
        }
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