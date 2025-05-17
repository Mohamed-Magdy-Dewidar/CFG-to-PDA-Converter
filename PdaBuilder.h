#ifndef PDABUILDER_H
#define PDABUILDER_H

#include <QObject>
#include "CFG.h"
#include "PDA.h"

class PdaBuilder : public QObject
{
    Q_OBJECT
public:
    explicit PdaBuilder(QObject *parent = nullptr);

    static PDA* buildFromCFG(const CFG& cfg);

};

#endif // PDABUILDER_H
