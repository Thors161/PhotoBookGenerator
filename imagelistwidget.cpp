#include "imagelistwidget.h"
#include "ui_imagelistwidget.h"

#include "image.h"
#include "imagelist.h"
#include "imagewidget.h"

ImageListWidget::ImageListWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImageListWidget)
{
    ui->setupUi(this);
    m_images = NULL;
    m_spacerItem = NULL;
}

ImageListWidget::~ImageListWidget()
{
    delete ui;
}

void ImageListWidget::setImages(ImageList* images)
{
    m_images = images;
}

void ImageListWidget::dispayImages()
{
    if (m_images == NULL)
        return;

    clear();

    int nr = 0;
    foreach(Image* image, *m_images)
    {
        ImageWidget* iw = new ImageWidget(this);
        iw->setImage(image);
        iw->setNr(nr);

        connect(iw, SIGNAL(imageChanged()), this, SIGNAL(imagesChanged()));
        connect(iw, SIGNAL(moveImage(int, int)), this, SLOT(moveImage(int, int)));

        m_imageWidgetList.append(iw);
        ui->verticalLayout->addWidget(iw);

        nr++;
    }

    m_spacerItem = new QSpacerItem(1, 1, QSizePolicy::Minimum, QSizePolicy::Expanding);
    ui->verticalLayout->addSpacerItem(m_spacerItem);
    update();
}

void ImageListWidget::updateImages()
{
    if (m_images == NULL)
        return;

    foreach(ImageWidget* imageWidget, m_imageWidgetList)
    {
        imageWidget->updateImage();
    }
}

void ImageListWidget::clear()
{
    m_imageWidgetList.clear();
    m_spacerItem = NULL;

    QLayoutItem* item;
    while ((item = ui->verticalLayout->takeAt(0)) != 0)
    {
        delete item->widget();
        delete item;
    }
}

void ImageListWidget::on_onlyIncludedImgsCheck_stateChanged(int state)
{
    foreach(ImageWidget* imageWidget, m_imageWidgetList)
    {
        imageWidget->setVisible(imageWidget->image()->include() || state != Qt::Checked);
    }
}

void ImageListWidget::moveImage(int from, int to)
{
    Q_CHECK_PTR(m_spacerItem);

    if (m_images == NULL)
        return;

    if (from == to)
        return;

    if ((from < 0) || (from >= m_images->size()))
        from = 0;

    if ((to < 0) || (to >= m_images->size()))
        to = m_images->size() - 1;

    m_images->move(from, to);

    // also change in view

    while ((ui->verticalLayout->takeAt(0)) != 0) {}

    m_imageWidgetList.move(from, to);

    int nr = 0;
    foreach(ImageWidget* imageWidget, m_imageWidgetList)
    {
        imageWidget->setNr(nr);
        ui->verticalLayout->addWidget(imageWidget);

        nr++;
    }

    ui->verticalLayout->addSpacerItem(m_spacerItem);
    update();

    emit imagesChanged();
}

