#include "CommandBuilder.hpp"

CommandBuilder::CommandBuilder()
    : mCommand(0)
{
}

quint8 CommandBuilder::getCommand() const
{
    return mCommand;
}

void CommandBuilder::reset()
{
    mCommand = 0;
}

void CommandBuilder::appendFlag(int key)
{
    switch( key )
    {
    case Qt::Key_W:
        mCommand |= RoboCommand::CAM_UP;
        break;
    case Qt::Key_S:
        mCommand |= RoboCommand::CAM_DOWN;
        break;
    case Qt::Key_A:
        mCommand |= RoboCommand::CAM_LEFT;
        break;
    case Qt::Key_D:
        mCommand |= RoboCommand::CAM_RIGTH;
        break;
    case Qt::Key_Left:
        mCommand |= RoboCommand::WHEEL_LEFT;
        break;
    case Qt::Key_Right:
        mCommand |= RoboCommand::WHEEL_RIGHT;
        break;
    case Qt::Key_Up:
        mCommand = mCommand | RoboCommand::WHEEL_LEFT | RoboCommand::WHEEL_RIGHT;
        break;
    default:
        break;
    }
    qDebug() << mCommand;
}

void CommandBuilder::removeFlag(int key)
{
    switch( key )
    {
    case Qt::Key_W:
        mCommand ^= RoboCommand::CAM_UP;
        break;
    case Qt::Key_S:
        mCommand ^= RoboCommand::CAM_DOWN;
        break;
    case Qt::Key_A:
        mCommand ^= RoboCommand::CAM_LEFT;
        break;
    case Qt::Key_D:
        mCommand ^= RoboCommand::CAM_RIGTH;
        break;
    case Qt::Key_Left:
        mCommand ^= RoboCommand::WHEEL_LEFT;
        break;
    case Qt::Key_Right:
        mCommand ^= RoboCommand::WHEEL_RIGHT;
        break;
    case Qt::Key_Up:
        mCommand = mCommand ^ RoboCommand::WHEEL_LEFT ^ RoboCommand::WHEEL_RIGHT;
        break;
    default:
        break;
    }
    qDebug() << mCommand;
}
