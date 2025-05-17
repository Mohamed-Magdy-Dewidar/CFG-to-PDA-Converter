#include "PdaBuilder.h"
#include "PDA.h"
#include <QStack>
#include <QDebug>
#include <algorithm> // For std::reverse

PdaBuilder::PdaBuilder(QObject *parent)
    : QObject(parent)
{}

PDA* PdaBuilder::buildFromCFG(const CFG &cfg)
{
    PDA *pda = new PDA();

    // Set basic PDA components
    QSet<QString> states = {"q0", "q1", "q2"};
    pda->setStates(states);
    pda->setInputAlphabet(cfg.getTerminals());

    QSet<QString> stackAlphabet = cfg.getVariables();
    stackAlphabet.unite(cfg.getTerminals());
    stackAlphabet.insert("Z"); // Bottom of stack marker
    pda->setStackAlphabet(stackAlphabet);

    pda->setStartState("q0");
    pda->setStartStackSymbol("Z");
    pda->setAcceptStates({"q2"});

    // Build transitions
    QMap<QString, QMap<QPair<QString, QString>, QList<QPair<QString, QString>>>> transitions;

    // Transition from q0 to q1 (push start symbol)
    transitions["q0"][qMakePair(QString("ε"), QString("ε"))] =
        {qMakePair(QString("q1"), cfg.getStartSymbol() + "Z")};

    // Add production rules

    QMap<QString, QStringList> productions = cfg.getProductions();
    for (const QString& var : productions.keys()) {
        for (const QString& prod : productions[var]) {
            QString stackOp = (prod == "ε") ? "ε" : prod;
            if (stackOp != "ε") {
                std::reverse(stackOp.begin(), stackOp.end());
            }
            transitions["q1"][qMakePair(QString("ε"), var)] =
                {qMakePair(QString("q1"), stackOp)};
        }
    }

    // Add terminal matching rules
    for (const QString& term : cfg.getTerminals()) {
        transitions["q1"][qMakePair(term, term)] =
            {qMakePair(QString("q1"), QString("ε"))};
    }

    // Transition to accept state
    transitions["q1"][qMakePair(QString("ε"), QString("Z"))] =
        {qMakePair(QString("q2"), QString("ε"))};

    pda->setTransitions(transitions);

    if (cfg.getStartSymbol().isEmpty()) {
        qWarning() << "Empty start symbol in CFG";
        return pda;  // return empty PDA or handle error
    }

    return pda;


}
