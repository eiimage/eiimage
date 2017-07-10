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

#ifndef COMBINEHSVOP_H
#define COMBINEHSVOP_H

#include "Operation.h"

class CombineHSVOp : public DoubleOperation
{
public:
    CombineHSVOp();

    void operator()(const imagein::Image_t<double>*, const std::map<const imagein::Image_t<double>*, std::string>&);

    bool needCurrentImg() const;
};

#endif // COMBINEHSVOP_H
