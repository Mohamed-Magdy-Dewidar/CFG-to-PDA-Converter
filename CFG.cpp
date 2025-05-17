#include "CFG.h"

CFG::CFG(QObject *parent)
    : QObject{parent}
{

}

void CFG::setVariables(const QSet<QString> &variables)
{
    this->variables = variables;
}

void CFG::setTerminals(const QSet<QString> &terminals)
{
    this->terminals = terminals;
}

void CFG::setStartSymbol(const QString &startSymbol)
{
    this->startSymbol = startSymbol;
}

void CFG::addProduction(const QString &variable, const QStringList &productions)
{
    this->productions[variable] = productions;
}

QSet<QString> CFG::getVariables() const
{
    return variables;
}

QSet<QString> CFG::getTerminals() const
{
    return terminals;
}

QString CFG::getStartSymbol() const
{
    return startSymbol;
}

QMap<QString, QStringList> CFG::getProductions() const
{
    return productions;
}

bool CFG::isValid() const
{
    if (!variables.contains(startSymbol))
        return false;

    for (const QString& var : productions.keys()) {
        if (!variables.contains(var))
            return false;
    }

    return true;
}
