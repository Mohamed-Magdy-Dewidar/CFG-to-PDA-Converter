#ifndef CFG_H
#define CFG_H

#include <QObject>
#include <QString>
#include <QSet>
#include <QMap>
#include <QList>


class CFG : public QObject
{
    Q_OBJECT
public:
    explicit CFG(QObject *parent = nullptr);

    void setVariables(const QSet<QString>& variables);
    void setTerminals(const QSet<QString>& terminals);
    void setStartSymbol(const QString& startSymbol);
    void addProduction(const QString& variable, const QStringList& productions);


    QSet<QString> getVariables() const;
    QSet<QString> getTerminals() const;
    QString getStartSymbol() const;
    QMap<QString, QStringList> getProductions() const;
    bool isValid() const;

private:
    QSet<QString> variables;
    QSet<QString> terminals;
    QString startSymbol;
    QMap<QString, QStringList> productions;


};



#endif // CFG_H




