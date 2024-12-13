#include "bookpropertiesdialog.h"
#include "ui_bookpropertiesdialog.h"

#include "book.h"

BookPropertiesDialog::BookPropertiesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BookPropertiesDialog)
{
    ui->setupUi(this);
    m_book = NULL;
}

BookPropertiesDialog::~BookPropertiesDialog()
{
    delete ui;
}

void BookPropertiesDialog::setShowOk(bool showOk)
{
    ui->btnOk->setVisible(showOk);
}

void BookPropertiesDialog::initialize(Book* book)
{
    m_book = book;

    // as we get the values from the book into the spinboxes, it should not be set on the book again

    disconnect(ui->spbCoverPageWidth, SIGNAL(valueChanged(double)), this, SLOT(on_spbCoverPageWidth_valueChanged(double)));
    disconnect(ui->spbCoverPageHeight, SIGNAL(valueChanged(double)), this, SLOT(on_spbCoverPageHeight_valueChanged(double)));
    disconnect(ui->spbPageWidth, SIGNAL(valueChanged(double)), this, SLOT(on_spbPageWidth_valueChanged(double)));
    disconnect(ui->spbPageHeight, SIGNAL(valueChanged(double)), this, SLOT(on_spbPageHeight_valueChanged(double)));
    disconnect(ui->spbTrim, SIGNAL(valueChanged(double)), this, SLOT(on_spbTrim_valueChanged(double)));

    ui->spbCoverPageWidth->setValue(m_book->coverPageWidthMm());
    ui->spbCoverPageHeight->setValue(m_book->coverPageHeightMm());
    ui->spbPageWidth->setValue(m_book->pageWidthMm());
    ui->spbPageHeight->setValue(m_book->pageHeightMm());
    ui->spbTrim->setValue(m_book->trimMm());

    connect(ui->spbCoverPageWidth, SIGNAL(valueChanged(double)), this, SLOT(on_spbCoverPageWidth_valueChanged(double)));
    connect(ui->spbCoverPageHeight, SIGNAL(valueChanged(double)), this, SLOT(on_spbCoverPageHeight_valueChanged(double)));
    connect(ui->spbPageWidth, SIGNAL(valueChanged(double)), this, SLOT(on_spbPageWidth_valueChanged(double)));
    connect(ui->spbPageHeight, SIGNAL(valueChanged(double)), this, SLOT(on_spbPageHeight_valueChanged(double)));
    connect(ui->spbTrim, SIGNAL(valueChanged(double)), this, SLOT(on_spbTrim_valueChanged(double)));
}

void BookPropertiesDialog::clear()
{
    m_book = NULL;
}

void BookPropertiesDialog::setDefaults()
{
    // adapt to your needs

    ui->spbCoverPageWidth->setValue(414.15);
    ui->spbCoverPageHeight->setValue(260.35);
    ui->spbPageWidth->setValue(206.37);
    ui->spbPageHeight->setValue(260.35);
    ui->spbTrim->setValue(3.17);
}

void BookPropertiesDialog::on_btnOk_clicked()
{
    accept();
}

void BookPropertiesDialog::on_spbCoverPageWidth_valueChanged(double arg1)
{
    if (m_book == NULL)
        return;

    m_book->setCoverPageWidthMm(arg1);

    emit bookPropertiesChanged();
}

void BookPropertiesDialog::on_spbCoverPageHeight_valueChanged(double arg1)
{
    if (m_book == NULL)
        return;

    m_book->setCoverPageHeightMm(arg1);

    emit bookPropertiesChanged();
}

void BookPropertiesDialog::on_spbPageWidth_valueChanged(double arg1)
{
    if (m_book == NULL)
        return;

    m_book->setPageWidthMm(arg1);

    emit bookPropertiesChanged();
}

void BookPropertiesDialog::on_spbPageHeight_valueChanged(double arg1)
{
    if (m_book == NULL)
        return;

    m_book->setPageHeightMm(arg1);

    emit bookPropertiesChanged();
}

void BookPropertiesDialog::on_spbTrim_valueChanged(double arg1)
{
    if (m_book == NULL)
        return;

    m_book->setTrimMm(arg1);

    emit bookPropertiesChanged();
}
