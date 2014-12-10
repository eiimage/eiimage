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

#ifndef DMMOP_H
#define DMMOP_H

#include <Operation.h>
#include <Image.h>
#include <Algorithm/MorphoMat.h>


class DMMOp : public Operation
{
public:
    DMMOp();

    void operator()(const imagein::Image*, const std::map<const imagein::Image*, std::string>&);

    bool needCurrentImg() const;
protected:
    imagein::Image* dilatation(const imagein::Image*, std::vector<imagein::MorphoMat::StructElem::Dir>);
    imagein::Image* erosion(const imagein::Image*, std::vector<imagein::MorphoMat::StructElem::Dir>);
    imagein::Image* opening(const imagein::Image*, std::vector<imagein::MorphoMat::StructElem::Dir>);
    imagein::Image* closing(const imagein::Image*, std::vector<imagein::MorphoMat::StructElem::Dir>);
    void dmm(const imagein::Image*, std::vector<imagein::MorphoMat::StructElem::Dir>, std::vector<unsigned int>);
};

#endif // DMMOP_H
