/*
 * Copyright 2011-2012 Benoit Averty, Samuel Babin, Matthieu Bergere, Thomas Letan, Sacha Percot-Tétu, Florian Teyssier
 * 
 * This file is part of DETIQ-T.
 * 
 * DETIQ-T is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * DETIQ-T is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with DETIQ-T.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "FilterEditor.h"

#include "FilterEditorItem.h"

#include <QScrollArea>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QDialogButtonBox>
#include <QtGui/QLabel>
#include <QtGui/QSpinBox>
#include <QtGui/QMdiArea>
#include <QtGui/QPushButton>

#include <QTimer>
#include <QFile>
#include <QtXml/QDomDocument>
#include <QtXml/QDomImplementation>
#include <QtXml/QDomElement>
#include <QTextStream>

using namespace filtrme;
using namespace imagein::algorithm;
using namespace std;

FilterEditor::FilterEditor()
{
  _nbFilters = 1;
  initUI();
}


void FilterEditor::initUI()
{ 
  _filterLayout = new QVBoxLayout();
  QWidget* widgetPrinc = new QWidget();
  widgetPrinc->setLayout(_filterLayout);
  
  QVBoxLayout* layout = new QVBoxLayout();
  QScrollArea* scroll = new QScrollArea();
  
  QHBoxLayout* hLayout = new QHBoxLayout();
  QWidget* widget = new QWidget();
  widget->setLayout(hLayout);
  QSpinBox *spinBoxNbFilters;
  QLabel *label_3;
  QLabel *label_4;
  label_3 = new QLabel();
  label_3->setText(tr("Name:"));
  _name = new QLineEdit();
  label_4 = new QLabel();
  label_4->setText(tr("Number of filters:"));
  spinBoxNbFilters = new QSpinBox();
  spinBoxNbFilters->setValue(_nbFilters);
  spinBoxNbFilters->setMinimum(1);
  hLayout->addWidget(label_3);
  hLayout->addWidget(_name);
  hLayout->addWidget(label_4);
  hLayout->addWidget(spinBoxNbFilters);
  _filterLayout->addWidget(widget);
  
  FilterEditorItem* item = new FilterEditorItem();
  _filterLayout->addWidget(item);
  
  QDialogButtonBox *buttonBox;
  buttonBox = new QDialogButtonBox();
  buttonBox->setOrientation(Qt::Horizontal);
  buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Save);
//  QPushButton* applyButton = buttonBox->addButton(QString::fromStdString("Apply"), QDialogButtonBox::ApplyRole);
  _filterLayout->addWidget(buttonBox);

  QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(save()));
  QObject::connect(buttonBox, SIGNAL(rejected()), this, SLOT(cancel()));
//  QObject::connect(applyButton, SIGNAL(clicked()), this, SLOT(apply()));
  QObject::connect(spinBoxNbFilters, SIGNAL(valueChanged(int)), this, SLOT(nbFiltersChanged(int)));
  _saveButton = buttonBox->button(QDialogButtonBox::Save);
  _saveButton->setEnabled(false);
  QObject::connect(_name, SIGNAL(textEdited(QString)), this, SLOT(nameChanged(QString)));

  scroll->setWidget(widgetPrinc);
  scroll->setWidgetResizable(true);
  layout->addWidget(scroll);
  this->setLayout(layout);
  this->setMinimumSize(670, 470);
  this->setWindowTitle(tr("FilterEditor"));
}

void FilterEditor::nameChanged(QString name) {
    _saveButton->setEnabled(!name.isEmpty());
}

void FilterEditor::save()
{
  vector<Filter*> filters;
  if(_name->text() == "")
  {
    QMessageBox msgBox(QMessageBox::Critical, tr("Error!"), tr("Your filter has to have a name to be saved."));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();
    return;
  }
  bool ok;
  filters = validFilters(&ok);
  if(ok) {
    saveXML(filters);
      this->accept();
  }
}

void FilterEditor::saveXML(vector<Filter*> filtersToSave)
{
  QFile file("filters.xml");
  // document
  QDomImplementation impl = QDomDocument().implementation();
  // document with document type
  QString name = "Filters";
  QString publicId = "-//Detiq-T//DTD Filters 1.0 //EN";
  QString systemId = "Detiq-T/DTD/filter.dtd";
  QDomDocument doc(impl.createDocumentType(name,publicId,systemId));
  QDomNode oldFilter;
  bool replace = false;
  
  if(!file.exists())
  {
    // add some XML comment at the beginning
    doc.appendChild(doc.createComment("This file contains filters"));
    doc.appendChild(doc.createTextNode("\n")); // for nicer output
    QDomElement e = doc.createElement("filtersList");
    doc.appendChild(e);
  }
  else
  {
    file.open(QIODevice::ReadOnly);
	  doc.setContent(&file);
	  file.close();
    
    QDomElement root = doc.documentElement();
    QDomNode child = root.firstChild();
	  while(!child.isNull())
	  {
      QDomElement e = child.toElement();
	    // We know how to treat appearance and geometry
	    if (e.attribute("name") == _name->text())
	    {
          QMessageBox msgBox(QMessageBox::Warning, tr("Warning!"), tr("This filter name is already use."));
        msgBox.setInformativeText(tr("Do you want to replace it?"));
        msgBox.setStandardButtons(QMessageBox::No|QMessageBox::Yes);
        msgBox.setDefaultButton(QMessageBox::No);
        int ret = msgBox.exec();
        if(ret == QMessageBox::Yes)
        {
          oldFilter = e;
          replace = true;
        }
        else
          return;
      }
      child = child.nextSibling();
	  }
  }

  // root node
  QDomElement filterNode = doc.createElement("filter");
  filterNode.setAttribute("name", _name->text());
  filterNode.setAttribute("nbFilters", QString::number(_nbFilters));

  // values
  for(int i = 0; i < _nbFilters; i++)
  {
    Filter* filterToSave = filtersToSave[i];
    int width = filterToSave->getWidth();
    int height = filterToSave->getHeight();
    
    QDomElement valuesNode = doc.createElement("values");
    valuesNode.setAttribute("width", QString::number(width));
    valuesNode.setAttribute("height", QString::number(height));

    QString s = "";
    for(int w = 0; w < width; w++)
    {
      for(int h = 0; h < height; h++)
      {
        s += QString::number(filterToSave->getPixelAt(w, h));
        if(w != width - 1 || h != height - 1)
          s += " ";
      }
    }
    valuesNode.appendChild(doc.createTextNode(s));
    filterNode.appendChild(valuesNode);
  }
  if(replace)
    doc.documentElement().replaceChild(filterNode, oldFilter);
  else
    doc.documentElement().appendChild(filterNode);
  
  if(file.open(QFile::WriteOnly))
  {
    QTextStream out(&file);
    out << doc.toString();
    file.close();
  }
}

vector<Filter*> FilterEditor::validFilters(bool* ok)
{
  vector<Filter*> filters;
  
  *ok = true;
  
  for(int i = 1; i <= _nbFilters; i++)
  {
    FilterEditorItem* item = (FilterEditorItem*)_filterLayout->itemAt(i)->widget();
    Filter* f = item->validFilter();
    if(f != NULL)
      filters.push_back(f);
    else
    {
      *ok = false;
      QMessageBox msgBox(QMessageBox::Critical, tr("Error!"), tr("Every square have to be completed by int value."));
      msgBox.setInformativeText(tr("Filter %1 isn't ok.").arg(i));
      msgBox.setStandardButtons(QMessageBox::Ok);
      msgBox.setDefaultButton(QMessageBox::Ok);
      msgBox.exec();
      break;
    }
  }      
  
  return filters;
}

void FilterEditor::cancel()
{
  QMessageBox msgBox(QMessageBox::Warning, tr("Warning!"), tr("Unsaved changes will be lost."));
  msgBox.setInformativeText(tr("Do you want to continue?"));
  msgBox.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
  msgBox.setDefaultButton(QMessageBox::No);
  int ret = msgBox.exec();
  if(ret == QMessageBox::Yes)
//    QTimer::singleShot(200, this, SLOT(quit()));
      this->reject();
}

void FilterEditor::nbFiltersChanged(const int nb)
{
  if(_nbFilters < nb)
  {
    FilterEditorItem* item = new FilterEditorItem();
    _filterLayout->insertWidget(_nbFilters + 1, item);
  }
  else if(_nbFilters > nb)
  {
    _filterLayout->removeItem(_filterLayout->itemAt(_nbFilters));
  }
  _nbFilters = nb;
}
