#pragma once

#include <Geometry2d/Point.hpp>
#include <boost/optional.hpp>
#include "planning/MotionInstant.hpp"
#include "Utils.hpp"

namespace Planning {

/*
 * This is a superclass for different MotionCommands.
 * Currently implemented are PathTarget, WorldVel, Pivot, DirectPathtarget, None
 */
class MotionCommand {
public:
    enum CommandType {
        PathTarget,
        WorldVel,
        Pivot,
        DirectPathTarget,
        LineKick,
        None
    };
    virtual ~MotionCommand() = default;
    CommandType getCommandType() const { return commandType; }
    virtual std::unique_ptr<Planning::MotionCommand> clone() const = 0;

protected:
    MotionCommand(const MotionCommand& that) = default;
    MotionCommand(CommandType command) : commandType(command) {}

private:
    // The type of command
    const CommandType commandType;
};

struct EmptyCommand : public MotionCommand {
    EmptyCommand() : MotionCommand(MotionCommand::None){};

    virtual std::unique_ptr<Planning::MotionCommand> clone() const override {
        return std::make_unique<EmptyCommand>();
    }
};

struct PathTargetCommand : public MotionCommand {
    virtual std::unique_ptr<Planning::MotionCommand> clone() const override {
        return std::make_unique<PathTargetCommand>(*this);
    }
    explicit PathTargetCommand(const MotionInstant& goal)
        : MotionCommand(MotionCommand::PathTarget), pathGoal(goal){};
    const MotionInstant pathGoal;
};

struct WorldVelTargetCommand : public MotionCommand {
    explicit WorldVelTargetCommand(Geometry2d::Point vel)
        : MotionCommand(MotionCommand::WorldVel), worldVel(vel){};
    virtual std::unique_ptr<Planning::MotionCommand> clone() const override {
        return std::make_unique<WorldVelTargetCommand>(*this);
    }
    const Geometry2d::Point worldVel;
};
struct PivotCommand : public MotionCommand {
    explicit PivotCommand(Geometry2d::Point pivotPoint,
                          Geometry2d::Point target, float radius)
        : MotionCommand(MotionCommand::Pivot),
          pivotPoint(pivotPoint),
          pivotTarget(target),
          radius(radius) {}

    virtual std::unique_ptr<Planning::MotionCommand> clone() const override {
        return std::make_unique<PivotCommand>(*this);
    }

    Geometry2d::Point pivotTarget;
    Geometry2d::Point pivotPoint;
    float radius;
};

struct DirectPathTargetCommand : public MotionCommand {
    virtual std::unique_ptr<Planning::MotionCommand> clone() const override {
        return std::make_unique<DirectPathTargetCommand>(*this);
    }
    explicit DirectPathTargetCommand(const MotionInstant& goal)
        : MotionCommand(MotionCommand::DirectPathTarget), pathGoal(goal){};
    const MotionInstant pathGoal;
};

struct LineKickCommand : public MotionCommand {
    explicit LineKickCommand(Geometry2d::Point target)
        : MotionCommand(MotionCommand::LineKick), target(target){};

    virtual std::unique_ptr<Planning::MotionCommand> clone() const override {
        return std::make_unique<LineKickCommand>(*this);
    };

    const Geometry2d::Point target;
};
}  // namespace Planning
