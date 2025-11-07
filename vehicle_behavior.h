#ifndef IVEHICLE_BEHAVIOR_H
#define IVEHICLE_BEHAVIOR_H

#include <cstdint>

class Executor; // 前向声明，避免循环依赖

class IVehicleBehavior {
public:
    virtual ~IVehicleBehavior() = default;

    // 指令行为方法，需要访问Executor的状态，因此传入Executor指针
    virtual void moveForward(Executor* executor) = 0;
    virtual void turnLeft(Executor* executor) = 0;
    virtual void turnRight(Executor* executor) = 0;
    virtual void executeTurnRound(Executor* executor) = 0;
};

#endif // IVEHICLE_BEHAVIOR_H
