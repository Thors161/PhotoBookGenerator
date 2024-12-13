#include "mypagelayoutoptionswidget.h"
#include "ui_mypagelayoutoptionswidget.h"

#include <QFontDialog>
#include <QColorDialog>

#include "mypagelayout.h"

MyPageLayoutOptionsWidget::MyPageLayoutOptionsWidget(QWidget *parent) :
    PageLayoutOptionsWidget(parent),
    ui(new Ui::MyPageLayoutOptionsWidget)
{
    ui->setupUi(this);
    m_myPageLayout = NULL;
    m_emitChanges = false;
}

MyPageLayoutOptionsWidget::~MyPageLayoutOptionsWidget()
{
    delete ui;
}

QString MyPageLayoutOptionsWidget::layoutName()
{
    return "Pages";
}

void MyPageLayoutOptionsWidget::setMyPageLayout(MyPageLayout* myPageLayout)
{
    m_myPageLayout = myPageLayout;

    m_emitChanges = false;

    ui->gutterSpb->setValue(m_myPageLayout->gutterMm());
    ui->imageSpacingSpb->setValue(m_myPageLayout->spacingMm());
    ui->imageCroppingChk->setChecked(m_myPageLayout->cropping());
    ui->captionPageMarginSpb->setValue(m_myPageLayout->captionPageMarginMm());
    ui->captionImageMarginSpb->setValue(m_myPageLayout->captionImageMarginMm());
    ui->captionMinWidthSpb->setValue(m_myPageLayout->captionMinHorSpaceMm());

    m_emitChanges = true;
}

void MyPageLayoutOptionsWidget::on_gutterSpb_valueChanged(double arg1)
{
    if (m_myPageLayout == NULL)
        return;

    m_myPageLayout->setGutterMm(arg1);

    if (m_emitChanges)
        emit pageLayoutOptionsChanged();
}

void MyPageLayoutOptionsWidget::on_imageSpacingSpb_valueChanged(double arg1)
{
    if (m_myPageLayout == NULL)
        return;

    m_myPageLayout->setSpacingMm(arg1);

    if (m_emitChanges)
        emit pageLayoutOptionsChanged();
}

void MyPageLayoutOptionsWidget::on_imageCroppingChk_stateChanged(int arg1)
{
    if (m_myPageLayout == NULL)
        return;

    if (arg1 == Qt::Checked)
        m_myPageLayout->setCropping(true);
    else if (arg1 == Qt::Unchecked)
        m_myPageLayout->setCropping(false);

    if (m_emitChanges)
        emit pageLayoutOptionsChanged();
}

void MyPageLayoutOptionsWidget::on_captionPageMarginSpb_valueChanged(double arg1)
{
    if (m_myPageLayout == NULL)
        return;

    m_myPageLayout->setCaptionPageMarginMm(arg1);

    if (m_emitChanges)
        emit pageLayoutOptionsChanged();
}

void MyPageLayoutOptionsWidget::on_captionImageMarginSpb_valueChanged(double arg1)
{
    if (m_myPageLayout == NULL)
        return;

    m_myPageLayout->setCaptionImageMarginMm(arg1);

    if (m_emitChanges)
        emit pageLayoutOptionsChanged();
}

void MyPageLayoutOptionsWidget::on_captionMinWidthSpb_valueChanged(double arg1)
{
    if (m_myPageLayout == NULL)
        return;

    m_myPageLayout->setCaptionMinHorSpaceMm(arg1);

    if (m_emitChanges)
        emit pageLayoutOptionsChanged();
}

void MyPageLayoutOptionsWidget::on_captionFontBut_clicked()
{
    if (m_myPageLayout == NULL)
        return;

    bool ok;
    QFont font = QFontDialog::getFont(
                &ok, m_myPageLayout->captionFont(), this);
    if (ok) {
        m_myPageLayout->setCaptionFont(font.toString());
    }

    if (m_emitChanges)
        emit pageLayoutOptionsChanged();
}

void MyPageLayoutOptionsWidget::on_bgColorBut_clicked()
{
    if (m_myPageLayout == NULL)
        return;

    QColor color = QColorDialog::getColor(m_myPageLayout->bgColor());

    if (!color.isValid())
        return;

    m_myPageLayout->setBgColor(color);

    if (m_emitChanges)
        emit pageLayoutOptionsChanged();
}

void MyPageLayoutOptionsWidget::on_captionColorBut_clicked()
{
    if (m_myPageLayout == NULL)
        return;

    QColor color = QColorDialog::getColor(m_myPageLayout->captionColor());

    if (!color.isValid())
        return;

    m_myPageLayout->setCaptionColor(color);

    if (m_emitChanges)
        emit pageLayoutOptionsChanged();
}
