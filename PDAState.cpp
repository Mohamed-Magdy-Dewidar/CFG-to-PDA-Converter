#include "PDAState.h"

PDAState::PDAState(QObject *parent)
    : QObject{parent}
{}




PDAState::PDAState(const QString &state, const QStack<QString> &stack) :
    currentState(state),
    stack(stack)
{
}

QString PDAState::getState() const
{
    return currentState;
}

QStack<QString> PDAState::getStack() const
{
    return stack;
}

void PDAState::setState(const QString &state)
{
    currentState = state;
}

void PDAState::setStack(const QStack<QString> &stack)
{
    this->stack = stack;
}
