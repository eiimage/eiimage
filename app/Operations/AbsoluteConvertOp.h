#ifndef ABSOLUTECONVERTOP_H
#define ABSOLUTECONVERTOP_H

#include <QObject>
#include "Operation.h"

class AbsoluteConvertOp : public DoubleOperation
{
public:
    AbsoluteConvertOp();

    bool needCurrentImg() const;

    void operator()(const imagein::Image_t<double>*, const std::map<const imagein::Image_t<double>*, std::string>&);
};


#endif // ABSOLUTECONVERTOP_H

