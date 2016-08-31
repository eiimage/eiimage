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

#ifndef PointOp_H
#define PointOp_H

#include <vector>
#include <map>
#include <limits>
#include <QCoreApplication>

#include "Operation.h"
#include "Image.h"

class QWidget;

class PointOp : public GenericOperation {

  public:

    PointOp();

    virtual void operator()(const genericinterface::ImageWindow* currentWnd, const std::vector<const genericinterface::ImageWindow*>&);

    bool needCurrentImg() const;
    virtual bool isValidImgWnd(const genericinterface::ImageWindow* imgWnd) const;

  private:
    typedef imagein::Image::depth_t depth_t;

    inline static depth_t normalize(intmax_t value) {
        intmax_t max = std::numeric_limits<depth_t>::max();
        intmax_t min = std::numeric_limits<depth_t>::min();
        value = std::min(max, std::max(min, value));
        return static_cast<depth_t>(value);
    }


    struct PixelOp {
        virtual depth_t operator()(depth_t pixel) {
            return normalize(this->op(pixel));
        }
        virtual intmax_t op(depth_t pixel) = 0;
        static PixelOp* fromString(QString op, QString expr);
    };
    struct DoublePixelOp {
        DoublePixelOp(double value_ = 0) : value(value_) {}
        virtual double operator()(double pixel) {
            return this->op(pixel);
        }
        virtual double op(double pixel) = 0;
        static DoublePixelOp* fromString(QString op, QString expr);
        double value;
    };

    struct PixIdent : PixelOp {
        intmax_t op(depth_t pixel) { return pixel; }
    };

    struct DoublePixIdent : DoublePixelOp {
        double op(double pixel) { return pixel; }
    };

    template<typename T>
    struct PixOp_t : public PixelOp {
        PixOp_t(T value_) : value(value_) {}
        T value;
    };

    struct PixAdd : PixOp_t<int> {
        PixAdd(int value_) : PixOp_t<int>(value_) {}
        intmax_t op(depth_t pixel) { return pixel + value; }
    };
    struct DoublePixAdd : DoublePixelOp {
        DoublePixAdd(double value_) : DoublePixelOp(value_) {}
        double op(double pixel) { return pixel + value; }
    };

    struct PixMul : PixOp_t<double> {
        PixMul(double value_) : PixOp_t<double>(value_) {}
        intmax_t op(depth_t pixel) { return pixel * value + 0.5; }
    };
    struct DoublePixMul : DoublePixelOp {
        DoublePixMul(double value_) : DoublePixelOp(value_) {}
        double op(double pixel) { return pixel * value; }
    };

    struct PixAnd : PixOp_t<depth_t> {
        PixAnd(depth_t value_) : PixOp_t<depth_t>(value_) {}
        intmax_t op(depth_t pixel) { return pixel & value; }
    };

    struct PixOr : PixOp_t<depth_t> {
        PixOr(depth_t value_) : PixOp_t<depth_t>(value_) {}
        intmax_t op(depth_t pixel) { return pixel | value; }
    };

    struct PixXor : PixOp_t<depth_t> {
        PixXor(depth_t value_) : PixOp_t<depth_t>(value_) {}
        intmax_t op(depth_t pixel) { return pixel ^ value; }
    };

    struct PixLshift : PixOp_t<unsigned int> {
        PixLshift(unsigned int value_) : PixOp_t<unsigned int>(value_) {}
        intmax_t op(depth_t pixel) { return pixel << value; }
    };
    
    struct PixRshift : PixOp_t<unsigned int> {
        PixRshift(unsigned int value_) : PixOp_t<unsigned int>(value_) {}
        intmax_t op(depth_t pixel) { return pixel >> value; }
    };

    struct ImageOp {
        virtual depth_t operator()(depth_t pixel1, depth_t pixel2) {
            return normalize(this->op(pixel1, pixel2));
        }
        virtual intmax_t op(depth_t pixel1, depth_t pixel2) = 0;
        static ImageOp* fromString(QString op);
    };

    struct DoubleImageOp {
        virtual double operator()(double pixel1, double pixel2) {
            return this->op(pixel1, pixel2);
        }
        virtual double op(double pixel1, double pixel2) = 0;
        static DoubleImageOp* fromString(QString op);
    };

    struct ImgIdent : ImageOp {
        intmax_t op(depth_t pix1, depth_t pix2) { return pix1; } 
    };
    struct DoubleImgIdent : DoubleImageOp {
        double op(double pix1, double pix2) { return pix1; }
    };

    struct ImgAdd : ImageOp {
        intmax_t op(depth_t pix1, depth_t pix2) { return pix1 + pix2; } 
    };
    struct DoubleImgAdd : DoubleImageOp {
        double op(double pix1, double pix2) { return pix1 + pix2; }
    };

    struct ImgSub : ImageOp {
        intmax_t op(depth_t pix1, depth_t pix2) { return pix1 - pix2; }
    };
    struct DoubleImgSub : DoubleImageOp {
        double op(double pix1, double pix2) { return pix1 - pix2; }
    };

    struct ImgMul : ImageOp {
        intmax_t op(depth_t pix1, depth_t pix2) { return pix1 * pix2; }
    };
    struct DoubleImgMul : DoubleImageOp {
        double op(double pix1, double pix2) { return pix1 * pix2; }
    };

    struct ImgDiv : ImageOp {
        intmax_t op(depth_t pix1, depth_t pix2) { if(pix2 != 0) {return pix1/pix2*255;} else {return 255;}}
    };

    struct DoubleImgDiv : DoubleImageOp {
        double op(double pix1, double pix2) { return pix1 / pix2; }
    };

    struct ImgAnd : ImageOp {
        intmax_t op(depth_t pix1, depth_t pix2) { return pix1 & pix2; } 
    };
    
    struct ImgOr : ImageOp {
        intmax_t op(depth_t pix1, depth_t pix2) { return pix1 | pix2; } 
    };
    
    struct ImgXor : ImageOp {
        intmax_t op(depth_t pix1, depth_t pix2) { return pix1 ^ pix2; } 
    };



};

#endif //!PointOp_H
