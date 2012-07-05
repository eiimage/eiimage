#include "Thresholding.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QButtonGroup>
#include <QRadioButton>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QGroupBox>
#include <QFormLayout>

#include "ImgWidget.h"


#include "Widgets/ImageWidgets/GenericHistogramView.h"

#include "Algorithm/Otsu.h"
#include "Algorithm/Binarization.h"
#include "Converter.h"

using namespace std;
using namespace imagein;
using namespace imagein::algorithm;
using namespace genericinterface;

Thresholding::Thresholding() : Operation("Thresholding") {
}

bool Thresholding::needCurrentImg() {
    return true;
}

void ThresholdDialog::marker1Moved(const QPointF& point) {
    _marker1->setXValue(point.x());
    _spinbox1->setValue(point.x());
}

void ThresholdDialog::marker2Moved(const QPointF& point) {
    _marker2->setXValue(point.x());
    _spinbox2->setValue(point.x());
}

void ThresholdDialog::spinbox1Changed(int i) {
    _marker1->setXValue(i);
}

void ThresholdDialog::spinbox2Changed(int i) {
    _marker2->setXValue(i);
}

void ThresholdDialog::doubleThreshold(bool activate) {
    if(activate) {
        _marker2->show();
        _spin1label->setText("Threshold #1 : ");
    }
    else {
        _marker2->hide();
        _spin1label->setText("Threshold : ");
    }
}

void ThresholdDialog::otsu() {
    int threshold = Otsu_t<GrayscaleImage::depth_t>::computeThreshold(_image);
    _marker1->setXValue(threshold);
    _spinbox1->setValue(threshold);
}

ThresholdDialog::ThresholdDialog(const GrayscaleImage* image)  : _image(image){
    this->setWindowTitle(tr("Thresholding"));
    this->setMinimumWidth(160);
    QVBoxLayout* layout = new QVBoxLayout();
    this->setLayout(layout);

    QGroupBox* threshGroup = new QGroupBox("Threshold", this);
    QHBoxLayout* threshLayout = new QHBoxLayout(threshGroup);
    _doubleBox = new QCheckBox("Double threshold");
    threshLayout->addWidget(_doubleBox);
    layout->addWidget(threshGroup);

    QHBoxLayout* box1layout = new QHBoxLayout();
    _spin1label = new QLabel("Threshold : "); 
    _spinbox1 = new QSpinBox();
    _spinbox1->setRange(0, 255);
    _spinbox1->setValue(127);
    QPushButton* otsuButton = new QPushButton("Otsu");
    box1layout->addWidget(_spin1label);
    box1layout->addWidget(_spinbox1);
    box1layout->addWidget(otsuButton);
    layout->addLayout(box1layout);

    QHBoxLayout* box2layout = new QHBoxLayout();
    QLabel* spin2label = new QLabel("Threshold #2 : "); 
    spin2label->setVisible(false);
    _spinbox2 = new QSpinBox();
    _spinbox2->setRange(0, 255);
    _spinbox2->setValue(255);
    _spinbox2->setVisible(false);
    box2layout->addWidget(spin2label);
    box2layout->addWidget(_spinbox2);
    layout->addLayout(box2layout);
    
    QHBoxLayout* radiolayout = new QHBoxLayout();
    QLabel* radioLabel = new QLabel("Color between thresholds :");
    QRadioButton* whiteButton = new QRadioButton(tr("White"));
    _blackButton = new QRadioButton(tr("Black"));
    radiolayout->addWidget(radioLabel);
    radiolayout->addWidget(whiteButton);
    radiolayout->addWidget(_blackButton);
    whiteButton->setChecked(true);
    layout->addLayout(radiolayout);
    radioLabel->setVisible(false);
    whiteButton->setVisible(false);
    _blackButton->setVisible(false);

    Rectangle rect(0, 0, image->getWidth(), image->getHeight());
    GenericHistogramView* histo = new GenericHistogramView(image, rect);
    QwtPlot* plot = histo->getGraphicalHistogram();

    _marker1 = new QwtPlotMarker();
    _marker1->setLineStyle(QwtPlotMarker::VLine);
    _marker1->setLinePen(QPen(Qt::black));
    _marker1->setXValue(64);
    _marker1->attach(plot);
    
    _marker2 = new QwtPlotMarker();
    _marker2->setLineStyle(QwtPlotMarker::VLine);
    _marker2->setLinePen(QPen(Qt::black));
    _marker2->setXValue(192);
    _marker2->attach(plot);
    _marker2->hide();
    
    layout->addWidget(plot);
    
    layout->setSizeConstraint(QLayout::SetFixedSize);
    
    QPushButton *okButton = new QPushButton(tr("Validate"), this);
    okButton->setDefault(true);
    layout->addWidget(okButton);
    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(histo, SIGNAL(leftMoved(const QPointF&)), this, SLOT(marker1Moved(const QPointF&)));
    connect(histo, SIGNAL(rightMoved(const QPointF&)), this, SLOT(marker2Moved(const QPointF&)));

    connect(_spinbox1, SIGNAL(valueChanged(int)), this, SLOT(spinbox1Changed(int)));
    connect(_spinbox2, SIGNAL(valueChanged(int)), this, SLOT(spinbox2Changed(int)));

    connect(_doubleBox, SIGNAL(toggled(bool)), _spinbox2, SLOT(setVisible(bool)));
    connect(_doubleBox, SIGNAL(toggled(bool)), spin2label, SLOT(setVisible(bool)));
    connect(_doubleBox, SIGNAL(toggled(bool)), radioLabel, SLOT(setVisible(bool)));
    connect(_doubleBox, SIGNAL(toggled(bool)), whiteButton, SLOT(setVisible(bool)));
    connect(_doubleBox, SIGNAL(toggled(bool)), _blackButton, SLOT(setVisible(bool)));
    connect(_doubleBox, SIGNAL(toggled(bool)), otsuButton, SLOT(setHidden(bool)));
    connect(_doubleBox, SIGNAL(toggled(bool)), this, SLOT(doubleThreshold(bool)));

    connect(otsuButton, SIGNAL(pressed()), this, SLOT(otsu()));
}

std::vector<QWidget*> Thresholding::operator()(const imagein::Image* image, const std::map<std::string, const imagein::Image*>&) {
    vector<QWidget*> result;
    
    GrayscaleImage* img = Converter<GrayscaleImage>::convert(*image);
    
    ThresholdDialog* dialog = new ThresholdDialog(img);
    
    QDialog::DialogCode code = static_cast<QDialog::DialogCode>(dialog->exec());
    
    if(code!=QDialog::Accepted) {
        return result;
    }

    Binarization_t<GrayscaleImage::depth_t>* algo;
    if(dialog->doubleThreshold()) {
        algo = new Binarization_t<GrayscaleImage::depth_t>(dialog->threshold1(), dialog->threshold2(), dialog->blackBand());
    }
    else {
        algo = new Binarization_t<GrayscaleImage::depth_t>(dialog->threshold1());
    }

    GrayscaleImage* resImg = algo->operator()(img);
    result.push_back(new ImgWidget(resImg, ""));

    return result;
}
