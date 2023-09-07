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

    void operator()(const genericinterface::ImageWindow* currentWnd, const std::vector<const genericinterface::ImageWindow*>&) override;

    bool needCurrentImg() const override;
    bool isValidImgWnd(const genericinterface::ImageWindow* imgWnd) const override;

  private:
    typedef imagein::Image::depth_t depth_t;

    inline static depth_t normalize(intmax_t value) {
        intmax_t max = std::numeric_limits<depth_t>::max();
        intmax_t min = std::numeric_limits<depth_t>::min();
        value = std::min(max, std::max(min, value));
        return static_cast<depth_t>(value);
    }

/*----------------------------------------------------------------------
                        Pixel Operation Templates
----------------------------------------------------------------------*/
    struct PixelOp {
        virtual depth_t operator()(depth_t pixel) {
            return normalize(this->op(pixel));
        }
        virtual intmax_t op(depth_t pixel) = 0;
        static PixelOp* fromString(const QString& op, const QString& expr);
    };
    struct DoublePixelOp {
        explicit DoublePixelOp(double value_ = 0) : value(value_) {}
        virtual double operator()(double pixel) {
            return this->op(pixel);
        }
        virtual double op(double pixel) = 0;
        static DoublePixelOp* fromString(const QString& op, const QString& expr);
        double value;
    };
/*----------------------------------------------------------------------
                       Pixel Identical Operation
----------------------------------------------------------------------*/
    struct PixIdent : PixelOp {
        intmax_t op(depth_t pixel) override { return pixel; }
    };

    struct DoublePixIdent : DoublePixelOp {
        double op(double pixel) override { return pixel; }
    };

    template<typename T>
    struct PixOp_t : public PixelOp {
        explicit PixOp_t(T value_) : value(value_) {}
        T value;
    };

/*----------------------------------------------------------------------
                      Pixel Arithmetic Operations
----------------------------------------------------------------------*/
    /*Subtraction and division are based on addition and multiplication, by processing the operand.*/
    struct PixAdd : PixOp_t<int> {
        explicit PixAdd(int value_) : PixOp_t<int>(value_) {}
        intmax_t op(depth_t pixel) override { return pixel + value; }
    };

    struct DoublePixAdd : DoublePixelOp {
        explicit DoublePixAdd(double value_) : DoublePixelOp(value_) {}
        double op(double pixel) override { return pixel + value; }
    };

    struct PixMul : PixOp_t<double> {
        explicit PixMul(double value_) : PixOp_t<double>(value_) {}
        intmax_t op(depth_t pixel) override { return (intmax_t)(lround(pixel * value)); }
    };

    struct DoublePixMul : DoublePixelOp {
        explicit DoublePixMul(double value_) : DoublePixelOp(value_) {}
        double op(double pixel) override { return pixel * value; }
    };

/*----------------------------------------------------------------------
                            Pixel    AND
----------------------------------------------------------------------*/
    struct PixBitwiseAnd : PixOp_t<depth_t> {
        explicit PixBitwiseAnd(depth_t value_) : PixOp_t<depth_t>(value_) {}
        intmax_t op(depth_t pixel) override { return pixel & value; }
    };

    /*Return zero only if any of operands is zero*/
    struct PixLogicalAnd : PixOp_t<depth_t> {
        explicit PixLogicalAnd(int value_) : PixOp_t<depth_t>(value_) {}
        intmax_t op(depth_t pixel) override { return (pixel==0 or value==0) ? 0 : pixel; }
    };
    struct DoublePixLogicalAnd : DoublePixelOp {
        explicit DoublePixLogicalAnd(int value_) : DoublePixelOp(value_) {}
        double op(double pixel) override { return (pixel==0 or value==0) ? 0 : pixel; }
    };
/*----------------------------------------------------------------------
                           Pixel    NOT
----------------------------------------------------------------------*/
    /*Logical flipping takes the complementary value to 255*/
    struct PixLogicalNot : PixOp_t<depth_t> {
        explicit PixLogicalNot(depth_t value_) : PixOp_t<depth_t>(value_) {}
        intmax_t op(depth_t pixel) override { return 255 - pixel; }
    };
    struct DoublePixLogicalNot : DoublePixelOp {
        explicit DoublePixLogicalNot(double value_) : DoublePixelOp(value_) {}
        double op(double pixel) override { return 255. - pixel; }
    };
/*----------------------------------------------------------------------
                             Pixel    OR
----------------------------------------------------------------------*/
    struct PixBitwiseOr : PixOp_t<depth_t> {
        explicit PixBitwiseOr(depth_t value_) : PixOp_t<depth_t>(value_) {}
        intmax_t op(depth_t pixel) override { return pixel | value; }
    };
    /*Return zero only if the operands are both zero*/
    struct PixLogicalOr : PixOp_t<depth_t> {
        explicit PixLogicalOr(depth_t value_) : PixOp_t<depth_t>(value_) {}
        intmax_t op(depth_t pixel) override { return (pixel==0 and value==0) ? 0 : pixel; }
    };
    struct DoublePixLogicalOr : DoublePixelOp {
        explicit DoublePixLogicalOr(int value_) : DoublePixelOp(value_) {}
        double op(double pixel) override { return (pixel==0 and value==0) ? 0 : pixel; }
    };
/*----------------------------------------------------------------------
                             Pixel    XOR
----------------------------------------------------------------------*/
    struct PixBitwiseXor : PixOp_t<depth_t> {
        explicit PixBitwiseXor(depth_t value_) : PixOp_t<depth_t>(value_) {}
        intmax_t op(depth_t pixel) override {return pixel ^ value; }
    };

    /*Return zero only if the operands have the same value*/
    struct PixLogicalXor : PixOp_t<depth_t> {
        explicit PixLogicalXor(depth_t value_) : PixOp_t<depth_t>(value_) {}
        intmax_t op(depth_t pixel) override { return (pixel==value) ? 0 : pixel; }
    };
    struct DoublePixLogicalXor : DoublePixelOp {
        explicit DoublePixLogicalXor(int value_) : DoublePixelOp(value_) {}
        double op(double pixel) override { return (pixel==value) ? 0 : pixel; }
    };
/*----------------------------------------------------------------------
                           Pixel    Shifting
----------------------------------------------------------------------*/
    struct PixLshift : PixOp_t<depth_t> {
        explicit PixLshift(depth_t value_) : PixOp_t<depth_t>(value_) {}
        intmax_t op(depth_t pixel) override { return pixel << value; }
    };
    struct PixRshift : PixOp_t<depth_t> {
        explicit PixRshift(depth_t value_) : PixOp_t<depth_t>(value_) {}
        intmax_t op(depth_t pixel) override { return pixel >> value; }
    };
    /*Used to handle the case where the input image is on uchar but the expected output is double
     * When the input image is already on double, the option of pixel shifting will not appear*/
    struct DoublePixLshift : DoublePixelOp {
        explicit DoublePixLshift(int value_) : DoublePixelOp(value_) {}
        double op(double pixel) override { return (int)pixel << (int)value; }
    };
    struct DoublePixRshift : DoublePixelOp {
        explicit DoublePixRshift(int value_) : DoublePixelOp(value_) {}
        double op(double pixel) override { return (int)pixel >> (int)value; }
    };
/*----------------------------------------------------------------------
                       Image Operation Templates
----------------------------------------------------------------------*/
    struct ImageOp {
        virtual depth_t operator()(depth_t pixel1, depth_t pixel2) {
            return normalize(this->op(pixel1, pixel2));
        }
        virtual intmax_t op(depth_t pixel1, depth_t pixel2) = 0;
        static ImageOp* fromString(const QString& op);
    };

    struct DoubleImageOp {
        virtual double operator()(double pixel1, double pixel2) {
            return this->op(pixel1, pixel2);
        }
        virtual double op(double pixel1, double pixel2) = 0;
        static DoubleImageOp* fromString(const QString& op);
    };
/*----------------------------------------------------------------------
                      Image Identical Operations
----------------------------------------------------------------------*/
    struct ImgIdent : ImageOp {
        intmax_t op(depth_t pix1, depth_t pix2) override { return pix1; }
    };
    struct DoubleImgIdent : DoubleImageOp {
        double op(double pix1, double pix2) override { return pix1; }
    };
/*----------------------------------------------------------------------
                       Image Arithmetic Operations
----------------------------------------------------------------------*/
    struct ImgAdd : ImageOp {
        intmax_t op(depth_t pix1, depth_t pix2) override { return pix1 + pix2; }
    };
    struct DoubleImgAdd : DoubleImageOp {
        double op(double pix1, double pix2) override { return pix1 + pix2; }
    };

    struct ImgSub : ImageOp {
        intmax_t op(depth_t pix1, depth_t pix2) override { return pix1 - pix2; }
    };
    struct DoubleImgSub : DoubleImageOp {
        double op(double pix1, double pix2) override { return pix1 - pix2; }
    };

    struct ImgMul : ImageOp {
        intmax_t op(depth_t pix1, depth_t pix2) override { return pix1 * pix2; }
    };
    struct DoubleImgMul : DoubleImageOp {
        double op(double pix1, double pix2) override { return pix1 * pix2; }
    };

    struct ImgDiv : ImageOp {
        /*Si division par 0, renvoie 255*/
        intmax_t op(depth_t pix1, depth_t pix2) override { if(pix2 != 0) {return pix1/pix2;} else {return 255;}}
    };

    struct DoubleImgDiv : DoubleImageOp {
        double op(double pix1, double pix2) override { return pix1 / pix2; }
    };
/*----------------------------------------------------------------------
                           Image    AND
----------------------------------------------------------------------*/
    struct ImgBitwiseAnd : ImageOp {
        intmax_t op(depth_t pix1, depth_t pix2) override { return pix1 & pix2; }
    };

    struct ImgLogicalAnd : ImageOp {
        intmax_t op(depth_t pix1, depth_t pix2) override { return pix1 && pix2; }
    };

    struct DoubleImgLogicalAnd : DoubleImageOp {
        double op(double pix1, double pix2) override { return pix1 && pix2; }
    };
/*----------------------------------------------------------------------
                           Image    OR
----------------------------------------------------------------------*/
    struct ImgBitwiseOr : ImageOp {
        intmax_t op(depth_t pix1, depth_t pix2) override { return pix1 | pix2; }
    };

    struct ImgLogicalOr : ImageOp {
        intmax_t op(depth_t pix1, depth_t pix2) override { return pix1 || pix2; }
    };

    struct DoubleImgLogicalOr : DoubleImageOp {
        double op(double pix1, double pix2) override { return pix1 || pix2; }
    };
/*----------------------------------------------------------------------
                            Image   XOR
----------------------------------------------------------------------*/
    struct ImgBitwiseXor : ImageOp {
        intmax_t op(depth_t pix1, depth_t pix2) override { return pix1 ^ pix2; }
    };

    struct ImgLogicalXor : ImageOp {
        intmax_t op(depth_t pix1, depth_t pix2) override { return (pix1&&(!pix2)) || ((!pix1)&&pix2); }
    };

    struct DoubleImgLogicalXor : DoubleImageOp {
        double op(double pix1, double pix2) override { return (pix1&&(!pix2)) || ((!pix1)&&pix2); }
    };
};

#endif //!PointOp_H
