/*
 * Copyright 2011-2012 INSA Rennes
 *
 * This file is part of EIImage.
 *
 * EIImage is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * EIImage is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with EIImage.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef TOOLS_H
#define TOOLS_H

#include <QCoreApplication>

namespace Tools {

    inline QString tr(const char* str) { return QCoreApplication::tr(str); }

    inline QString colorName(int i, int n) {
        if((n==1 || n==2) && i==0) return tr("Black");
        if((n==2 && i==1) || (n==4 && i==3)) return tr("Alpha");
        switch(i) { case 0: return tr("Red"); case 1: return tr("Green"); case 2: return tr("Blue"); default: return tr("Color"); }
    }

}

#endif // TOOLS_H
