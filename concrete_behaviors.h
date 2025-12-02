#ifndef CONCRETE_BEHAVIORS_H
#define CONCRETE_BEHAVIORS_H

#include "vehicle_behavior.h"
#include "executor.h"

// 跑车行为
// Speed is fast.
// M: Forward 2 steps.
// L: Turn Left 90, then Forward 1 step.
// R: Turn Right 90, then Forward 1 step.
class SportsCarBehavior : public IVehicleBehavior {
public:
    void moveForward(Executor* executor) override {
        executor->moveForward();
        executor->moveForward();
    }

    void turnLeft(Executor* executor) override {
        executor->turnLeft();
        executor->moveForward();
    }

    void turnRight(Executor* executor) override {
        executor->turnRight();
        executor->moveForward();
    }

    void executeTurnRound(Executor* executor) override {
        // Default implementation based on Normal behavior
        executor->turnLeft();
        executor->moveForward();
        executor->turnLeft();
    }
};

// 公交车行为
// Bus body is long.
// M: Forward 1 step.
// L: Forward 1 step, then Turn Left 90.
// R: Forward 1 step, then Turn Right 90.
class BusBehavior : public IVehicleBehavior {
public:
    void moveForward(Executor* executor) override {
        executor->moveForward();
    }

    void turnLeft(Executor* executor) override {
        executor->moveForward();
        executor->turnLeft();
    }

    void turnRight(Executor* executor) override {
        executor->moveForward();
        executor->turnRight();
    }

    void executeTurnRound(Executor* executor) override {
        // Default implementation based on Normal behavior
        executor->turnLeft();
        executor->moveForward();
        executor->turnLeft();
    }
};

#endif // CONCRETE_BEHAVIORS_H
