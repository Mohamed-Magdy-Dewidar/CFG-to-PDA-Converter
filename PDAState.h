#ifndef PDASTATE_H
#define PDASTATE_H

#include <QObject>
#include <QString>
#include <QStack>



class PDAState : public QObject
{
    Q_OBJECT
public:
    explicit PDAState(QObject *parent = nullptr);
    PDAState(const QString& state, const QStack<QString>& stack);

    QString getState() const;
    QStack<QString> getStack() const;

    void setState(const QString& state);
    void setStack(const QStack<QString>& stack);

private:
    QString currentState;
    QStack<QString> stack;

signals:
};

#endif // PDASTATE_H
