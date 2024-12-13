#include "pagewidget.h"
#include "ui_pagewidget.h"

#include <QPainter>

PageWidget::PageWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageWidget)
{
    ui->setupUi(this);
    m_showLayoutLines = false;
}

PageWidget::~PageWidget()
{
    delete ui;
}

void PageWidget::setPage(const Page& page)
{
    m_page = page;

    const double scale = 2;

    setMinimumSize(QSize(page.widthMm() * scale, page.heightMm() * scale));
    setMaximumSize(QSize(page.widthMm() * scale, page.heightMm() * scale));

    update();
}

void PageWidget::setShowLayoutLines(bool showLayoutLines)
{
    m_showLayoutLines = showLayoutLines;
}

void PageWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    m_page.paint(painter, this, m_showLayoutLines, !m_showLayoutLines, false);
}
