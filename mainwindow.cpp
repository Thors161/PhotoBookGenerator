#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "myproject.h"
#include "imagelist.h"
#include "book.h"
#include "editor.h"

#include "bookpropertiesdialog.h"
#include "pagelayoutpropertiesdialog.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QDir>
#include <QSettings>
#include <QCloseEvent>
#include <QTime>

namespace
{
Project* createProject()
{
    return new MyProject();
}
} // anonymous

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_project = NULL;
    m_dirty = false;

    connect(ui->imageListWidget, SIGNAL(imagesChanged()), this, SLOT(imagesChanged()));
    connect(ui->pageListWidget, SIGNAL(pageLayoutOptionsChanged()), this, SLOT(pageLayoutOptionsChanged()));
    connect(ui->pageListWidget, SIGNAL(showLayoutLinesChanged()), this, SLOT(showLayoutLinesChanged()));

    ui->actionClear->setEnabled(false);
    ui->actionClose->setEnabled(false);
    ui->actionSave->setEnabled(false);
    ui->actionSave_as->setEnabled(false);
    ui->actionAdd->setEnabled(false);
    ui->actionPs->setEnabled(false);
    ui->actionPdf->setEnabled(false);
    ui->actionBook->setEnabled(false);
    ui->actionLayout->setEnabled(false);
    ui->actionShow_date_first_of_month->setEnabled(false);

    updateWindowTitle();

    m_pageLayoutPropertiesDialog = new PageLayoutPropertiesDialog(this);
    connect(m_pageLayoutPropertiesDialog, SIGNAL(pageLayoutOptionsChanged()), this, SLOT(pageLayoutOptionsChanged()));

    m_bookPropertiesDialog = new BookPropertiesDialog(this);
    connect(m_bookPropertiesDialog, SIGNAL(bookPropertiesChanged()), this, SLOT(bookPropertiesChanged()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    on_actionExit_triggered();
    event->ignore();
}

void MainWindow::imagesChanged()
{
    Q_CHECK_PTR(m_project);

    m_dirty = true;
    updateWindowTitle();

    m_project->edit();
    ui->pageListWidget->showPages(m_project->book());
    ui->statusBar->showMessage(tr("Edited %1 pages").arg(m_project->book()->pages().size()));
}

void MainWindow::pageLayoutOptionsChanged()
{
    Q_CHECK_PTR(m_project);

    m_dirty = true;
    updateWindowTitle();

    m_project->edit();
    ui->pageListWidget->showPages(m_project->book());
    ui->statusBar->showMessage(tr("Edited %1 pages").arg(m_project->book()->pages().size()));
}

void MainWindow::bookPropertiesChanged()
{
    Q_CHECK_PTR(m_project);

    m_dirty = true;
    updateWindowTitle();

    m_project->edit();
    ui->pageListWidget->showPages(m_project->book());
    ui->statusBar->showMessage(tr("Edited %1 pages").arg(m_project->book()->pages().size()));
}

void MainWindow::showLayoutLinesChanged()
{
    if (m_project == NULL)
        return;

    m_project->edit();
    ui->pageListWidget->showPages(m_project->book());
}

void MainWindow::on_actionSave_triggered()
{
    Q_CHECK_PTR(m_project);

    if (m_project->projectFileName().isEmpty())
    {
        on_actionSave_as_triggered();
        return;
    }

    QString error;
    if (!m_project->save(m_project->projectFilePath(), &error))
        QMessageBox::warning(this, tr("Photobook generator"), error, QMessageBox::Ok);

    m_dirty = false;
    updateWindowTitle();
}

void MainWindow::on_actionSave_as_triggered()
{
    Q_CHECK_PTR(m_project);

    QSettings settings("Sietse Dijkstra", "Photobook generator");

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save project"),
                                                    settings.value("projectDir", ".").toString(),
                                                    tr("Photobook project files (*.pbp)"));

    if (fileName.isEmpty())
        return;

    settings.setValue("projectDir", QFileInfo(fileName).path());

    if (!fileName.endsWith(".pbp"))
        fileName += ".pbp";

    QString error;
    if (!m_project->save(fileName, &error))
        QMessageBox::warning(this, tr("Photobook generator"), error, QMessageBox::Ok);

    m_dirty = false;
    updateWindowTitle();
}

void MainWindow::on_actionOpen_triggered()
{
    if (m_project != NULL && m_dirty)
    {
        int ret = QMessageBox::warning(this, tr("Photobook generator"), tr("Are you sure? This will close your current project without saving."), QMessageBox::Ok, QMessageBox::Cancel);
        if (ret == QMessageBox::Cancel)
            return;
    }

    QSettings settings("Sietse Dijkstra", "Photobook generator");

    QString fileName = QFileDialog::getOpenFileName(this,
            tr("Open project"), settings.value("projectDir", ".").toString(),
                                                    tr("Photobook project files (*.pbp)"));

    if (fileName.isEmpty())
        return;

    settings.setValue("projectDir", QFileInfo(fileName).path());

    clearWidgets();

    if (m_project != NULL)
        delete m_project;

    m_project = createProject();

    QString error;
    if (!m_project->load(fileName, &error))
    {
        QMessageBox::warning(this, tr("Photobook generator"), error, QMessageBox::Ok);
        return;
    }

    m_project->edit();

    ui->imageListWidget->setImages(m_project->images());
    ui->imageListWidget->dispayImages();

    ui->pageListWidget->showPages(m_project->book());

    ui->actionClear->setEnabled(true);
    ui->actionClose->setEnabled(true);
    ui->actionSave->setEnabled(true);
    ui->actionSave_as->setEnabled(true);
    ui->actionAdd->setEnabled(true);
    ui->actionPs->setEnabled(true);
    ui->actionPdf->setEnabled(true);
    ui->actionBook->setEnabled(true);
    ui->actionLayout->setEnabled(true);
    ui->actionShow_date_first_of_month->setEnabled(true);

    m_dirty = false;
    updateWindowTitle();
}

void MainWindow::on_actionAdd_triggered()
{
    Q_CHECK_PTR(m_project);

    QSettings settings("Sietse Dijkstra", "Photobook generator");

    QStringList fileNames = QFileDialog::getOpenFileNames(
                this, tr("Add images"), settings.value("imageAddDir", ".").toString(),
                tr("Image files (*.png *.jpg *.JPG *.JPEG *.jpeg *.PNG)"));

    if (fileNames.isEmpty())
        return;

    settings.setValue("imageAddDir", QFileInfo(fileNames.at(0)).path());

    QTime t;
    t.start();
    m_project->appendImages(fileNames);

    int t1_2 = t.elapsed();

    m_project->edit();

    int t1 = t.elapsed();

    ui->imageListWidget->setImages(m_project->images());

    int t2 = t.elapsed();

    ui->imageListWidget->dispayImages();

    int t3 = t.elapsed();

    ui->pageListWidget->showPages(m_project->book());

    int t5 = t.elapsed();
    int t6 = t.elapsed();

    m_dirty = true;
    updateWindowTitle();
}

void MainWindow::on_actionClear_triggered()
{
    Q_CHECK_PTR(m_project);

    clearWidgets();
    m_project->clear();

    m_project->edit();
    ui->pageListWidget->showPages(m_project->book());

    m_dirty = true;
    updateWindowTitle();
}

void MainWindow::on_actionLayout_triggered()
{
    Q_CHECK_PTR(m_project);

    Editor* editor = m_project->editor();

    Q_CHECK_PTR(editor);
    Q_CHECK_PTR(m_pageLayoutPropertiesDialog);

    m_pageLayoutPropertiesDialog->initialize(editor);
    m_pageLayoutPropertiesDialog->show();
}

void MainWindow::on_actionNew_triggered()
{
    if (m_project != NULL && m_dirty)
    {
        int ret = QMessageBox::warning(this, tr("Photobook generator"), tr("Are you sure,? This will close your current project without saving."), QMessageBox::Ok, QMessageBox::Cancel);
        if (ret == QMessageBox::Cancel)
            return;
    }

    clearWidgets();

    if (m_project != NULL)
        delete m_project;

    m_project = createProject();

    Book* book = m_project->book();

    m_bookPropertiesDialog->setShowOk(true);
    m_bookPropertiesDialog->initialize(book);
    m_bookPropertiesDialog->setDefaults();
    m_bookPropertiesDialog->exec();

    ui->actionClear->setEnabled(true);
    ui->actionClose->setEnabled(true);
    ui->actionSave->setEnabled(true);
    ui->actionSave_as->setEnabled(true);
    ui->actionAdd->setEnabled(true);
    ui->actionPs->setEnabled(true);
    ui->actionPdf->setEnabled(true);
    ui->actionBook->setEnabled(true);
    ui->actionLayout->setEnabled(true);
    ui->actionShow_date_first_of_month->setEnabled(true);

    m_dirty = false;
    updateWindowTitle();
}

void MainWindow::on_actionClose_triggered()
{
    Q_CHECK_PTR(m_project);

    if (m_dirty)
    {
        int ret = QMessageBox::warning(this, tr("Photobook generator"), tr("Are you sure? This will close your current project without saving."), QMessageBox::Ok, QMessageBox::Cancel);
        if (ret == QMessageBox::Cancel)
            return;
    }

    clearWidgets();

    m_pageLayoutPropertiesDialog->close();
    m_pageLayoutPropertiesDialog->clear();

    m_bookPropertiesDialog->close();
    m_bookPropertiesDialog->clear();

    delete m_project;
    m_project = NULL;

    ui->actionClear->setEnabled(false);
    ui->actionClose->setEnabled(false);
    ui->actionSave->setEnabled(false);
    ui->actionSave_as->setEnabled(false);
    ui->actionAdd->setEnabled(false);
    ui->actionPs->setEnabled(false);
    ui->actionPdf->setEnabled(false);
    ui->actionBook->setEnabled(false);
    ui->actionLayout->setEnabled(false);
    ui->actionShow_date_first_of_month->setEnabled(false);

    m_dirty = false;
    updateWindowTitle();
}

void MainWindow::on_actionExit_triggered()
{
    if (m_project != NULL && m_dirty)
    {
        int ret = QMessageBox::warning(this, tr("Photobook generator"), tr("Are you sure? This will close your current project without saving."), QMessageBox::Ok, QMessageBox::Cancel);
        if (ret == QMessageBox::Cancel)
            return;
    }

    QApplication::quit();
}

void MainWindow::clearWidgets()
{
    ui->imageListWidget->clear();
    ui->pageListWidget->clear();
}

void MainWindow::updateWindowTitle()
{
    QString projectString;
    QString dirtyString;

    if (m_project == NULL)
        projectString = tr("No project");
    else
    {
        if (m_dirty)
            dirtyString = "[*]";

        if (m_project->projectFileName().isEmpty())
            projectString = tr("Unsaved project");
        else
            projectString = m_project->projectFileName();
    }

    setWindowTitle(QString("%1%2 - Lazy photo book generator").arg(projectString).arg(dirtyString));
    setWindowFilePath(projectString);
    setWindowModified(m_dirty);
}

void MainWindow::on_actionPdf_triggered()
{
    Q_CHECK_PTR(m_project);

    QSettings settings("Sietse Dijkstra", "Photobook generator");

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save project"),
                                                    settings.value("exportDir", ".").toString(),
                                                    tr("Pdf files (*.pdf)"));

    if (fileName.isEmpty())
        return;

    settings.setValue("exportDir", QFileInfo(fileName).path());

    if (!fileName.endsWith(".pdf"))
        fileName += ".pdf";

    if (!m_project->render(fileName))
        QMessageBox::warning(this, tr("Photobook generator"), tr("Error rendering book"), QMessageBox::Ok);
}

void MainWindow::on_actionPs_triggered()
{
    Q_CHECK_PTR(m_project);

    QSettings settings("Sietse Dijkstra", "Photobook generator");

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save project"),
                                                    settings.value("exportDir", ".").toString(),
                                                    tr("Ps files (*.ps)"));

    if (fileName.isEmpty())
        return;

    settings.setValue("exportDir", QFileInfo(fileName).path());

    if (!fileName.endsWith(".ps"))
        fileName += ".ps";

    if (!m_project->render(fileName))
        QMessageBox::warning(this, tr("Photobook generator"), tr("Error rendering book"), QMessageBox::Ok);
}

void MainWindow::on_actionShow_date_first_of_month_triggered()
{
    Q_CHECK_PTR(m_project);

    m_project->showDateFirstInMonth();
    m_project->edit();

    ui->imageListWidget->updateImages();
    ui->pageListWidget->showPages(m_project->book());

    m_dirty = true;
    updateWindowTitle();
}

void MainWindow::on_actionBook_triggered()
{
    Q_CHECK_PTR(m_project);

    Book* book = m_project->book();

    Q_CHECK_PTR(book);
    Q_CHECK_PTR(m_bookPropertiesDialog);

    m_bookPropertiesDialog->setShowOk(false);
    m_bookPropertiesDialog->initialize(book);
    m_bookPropertiesDialog->show();
}

void MainWindow::on_actionResort_by_date_triggered()
{
    QTime t;
    t.start();
    m_project->sortByDate();

    int t1_2 = t.elapsed();

    m_project->edit();

    int t1 = t.elapsed();

    ui->imageListWidget->setImages(m_project->images());

    int t2 = t.elapsed();

    ui->imageListWidget->dispayImages();

    int t3 = t.elapsed();

    ui->pageListWidget->showPages(m_project->book());

    int t5 = t.elapsed();
    int t6 = t.elapsed();

    m_dirty = true;
    updateWindowTitle();
}
