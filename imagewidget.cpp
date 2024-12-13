#include "imagewidget.h"
#include "ui_imagewidget.h"

#include "image.h"
#include <QFileInfo>
#include <QPixmap>

ImageWidget::ImageWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImageWidget)
{
    ui->setupUi(this);
    m_image = NULL;
    m_nr = 0;
}

ImageWidget::~ImageWidget()
{
    delete ui;
}

void ImageWidget::setImage(Image* image)
{
    m_image = image;
    displayImage();
}

Image const * const ImageWidget::image() const
{
    return m_image;
}

void ImageWidget::setNr(int nr)
{
    m_nr = nr;
    ui->nrLabel->setText(QString::number(nr));
    ui->moveToSpinBox->setValue(nr);
}

void ImageWidget::updateImage()
{
    displayImage();
}

void ImageWidget::on_includeCheck_stateChanged(int state)
{
    if (m_image == NULL)
        return;

    if (state == Qt::Unchecked)
        m_image->setInclude(false);
    else if (state == Qt::Checked)
        m_image->setInclude(true);

    emit imageChanged();
}

void ImageWidget::on_firstOnPageCheck_stateChanged(int state)
{
    if (m_image == NULL)
        return;

    if (state == Qt::Unchecked)
        m_image->setFirstOnPage(false);
    else if (state == Qt::Checked)
        m_image->setFirstOnPage(true);

    emit imageChanged();
}


void ImageWidget::on_showDateCheck_stateChanged(int state)
{
    if (m_image == NULL)
        return;

    if (state == Qt::Unchecked)
        m_image->setShowDate(false);
    else if (state == Qt::Checked)
        m_image->setShowDate(true);

    emit imageChanged();
}

void ImageWidget::on_captionEdit_textChanged(const QString &text)
{
    if (m_image == NULL)
        return;

    m_image->setCaption(text);

    emit imageChanged();
}

void ImageWidget::on_shareOnPageRadio0_toggled(bool checked)
{
    if (checked)
        shareOnPageChanged(0);
}

void ImageWidget::on_shareOnPageRadio1_toggled(bool checked)
{
    if (checked)
        shareOnPageChanged(1);
}

void ImageWidget::on_shareOnPageRadio2_toggled(bool checked)
{
    if (checked)
        shareOnPageChanged(2);
}

void ImageWidget::on_shareOnPageRadio3_toggled(bool checked)
{
    if (checked)
        shareOnPageChanged(3);
}

void ImageWidget::on_shareOnPageRadio4_toggled(bool checked)
{
    if (checked)
        shareOnPageChanged(4);
}

void ImageWidget::on_shareOnPageRadio5_toggled(bool checked)
{
    if (checked)
        shareOnPageChanged(5);
}

void ImageWidget::on_shareOnPageRadio6_toggled(bool checked)
{
    if (checked)
        shareOnPageChanged(6);
}

void ImageWidget::on_shareOnPageRadio7_toggled(bool checked)
{
    if (checked)
        shareOnPageChanged(7);
}

void ImageWidget::on_moveToButton_clicked()
{
    emit moveImage(m_nr, ui->moveToSpinBox->value());
}

void ImageWidget::displayImage()
{
    if (m_image == NULL)
        return;

    const int previewWidth = 160;
    QPixmap pixmap = QPixmap::fromImage(m_image->preview().scaledToWidth(previewWidth));

    ui->fileNameLabel->setText(QFileInfo(m_image->fileName()).fileName());
    ui->includeCheck->setChecked(m_image->include());
    ui->imageLabel->setMinimumSize(pixmap.size());
    ui->imageLabel->setMaximumSize(pixmap.size());
    ui->imageLabel->setPixmap(pixmap);
    ui->captionEdit->setText(m_image->caption());
    ui->firstOnPageCheck->setChecked(m_image->firstOnPage());
    ui->showDateCheck->setChecked(m_image->showDate());
    if (m_image->dateTime().isValid())
        ui->dateLabel->setText(m_image->dateTime().toString("dd-MM-yyyy"));

    if (m_image->pageSharing() == 0)
        ui->shareOnPageRadio0->setChecked(true);
    if (m_image->pageSharing() == 1)
        ui->shareOnPageRadio1->setChecked(true);
    if (m_image->pageSharing() == 2)
        ui->shareOnPageRadio2->setChecked(true);
    if (m_image->pageSharing() == 3)
        ui->shareOnPageRadio3->setChecked(true);
    if (m_image->pageSharing() == 4)
        ui->shareOnPageRadio4->setChecked(true);
    if (m_image->pageSharing() == 5)
        ui->shareOnPageRadio5->setChecked(true);
    if (m_image->pageSharing() == 6)
        ui->shareOnPageRadio6->setChecked(true);
    if (m_image->pageSharing() == 7)
        ui->shareOnPageRadio7->setChecked(true);
}

void ImageWidget::shareOnPageChanged(int value)
{
    if (m_image == NULL)
        return;

    m_image->setPageSharing(value);

    emit imageChanged();
}

