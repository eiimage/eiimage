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

#ifndef MEANSQUAREDERROROP_H
#define MEANSQUAREDERROROP_H

#include "Operation.h"

class MeanSquaredErrorOp : public GenericOperation
{
public:
    MeanSquaredErrorOp();
    void operator()(const genericinterface::ImageWindow* currentWnd, const std::vector<const genericinterface::ImageWindow*>& imgWndList);

    bool needCurrentImg() const override;
    bool isValidImgWnd(const genericinterface::ImageWindow* imgWnd) const override;
};

#endif // MEANSQUAREDERROROP_H
