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

#ifndef INVERSEPYRAMIDDIALOG_H
#define INVERSEPYRAMIDDIALOG_H

#include <QDialog>
#include "../Algorithms/Pyramid.h"

namespace Ui {
class InversePyramidDialog;
}

class InversePyramidDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit InversePyramidDialog(QWidget *parent = 0);
    ~InversePyramidDialog();
    Pyramid::filtre getFilter() const;
    int getNbStep() const;
    int getStep() const;

    
private:
    Ui::InversePyramidDialog *ui;
};

#endif // INVERSEPYRAMIDDIALOG_H
