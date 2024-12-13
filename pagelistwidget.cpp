#include "pagelistwidget.h"
#include "ui_pagelistwidget.h"

#include "book.h"
#include "pagewidget.h"
#include "pagelayout.h"
#include "editor.h"
#include "pagelayoutoptionswidget.h"

PageListWidget::PageListWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PageListWidget)
{
    ui->setupUi(this);
}

PageListWidget::~PageListWidget()
{
    delete ui;
}

void PageListWidget::showPages(Book const* book)
{
    Q_CHECK_PTR(book);

    PageWidget* coverPageWidget = NULL;
    if (ui->verticalLayout->count() >= 1)
        coverPageWidget = static_cast<PageWidget*>(ui->verticalLayout->itemAt(0)->widget());
    else
    {
        coverPageWidget = new PageWidget(this);
        coverPageWidget->setShowLayoutLines(ui->showLayoutCheck->checkState() == Qt::Checked);
        ui->verticalLayout->addWidget(coverPageWidget);
    }

    coverPageWidget->setPage(book->coverPage());


    int i = 1;
    const QList<Page>& pages = book->pages();
    foreach(const Page& page, pages)
    {
        PageWidget* pageWidget = 0;
        if (ui->verticalLayout->count() > (i))
            pageWidget = static_cast<PageWidget*>(ui->verticalLayout->itemAt(i)->widget());
        else
        {
            pageWidget = new PageWidget(this);
            pageWidget->setShowLayoutLines(ui->showLayoutCheck->checkState() == Qt::Checked);
            ui->verticalLayout->addWidget(pageWidget);
        }

        pageWidget->setPage(page);

        i++;
    }

    for (int j = i; j < ui->verticalLayout->count(); j++)
    {
        QLayoutItem* item = ui->verticalLayout->takeAt(j);
        delete item->widget();
        delete item;
    }
}

void PageListWidget::clear()
{
    QLayoutItem* item;
    while ((item = ui->verticalLayout->takeAt(0)) != 0)
    {
        delete item->widget();
        delete item;
    }
}

void PageListWidget::on_showLayoutCheck_stateChanged(int arg1)
{
    bool show = false;
    if (arg1 == Qt::Checked)
        show = true;
    else if (arg1 == Qt::Unchecked)
        show = false;
    else
        return;

    for (int i = 0; i < ui->verticalLayout->count(); i++)
    {
        PageWidget* pageWidget = static_cast<PageWidget*>(ui->verticalLayout->itemAt(i)->widget());
        pageWidget->setShowLayoutLines(show);
    }

    emit showLayoutLinesChanged();
}
