#ifndef MYCOVERPAGEPROPERTIESWIDGET_H
#define MYCOVERPAGEPROPERTIESWIDGET_H

#include <QWidget>

#include <pagelayoutoptionswidget.h>

class MyCoverPageLayout;

namespace Ui {
class MyCoverPagePropertiesWidget;
}

class MyCoverPagePropertiesWidget : public PageLayoutOptionsWidget
{
    Q_OBJECT

public:
    explicit MyCoverPagePropertiesWidget(QWidget *parent = 0);
    ~MyCoverPagePropertiesWidget();

    QString layoutName(); // override

    void setMyCoverPageLayout(MyCoverPageLayout* myCoverPageLayout);


private slots:
    void on_btnTitleFont_clicked();

    void on_editTitle_textChanged(const QString &arg1);

    void on_edtSubTitle_textChanged(const QString &arg1);

    void on_btnSubTitleFont_clicked();

    void on_btnTextColor_clicked();

    void on_btnTitleBarColor_clicked();

    void on_btnPattern_clicked();

    void on_spbPatternWidth_valueChanged(double arg1);

    void on_spbBarTop_valueChanged(double arg1);

    void on_spbBarHeight_valueChanged(double arg1);

    void on_spbSpineWidth_valueChanged(double arg1);

private:
    Ui::MyCoverPagePropertiesWidget *ui;

    MyCoverPageLayout* m_myCoverPageLayout;
    bool m_emitChanges;
};

#endif // MYCOVERPAGEPROPERTIESWIDGET_H
