#ifndef PAGELISTWIDGET_H
#define PAGELISTWIDGET_H

#include <QWidget>

class Book;
class Editor;

namespace Ui {
class PageListWidget;
}

class PageListWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit PageListWidget(QWidget *parent = 0);
    ~PageListWidget();
    
    void showPages(Book const* book);
    void clear();

signals:
    void pageLayoutOptionsChanged();
    void showLayoutLinesChanged();

private slots:
    void on_showLayoutCheck_stateChanged(int arg1);

private:
    Ui::PageListWidget *ui;
};

#endif // PAGELISTWIDGET_H
