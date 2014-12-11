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

#ifndef FLIPOP_H
#define FLIPOP_H

#include <QApplication>

#include <Operation.h>
#include <Image.h>

class FlipOp : public Operation
{
public:
    enum Direction {Horizontal, Vertical};
    FlipOp(Direction);

    void operator()(const imagein::Image*, const std::map<const imagein::Image*, std::string>&);

    static QString tr(const char* str) { return QApplication::tr(str); }

    bool needCurrentImg() const;
private:
    Direction _dir;
};

#endif // FLIPOP_H
