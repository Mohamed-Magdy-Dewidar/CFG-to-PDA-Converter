#include "CFGParser.h"
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>
#include <QDebug>

CFGParser::CFGParser(QObject *parent) : QObject(parent)
{
}

CFG* CFGParser::parseFromString(const QString &input)
{
    CFG* cfg = new CFG(); // Caller must manage this memory

    QStringList lines = input.split('\n', Qt::SkipEmptyParts);
    if (lines.isEmpty()) {
        qWarning() << "Empty input string provided";
        return cfg;
    }

    // Parse variables (first line)
    cfg->setVariables(parseSymbols(lines.value(0)));

    // Parse terminals (second line)
    if (lines.size() > 1) {
        cfg->setTerminals(parseSymbols(lines.value(1)));
    }

    // Parse start symbol (third line)
    if (lines.size() > 2) {
        cfg->setStartSymbol(lines.value(2).trimmed());
    }

    // Parse productions (remaining lines)
    if (lines.size() > 3) {
        QMap<QString, QStringList> productions = parseProductions(lines.mid(3));
        for (auto it = productions.constBegin(); it != productions.constEnd(); ++it) {
            cfg->addProduction(it.key(), it.value());
        }
    }

    return cfg;
}

CFG* CFGParser::parseFromFile(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open file:" << filePath;
        return new CFG(); // Return empty CFG
    }

    QTextStream in(&file);
    QString content = in.readAll();
    file.close();

    return parseFromString(content);
}

QSet<QString> CFGParser::parseSymbols(const QString &input)
{
    QSet<QString> symbols;
    QStringList symbolList = input.split(' ', Qt::SkipEmptyParts);
    for (const QString &symbol : symbolList) {
        symbols.insert(symbol.trimmed());
    }
    return symbols;
}

QMap<QString, QStringList> CFGParser::parseProductions(const QStringList &lines)
{
    QMap<QString, QStringList> productions;
    QRegularExpression re(R"((\w+)\s*->\s*(.+))"); // Matches "A -> aB | b"

    for (const QString &line : lines) {
        QRegularExpressionMatch match = re.match(line);
        if (match.hasMatch()) {
            QString nonTerminal = match.captured(1).trimmed();
            QStringList rightSides = match.captured(2).split('|', Qt::SkipEmptyParts);

            // Trim each right side
            for (QString &rightSide : rightSides) {
                rightSide = rightSide.trimmed();
            }

            productions[nonTerminal].append(rightSides);
        } else {
            qWarning() << "Malformed production line:" << line;
        }
    }

    return productions;
}
