#ifndef PAGELAYOUTOPTIONSWIDGET_H
#define PAGELAYOUTOPTIONSWIDGET_H

#include <QWidget>
#include <QString>

class PageLayoutOptionsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PageLayoutOptionsWidget(QWidget *parent = 0);
    virtual ~PageLayoutOptionsWidget();

    virtual QString layoutName() = 0;

signals:
    void pageLayoutOptionsChanged();
};

#endif // PAGELAYOUTOPTIONSWIDGET_H
