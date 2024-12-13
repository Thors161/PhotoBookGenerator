#include "mycoverpagepropertieswidget.h"
#include "ui_mycoverpagepropertieswidget.h"

#include <QFontDialog>
#include <QColorDialog>
#include <QFileDialog>

#include "mycoverpagelayout.h"

MyCoverPagePropertiesWidget::MyCoverPagePropertiesWidget(QWidget *parent) :
    PageLayoutOptionsWidget(parent),
    ui(new Ui::MyCoverPagePropertiesWidget)
{
    ui->setupUi(this);
    m_myCoverPageLayout = NULL;
    m_emitChanges = false;
}

MyCoverPagePropertiesWidget::~MyCoverPagePropertiesWidget()
{
    delete ui;
}

QString MyCoverPagePropertiesWidget::layoutName()
{
    return "Cover";
}

void MyCoverPagePropertiesWidget::setMyCoverPageLayout(MyCoverPageLayout* myCoverPageLayout)
{
    m_myCoverPageLayout = myCoverPageLayout;

    m_emitChanges = false;

    ui->editTitle->setText(m_myCoverPageLayout->title());
    ui->edtSubTitle->setText(m_myCoverPageLayout->subTitle());
    ui->spbPatternWidth->setValue(m_myCoverPageLayout->patternWidthMm());
    ui->spbSpineWidth->setValue(m_myCoverPageLayout->spineWidthMm());
    ui->spbBarTop->setValue(m_myCoverPageLayout->titleBarTopMm());
    ui->spbBarHeight->setValue(m_myCoverPageLayout->titleBarHeightMm());

    m_emitChanges = true;
}

void MyCoverPagePropertiesWidget::on_btnTitleFont_clicked()
{
    if (m_myCoverPageLayout == NULL)
        return;

    bool ok = false;
    QFont font = QFontDialog::getFont(&ok, m_myCoverPageLayout->titleFont(), this);
    if (!ok)
        return;

    m_myCoverPageLayout->setTitleFont(font);

    if (m_emitChanges)
        emit pageLayoutOptionsChanged();
}

void MyCoverPagePropertiesWidget::on_editTitle_textChanged(const QString &arg1)
{
    if (m_myCoverPageLayout == NULL)
        return;

    m_myCoverPageLayout->setTitle(arg1);

    if (m_emitChanges)
        emit pageLayoutOptionsChanged();
}

void MyCoverPagePropertiesWidget::on_edtSubTitle_textChanged(const QString &arg1)
{
    if (m_myCoverPageLayout == NULL)
        return;

    m_myCoverPageLayout->setSubTitle(arg1);

    if (m_emitChanges)
        emit pageLayoutOptionsChanged();
}

void MyCoverPagePropertiesWidget::on_btnSubTitleFont_clicked()
{
    if (m_myCoverPageLayout == NULL)
        return;

    bool ok = false;
    QFont font = QFontDialog::getFont(&ok, m_myCoverPageLayout->subTitleFont(), this);
    if (!ok)
        return;

    m_myCoverPageLayout->setSubTitleFont(font);

    if (m_emitChanges)
        emit pageLayoutOptionsChanged();
}

void MyCoverPagePropertiesWidget::on_btnTextColor_clicked()
{
    if (m_myCoverPageLayout == NULL)
        return;

    QColor color = QColorDialog::getColor(m_myCoverPageLayout->textColor());

    if (!color.isValid())
        return;

    m_myCoverPageLayout->setTextColor(color);

    if (m_emitChanges)
        emit pageLayoutOptionsChanged();
}

void MyCoverPagePropertiesWidget::on_btnTitleBarColor_clicked()
{
    if (m_myCoverPageLayout == NULL)
        return;

    QColor color = QColorDialog::getColor(m_myCoverPageLayout->titleBarColor());

    if (!color.isValid())
        return;

    m_myCoverPageLayout->setTitleBarColor(color);

    if (m_emitChanges)
        emit pageLayoutOptionsChanged();
}

void MyCoverPagePropertiesWidget::on_btnPattern_clicked()
{
    if (m_myCoverPageLayout == NULL)
        return;

    QString pattern = QFileDialog::getOpenFileName(this, tr("Select pattern"), m_myCoverPageLayout->pattern(), tr("Image Files (*.png *.jpg *.bmp)"));

    if (pattern.isNull())
        return;

    m_myCoverPageLayout->setPattern(pattern);

    if (m_emitChanges)
        emit pageLayoutOptionsChanged();
}

void MyCoverPagePropertiesWidget::on_spbPatternWidth_valueChanged(double arg1)
{
    if (m_myCoverPageLayout == NULL)
        return;

    m_myCoverPageLayout->setPatternWidthMm(arg1);

    if (m_emitChanges)
        emit pageLayoutOptionsChanged();
}

void MyCoverPagePropertiesWidget::on_spbBarTop_valueChanged(double arg1)
{
    if (m_myCoverPageLayout == NULL)
        return;

    m_myCoverPageLayout->setTitleBarTopMm(arg1);

    if (m_emitChanges)
        emit pageLayoutOptionsChanged();
}

void MyCoverPagePropertiesWidget::on_spbBarHeight_valueChanged(double arg1)
{
    if (m_myCoverPageLayout == NULL)
        return;

    m_myCoverPageLayout->setTitleBarHeightMm(arg1);

    if (m_emitChanges)
        emit pageLayoutOptionsChanged();
}

void MyCoverPagePropertiesWidget::on_spbSpineWidth_valueChanged(double arg1)
{
    if (m_myCoverPageLayout == NULL)
        return;

    m_myCoverPageLayout->setSpineWidthMm(arg1);

    if (m_emitChanges)
        emit pageLayoutOptionsChanged();
}
