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

#include "FilterChoice.h"

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QStringList>
#include <QLabel>
#include <QLineEdit>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QtCore/QVariant>
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QComboBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QHeaderView>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include <QTableView>
#include <QLineEdit>
#include <QCheckBox>

#include <QFile>
#include <QDomDocument>
#include <QDomImplementation>
#include <QDomElement>
#include <QTextStream>

#include <QGroupBox>
#include <QRadioButton>
#include <QHBoxLayout>

#include <QFormLayout>
#include <QSpacerItem>
#include <GenericInterface.h>

#include <algorithm>

using namespace filtrme;
using namespace genericinterface;
using namespace imagein;
using namespace algorithm;
using namespace std;
/*ajouter option de calcule/affichage */
FilterChoice::FilterChoice(QWidget* parent) : QDialog(parent)
{
  initUI();
}

/**
 * @brief
 *
 */
void FilterChoice::initUI()
{
    _normvalue=9;

    this->setWindowTitle(tr("FilterChoice"));
    QLayout* layout = new QVBoxLayout(this);
    QWidget* mainWidget = new QWidget();
    layout->addWidget(mainWidget);
    QHBoxLayout* mainLayout = new QHBoxLayout(mainWidget);
    QWidget* leftWidget = new QWidget();
    QLayout* leftLayout = new QVBoxLayout(leftWidget);


    QGroupBox* stdOrCustomBox = new QGroupBox(tr("Select custom or standard filter"));
    _stdButton = new QRadioButton(tr("Standard filter"));
    _customButton = new QRadioButton(tr("Custom filter"));
    QHBoxLayout* hbox = new QHBoxLayout(stdOrCustomBox);

    _stdButton->setChecked(true);

    hbox->addWidget(_stdButton);
    hbox->addWidget(_customButton);

    QGroupBox* confBox = new QGroupBox(tr("Filter configuration"));
    QFormLayout* confLayout = new QFormLayout(confBox);

    /* CUSTOM FILTER */

    _labelCustom = new QLabel(this);
    _labelCustom->setText(tr("Path to custom filter:"));
    _filterPath = new QLineEdit("/");
    _filterPath->setReadOnly(true);

    _filterPathSelect = new QPushButton("...");
    _filterPathSelect->setFixedWidth(40);
    _pathLayout = new QHBoxLayout();
    _pathLayout->addWidget(_filterPath);
    _pathLayout->addWidget(_filterPathSelect);
    _labelCustom->setVisible(false);
    _filterPath->setVisible(false);
    _filterPathSelect->setVisible(false);
    confLayout->addRow(_labelCustom, _pathLayout);

    /* FILTER CHOICE */

    _label = new QLabel(this);
    _label->setText(tr("Filter:"));
    if(_a){
        _blurChoices = new QComboBox(this);
        QStringList blurs = initFilters();
        _blurChoices->addItems(blurs);
        _a=false;
    }
    QObject::connect(_customButton, SIGNAL(toggled(bool)), this, SLOT(updateBlur(bool)));
    QObject::connect(_blurChoices, SIGNAL(currentIndexChanged(int)), this, SLOT(currentBlurChanged(int)));
    confLayout->addRow(_label, _blurChoices);

    /* POLICIES CHOICE */

    QLabel* label_2 = new QLabel(this);
    label_2->setText(tr("Edge policy: "));
    _policyChoices = new QComboBox(this);
    QStringList policies = QStringList() << tr("Black") << tr("Mirror") << tr("Nearest") << tr("Spherical");
    _policyChoices->addItems(policies);
    _policyChoices->setCurrentIndex(2);
    confLayout->addRow(label_2, _policyChoices);

    _labelNumber = new QLabel(this);
    _labelNumber->setText(tr("Filter size:"));
    _number = new QSpinBox(this);
    _number->setValue(3);
    _number->setMinimum(1);
    confLayout->addRow(_labelNumber, _number);

    _stdDevLabel = new QLabel(tr("Standard deviation : "));
    _stdDevBox = new QDoubleSpinBox(this);
    _stdDevBox->setValue(1.);
    _stdDevBox->setRange(0., 256.);
    _stdDevBox->setSingleStep(0.1);
    confLayout->addRow(_stdDevLabel, _stdDevBox);
    _stdDevLabel->setVisible(false);
    _stdDevBox->setVisible(false);

    _checkbox_2 = new QCheckBox(tr("Normalisation des coefficients"));
    _spinbox = new QDoubleSpinBox();
    _spinbox->setMaximum(2147483647);
    _label_3 = new QLabel(tr("Facteur de normalisation :"));
    confLayout->addRow(_checkbox_2);
    confLayout->addRow(_label_3, _spinbox);


    QGroupBox* radioBox = new QGroupBox(tr("Resulting image type"));
    _stdResButton = new QRadioButton(tr("UChar"));
    _dblResButton = new QRadioButton(tr("Double"));
    radioBox->setLayout(new QHBoxLayout());
    radioBox->layout()->addWidget(_stdResButton);
    radioBox->layout()->addWidget(_dblResButton);

    QGroupBox* checkbox = new QGroupBox(tr("Options"));
    QHBoxLayout* optLayout = new QHBoxLayout();
    _offsetButton = new QCheckBox(tr("Offset"));
    _scalingButton = new QCheckBox(tr("Scaling"));
    _offsetButton->setChecked(true);
    _scalingButton->setChecked(true);
    _offsetButton->setAutoExclusive(false);
    _scalingButton->setAutoExclusive(false);
    optLayout->addWidget(_offsetButton);
    optLayout->addWidget(_scalingButton);
    checkbox->setLayout(optLayout);

    leftLayout->addWidget(stdOrCustomBox);
    leftLayout->addWidget(confBox);
    leftLayout->addWidget(radioBox);
    leftLayout->addWidget(checkbox);


    mainLayout->addWidget(leftWidget);

    QObject::connect(_checkbox_2, SIGNAL(toggled(bool)), this, SLOT(showNormalisationOpt(bool)));
    QObject::connect(_customButton, SIGNAL(toggled(bool)), this, SLOT(showCustom(bool)));
    QObject::connect(_number, SIGNAL(valueChanged(const QString&)), this, SLOT(dataChanged(const QString&)));
    QObject::connect(_stdDevBox, SIGNAL(valueChanged(const QString&)), this, SLOT(dataChanged(const QString&)));
    QObject::connect(_filterPathSelect, SIGNAL(clicked()), this, SLOT(openFile()));
    QObject::connect(_stdResButton, SIGNAL(toggled(bool)), this, SLOT(updateOptions(bool)));
    QObject::connect(_spinbox, SIGNAL(valueChanged(double)), this, SLOT(displayNormalisation(double)));
    QObject::connect(_stdDevBox, SIGNAL(valueChanged(double)), this, SLOT(displayNormalisation(double)));
    QObject::connect(_number, SIGNAL(valueChanged(double)), this, SLOT(displayNormalisation(double)));

    QWidget* rightWidget = new QWidget();
    QVBoxLayout* rightLayout = new QVBoxLayout(rightWidget);

    /* INIT FILTER */
    _filterView = new QTableWidget(3, 3, this);
    _filterView->verticalHeader()->hide();
    _filterView->horizontalHeader()->hide();
    _filterView->setDragEnabled(false);
    _filterView->setCornerButtonEnabled(false);

    int numPixels = _number->value();
    for(int i = 0; i < numPixels; i++)
    {
      _filterView->setColumnWidth(i, _filterView->rowHeight(0));
      for(int j = 0; j < numPixels; j++)
      {
        QTableWidgetItem* item = new QTableWidgetItem("1");
        item->setTextAlignment(Qt::AlignHCenter);
        item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        _filterView->setItem(j, i, item);
      }
    }

    rightLayout->addWidget(_filterView);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(this);
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel);
    QPushButton* applyButton = buttonBox->addButton(tr("Apply filter"), QDialogButtonBox::ApplyRole);
    _deleteButton = buttonBox->addButton(tr("Delete filter"), QDialogButtonBox::ActionRole);
    _deleteButton->setEnabled(false);

    QObject::connect(applyButton, SIGNAL(clicked()), this, SLOT(validate()));
    QObject::connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    QObject::connect(_deleteButton, SIGNAL(clicked()), this, SLOT(deleteFilter()));

    mainLayout->addWidget(rightWidget);
    layout->addWidget(buttonBox);

    _checkbox_2->setChecked(true);
    updateOptions(_stdResButton->isChecked());

}

/**
 * @brief
 *
 * Put the filters in the selection filter spinner, and updates the _filters
 */
QStringList FilterChoice::initFilters() {
  QStringList blurs = QStringList();

  if(!_customButton->isChecked()){
      blurs << tr("Uniform") << tr("Gaussian") << tr("Prewitt") << tr("Roberts") << tr("Sobel") << tr("SquareLaplacien");
      _filters.push_back(Filter::uniform(3));
      _filters.push_back(Filter::gaussian(3, 1.));
      _filters.push_back(Filter::prewitt(3));
      _filters.push_back(Filter::roberts());
      _filters.push_back(Filter::sobel());
      _filters.push_back(Filter::squareLaplacien());
  }
  else{
      //Personal filters
      QFile file(_path.toUtf8());
      if(file.exists())
      {
        QDomDocument doc("");
        file.open(QIODevice::ReadOnly);
          doc.setContent(&file);
          file.close();

        QDomElement root = doc.documentElement();
        QDomNode child = root.firstChild();
          while(!child.isNull())
          {
          QDomElement e = child.toElement();
            // We know how to treat appearance and geometry
          blurs.push_back(e.attribute("name"));

          int nbFilters = e.attribute("nbFilters").toInt();

          std::vector<Filter*> temp;
          QDomNode grandChild = e.firstChild();
          for(int i = 0; i < nbFilters && !grandChild.isNull(); i++)
          {
            QDomElement grandChildElement = grandChild.toElement();
            if(!grandChildElement.isNull())
            {
              Filter* f = new Filter(grandChildElement.attribute("width").toInt(), grandChildElement.attribute("height").toInt());
              // We know how to treat color
              if (grandChildElement.tagName() == "values")
              {
                std::string str = grandChildElement.text().toStdString();
                std::string word;
                std::stringstream stream(str);
                unsigned int w = 0, h = 0;
                while(getline(stream, word, ' '))
                {
                    f->setPixelAt(w, h, QString::fromStdString(word).toDouble());

                  if(w == f->getWidth() - 1)
                  {
                    w = 0;
                    h++;
                  }
                  else
                    w++;
                }
              }
              temp.push_back(f);
            }
            grandChild = grandChild.nextSibling();
          }
          _filters.push_back(temp);

          child = child.nextSibling();
          }
      }
  }
  return blurs;
}
/**
 * @brief FilterChoice::updateBlur
 * @param z
 */
void FilterChoice::updateBlur(bool){
    _filters.clear();
    if(_blurChoices->count()!=0){
        _blurChoices->clear();
    }
    QStringList blurs = initFilters();
    _blurChoices->addItems(blurs);
}

/**
 * @brief FilterChoice::showCustom()
 *
 * @param a selects whether or not the custom filters are enabled
 */
void FilterChoice::showCustom(bool a){
    if(a){
        _labelCustom->setVisible(true);
        _filterPath->setVisible(true);
        _filterPathSelect->setVisible(true);
    }

    else{
        _labelCustom->setVisible(false);
        _filterPath->setVisible(false);
        _filterPathSelect->setVisible(false);
    }
    _a=false;
    if(a){
        if(_filterPath->text().toStdString().compare("/")==0){
            _checkbox_2->setChecked(false);
            _checkbox_2->setEnabled(false);
            _label_3->setEnabled(false);
            _spinbox->setEnabled(false);
            _spinbox->setValue(1);
        }
        else{
            _checkbox_2->setChecked(false);
            _checkbox_2->setEnabled(true);
        }
    }
}

/**
 * @brief
 *
 *
 * @param int
 */
void FilterChoice::currentBlurChanged(int a)
{
    _checkbox_2->setEnabled(true);
    updateDisplay();
    if(_spinbox->isEnabled())
        updateNormValue();

}

/**
 * @brief
 *
 * @param
 */
void FilterChoice::dataChanged(const QString&)
{
    if(_checkbox_2->isChecked())
        updateNormValue();
    updateDisplay();
}

/**
 * @brief
 *
 * Validate the application of the filter.
 * If no filter selected, returns a critical window
 *
 */
void FilterChoice::validate()
{
  int num = _number->value();

  if(_blurChoices->currentIndex()==-1){
      QMessageBox msgBox(QMessageBox::Critical, tr("Error!"), tr("Filter application is impossible."));
      msgBox.setInformativeText(tr("Filter selection is empty, please select a filter."));
      msgBox.setStandardButtons(QMessageBox::Ok);
      msgBox.setDefaultButton(QMessageBox::Ok);
      msgBox.exec();
  }else{
      if(!_customButton->isChecked()){
          switch(_blurChoices->currentIndex())
          {
            case 0:
              _filtering = new Filtering(Filtering::uniformBlur(num));
              break;
            case 1:
              _filtering = new Filtering(Filtering::gaussianBlur(num, _stdDevBox->value()));
              break;
            case 2:
              _filtering = new Filtering(Filtering::prewitt(num));
              break;
            default:
              _filtering = new Filtering(_filters[_blurChoices->currentIndex()]);
          }
      }
      else
          _filtering = new Filtering(_filters[_blurChoices->currentIndex()]);

      switch(_policyChoices->currentIndex())
      {
        case 1:
          _filtering->setPolicy(Filtering::POLICY_MIRROR);
          break;
        case 2:
          _filtering->setPolicy(Filtering::POLICY_NEAREST);
          break;
        case 3:
          _filtering->setPolicy(Filtering::POLICY_TOR);
          break;
        default:
          _filtering->setPolicy(Filtering::POLICY_BLACK);
      }
      _filtering->setNormalisation(_spinbox->value());
      this->accept();
  }
}

/**
 * @brief
 *
 * ???
 *
 */
void FilterChoice::cancel()
{
//  emit(cancelAction());
}

/**
 * @brief
 *
 *  Deletes a filter in the selected XML file
 */
void FilterChoice::deleteFilter()
{
  QMessageBox msgBox(QMessageBox::Warning, tr("Warning!"), tr("This filter will be permanently deleted ?"));
  msgBox.setInformativeText(tr("Do you want to continue?"));
  msgBox.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
  msgBox.setDefaultButton(QMessageBox::No);

  if(msgBox.exec() == QMessageBox::Yes)
  {
    QString name = _blurChoices->itemText(_blurChoices->currentIndex());
    _blurChoices->removeItem(_blurChoices->currentIndex());
    _blurChoices->setCurrentIndex(0);
    QFile file(_path.toUtf8());
    if(file.exists())
    {
      QDomDocument doc("");
      file.open(QIODevice::ReadOnly);
      doc.setContent(&file);
      file.close();

      QDomElement root = doc.documentElement();
      QDomNode child = root.firstChild();
      while(!child.isNull())
      {
        QDomElement e = child.toElement();
        // We know how to treat appearance and geometry
        if (e.attribute("name") == name)
        {
          doc.documentElement().removeChild(child);
          break;
        }
        child = child.nextSibling();
      }

      if(file.open(QFile::WriteOnly))
      {
        QTextStream out(&file);
        out << doc.toString();
        file.close();
      }
    }
  }
  this->updateBlur(true);
}

/**
 * @brief
 *
 *  Updates the options the user has depending on then filter used
 */
void FilterChoice::updateDisplay()
{
      std::vector<Filter*> filters;
      _deleteButton->setEnabled(false);
      int num = _number->value();
      if(_blurChoices->currentIndex()!=-1){
          if(!_customButton->isChecked()){
                switch(_blurChoices->currentIndex())
                {
                    case 0:
                        filters = Filter::uniform(num);
                        _number->show();
                        _labelNumber->show();
                        _stdDevBox->hide();
                        _stdDevLabel->hide();
                break;
                    case 1:
                        filters = Filter::gaussian(num, _stdDevBox->value());
                        _number->show();
                        _labelNumber->show();
                        _stdDevBox->show();
                        _stdDevLabel->show();
                break;
                    case 2:
                        filters = Filter::prewitt(num);
                        _number->show();
                        _labelNumber->show();
                        _stdDevBox->hide();
                        _stdDevLabel->hide();
                break;
                    default:
                        filters = _filters[_blurChoices->currentIndex()];
                        _number->hide();
                        _labelNumber->hide();
                        _stdDevBox->hide();
                        _stdDevLabel->hide();
                }
            }else{
              filters = _filters[_blurChoices->currentIndex()];
              _number->hide();
              _labelNumber->hide();
              _stdDevBox->hide();
              _stdDevLabel->hide();
          }
        }else{
            _number->hide();
            _labelNumber->hide();
            _stdDevBox->hide();
            _stdDevLabel->hide();
        }

    if(_customButton->isChecked()){
        _deleteButton->setEnabled(true);
    }

  unsigned int height(0), width(0);

  for(unsigned int i = 0; i < filters.size(); i++)
  {
    if(height > 0)
      height++;
    height += filters[i]->getHeight();
    if(filters[i]->getWidth() > width)
      width = filters[i]->getWidth();
  }
  _filterView->setRowCount(height);
  _filterView->setColumnCount(width);
  for(unsigned int j = 0; j < height; j++)
  {
    for(unsigned int i = 0; i < width; i++)
    {
      QTableWidgetItem* item = new QTableWidgetItem("");
      item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        _filterView->setItem(j, i, item);
    }
  }

  height = 0;
  char buffer[20];
  for(unsigned int i = 0; i < filters.size(); i++)
  {
    for(unsigned int j = 0; j < filters[i]->getHeight(); j++)
    {
      for(unsigned int k = 0; k < filters[i]->getWidth(); k++)
      {
        double value = filters[i]->getPixelAt(k, j);
        QTableWidgetItem* item;
        if(_spinbox->isEnabled() && _spinbox->value()!=1 && value!=0){
            sprintf(buffer, "%g/%g", value, _spinbox->value());
            item = new QTableWidgetItem(QString(buffer));
        }else
            item = new QTableWidgetItem(QString::number(value));
        item->setTextAlignment(Qt::AlignHCenter);
        item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        _filterView->setItem(height + j, k, item);
        _filterView->setColumnWidth(k, _filterView->rowHeight(0));
      }
    }

    height += filters[i]->getHeight();
    for(unsigned int k = 0; k < filters[i]->getWidth(); k++)
    {
      QTableWidgetItem* item = new QTableWidgetItem("");
      item->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
      _filterView->setItem(height, k, item);
    }

    height++;
  }

  _filterView->resizeColumnsToContents();
  _filterView->resizeRowsToContents();
}

/**
 * @brief FilterChoice::openFile
 *
 * Allows to open the filter XML file anywhere on the computer
 */
void FilterChoice::openFile() {
    QString file = QFileDialog::getOpenFileName(this, tr("Open a file"), QString(), tr("XML Documents (*.xml)"));
    if(file.size()==0) return;
    _filterPath->setText(file);
    updatePath();
}

/**
 * @brief FilterChoice::updatePath
 *
 * Updates the path display to the user's filter, and updates the spinner displaying all the choices in the given XML file
 */
void FilterChoice::updatePath(){
    _path = _filterPath->text();
    updateBlur(true);
}

/**
 * @brief FilterChoice::updateOptions
 * @param a
 */
void FilterChoice::updateOptions(bool a){
    if(a){
        _offsetButton->setChecked(true);
        _scalingButton->setChecked(true);
        _offsetButton->setEnabled(true);
        _scalingButton->setEnabled(true);
    }else{
        _offsetButton->setChecked(false);
        _scalingButton->setChecked(false);
        _offsetButton->setEnabled(false);
        _scalingButton->setEnabled(false);
    }
}

void FilterChoice::showNormalisationOpt(bool a){
    _spinbox->setEnabled(a);
    _label_3->setEnabled(a);
    if(a){
        this->updateNormValue();
    }else{
        _spinbox->setValue(1);
    }
}

void FilterChoice::displayNormalisation(double){
    this->updateDisplay();
}

void FilterChoice::updateNormValue(){
    if(_blurChoices->currentIndex()>=0){
        double normValue = 0;
        std::vector<Filter*> filters;
        int num = _number->value();
        if(!_customButton->isChecked()){
          switch(_blurChoices->currentIndex())
          {
              case 0:
                  filters = Filter::uniform(num);
          break;
              case 1:
                  filters = Filter::gaussian(num, _stdDevBox->value());
          break;
              case 2:
                  filters = Filter::prewitt(num);
          break;
              default:
                  filters = _filters[_blurChoices->currentIndex()];
          }
        }
        else{
            filters = _filters[_blurChoices->currentIndex()];
        }


        for(unsigned int i = 0; i < filters.size(); i++)
        {
          double sum_pos = 0;
          double sum_neg = 0;
          for(unsigned int j = 0; j < filters[i]->getHeight(); j++)
          {
            for(unsigned int k = 0; k < filters[i]->getWidth(); k++)
            {
              filters[i]->getPixelAt(k, j) > 0 ? sum_pos += (double) filters[i]->getPixelAt(k, j) :  sum_neg += (double) filters[i]->getPixelAt(k, j);
            }
          }
          if(sum_pos + sum_neg != 0){
            normValue = max(normValue, abs(sum_pos + sum_neg));
          }
          else{
            normValue = max(normValue, sum_pos);
          }
          if(normValue == 0) normValue = 1;
        }
          _spinbox->setValue(normValue);
    }
    else{
        _checkbox_2->setChecked(false);
    }
}
