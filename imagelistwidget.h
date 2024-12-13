#ifndef IMAGELISTWIDGET_H
#define IMAGELISTWIDGET_H

#include <QWidget>
#include <QList>

class QSpacerItem;

class Image;
class ImageList;
class ImageWidget;

namespace Ui {
class ImageListWidget;
}

class ImageListWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit ImageListWidget(QWidget *parent = 0);
    ~ImageListWidget();
    
    void setImages(ImageList* images);
    void dispayImages();
    void updateImages();
    void clear();

public slots:
    void moveImage(int from, int to);

signals:
    void imagesChanged();

private slots:
    void on_onlyIncludedImgsCheck_stateChanged(int state);

private:
    ImageList* m_images;
    QList<ImageWidget*> m_imageWidgetList;
    QSpacerItem* m_spacerItem;

    Ui::ImageListWidget *ui;
};

#endif // IMAGELISTWIDGET_H
