#include "pagelayoutpropertiesdialog.h"
#include "ui_pagelayoutpropertiesdialog.h"

#include "editor.h"
#include "pagelayout.h"
#include "pagelayoutoptionswidget.h"

PageLayoutPropertiesDialog::PageLayoutPropertiesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PageLayoutPropertiesDialog)
{
    ui->setupUi(this);
}

PageLayoutPropertiesDialog::~PageLayoutPropertiesDialog()
{
    delete ui;
}

void PageLayoutPropertiesDialog::initialize(Editor* editor)
{
    clear();

    const QList<PageLayout*>& pageLayouts = editor->pageLayouts();
    foreach(PageLayout* pageLayout, pageLayouts)
    {
        PageLayoutOptionsWidget* pageLayoutOptionsWidget = pageLayout->createPageLayoutOptionsWidget(ui->tabWidget);

        if (pageLayoutOptionsWidget != NULL)
        {
            connect(pageLayoutOptionsWidget, SIGNAL(pageLayoutOptionsChanged()), this, SIGNAL(pageLayoutOptionsChanged()));
            ui->tabWidget->addTab(pageLayoutOptionsWidget, pageLayoutOptionsWidget->layoutName());
        }
    }
}

void PageLayoutPropertiesDialog::clear()
{
    ui->tabWidget->clear();
}
