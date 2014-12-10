/*
 * Copyright 2011-2012 INSA Rennes
 *
 * This file is part of INSAimage.
 *
 * INSAimage is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * INSAimage is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with INSAimage.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef HOUGHOP_H
#define HOUGHOP_H

#include <Operation.h>

class HoughOp : public Operation
{
public:
    HoughOp();

    void operator()(const imagein::Image*, const std::map<const imagein::Image*, std::string>&);

    bool needCurrentImg() const;
};

#endif // HOUGHOP_H
