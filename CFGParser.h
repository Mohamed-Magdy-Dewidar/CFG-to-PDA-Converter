#ifndef CFGPARSER_H
#define CFGPARSER_H

#include "CFG.h"
#include <QObject>
#include <QString>
#include <QSet>
#include <QMap>

class CFGParser : public QObject
{
    Q_OBJECT
public:
    explicit CFGParser(QObject *parent = nullptr);

    // Returns a pointer to heap-allocated CFG (caller takes ownership)
    static CFG* parseFromString(const QString& input);
    static CFG* parseFromFile(const QString& filePath);

private:
    static QSet<QString> parseSymbols(const QString& input);
    static QMap<QString, QStringList> parseProductions(const QStringList& lines);
};

#endif // CFGPARSER_H
