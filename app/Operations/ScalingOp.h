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

#ifndef SCALINGOP_H
#define SCALINGOP_H

#include <Operation.h>

class ScalingOp : public GenericOperation
{
public:
    ScalingOp();
    enum Interpolation {NearestInterpolation, BilinearInterpolation, ParabolicInterpolation, SplineInterpolation};
    template<typename D>
    struct Nearest {
        static void interpolate(typename imagein::Image_t<D>::ConstLine src, typename imagein::Image_t<double>::Line dst);
    };
    template<typename D>
    struct Bilinear {
        static void interpolate(typename imagein::Image_t<D>::ConstLine src, typename imagein::Image_t<double>::Line dst);
    };
    template<typename D>
    struct Parabolic {
        static void interpolate(typename imagein::Image_t<D>::ConstLine src, typename imagein::Image_t<double>::Line dst);
    };
    template<typename D>
    struct Spline {
        static void interpolate(typename imagein::Image_t<D>::ConstLine src, typename imagein::Image_t<double>::Line dst);
    };

    virtual void operator()(const genericinterface::ImageWindow* currentWnd, const std::vector<const genericinterface::ImageWindow*>&);

    bool needCurrentImg() const;
    virtual bool isValidImgWnd(const genericinterface::ImageWindow* imgWnd) const;

//    template<typename D>
    template<typename D, template<typename> class I>
    static imagein::Image_t<D>* scale(const imagein::Image_t<D>* image, double xScale, double yScale);

//    template<typename D>
//    static void linearFill(imagein::Image_t<D>
};

#endif // SCALINGOP_H
