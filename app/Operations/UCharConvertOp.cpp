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


#include "UCharConvertOp.h"
#include "../Tools.h"
#include <QApplication>
#include <QObject>
#include "Operation.h"
#include "ucharconvertdialog.h"


UCharConvertOp::UCharConvertOp() : DoubleOperation(qApp->translate("Operations", "UChar converter").toStdString())
{
}

bool UCharConvertOp::needCurrentImg() const{
    return true;
}

void UCharConvertOp::operator()(const imagein::Image_t<double>*, const std::map<const imagein::Image_t<double>*, std::string>&){
    UCharConvertDialog* dialog = new UCharConvertDialog(QApplication::activeWindow());
    QDialog::DialogCode code = static_cast<QDialog::DialogCode>(dialog->exec());

    if(code!=QDialog::Accepted) return;
}
