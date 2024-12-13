#ifndef MYPAGELAYOUT_H
#define MYPAGELAYOUT_H

#include "pagelayout.h"

#include <QRectF>
#include <QColor>
#include <QFont>
#include <QDomElement>

class QDomDocument;

class MyPageLayout : public PageLayout
{
public:
    MyPageLayout();
    MyPageLayout(const QDomElement& myPageLayoutElement);
    virtual ~MyPageLayout();

    virtual MyPageLayout* clone() const;

    void setBgColor(const QColor& bgColor);
    const QColor& bgColor() const;
    void setCropping(bool cropping);
    bool cropping() const;
    void setGutterMm(double gutterMm);
    double gutterMm() const;
    void setSpacingMm(double spacingMm);
    double spacingMm() const;
    void setCaptionPageMarginMm(double captionPageMarginMm);
    double captionPageMarginMm() const;
    void setCaptionImageMarginMm(double captionImageMarginMm);
    double captionImageMarginMm() const;
    void setCaptionFont(const QString& captionFont);
    const QFont& captionFont() const;
    void setCaptionColor(const QColor& captionColor);
    const QColor& captionColor() const;
    void setCaptionMinHorSpaceMm(double captionMinHorSpaceMm);
    double captionMinHorSpaceMm() const;

    virtual PageLayoutOptionsWidget* createPageLayoutOptionsWidget(QWidget* parent);
    virtual void paint(const Page& page, QPainter& painter, QPaintDevice* device, bool showLayoutLines, bool nativeResolution) const;
    virtual QDomElement xml(QDomDocument* document) const;

private:
    void paintOneImage(const Page& page, QPainter& painter, QPaintDevice* device, bool showLayoutLines, bool nativeResolution) const;
    void paintTwoImages(const Page& page, QPainter& painter, QPaintDevice* device, bool showLayoutLines, bool nativeResolution) const;
    void paintThreeImages(const Page& page, QPainter& painter, QPaintDevice* device, bool showLayoutLines, bool nativeResolution) const;

    QRectF printableRect() const;

    QColor m_bgColor;
    bool m_cropping;
    double m_gutterMm;
    double m_spacingMm;
    double m_captionPageMarginMm;
    double m_captionImageMarginMm;
    QFont m_captionFont;
    QColor m_captionColor;
    double m_captionMinHorSpaceMm;
};

#endif // MYPAGELAYOUT_H
