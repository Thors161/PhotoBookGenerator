#include "mycoverpagelayout.h"
#include "page.h"
#include <QPaintDevice>
#include <QPainter>
#include <QDomDocument>
#include <QBrush>
#include "mycoverpagepropertieswidget.h"

namespace
{
void drawText(const QString& text, const QFont& font, const QColor& color, const QSizeF& size, int flags, const QTransform& transform, const QTransform& mmToPaintDevice, QPainter& painter, bool showLayoutLines)
{
    painter.save();

    // draw caption

    // as we are printing on multiple devices with different resolutions,
    // the point size of a font should be ok. However, this is in the paint
    // engine automatically done. Is works different for images and fonts.

    // according to qt docs:
    // Note: When rendering text on a QPrinter device, it is important to realize that the size of text, when specified in points, is independent of the resolution specified for the device itself. Therefore, it may be useful to specify the font size in pixels when combining text with graphics to ensure that their relative sizes are what you expect.

    // Therefore the size is pecified in pixels of the device painter on. This is
    // integer precision. The positioning will be different on high and low res paint
    // devices (when pixelSize if font is for example ).
    // Therefore set the size fixed to 100, and scale to the desired size using
    // a transform :). The is 0.3527 value is the pt to mm value.

    QFont sizedFont(font);
    const int fixedFontSizePx = 250;
    sizedFont.setPixelSize(fixedFontSizePx);
    const double fixedFontSizeToMmFactor = font.pointSizeF() * 0.3527 / static_cast<double>(fixedFontSizePx);
    const QTransform fixedFontSizeToMmTransform = QTransform().scale(fixedFontSizeToMmFactor, fixedFontSizeToMmFactor);
    const QTransform mmToFixedFontSizeTransform = QTransform().scale(1/fixedFontSizeToMmFactor, 1/fixedFontSizeToMmFactor);

    QRectF rect = QRectF(QPoint(0, 0), size);

    if (showLayoutLines)
    {
        // draw caption box before transform is set on painter. we want to have line of 1 px.
        // instead transform box
        painter.setPen(QPen(Qt::green, 1, Qt::DashLine));
        painter.drawRect((transform * mmToPaintDevice).mapRect(rect));
    }

    // transform box to undo the effects of the fizedfontsize scaling with the inverse of that.
    rect = mmToFixedFontSizeTransform.mapRect(rect);
    painter.setTransform(fixedFontSizeToMmTransform * transform * mmToPaintDevice);
    painter.setFont(sizedFont);
    painter.setPen(color);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawText(rect, flags, text);

//  uncomment to debug actual transformed box
//    if (showLayoutLines)
//    {
//        // draw caption box
//        painter.setPen(QPen(Qt::red, 50, Qt::DashLine));
//        painter.drawRect(rect);
//    }

    painter.restore();

}
}

MyCoverPageLayout::MyCoverPageLayout() : PageLayout()
{
    m_title = "title";
    m_subTitle = "subtitle";
    m_pattern = "";
    m_patternWidthMm = 50;

    m_titleFont.fromString("URW Palladio L,60,-1,5,50,0,0,0,0,0");
    m_subTitleFont.fromString("URW Palladio L,30,-1,5,50,0,0,0,0,0");
    m_textColor = QColor(40, 40, 40);

    m_titleBarColor = Qt::white;

    m_titleBarTopMm = 50;
    m_titleBarHeightMm = 50;
    m_spineWidthMm = 10;
}

MyCoverPageLayout::MyCoverPageLayout(const QDomElement& myCoverPageLayoutElement)
{
    QDomElement element;

    element = myCoverPageLayoutElement.firstChildElement("title");
    m_title = element.text();

    element = myCoverPageLayoutElement.firstChildElement("subTitle");
    m_subTitle = element.text();

    element = myCoverPageLayoutElement.firstChildElement("pattern");
    m_pattern = element.text();

    element = myCoverPageLayoutElement.firstChildElement("patternWidthMm");
    m_patternWidthMm = element.text().toDouble();

    element = myCoverPageLayoutElement.firstChildElement("titleFont");
    m_titleFont.fromString(element.text());

    element = myCoverPageLayoutElement.firstChildElement("subTitleFont");
    m_subTitleFont.fromString(element.text());

    element = myCoverPageLayoutElement.firstChildElement("textColor");
    m_textColor.setNamedColor(element.text());

    element = myCoverPageLayoutElement.firstChildElement("titleBarColor");
    m_titleBarColor.setNamedColor(element.text());

    element = myCoverPageLayoutElement.firstChildElement("titleBarTopMm");
    m_titleBarTopMm = element.text().toDouble();

    element = myCoverPageLayoutElement.firstChildElement("titleBarHeightMm");
    m_titleBarHeightMm = element.text().toDouble();

    element = myCoverPageLayoutElement.firstChildElement("spineWidthMm");
    m_spineWidthMm = element.text().toDouble();
}

MyCoverPageLayout::~MyCoverPageLayout()
{
}

MyCoverPageLayout* MyCoverPageLayout::clone() const
{
    return new MyCoverPageLayout(*this);
}

void MyCoverPageLayout::setTitle(const QString& title)
{
    m_title = title;
}

const QString& MyCoverPageLayout::title() const
{
    return m_title;
}

void MyCoverPageLayout::setTitleFont(const QFont& titleFont)
{
    m_titleFont = titleFont;
}

const QFont& MyCoverPageLayout::titleFont() const
{
    return m_titleFont;
}

void MyCoverPageLayout::setSubTitle(const QString& subTitle)
{
    m_subTitle = subTitle;
}

const QString& MyCoverPageLayout::subTitle() const
{
    return m_subTitle;
}

void MyCoverPageLayout::setSubTitleFont(const QFont& subTitleFont)
{
    m_subTitleFont = subTitleFont;
}

const QFont& MyCoverPageLayout::subTitleFont() const
{
    return m_subTitleFont;
}

void MyCoverPageLayout::setTextColor(const QColor& textColor)
{
    m_textColor = textColor;
}

const QColor& MyCoverPageLayout::textColor() const
{
    return m_textColor;
}

void MyCoverPageLayout::setPattern(const QString& pattern)
{
    m_pattern = pattern;
}

const QString& MyCoverPageLayout::pattern() const
{
    return m_pattern;
}

void MyCoverPageLayout::setPatternWidthMm(double patternWidthMm)
{
    m_patternWidthMm = patternWidthMm;
}

double MyCoverPageLayout::patternWidthMm() const
{
    return m_patternWidthMm;
}

void MyCoverPageLayout::setTitleBarColor(const QColor& titleBarColor)
{
    m_titleBarColor = titleBarColor;
}

const QColor& MyCoverPageLayout::titleBarColor() const
{
    return m_titleBarColor;
}

void MyCoverPageLayout::setTitleBarTopMm(double titleBarTopMm)
{
    m_titleBarTopMm = titleBarTopMm;
}

double MyCoverPageLayout::titleBarTopMm() const
{
    return m_titleBarTopMm;
}

void MyCoverPageLayout::setTitleBarHeightMm(double titleBarHeightMm)
{
    m_titleBarHeightMm = titleBarHeightMm;
}

double MyCoverPageLayout::titleBarHeightMm() const
{
    return m_titleBarHeightMm;
}

void MyCoverPageLayout::setSpineWidthMm(double spineWidthMm)
{
    m_spineWidthMm = spineWidthMm;
}

double MyCoverPageLayout::spineWidthMm() const
{
    return m_spineWidthMm;
}

PageLayoutOptionsWidget* MyCoverPageLayout::createPageLayoutOptionsWidget(QWidget* parent)
{
    MyCoverPagePropertiesWidget* myCoverPagePropertiesWidget = new MyCoverPagePropertiesWidget(parent);
    myCoverPagePropertiesWidget->setMyCoverPageLayout(this);
    return myCoverPagePropertiesWidget;
}

void MyCoverPageLayout::paint(const Page& page, QPainter& painter, QPaintDevice* device, bool showLayoutLines, bool nativeResolution) const
{
    QTransform mmToPaintDevice = page.transformMmToPaintDevice(device);

    QImage pattern;
    if (m_pattern.isEmpty())
        pattern = QImage(":/photoBookGenerator/resources/ruit.png");
    else
        pattern = QImage(m_pattern);

    QRect rect(0, 0, m_patternWidthMm, m_patternWidthMm * pattern.height() / pattern.width());
    QRect deviceRect = mmToPaintDevice.mapRect(rect);
    QImage devicePattern = pattern.scaled(deviceRect.width(), deviceRect.height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

    QBrush brush;
    brush.setTexture(QPixmap::fromImage(devicePattern));

    painter.fillRect(QRect(0, 0, device->width(), device->height()), brush);

    brush = QBrush(m_titleBarColor);

    QRectF bar(0, m_titleBarTopMm, page.widthMm(), m_titleBarHeightMm);

    QPen pen(Qt::NoPen);

    painter.setPen(pen);
    painter.setBrush(brush);
    painter.drawRect(mmToPaintDevice.mapRect(bar));

    // spine
    QFont spineFont(m_titleFont);
    spineFont.setPointSize(10);

    QRectF spineTitleRectMm((page.widthMm() * 0.5) + (m_spineWidthMm * 0.5), m_titleBarTopMm, m_titleBarHeightMm, m_spineWidthMm);
    QTransform transform = QTransform().rotate(90) * QTransform().translate(spineTitleRectMm.left(), spineTitleRectMm.top());

    drawText(m_title, spineFont, m_textColor, spineTitleRectMm.size(), Qt::AlignCenter, transform, mmToPaintDevice, painter, showLayoutLines);

    // title
    double titleBoxLeft = (page.widthMm() * 0.5) + (m_spineWidthMm * 0.5);
    double titleBoxRight = (page.widthMm() - page.trimMm());
    double titleBoxTop = m_titleBarTopMm + (m_titleBarHeightMm * 0.1);
    double titleBoxBottom = titleBoxTop + (m_titleBarHeightMm * 0.5);
    QRectF titleRectMm(titleBoxLeft, titleBoxTop, titleBoxRight - titleBoxLeft, titleBoxBottom - titleBoxTop);


    transform = QTransform().translate(titleRectMm.left(), titleRectMm.top());
    drawText(m_title, m_titleFont, m_textColor, titleRectMm.size(), Qt::AlignCenter, transform, mmToPaintDevice, painter, showLayoutLines);

    // subtitle
    QRectF subTitleRectMm(titleBoxLeft, titleBoxBottom + (m_titleBarHeightMm * 0.1), titleBoxRight - titleBoxLeft, m_titleBarHeightMm * 0.2);

    transform = QTransform().translate(subTitleRectMm.left(), subTitleRectMm.top());
    drawText(m_subTitle, m_subTitleFont, m_textColor, subTitleRectMm.size(), Qt::AlignCenter, transform, mmToPaintDevice, painter, showLayoutLines);


}

QDomElement MyCoverPageLayout::xml(QDomDocument* document) const
{
    Q_CHECK_PTR(document);

    QDomElement myCoverPageLayoutElement = document->createElement("myCoverPageLayout");
    QDomElement element;
    QDomText text;

    element = document->createElement("title");
    text = document->createTextNode(m_title);
    element.appendChild(text);
    myCoverPageLayoutElement.appendChild(element);

    element = document->createElement("subTitle");
    text = document->createTextNode(m_subTitle);
    element.appendChild(text);
    myCoverPageLayoutElement.appendChild(element);

    element = document->createElement("pattern");
    text = document->createTextNode(m_pattern);
    element.appendChild(text);
    myCoverPageLayoutElement.appendChild(element);

    element = document->createElement("patternWidthMm");
    text = document->createTextNode(QString::number(m_patternWidthMm));
    element.appendChild(text);
    myCoverPageLayoutElement.appendChild(element);

    element = document->createElement("titleFont");
    text = document->createTextNode(m_titleFont.toString());
    element.appendChild(text);
    myCoverPageLayoutElement.appendChild(element);

    element = document->createElement("subTitleFont");
    text = document->createTextNode(m_subTitleFont.toString());
    element.appendChild(text);
    myCoverPageLayoutElement.appendChild(element);

    element = document->createElement("textColor");
    text = document->createTextNode(m_textColor.name());
    element.appendChild(text);
    myCoverPageLayoutElement.appendChild(element);

    element = document->createElement("titleBarColor");
    text = document->createTextNode(m_titleBarColor.name());
    element.appendChild(text);
    myCoverPageLayoutElement.appendChild(element);

    element = document->createElement("titleBarTopMm");
    text = document->createTextNode(QString::number(m_titleBarTopMm));
    element.appendChild(text);
    myCoverPageLayoutElement.appendChild(element);

    element = document->createElement("titleBarHeightMm");
    text = document->createTextNode(QString::number(m_titleBarHeightMm));
    element.appendChild(text);
    myCoverPageLayoutElement.appendChild(element);

    element = document->createElement("spineWidthMm");
    text = document->createTextNode(QString::number(m_spineWidthMm));
    element.appendChild(text);
    myCoverPageLayoutElement.appendChild(element);

    return myCoverPageLayoutElement;
}
