#ifndef UCHARCONVERTOP_H
#define UCHARCONVERTOP_H

#include <QObject>
#include "Operation.h"
#include "UCharConvertDialog.h"
#include <QLabel>
#include <QSpinBox>
#include <QComboBox>


class UCharConvertOp : public DoubleOperation
{
public:
    UCharConvertOp();

    bool needCurrentImg() const;

    void operator()(const imagein::Image_t<double>*, const std::map<const imagein::Image_t<double>*, std::string>&);
private : 
    enum{CROP, NORMALIZE, OFFSET, OFFSETNSCALE, SCALE} conversionTYPE;
};

#endif // UCHARCONVERTOP_H
