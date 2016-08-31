#ifndef MEDIANOP_H
#define MEDIANOP_H

#include "Operation.h"

class MedianOp : public Operation
{
public:
    MedianOp();

    void operator()(const imagein::Image*, const std::map<const imagein::Image*, std::string>&);

    bool needCurrentImg() const;
};

#endif // MEDIANOP_H
