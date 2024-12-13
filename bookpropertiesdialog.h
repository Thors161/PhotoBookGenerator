#ifndef BOOKPROPERTIESDIALOG_H
#define BOOKPROPERTIESDIALOG_H

#include <QDialog>

class Book;

namespace Ui {
class BookPropertiesDialog;
}

class BookPropertiesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BookPropertiesDialog(QWidget *parent = 0);
    ~BookPropertiesDialog();

    void setShowOk(bool showOk);

    void initialize(Book* book);
    void clear();
    void setDefaults();

signals:
    void bookPropertiesChanged();

private slots:
    void on_btnOk_clicked();

    void on_spbCoverPageWidth_valueChanged(double arg1);

    void on_spbCoverPageHeight_valueChanged(double arg1);

    void on_spbPageWidth_valueChanged(double arg1);

    void on_spbPageHeight_valueChanged(double arg1);

    void on_spbTrim_valueChanged(double arg1);

private:
    Ui::BookPropertiesDialog *ui;

    Book* m_book;
};

#endif // BOOKPROPERTIESDIALOG_H
