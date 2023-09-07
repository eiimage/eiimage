#ifndef DOUBLECONVERTOP_H
#define DOUBLECONVERTOP_H

#include <QObject>
#include "Operation.h"

class DoubleConvertOp : public Operation
{
public:
    DoubleConvertOp();

    bool needCurrentImg() const;

    void operator()(const imagein::Image*, const std::map<const imagein::Image*, std::string>&);
};

#endif // DOUBLECONVERTOP_H
