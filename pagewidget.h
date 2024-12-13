#ifndef PAGEWIDGET_H
#define PAGEWIDGET_H

#include <QWidget>

#include "page.h"

namespace Ui {
class PageWidget;
}

class PageWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit PageWidget(QWidget *parent = 0);
    ~PageWidget();
    
    void setPage(const Page& page);
    void setShowLayoutLines(bool showLayoutLines);

protected:
    void paintEvent(QPaintEvent* event);

private:
    Ui::PageWidget *ui;

    Page m_page;
    bool m_showLayoutLines;
};

#endif // PAGEWIDGET_H
