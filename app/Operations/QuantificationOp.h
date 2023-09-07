/*
 * Copyright 2011-2012 INSA Rennes
 *
 * This file is part of ImageINSA.
 *
 * ImageINSA is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ImageINSA is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ImageINSA.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef QUANTIFICATIONOP_H
#define QUANTIFICATIONOP_H

#include <Operation.h>
#include "Quantification.h"
#include "QuantificationDialog.h"
class QuantificationOp : public GenericOperation
{
public:


    QuantificationOp();

    static std::string quantificationOpLog(unsigned int channel, Quantification * quant);

    void operator()(const genericinterface::ImageWindow* currentWnd, const std::vector<const genericinterface::ImageWindow*>& imgWndList) override;

    bool needCurrentImg() const override;

    void setTest(bool);

    void setQuantif(int);

    void setValues(int);

    bool isValidImgWnd(const genericinterface::ImageWindow* imgWnd) const override;

private:
    bool _test;
    int _quantif;
    int _values;
};

#endif // QUANTIFICATIONOP_H
