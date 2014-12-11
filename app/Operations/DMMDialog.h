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

#ifndef DMMDIALOG_H
#define DMMDIALOG_H

#include <Algorithm/MorphoMat.h>
#include "Widgets/StructElemViewer.h"
#include <QGraphicsView>
#include <QDialog>
#include <QComboBox>
#include <QPushButton>
#include <vector>

class DMMDialog : public QDialog
{
    Q_OBJECT
public:
    enum Op { Erosion, Dilatation, Opening, Closing, Dmm};
    explicit DMMDialog(QWidget *parent = 0);
    virtual ~DMMDialog();
    inline std::vector<imagein::MorphoMat::StructElem::Dir> getBasicElements() { return _basicElements; }
    Op getSelectedOp();
    inline std::vector<unsigned int> getLevels() { return _levels; }

signals:
    
public slots:
    void dilateLeft();
    void dilateTopLeft();
    void dilateTop();
    void dilateTopRight();
    void dilateRight();
    void dilateBottomRight();
    void dilateBottom();
    void dilateBottomLeft();
    void newLevel();
    void opChanged(QString);
    void setCenter(bool center) { _center = center; }

protected:
    std::vector<StructElemViewer::ElemView> _views;
    std::vector<unsigned int> _levels;
    StructElemViewer* _structElemViewer;
    QGraphicsView* _structElemView;
    QComboBox* _operatorBox;
    void refreshStructElemView();
    QPushButton* _newLevelButton;
    std::vector<imagein::MorphoMat::StructElem::Dir> _basicElements;
    int _centerX, _centerY;
    bool _center;
};

#endif // DMMDIALOG_H
