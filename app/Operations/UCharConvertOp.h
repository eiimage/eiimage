#ifndef UCHARCONVERTOP_H
#define UCHARCONVERTOP_H

#include <QObject>
#include "Operation.h"
#include "ucharconvertdialog.h"


class UCharConvertOp : public DoubleOperation
{
public:
    UCharConvertOp();

    bool needCurrentImg() const;

    void operator()(const imagein::Image_t<double>*, const std::map<const imagein::Image_t<double>*, std::string>&);
};

#endif // UCHARCONVERTOP_H
