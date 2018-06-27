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
#include <QButtonGroup>
#include <QDialog>
#include <QDialogButtonBox>
#include <QLabel>
#include <QSpinBox>
#include <QMdiArea>
#include <QPushButton>
#include <QFont>

#include <QTimer>
#include <QFile>
#include <QtXml/QDomDocument>
#include <QtXml/QDomImplementation>
#include <QtXml/QDomElement>
#include <QTextStream>
#include <QFileDialog>
#include <regex>

using namespace filtrme;
using namespace imagein::algorithm;
using namespace std;

/**
 * @brief FilterEditor::FilterEditor
 *
 * Constructor
 */
FilterEditor::FilterEditor()
{
  _nbFilters = 1;
  initUI();
}

/**
 * @brief FilterEditor::initUI
 */
void FilterEditor::initUI()
{ 
  _filterLayout = new QVBoxLayout();
  QWidget* widgetPrinc = new QWidget();
  widgetPrinc->setLayout(_filterLayout);
  
  QVBoxLayout* layout = new QVBoxLayout();
  QScrollArea* scroll = new QScrollArea();
  
  QWidget* widgetPath = new QWidget();
  QHBoxLayout* hLayoutPath = new QHBoxLayout(widgetPath);
  _buttonPath = new QPushButton("...");
  _buttonPath->setFixedWidth(50);
  QLabel* labelPath = new QLabel(tr("Filter file:"));
  QLabel* labelPath2 = new QLabel(tr("Select a filter file:"));
  _linePath = new QLineEdit(tr("No XML file selected!"));
  _linePath->setReadOnly(true);
  hLayoutPath->addWidget(labelPath);
  hLayoutPath->addWidget(_linePath);
  hLayoutPath->addWidget(labelPath2);
  hLayoutPath->addWidget(_buttonPath);
  _filterLayout->addWidget(widgetPath);

  QHBoxLayout* hLayout = new QHBoxLayout();
  QWidget* widget = new QWidget();
  widget->setLayout(hLayout);
  QSpinBox *spinBoxNbFilters;
  QLabel *label_3;
  QLabel *label_4;
  label_3 = new QLabel();
  label_3->setText(tr("Name of your filter:"));
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
  QLabel* labelInfo = new QLabel(tr("You need to select a file and a name for your filter to save it"));
  QFont* fontInfo = new QFont(labelInfo->font());
  fontInfo->setItalic(true);
  labelInfo->setFont(*fontInfo);

  _saveButton = buttonBox->button(QDialogButtonBox::Save);
  _saveButton->setEnabled(false);

  QHBoxLayout* hLayoutInfo = new QHBoxLayout();
  hLayoutInfo->addWidget(labelInfo);
  hLayoutInfo->addWidget(buttonBox);

  scroll->setWidget(widgetPrinc);
  scroll->setWidgetResizable(true);
  layout->addWidget(scroll);
  layout->addLayout(hLayoutInfo);
  this->setLayout(layout);
  this->setMinimumSize(670, 470);
  this->setWindowTitle(tr("FilterEditor"));

  QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(save()));
  QObject::connect(buttonBox, SIGNAL(rejected()), this, SLOT(cancel()));
  QObject::connect(spinBoxNbFilters, SIGNAL(valueChanged(int)), this, SLOT(nbFiltersChanged(int)));
  QObject::connect(_name, SIGNAL(textEdited(QString)), this, SLOT(nameChanged(QString)));
  QObject::connect(_buttonPath, SIGNAL(clicked()), this, SLOT(saveFile()));
}

/**
 * @brief FilterEditor::saveFile
 */
void FilterEditor::saveFile() {
    QString file = QFileDialog::getSaveFileName(this, tr("Chose a file filter"), QString(), tr("XML Files (*.xml)"));
    if(file.size()==0) return;
    _linePath->setText(file.toUtf8());
}

/**
 * @brief FilterEditor::nameChanged
 * @param name
 */
void FilterEditor::nameChanged(QString name) {
    _saveButton->setEnabled(!name.isEmpty());
}

/**
 * @brief FilterEditor::save
 */
void FilterEditor::save()
{
  vector<Filter*> filters;
  if(_name->text() == "")
  {
    QMessageBox msgBox(QMessageBox::Critical, tr("Error!"), tr("Your filter needs a name to be saved."));
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();
    return;
  }
  //condition regex sur le format du fichier .xml
  // if(...


  bool ok;
  filters = validFilters(&ok);
  if(ok) {
    saveXML(filters);
      this->accept();
  }
}

/**
 * @brief FilterEditor::saveXML
 * @param filtersToSave
 */
void FilterEditor::saveXML(vector<Filter*> filtersToSave)
{
  QFile file(_linePath->text());
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
                QMessageBox msgBox(QMessageBox::Warning, tr("Warning!"), tr("This filter name is already used."));
                msgBox.setInformativeText(tr("Do you want to overwrite it?"));
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
    for(int h = 0; h < height; h++)
    {
      for(int w = 0; w < width; w++)
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

/**
 * @brief FilterEditor::validFilters
 * @param ok
 * @return
 */
vector<Filter*> FilterEditor::validFilters(bool* ok)
{
  vector<Filter*> filters;
  
  *ok = true;
  
  for(int i = 2; i <= 1+_nbFilters; i++)
  {
    FilterEditorItem* item = (FilterEditorItem*)_filterLayout->itemAt(i)->widget();
    cout << "type d'item :" << _filterLayout->itemAt(i) << " \n";
    Filter* f = item->validFilter();
    cout << "out validFilter\n";
    cout << "val de f : " << f << "\n";
    if(f != NULL)
      filters.push_back(f);
    else
    {
      *ok = false;
      QMessageBox msgBox(QMessageBox::Critical, tr("Error!"), tr("Every square has to be filled with an integer."));
      msgBox.setInformativeText(tr("Filter %1 is not a valid filter.").arg(i-1));
      msgBox.setStandardButtons(QMessageBox::Ok);
      msgBox.setDefaultButton(QMessageBox::Ok);
      msgBox.exec();
      break;
    }
  }      
  
  return filters;
}

/**
 * @brief FilterEditor::cancel
 *
 * Manages popup window that comes when you press cancel.
 */
void FilterEditor::cancel()
{
  QMessageBox msgBox(QMessageBox::Warning, tr("Warning!"), tr("Unsaved changes will be lost."));
  msgBox.setInformativeText(tr("Do you want to continue?"));
  msgBox.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
  msgBox.setDefaultButton(QMessageBox::No);
  int ret = msgBox.exec();
  if(ret == QMessageBox::Yes)
      this->reject();
}


/**
 * @brief FilterEditor::nbFiltersChanged
 * @param nb
 */
void FilterEditor::nbFiltersChanged(const int nb)
{
  if(_nbFilters < nb)
  {
      while(_nbFilters!= nb){
         _nbFilters++;
        FilterEditorItem* item = new FilterEditorItem();
        _filterLayout->insertWidget(_nbFilters, item);
      }
  }
  else if(_nbFilters > nb)
  {
      while(_nbFilters != nb){
            _filterLayout->removeItem(_filterLayout->itemAt(_nbFilters));
            _nbFilters--;
      }
  }
  _nbFilters = nb;
}
