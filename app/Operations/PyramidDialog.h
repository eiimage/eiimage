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

#ifndef PYRAMIDDIALOG_H
#define PYRAMIDDIALOG_H

#include <QDialog>
#include "../Algorithms/Pyramid.h"
#include <cstring>
#include <cstdlib>

namespace Ui {
class PyramidDialog;
}

class PyramidDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PyramidDialog(QWidget *parent = 0);
    ~PyramidDialog();
    Pyramid::Filtre getFilter(std::string &to_print) const;
    int getNbStep();
    bool isGaussian() const;
    bool onlyOneStep() const;
    int onlyStep() const;

    void setFilter(int);
    void setOneStep(int);
    void setOneStepChecked(bool);
    void updateMaxLevel(int);

private:
    Ui::PyramidDialog *ui;
};

#endif // PYRAMIDDIALOG_H
