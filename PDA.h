#ifndef PDA_H
#define PDA_H

#include <QObject>

#include <QString>
#include <QSet>
#include <QMap>
#include <QList>





class PDA : public QObject
{
    Q_OBJECT
public:
    explicit PDA(QObject *parent = nullptr);
    void setStates(const QSet<QString>& states);
    void setInputAlphabet(const QSet<QString>& alphabet);
    void setStackAlphabet(const QSet<QString>& alphabet);
    void setTransitions(const QMap<QString, QMap<QPair<QString, QString>, QList<QPair<QString, QString>>>>& transitions);
    void setStartState(const QString& state);
    void setStartStackSymbol(const QString& symbol);
    void setAcceptStates(const QSet<QString>& states);

    QSet<QString> getStates() const;
    QSet<QString> getInputAlphabet() const;
    QSet<QString> getStackAlphabet() const;
    QSet<QString> getVariables() const;
    QSet<QString> getTerminals() const;
    QMap<QString, QMap<QPair<QString, QString>, QList<QPair<QString, QString>>>> getTransitions() const;
    QString getStartState() const;
    QString getStartStackSymbol() const;
    QSet<QString> getAcceptStates() const;

private:
    QSet<QString> states;
    QSet<QString> inputAlphabet;
    QSet<QString> stackAlphabet;
    QMap<QString, QMap<QPair<QString, QString>, QList<QPair<QString, QString>>>> transitions;
    QString startState;
    QString startStackSymbol;
    QSet<QString> acceptStates;
};

#endif // PDA_H
