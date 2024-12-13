#ifndef PAGELAYOUTPROPERTIESDIALOG_H
#define PAGELAYOUTPROPERTIESDIALOG_H

#include <QDialog>

class Editor;

namespace Ui {
class PageLayoutPropertiesDialog;
}

class PageLayoutPropertiesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PageLayoutPropertiesDialog(QWidget *parent = 0);
    ~PageLayoutPropertiesDialog();

    void initialize(Editor* editor);
    void clear();

signals:
    void pageLayoutOptionsChanged();

private:
    Ui::PageLayoutPropertiesDialog *ui;
};

#endif // PAGELAYOUTPROPERTIESDIALOG_H
