#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QWidget>

class Image;

namespace Ui {
class ImageWidget;
}

class ImageWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit ImageWidget(QWidget *parent = 0);
    ~ImageWidget();
    
    void setImage(Image* image);
    Image const * const image() const;
    void setNr(int nr);
    void updateImage();

signals:
    void imageChanged();
    void moveImage(int from, int to);

private slots:
    void on_includeCheck_stateChanged(int state);
    void on_firstOnPageCheck_stateChanged(int state);
    void on_showDateCheck_stateChanged(int state);
    void on_captionEdit_textChanged(const QString &text);

    void on_shareOnPageRadio0_toggled(bool checked);
    void on_shareOnPageRadio1_toggled(bool checked);
    void on_shareOnPageRadio2_toggled(bool checked);
    void on_shareOnPageRadio3_toggled(bool checked);
    void on_shareOnPageRadio4_toggled(bool checked);
    void on_shareOnPageRadio5_toggled(bool checked);
    void on_shareOnPageRadio6_toggled(bool checked);
    void on_shareOnPageRadio7_toggled(bool checked);

    void on_moveToButton_clicked();

private:
    void displayImage();
    void shareOnPageChanged(int value);

    Ui::ImageWidget *ui;

    Image* m_image;
    int m_nr;
};

#endif // IMAGEWIDGET_H
