#include "PDA.h"

PDA::PDA(QObject *parent)
    : QObject{parent}
{

}


void PDA::setStates(const QSet<QString> &states)
{
    this->states = states;
}

void PDA::setInputAlphabet(const QSet<QString> &alphabet)
{
    this->inputAlphabet = alphabet;
}

void PDA::setStackAlphabet(const QSet<QString> &alphabet)
{
    this->stackAlphabet = alphabet;
}

void PDA::setTransitions(const QMap<QString, QMap<QPair<QString, QString>, QList<QPair<QString, QString>>>>& transitions)
{
    this->transitions = transitions;
}

void PDA::setStartState(const QString &state)
{
    this->startState = state;
}

void PDA::setStartStackSymbol(const QString &symbol)
{
    this->startStackSymbol = symbol;
}

void PDA::setAcceptStates(const QSet<QString> &states)
{
    this->acceptStates = states;
}

QSet<QString> PDA::getStates() const
{
    return states;
}

QSet<QString> PDA::getInputAlphabet() const
{
    return inputAlphabet;
}

QSet<QString> PDA::getStackAlphabet() const
{
    return stackAlphabet;
}

QMap<QString, QMap<QPair<QString, QString>, QList<QPair<QString, QString>>>> PDA::getTransitions() const
{
    return transitions;
}

QString PDA::getStartState() const
{
    return startState;
}

QString PDA::getStartStackSymbol() const
{
    return startStackSymbol;
}

QSet<QString> PDA::getAcceptStates() const
{
    return acceptStates;
}

QSet<QString> PDA::getVariables() const
{
    QSet<QString> vars;
    for (const QString& sym : stackAlphabet) {
        if (sym.length() == 1 && sym[0].isUpper()) {  // Simple heuristic
            vars.insert(sym);
        }
    }
    return vars;
}

QSet<QString> PDA::getTerminals() const
{
    return inputAlphabet;  // Terminals are the input alphabet in standard conversion
}


