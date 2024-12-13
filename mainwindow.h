#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class Project;
class PageLayoutPropertiesDialog;
class BookPropertiesDialog;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
    void closeEvent(QCloseEvent *event);

private slots:
    void imagesChanged();
    void pageLayoutOptionsChanged();
    void bookPropertiesChanged();
    void showLayoutLinesChanged();

    void on_actionSave_triggered();
    void on_actionOpen_triggered();
    void on_actionAdd_triggered();
    void on_actionClear_triggered();
    void on_actionSave_as_triggered();
    void on_actionLayout_triggered();

    void on_actionNew_triggered();

    void on_actionClose_triggered();

    void on_actionExit_triggered();

    void on_actionPdf_triggered();
    void on_actionPs_triggered();

    void on_actionShow_date_first_of_month_triggered();

    void on_actionBook_triggered();

    void on_actionResort_by_date_triggered();

private:
    void updateWindowTitle();
    void clearWidgets();

    Project* m_project;
    bool m_dirty;

    PageLayoutPropertiesDialog* m_pageLayoutPropertiesDialog;
    BookPropertiesDialog* m_bookPropertiesDialog;

    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
