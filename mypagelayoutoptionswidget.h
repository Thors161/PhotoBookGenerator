#ifndef MYPAGELAYOUTOPTIONSWIDGET_H
#define MYPAGELAYOUTOPTIONSWIDGET_H

#include "pagelayoutoptionswidget.h"

class MyPageLayout;

namespace Ui {
class MyPageLayoutOptionsWidget;
}

class MyPageLayoutOptionsWidget : public PageLayoutOptionsWidget
{
    Q_OBJECT
    
public:
    explicit MyPageLayoutOptionsWidget(QWidget *parent = 0);
    ~MyPageLayoutOptionsWidget();
    
    QString layoutName(); // override

    void setMyPageLayout(MyPageLayout* myPageLayout);

private slots:
    void on_gutterSpb_valueChanged(double arg1);

    void on_imageSpacingSpb_valueChanged(double arg1);

    void on_imageCroppingChk_stateChanged(int arg1);

    void on_captionPageMarginSpb_valueChanged(double arg1);

    void on_captionImageMarginSpb_valueChanged(double arg1);

    void on_captionMinWidthSpb_valueChanged(double arg1);

    void on_captionFontBut_clicked();

    void on_bgColorBut_clicked();

    void on_captionColorBut_clicked();

private:
    Ui::MyPageLayoutOptionsWidget *ui;

    MyPageLayout* m_myPageLayout;
    bool m_emitChanges;
};

#endif // MYPAGELAYOUTOPTIONSWIDGET_H
