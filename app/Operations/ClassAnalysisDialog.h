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

#ifndef CLASSANALYSISDIALOG_H
#define CLASSANALYSISDIALOG_H

#include <QDialog>
#include "../Widgets/ImageZoneSelector.h"

namespace Ui {
class ClassAnalysisDialog;
}

class ClassAnalysisDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit ClassAnalysisDialog(QWidget *parent, const Image *img);
    ~ClassAnalysisDialog();
    bool isLearningStep() const;
    bool isClassificationStep() const;
    QString getFileName() const;
    int getWindowSize() const;
    std::vector<imagein::Rectangle> getSelections() const;

public slots:
    void on_fileEdit_textChanged(QString);

private slots:
    void checkData();
    void on_fileButton_clicked();
    void on_stepBox_currentIndexChanged(int);

private:
    Ui::ClassAnalysisDialog *ui;
   ImageZoneSelector* _imgZoneSelector;
   QLabel* _label;
};

#endif // CLASSANALYSISDIALOG_H
