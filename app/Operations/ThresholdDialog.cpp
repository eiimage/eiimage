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

#include "Widgets/ImageWidgets/GenericHistogramView.h"

#include "ThresholdDialog.h"
#include "Algorithm/Otsu.h"
#include <qwt_plot.h>

#include "../../lib/detiq-t/GenericInterface/Widgets/ImageWidgets/ThumbnailView.h"

using namespace imagein;
using namespace imagein::algorithm;
using namespace genericinterface;

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
     updatePreview();

}

void ThresholdDialog::spinbox2Changed(int i) {
    _marker2->setXValue(i);
    updatePreview();
}

void ThresholdDialog::doubleThreshold(bool activate) {
    if(activate) {
        _marker2->show();
        _spin1label->setText(tr("Threshold #1 : "));
    }
    else {
        _marker2->hide();
        _spin1label->setText(tr("Threshold : "));
    }
}

void ThresholdDialog::otsu() {
    int threshold = Otsu_t<GrayscaleImage::depth_t>::computeThreshold(_image);
    _marker1->setXValue(threshold);
    _spinbox1->setValue(threshold);

}

void ThresholdDialog::updatePreview(){

    if(_previewBox->isChecked()){
        Binarization_t<GrayscaleImage::depth_t>* algo;
        if(doubleThreshold()) {
            algo = new Binarization_t<GrayscaleImage::depth_t>(_spinbox1->value(), _spinbox2->value(), !_blackButton->isChecked());
        }
        else {
            algo = new Binarization_t<GrayscaleImage::depth_t>(_spinbox1->value());
        }
        _previewImage = algo->operator()(_image);
        _preview->setImage(_previewImage);
        delete algo;
        delete _previewImage;
        _preview->repaint();
    }


 }

void ThresholdDialog::showPreview(bool activated){
    if(activated){
      //  updatePreview();
    }else{
        _preview->setImage(_image);
        _preview->repaint();
    }
}

ThresholdDialog::ThresholdDialog(const GrayscaleImage* image, bool converted)  : _image(image){
    this->setWindowTitle(tr("ThresholdOp"));
    this->setMinimumWidth(160);

    QGridLayout * layout = new QGridLayout();
    this->setLayout(layout);

    QVBoxLayout* Vboxlayout = new QVBoxLayout();
    if(converted) {
        Vboxlayout->addWidget(new QLabel(tr("<font color=red><i>Information : The input image has been converted to grayscale.</i></font>")));
    }
    QGroupBox* threshGroup = new QGroupBox(tr("Threshold"), this);
    QHBoxLayout* threshLayout = new QHBoxLayout(threshGroup);
    _doubleBox = new QCheckBox(tr("Double threshold (right clic to move the second threshold)"));
    threshLayout->addWidget(_doubleBox);
    Vboxlayout->addWidget(threshGroup);

    QHBoxLayout* box1layout = new QHBoxLayout();
    _spin1label = new QLabel(tr("Threshold : "));
    _spinbox1 = new QSpinBox();
    _spinbox1->setRange(0, 255);
    _spinbox1->setValue(127);
    QPushButton* otsuButton = new QPushButton(tr("Otsu"));
    box1layout->addWidget(_spin1label);
    box1layout->addWidget(_spinbox1);
    box1layout->addWidget(otsuButton);
    Vboxlayout->addLayout(box1layout);

    QHBoxLayout* box2layout = new QHBoxLayout();
    QLabel* spin2label = new QLabel(tr("Threshold #2 : "));
    spin2label->setVisible(false);
    _spinbox2 = new QSpinBox();
    _spinbox2->setRange(0, 255);
    _spinbox2->setValue(255);
    _spinbox2->setVisible(false);
    box2layout->addWidget(spin2label);
    box2layout->addWidget(_spinbox2);
    Vboxlayout->addLayout(box2layout);
    
    QHBoxLayout* radiolayout = new QHBoxLayout();
    QLabel* radioLabel = new QLabel(tr("Color between thresholds :"));
    QRadioButton* whiteButton = new QRadioButton(tr("White"));
    _blackButton = new QRadioButton(tr("Black"));
    radiolayout->addWidget(radioLabel);
    radiolayout->addWidget(whiteButton);
    radiolayout->addWidget(_blackButton);
    whiteButton->setChecked(true);
    Vboxlayout->addLayout(radiolayout);
    radioLabel->setVisible(false);
    whiteButton->setVisible(false);
    _blackButton->setVisible(false);

    Rectangle rect(0, 0, image->getWidth(), image->getHeight());
    GenericHistogramView* histo = new GenericHistogramView(image, rect);
    QwtPlot* plot = histo->getGraphicalHistogram();


    _marker1 = new QwtPlotMarker();
    _marker1->setLineStyle(QwtPlotMarker::VLine);
    _marker1->setLinePen(QPen(Qt::black));
    _marker1->setXValue(127);
    _marker1->attach(plot);
    
    _marker2 = new QwtPlotMarker();
    _marker2->setLineStyle(QwtPlotMarker::VLine);
    _marker2->setLinePen(QPen(Qt::black));
    _marker2->setXValue(255);
    _marker2->attach(plot);
    _marker2->hide();

    _preview = new ImageWidget(this, _image);
    _preview->setFixedSize(256*_preview->pixmap().width()/_preview->pixmap().height(), 256);
    layout->setColumnMinimumWidth(0,256*_preview->pixmap().width()/_preview->pixmap().height());
    layout->addWidget(_preview,0,0,0,0,Qt::AlignTop);
    Vboxlayout->addWidget(plot);
    layout->setRowMinimumHeight(0,256);
    layout->setColumnMinimumWidth(1,20);
    layout->addLayout(Vboxlayout,0,2,0,2,Qt::AlignLeft);
    _previewBox = new QCheckBox(tr("Aperçu"));
    _previewBox->setChecked(true);
    updatePreview();
    layout->addWidget(_previewBox,1,0,1,0,Qt::AlignTop);


    layout->setSizeConstraint(QLayout::SetMinimumSize);
    
    QPushButton *okButton = new QPushButton(tr("Validate"), this);
    okButton->setDefault(true);
    Vboxlayout->addWidget(okButton);

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
    connect(_previewBox, SIGNAL(toggled(bool)), this, SLOT(showPreview(bool)));
    connect(_previewBox, SIGNAL(toggled(bool)), this, SLOT(updatePreview()));

    connect(otsuButton, SIGNAL(pressed()), this, SLOT(otsu()));

    connect(_doubleBox, SIGNAL(toggled(bool)), this, SLOT(updatePreview()));
    connect(_blackButton, SIGNAL(toggled(bool)), this, SLOT(updatePreview()));
//    connect (this, SIGNAL(spinbox1Changed(int)), this, SLOT(updatePreview(bool)));
//    connect (this, SIGNAL(spinbox2Changed(int)), this, SLOT(updatePreview(bool)));


}

