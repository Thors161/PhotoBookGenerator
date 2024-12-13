#ifndef MYCOVERPAGELAYOUT_H
#define MYCOVERPAGELAYOUT_H

#include "pagelayout.h"

#include <QRectF>
#include <QColor>
#include <QFont>
#include <QDomElement>
#include <QImage>

class QDomDocument;

class MyCoverPageLayout : public PageLayout
{
public:
    MyCoverPageLayout();
    MyCoverPageLayout(const QDomElement& myCoverPageLayoutElement);
    virtual ~MyCoverPageLayout();

    virtual MyCoverPageLayout* clone() const;

    void setTitle(const QString& title);
    const QString& title() const;
    void setTitleFont(const QFont& titleFont);
    const QFont& titleFont() const;
    void setSubTitle(const QString& subTitle);
    const QString& subTitle() const;
    void setSubTitleFont(const QFont& subTitleFont);
    const QFont& subTitleFont() const;
    void setTextColor(const QColor& textColor);
    const QColor& textColor() const;
    void setPattern(const QString& pattern);
    const QString& pattern() const;
    void setPatternWidthMm(double patternWidthMm);
    double patternWidthMm() const;
    void setTitleBarColor(const QColor& titleBarColor);
    const QColor& titleBarColor() const;
    void setTitleBarTopMm(double titleBarTopMm);
    double titleBarTopMm() const;
    void setTitleBarHeightMm(double titleBarHeightMm);
    double titleBarHeightMm() const;
    void setSpineWidthMm(double spineWidthMm);
    double spineWidthMm() const;

    virtual PageLayoutOptionsWidget* createPageLayoutOptionsWidget(QWidget* parent);
    virtual void paint(const Page& page, QPainter& painter, QPaintDevice* device, bool showLayoutLines, bool nativeResolution) const;
    virtual QDomElement xml(QDomDocument* document) const;

private:
    QString m_title;
    QString m_subTitle;
    QString m_pattern;
    double m_patternWidthMm;
    QFont m_titleFont;
    QFont m_subTitleFont;
    QColor m_textColor;
    QColor m_titleBarColor;
    double m_titleBarTopMm;
    double m_titleBarHeightMm;
    double m_spineWidthMm;

    QRectF printableRect() const;
};

#endif // MYCOVERPAGELAYOUT_H
