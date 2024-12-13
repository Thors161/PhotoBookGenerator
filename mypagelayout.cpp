#include "mypagelayout.h"
#include "page.h"
#include <QPaintDevice>
#include <QPainter>
#include <QDomDocument>
#include "mypagelayoutoptionswidget.h"

namespace
{
bool leftPage(const Page& page)
{
    return (page.pageSide() == Page::LeftPage);
}

bool rightPage(const Page& page)
{
    return (page.pageSide() == Page::RightPage);
}

bool isLandscape(Image const* image)
{
    return !image->portait();
}

void drawCaption(const QString& text, const QFont& font, const QColor& color, const QSizeF& size, int flags, const QTransform& transform, const QTransform& mmToPaintDevice, QPainter& painter, bool showLayoutLines)
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

void drawCaptionIndicator(const QPainterPath& indicatorPath, const QColor& color, const QTransform& transform, QPainter& painter, bool showLayoutLines)
{
    if (indicatorPath.isEmpty())
        return;

    painter.save();
    if (showLayoutLines)
    {
        // draw caption box
        painter.setPen(QPen(Qt::green, 1, Qt::DashLine));
        painter.drawRect(transform.mapRect(QRectF(0, 0, 1, 1)));
    }
    // draw caption
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setTransform(transform);
    painter.setBrush(color);
    painter.setPen(Qt::NoPen);
    painter.drawPath(indicatorPath);
    painter.restore();
}


// combine caption text and date
QString caption(Image const* image)
{
    QString caption = image->caption();

    if (image->showDate() && image->dateTime().isValid())
    {
        if (!caption.isEmpty())
            caption.append(" ");

        caption.append(image->dateTime().toString("(dd-MM-yyyy)"));
    }

    return caption;
}

QPainterPath singleArrowUp()
{
    QPainterPath path;
    path.moveTo(0.25, 0.875);
    path.lineTo(0.5, 0.5);
    path.lineTo(0.75, 0.875);
    path.lineTo(0.25, 0.875);
    return path;
}

QPainterPath doubleArrowUp()
{
    QPainterPath path;
    path.moveTo(0.25, 0.5);
    path.lineTo(0.5, 0.125);
    path.lineTo(0.75, 0.5);
    path.lineTo(0.25, 0.5);
    path.moveTo(0.25, 0.875);
    path.lineTo(0.5, 0.5);
    path.lineTo(0.75, 0.875);
    path.lineTo(0.25, 0.875);
    return path;
}

// return the proper ratio to make time images the same width, based on image ratio's independent of image resolution.
// the returned width of scaled2 is the same s1.
void sameWidth(const QSizeF& s1, const QSizeF& s2, QSizeF* scaled2)
{
    Q_CHECK_PTR(scaled2);

    const double widthRatio = s1.width() / s2.width();

    scaled2->setWidth(s1.width());
    scaled2->setHeight(s2.height() * widthRatio);
}

void sameWidth(const QSizeF& s1, const QSizeF& s2, const QSizeF& s3, QSizeF* scaled2, QSizeF* scaled3)
{
    sameWidth(s1, s2, scaled2);
    sameWidth(s1, s3, scaled3);
}

// return the proper ratio to make time images the same height, based on image ratio's independent of image resolution.
// the returned height of scaled2 is the same s1.
void sameHeight(const QSizeF& s1, const QSizeF& s2, QSizeF* scaled2)
{
    Q_CHECK_PTR(scaled2);

    const double heightRatio = s1.height() / s2.height();

    scaled2->setWidth(s2.width() * heightRatio);
    scaled2->setHeight(s2.height());
}

void sameHeight(const QSizeF& s1, const QSizeF& s2, const QSizeF& s3, QSizeF* scaled2, QSizeF* scaled3)
{
    sameHeight(s1, s2, scaled2);
    sameHeight(s1, s3, scaled3);
}


} // namespace

// My own page layout, aimed at blurb book 200x250
// gutter blank space for captions (written later in the book and printed)
// default 3 images high (landscape images)
// in case one image:
//  1x landscape image: remove gutter, blank top/down 1/3 2/3 blank space
//  1x portrait image: use gutter
// in case two images:
//  2x landscape image: default gutter, crop images
//  2x portrait image: prohibited
//  1x portrait, 1x landscape image: use defautl gutter, crop remainder (order as in list)
// in case three images:
//  3x landscape: use default gutter, crop remainder
//  1x landscape, 2x portrait: use default gutter crop remainder
//     landscape top, portrait bottom if landscape first. if landscape last: bottom. If landscape middle: bottom.
//  2x landscape, 1x portrait: prohibited
//  3x portrait: prohibited

MyPageLayout::MyPageLayout() : PageLayout()
{
    m_cropping = true;
    m_gutterMm = 0;
    m_spacingMm = 0;
    m_captionPageMarginMm = 5;
    m_captionImageMarginMm = 2.5;
    m_captionFont.fromString("URW Palladio L,3,-1,5,50,0,0,0,0,0"); // we cannot use fontmetrics as we specified the size in mm :( (int for ascent and descend is not precie enough)
    m_captionColor = QColor(40, 40, 40);
    m_captionMinHorSpaceMm = 40;
}

MyPageLayout::MyPageLayout(const QDomElement& myPageLayoutElement)
{
    QDomElement element;

    element = myPageLayoutElement.firstChildElement("bgColor");
    m_bgColor.setNamedColor(element.text());

    element = myPageLayoutElement.firstChildElement("cropping");
    m_cropping = element.text() == "true";

    element = myPageLayoutElement.firstChildElement("gutterMm");
    m_gutterMm = element.text().toDouble();

    element = myPageLayoutElement.firstChildElement("spacingMm");
    m_spacingMm = element.text().toDouble();

    element = myPageLayoutElement.firstChildElement("captionPageMarginMm");
    m_captionPageMarginMm = element.text().toDouble();

    element = myPageLayoutElement.firstChildElement("captionImageMarginMm");
    m_captionImageMarginMm = element.text().toDouble();

    element = myPageLayoutElement.firstChildElement("captionFont");
    m_captionFont.fromString(element.text());

    element = myPageLayoutElement.firstChildElement("captionColor");
    m_captionColor.setNamedColor(element.text());

    element = myPageLayoutElement.firstChildElement("captionMinHorSpaceMm");
    m_captionMinHorSpaceMm = element.text().toDouble();
}

MyPageLayout::~MyPageLayout()
{
}

MyPageLayout* MyPageLayout::clone() const
{
    return new MyPageLayout(*this);
}

void MyPageLayout::setBgColor(const QColor& bgColor)
{
    m_bgColor = bgColor;
}

const QColor& MyPageLayout::bgColor() const
{
    return m_bgColor;
}

void MyPageLayout::setCropping(bool cropping)
{
    m_cropping = cropping;
}

bool MyPageLayout::cropping() const
{
    return m_cropping;
}

void MyPageLayout::setGutterMm(double gutterMm)
{
    m_gutterMm = gutterMm;
}

double MyPageLayout::gutterMm() const
{
    return m_gutterMm;
}

void MyPageLayout::setSpacingMm(double spacingMm)
{
    m_spacingMm = spacingMm;
}

double MyPageLayout::spacingMm() const
{
    return m_spacingMm;
}

void MyPageLayout::setCaptionPageMarginMm(double captionPageMarginMm)
{
    m_captionPageMarginMm = captionPageMarginMm;
}

double MyPageLayout::captionPageMarginMm() const
{
    return m_captionPageMarginMm;
}

void MyPageLayout::setCaptionImageMarginMm(double captionImageMarginMm)
{
    m_captionImageMarginMm = captionImageMarginMm;
}

double MyPageLayout::captionImageMarginMm() const
{
    return m_captionImageMarginMm;
}

void MyPageLayout::setCaptionFont(const QString& captionFont)
{
    m_captionFont.fromString(captionFont);
}

const QFont& MyPageLayout::captionFont() const
{
    return m_captionFont;
}

void MyPageLayout::setCaptionColor(const QColor& captionColor)
{
    m_captionColor = captionColor;
}

const QColor& MyPageLayout::captionColor() const
{
    return m_captionColor;
}

void MyPageLayout::setCaptionMinHorSpaceMm(double captionMinHorSpaceMm)
{
    m_captionMinHorSpaceMm = captionMinHorSpaceMm;
}

double MyPageLayout::captionMinHorSpaceMm() const
{
    return m_captionMinHorSpaceMm;
}

PageLayoutOptionsWidget* MyPageLayout::createPageLayoutOptionsWidget(QWidget* parent)
{
    MyPageLayoutOptionsWidget* myLayoutOptionsWidget = new MyPageLayoutOptionsWidget(parent);
    myLayoutOptionsWidget->setMyPageLayout(this);
    return myLayoutOptionsWidget;
}

void MyPageLayout::paint(const Page& page, QPainter& painter, QPaintDevice* device, bool showLayoutLines, bool nativeResolution) const
{
    painter.fillRect(QRect(0, 0, device->width(), device->height()), m_bgColor);

    if (page.images().size() == 1)
        paintOneImage(page, painter, device, showLayoutLines, nativeResolution);
    if (page.images().size() == 2)
        paintTwoImages(page, painter, device, showLayoutLines, nativeResolution);
    if (page.images().size() == 3)
        paintThreeImages(page, painter, device, showLayoutLines, nativeResolution);
}

QDomElement MyPageLayout::xml(QDomDocument* document) const
{
    Q_CHECK_PTR(document);

    QDomElement myPageLayoutElement = document->createElement("myPageLayout");
    QDomElement element;
    QDomText text;

    element = document->createElement("bgColor");
    text = document->createTextNode(m_bgColor.name());
    element.appendChild(text);
    myPageLayoutElement.appendChild(element);

    element = document->createElement("cropping");
    text = document->createTextNode(m_cropping?"true":"false");
    element.appendChild(text);
    myPageLayoutElement.appendChild(element);

    element = document->createElement("gutterMm");
    text = document->createTextNode(QString::number(m_gutterMm));
    element.appendChild(text);
    myPageLayoutElement.appendChild(element);

    element = document->createElement("spacingMm");
    text = document->createTextNode(QString::number(m_spacingMm));
    element.appendChild(text);
    myPageLayoutElement.appendChild(element);

    element = document->createElement("captionPageMarginMm");
    text = document->createTextNode(QString::number(m_captionPageMarginMm));
    element.appendChild(text);
    myPageLayoutElement.appendChild(element);

    element = document->createElement("captionImageMarginMm");
    text = document->createTextNode(QString::number(m_captionImageMarginMm));
    element.appendChild(text);
    myPageLayoutElement.appendChild(element);

    element = document->createElement("captionFont");
    text = document->createTextNode(m_captionFont.toString());
    element.appendChild(text);
    myPageLayoutElement.appendChild(element);

    element = document->createElement("captionMinHorSpaceMm");
    text = document->createTextNode(QString::number(m_captionMinHorSpaceMm));
    element.appendChild(text);
    myPageLayoutElement.appendChild(element);

    element = document->createElement("captionColor");
    text = document->createTextNode(m_captionColor.name());
    element.appendChild(text);
    myPageLayoutElement.appendChild(element);

    return myPageLayoutElement;
}

// in case one image:
//  1x landscape image: remove gutter, blank top/down 1/3 2/3 blank space
//  1x portrait image: use gutter
void MyPageLayout::paintOneImage(const Page& page, QPainter& painter, QPaintDevice* device, bool showLayoutLines, bool nativeResolution) const
{
    Q_ASSERT(page.images().size() == 1);

    QRectF imageRect;
    QSizeF captionSize;
    QTransform captionTransform;
    int captionFlags = 0;

    Image const* image =  page.images().at(0);
    if (isLandscape(image))
    {
        // landscape
        const double imageHeightMm = page.widthMm() / image->ratio();
        imageRect = QRectF(0, 0.333 * (page.heightMm() - imageHeightMm), page.widthMm(), imageHeightMm);

        //caption
        captionSize = QSizeF(page.widthMm() - (m_gutterMm + page.trimMm() + m_captionPageMarginMm), page.heightMm() - (imageRect.bottom() + page.trimMm() + m_captionPageMarginMm + m_captionImageMarginMm));
        captionTransform = QTransform().translate(leftPage(page)?(page.trimMm() + m_captionPageMarginMm):m_gutterMm, imageRect.bottom() + m_captionImageMarginMm);

        captionFlags = Qt::TextWordWrap | Qt::AlignTop;
        if (leftPage(page))
            captionFlags |= Qt::AlignLeft;
        else if (rightPage(page))
            captionFlags |= Qt::AlignRight;
    }
    else
    {
        // portrait
        imageRect = QRectF(leftPage(page)?0:m_gutterMm, 0, page.widthMm() - m_gutterMm, page.heightMm());

        // captions
        // captions are always drawn horizontal next to the image. If the whitespace is too small
        // whiteSpaceMm < (m_captionMinHorSpaceMm + m_gutterMm), then the gutter is reduced to the 2 *captionPageMargin.
        const double whiteSpaceMm = page.widthMm() - imageRect.width();
        double adjustedGutterMm = 2 * m_captionPageMarginMm;

        if (whiteSpaceMm < (m_captionImageMarginMm + adjustedGutterMm))
        {
            // cannot do.
        }
        else
        {
            captionSize = QSizeF(page.heightMm() - (page.trimMm() + m_captionPageMarginMm + page.trimMm() + m_captionPageMarginMm), page.widthMm() - (imageRect.width() + m_captionPageMarginMm + m_captionImageMarginMm));

            if (leftPage(page))
                captionTransform = QTransform().rotate(-90) * QTransform().translate(imageRect.right() + m_captionImageMarginMm , page.heightMm() - (page.trimMm() + m_captionPageMarginMm));
            else if (rightPage(page))
                captionTransform = QTransform().rotate(90) * QTransform().translate(imageRect.left() - m_captionImageMarginMm , page.trimMm() + m_captionPageMarginMm);

            captionFlags = Qt::TextWordWrap | Qt::AlignLeft | Qt::AlignTop;
        }
    }

    drawImageMaintainingRatio(m_cropping, page.scaleToPaintDevice(imageRect, device), image, painter, nativeResolution);
    drawCaption(caption(image), m_captionFont, m_captionColor, captionSize, captionFlags, captionTransform, page.transformMmToPaintDevice(device), painter, showLayoutLines);
}

// in case two images:
//  2x landscape image: default gutter, crop images
//  2x portrait image: prohibited
//  1x portrait, 1x landscape image: use defautl gutter, crop remainder (order as in list)
void MyPageLayout::paintTwoImages(const Page& page, QPainter& painter, QPaintDevice* device, bool showLayoutLines, bool nativeResolution) const
{
    Q_ASSERT(page.images().size() == 2);

    QRectF imageRect1;
    QRectF imageRect2;

    Image const* image1 =  page.images().at(0);
    Image const* image2 =  page.images().at(1);

    // make same width
    QSizeF scaled2;
    sameWidth(image1->size(), image2->size(), &scaled2);
    const double image1ScaledHeight = image1->size().height();
    const double image2ScaledHeight = scaled2.height();

    const double image1HeightMm = (page.heightMm() - m_spacingMm) * (image1ScaledHeight / (image1ScaledHeight + image2ScaledHeight));
    const double image2HeightMm = (page.heightMm() - m_spacingMm) * (image2ScaledHeight / (image1ScaledHeight + image2ScaledHeight));

    const double normalImage1WidthMm = image1HeightMm * image1->ratio();
    const double maxImageWidthMm = page.widthMm() - m_gutterMm;

    // respect gutter
    const double imageWidthMm = qMin(normalImage1WidthMm, maxImageWidthMm);

    imageRect1 = QRectF(leftPage(page)?0:(page.widthMm() - imageWidthMm), 0, imageWidthMm, image1HeightMm);
    imageRect2 = QRectF(imageRect1.left(), imageRect1.bottom() + m_spacingMm, imageWidthMm, image2HeightMm);

    drawImageMaintainingRatio(m_cropping, page.scaleToPaintDevice(imageRect1, device), image1, painter, nativeResolution);
    drawImageMaintainingRatio(m_cropping, page.scaleToPaintDevice(imageRect2, device), image2, painter, nativeResolution);

    // captions
    QSizeF captionSize1;
    QSizeF captionSize2;
    QTransform captionTransform1;
    QTransform captionTransform2;
    int captionFlags1 = 0;
    int captionFlags2 = 0;

    // captions are always drawn horizontal next to the image. If the whitespace is too small
    // whiteSpaceMm < (m_captionMinHorSpaceMm + m_gutterMm), then the gutter is reduced to the 2 *captionPageMargin.
    double captionWidth = page.widthMm() - (m_gutterMm + m_captionImageMarginMm + imageWidthMm);

    if (captionWidth < m_captionMinHorSpaceMm)
    {
        // vertical

        // reduce gutter to 2 * m_captionPageMarginMm
        const double adjustedGutterMm = (2 * m_captionPageMarginMm);
        captionWidth = page.widthMm() - (adjustedGutterMm + m_captionImageMarginMm + imageWidthMm);

        if (captionWidth <= 0)
        {
            // cannot do.
        }
        else
        {
            //caption1
            captionSize1 = QSizeF(imageRect1.height() - (page.trimMm() + m_captionPageMarginMm + m_captionImageMarginMm), captionWidth);

            if (leftPage(page))
                captionTransform1 = QTransform().rotate(-90) * QTransform().translate(imageRect1.right() + m_captionImageMarginMm, imageRect1.bottom() - m_captionImageMarginMm);
            else if (rightPage(page))
                captionTransform1 = QTransform().rotate(90) * QTransform().translate(imageRect1.left() - m_captionImageMarginMm, page.trimMm() + m_captionPageMarginMm);

            captionFlags1 = Qt::TextWordWrap | Qt::AlignLeft | Qt::AlignTop;

            //caption2
            captionSize2 = QSizeF(imageRect2.height() - (page.trimMm() + m_captionPageMarginMm + m_captionImageMarginMm), captionWidth);

            if (leftPage(page))
                captionTransform2 = QTransform().rotate(-90) * QTransform().translate(imageRect2.right() + m_captionImageMarginMm, imageRect2.bottom() - (page.trimMm() + m_captionPageMarginMm));
            else if (rightPage(page))
                captionTransform2 = QTransform().rotate(90) * QTransform().translate(imageRect2.left() - m_captionImageMarginMm, imageRect2.top() + m_captionImageMarginMm);

            captionFlags2 = Qt::TextWordWrap | Qt::AlignLeft | Qt::AlignTop;
        }
    }
    else
    {
        // horizontal

        if (captionWidth <= 0)
        {
            // cannot do.
        }
        else
        {
            captionSize1 = QSizeF(captionWidth, imageRect1.height() - (m_captionImageMarginMm + m_captionPageMarginMm + page.trimMm()));
            captionSize2 = QSizeF(captionWidth, imageRect2.height() - (m_captionImageMarginMm + m_captionPageMarginMm + page.trimMm()));

            captionTransform1 = QTransform().translate(leftPage(page)?(imageRect1.width() + m_captionImageMarginMm):m_gutterMm, imageRect1.top() + page.trimMm() + m_captionPageMarginMm);
            captionTransform2 = QTransform().translate(leftPage(page)?(imageRect2.width() + m_captionImageMarginMm):m_gutterMm, imageRect2.top() + m_captionImageMarginMm);

            captionFlags1 = Qt::TextWordWrap | Qt::AlignTop;
            if (leftPage(page))
                captionFlags1 |= Qt::AlignLeft;
            else if (rightPage(page))
                captionFlags1 |= Qt::AlignRight;

            captionFlags2 = captionFlags1;
        }
    }

    drawCaption(caption(image1), m_captionFont, m_captionColor, captionSize1, captionFlags1, captionTransform1, page.transformMmToPaintDevice(device), painter, showLayoutLines);
    drawCaption(caption(image2), m_captionFont, m_captionColor, captionSize2, captionFlags2, captionTransform2, page.transformMmToPaintDevice(device), painter, showLayoutLines);
}

//  3x landscape: scale to fit on side, respect gutter as min
//  1x landscape, 2x portrait: use default gutter crop remainder
//     landscape top, portrait bottom if landscape first. if landscape last: bottom. If landscape middle: bottom.
//  2x landscape, 1x portrait: prohibited
//  3x portrait: prohibited
void MyPageLayout::paintThreeImages(const Page& page, QPainter& painter, QPaintDevice* device, bool showLayoutLines, bool nativeResolution) const
{
    Q_ASSERT(page.images().size() == 3);

    QRectF imageRect1;
    QRectF imageRect2;
    QRectF imageRect3;

    QSizeF captionSize1;
    QSizeF captionSize2;
    QSizeF captionSize3;
    QTransform captionTransform1;
    QTransform captionTransform2;
    QTransform captionTransform3;
    int captionFlags1 = 0;
    int captionFlags2 = 0;
    int captionFlags3 = 0;
    QPainterPath captionIndicator1;
    QPainterPath captionIndicator2;
    QPainterPath captionIndicator3;
    QTransform captionIndicatorTransform1;
    QTransform captionIndicatorTransform2;
    QTransform captionIndicatorTransform3;
    const double captionIndicatorSizeMm = QFontMetrics(m_captionFont).ascent() * 0.3527 * 0.85;

    Image const* image1 = page.images().at(0);
    Image const* image2 = page.images().at(1);
    Image const* image3 = page.images().at(2);

    if (isLandscape(image1) && isLandscape(image2) && isLandscape(image3))
    {
        // make same width
        QSizeF scaled2, scaled3;
        sameWidth(image1->size(), image2->size(), image3->size(), &scaled2, &scaled3);
        const double image1ScaledHeight = image1->size().height();
        const double image2ScaledHeight = scaled2.height();
        const double image3ScaledHeight = scaled3.height();

        const double image1HeightMm = (page.heightMm() - (2 * m_spacingMm)) * (image1ScaledHeight / (image1ScaledHeight + image2ScaledHeight + image3ScaledHeight));
        const double image2HeightMm = (page.heightMm() - (2 * m_spacingMm)) * (image2ScaledHeight / (image1ScaledHeight + image2ScaledHeight + image3ScaledHeight));
        const double image3HeightMm = (page.heightMm() - (2 * m_spacingMm)) * (image3ScaledHeight / (image1ScaledHeight + image2ScaledHeight + image3ScaledHeight));

        // tweak image ratio for better coverage! (when three 4/3 images on page, big gutter...)
        double image1Ratio = image1->ratio();
        if (image1Ratio < 1.5) // 3/2 make settable?
            image1Ratio = 1.5;

        const double normalImage1WidthMm = image1HeightMm * image1Ratio;
        const double maxImageWidthMm = page.widthMm() - m_gutterMm;

        // respect gutter
        const double imageWidthMm = qMin(normalImage1WidthMm, maxImageWidthMm);

        imageRect1 = QRectF(leftPage(page)?0:(page.widthMm() - imageWidthMm), 0, imageWidthMm, image1HeightMm);
        imageRect2 = QRectF(imageRect1.left(), imageRect1.bottom() + m_spacingMm, imageWidthMm, image2HeightMm);
        imageRect3 = QRectF(imageRect1.left(), imageRect2.bottom() + m_spacingMm, imageWidthMm, image3HeightMm);

        // captions
        // captions are always drawn horizontal next to the image. If the whitespace is too small
        // whiteSpaceMm < (m_captionMinHorSpaceMm + m_gutterMm), then the gutter is reduced to the 2 *captionPageMargin.
        double captionWidth = page.widthMm() - (m_gutterMm + m_captionImageMarginMm + imageWidthMm);

        if (captionWidth < m_captionMinHorSpaceMm)
        {
            // vertical

            // reduce gutter to 2 * m_captionPageMarginMm
            const double adjustedGutterMm = (2 * m_captionPageMarginMm);
            captionWidth = page.widthMm() - (adjustedGutterMm + m_captionImageMarginMm + imageWidthMm);

            if (captionWidth <= 0)
            {
                // cannot do.
            }
            else
            {
                //caption1
                captionSize1 = QSizeF(imageRect1.height() - (page.trimMm() + m_captionPageMarginMm + m_captionImageMarginMm), captionWidth);

                if (leftPage(page))
                    captionTransform1 = QTransform().rotate(-90) * QTransform().translate(imageRect1.right() + m_captionImageMarginMm, imageRect1.bottom() - m_captionImageMarginMm);
                else if (rightPage(page))
                    captionTransform1 = QTransform().rotate(90) * QTransform().translate(imageRect1.left() - m_captionImageMarginMm, page.trimMm() + m_captionPageMarginMm);

                captionFlags1 = Qt::TextWordWrap | Qt::AlignLeft | Qt::AlignTop;

                //caption2
                captionSize2 = QSizeF(imageRect2.height() - (m_captionImageMarginMm + m_captionImageMarginMm), captionWidth);

                if (leftPage(page))
                    captionTransform2 = QTransform().rotate(-90) * QTransform().translate(imageRect2.right() + m_captionImageMarginMm, imageRect2.bottom() - m_captionImageMarginMm);
                else if (rightPage(page))
                    captionTransform2 = QTransform().rotate(90) * QTransform().translate(imageRect2.left() - m_captionImageMarginMm, imageRect2.top() + m_captionImageMarginMm);

                captionFlags2 = Qt::TextWordWrap | Qt::AlignLeft | Qt::AlignTop;

                //caption3
                captionSize3 = QSizeF(imageRect3.height() - (page.trimMm() + m_captionPageMarginMm + m_captionImageMarginMm), captionWidth);

                if (leftPage(page))
                    captionTransform3 = QTransform().rotate(-90) * QTransform().translate(imageRect3.right() + m_captionImageMarginMm, imageRect3.bottom() - (page.trimMm() + m_captionPageMarginMm));
                else if (rightPage(page))
                    captionTransform3 = QTransform().rotate(90) * QTransform().translate(imageRect3.left() - m_captionImageMarginMm, imageRect3.top() + m_captionImageMarginMm);

                captionFlags3 = Qt::TextWordWrap | Qt::AlignLeft | Qt::AlignTop;
            }
        }
        else
        {
            // horizontal

            if (captionWidth <= 0)
            {
                // cannot do.
            }
            else
            {
                captionSize1 = QSizeF(captionWidth, imageRect1.height() - (m_captionImageMarginMm + m_captionPageMarginMm + page.trimMm()));
                captionSize2 = QSizeF(captionWidth, imageRect2.height() - (m_captionImageMarginMm + m_captionImageMarginMm));
                captionSize3 = QSizeF(captionWidth, imageRect3.height() - (m_captionImageMarginMm + m_captionPageMarginMm + page.trimMm()));

                captionTransform1 = QTransform().translate(leftPage(page)?(imageRect1.width() + m_captionImageMarginMm):(imageRect1.left() - (m_captionImageMarginMm + captionSize1.width())), imageRect1.top() + page.trimMm() + m_captionPageMarginMm);
                captionTransform2 = QTransform().translate(leftPage(page)?(imageRect2.width() + m_captionImageMarginMm):(imageRect2.left() - (m_captionImageMarginMm + captionSize2.width())), imageRect2.top() + m_captionImageMarginMm);
                captionTransform3 = QTransform().translate(leftPage(page)?(imageRect3.width() + m_captionImageMarginMm):(imageRect3.left() - (m_captionImageMarginMm + captionSize2.width())), imageRect3.top() + m_captionImageMarginMm);

                captionFlags1 = Qt::TextWordWrap | Qt::AlignTop;
                if (leftPage(page))
                    captionFlags1 |= Qt::AlignLeft;
                else if (rightPage(page))
                    captionFlags1 |= Qt::AlignRight;

                captionFlags2 = captionFlags1;
                captionFlags3 = captionFlags1;
            }
        }
    }
    else if ((isLandscape(image1) && !isLandscape(image2) && !isLandscape(image3))
        || (!isLandscape(image1) && isLandscape(image2) && !isLandscape(image3)))
    {
        // landscape at top

        if (!isLandscape(image1) && isLandscape(image2) && !isLandscape(image3))
        {
            // swap 1 and 2, to make landscape (image 1) at top
            Image const* tmp = image1;
            image1 = image2;
            image2 = tmp;
        }

        // bottom images same height
        QSizeF scaled3;
        sameHeight(image2->size(), image3->size(), &scaled3);
        const double image2ScaledWidth = image2->size().width();
        const double image3ScaledWidth = scaled3.width();

        // top image and bottom images same width
        const double imagesBottomScaledHeight = image2->size().height();

        QSizeF scaled1;
        sameWidth(QSizeF(image2ScaledWidth + image3ScaledWidth, imagesBottomScaledHeight), image1->size(), &scaled1); // we ignore the m_spacingMm betweeon the two bottom images
        const double imageTopScaledHeight = scaled1.height();

        // to mm
        const double imageTopHeightMm = (page.heightMm() - m_spacingMm) * (imageTopScaledHeight / (imageTopScaledHeight + imagesBottomScaledHeight));
        const double imageBottomHeightMm = (page.heightMm() - m_spacingMm) * (imagesBottomScaledHeight / (imageTopScaledHeight + imagesBottomScaledHeight));

        const double normalImageTopWidthMm = imageTopHeightMm * image1->ratio();
        const double maxImageWidthMm = page.widthMm() - m_gutterMm;

        // respect gutter
        const double imageWidthMm = qMin(normalImageTopWidthMm, maxImageWidthMm);

        const double image2WidthMm = (imageWidthMm - m_spacingMm) * (image2ScaledWidth / (image2ScaledWidth + image3ScaledWidth));
        const double image3WidthMm = (imageWidthMm - m_spacingMm) * (image3ScaledWidth / (image2ScaledWidth + image3ScaledWidth));

        imageRect1 = QRectF(leftPage(page)?0:(page.widthMm() - imageWidthMm), 0, imageWidthMm, imageTopHeightMm);
        imageRect2 = QRectF(imageRect1.left(), imageRect1.bottom() + m_spacingMm, image2WidthMm, imageBottomHeightMm);
        imageRect3 = QRectF(imageRect2.right() + m_spacingMm, imageRect1.bottom() + m_spacingMm, image3WidthMm, imageBottomHeightMm);

        // captions
        // captions are always drawn horizontal next to the image. If the whitespace is too small
        // whiteSpaceMm < (m_captionMinHorSpaceMm + m_gutterMm), then the gutter is reduced to the 2 *captionPageMargin.
        const double whiteSpaceMm = page.widthMm() - imageWidthMm;
        double adjustedGutterMm = 2 * m_captionPageMarginMm;

        if (whiteSpaceMm < (m_captionImageMarginMm + adjustedGutterMm))
        {
            // cannot do.
        }
        else
        {
            //caption1
            captionSize1 = QSizeF(imageRect1.height() - (page.trimMm() + m_captionPageMarginMm + m_captionImageMarginMm), page.widthMm() - (imageRect1.width() + adjustedGutterMm + m_captionImageMarginMm));

            if (leftPage(page))
                captionTransform1 = QTransform().rotate(-90) * QTransform().translate(imageRect1.right() + m_captionImageMarginMm, imageRect1.bottom() - m_captionImageMarginMm);
            else if (rightPage(page))
                captionTransform1 = QTransform().rotate(90) * QTransform().translate(imageRect1.left() - m_captionImageMarginMm, page.trimMm() + m_captionPageMarginMm);

            captionFlags1 = Qt::TextWordWrap | Qt::AlignLeft | Qt::AlignTop;

            if (leftPage(page))
            {
                //caption2
                if (!caption(image2).isEmpty())
                {
                    // one line fixed. todo: determine used space?
                    QFontMetrics fm(m_captionFont);
                    captionSize2 = QSizeF(imageRect2.height() - (page.trimMm() + m_captionPageMarginMm + m_captionImageMarginMm + captionIndicatorSizeMm), fm.height() * 0.3527);
                    captionTransform2 = QTransform().rotate(-90) * QTransform().translate(imageRect3.right() + m_captionImageMarginMm, imageRect2.bottom() - (page.trimMm() + m_captionPageMarginMm + captionIndicatorSizeMm));
                    captionFlags2 = Qt::TextWordWrap | Qt::AlignLeft | Qt::AlignTop;

                    captionIndicator2 = doubleArrowUp();
                    captionIndicatorTransform2 = QTransform().scale(captionIndicatorSizeMm, captionIndicatorSizeMm) * QTransform().rotate(-90) * QTransform().translate(imageRect3.right() + m_captionImageMarginMm, imageRect2.bottom() - (page.trimMm() + m_captionPageMarginMm)) * (page.transformMmToPaintDevice(device));
                }

                //caption3
                if (!caption(image3).isEmpty())
                {
                    const double captionSize3Height = page.widthMm() - (imageRect2.width() + m_spacingMm + imageRect3.width() + adjustedGutterMm + m_captionImageMarginMm + captionSize2.height());
                    if (captionSize3Height > 0)
                    {
                        captionSize3 = QSizeF(imageRect2.height() - (page.trimMm() + m_captionPageMarginMm + m_captionImageMarginMm + captionIndicatorSizeMm), captionSize3Height);
                        captionTransform3 = QTransform().rotate(-90) * QTransform().translate(imageRect3.right() + m_captionImageMarginMm + captionSize2.height(), imageRect2.bottom() - (page.trimMm() + m_captionPageMarginMm + captionIndicatorSizeMm));
                        captionFlags3 = Qt::TextWordWrap | Qt::AlignLeft | Qt::AlignTop;

                        captionIndicator3 = singleArrowUp();
                        captionIndicatorTransform3 = QTransform().scale(captionIndicatorSizeMm, captionIndicatorSizeMm) * QTransform().rotate(-90) * QTransform().translate(imageRect3.right() + m_captionImageMarginMm + captionSize2.height(), imageRect2.bottom() - (page.trimMm() + m_captionPageMarginMm)) * (page.transformMmToPaintDevice(device));
                    }
                }
            }
            else if (rightPage(page))
            {
                //caption3
                if (!caption(image3).isEmpty())
                {
                    // one line fixed. todo: determine used space?
                    QFontMetrics fm(m_captionFont);
                    captionSize3 = QSizeF(imageRect3.height() - (page.trimMm() + m_captionPageMarginMm + m_captionImageMarginMm + captionIndicatorSizeMm), fm.height() * 0.3527);
                    captionTransform3 = QTransform().rotate(90) * QTransform().translate(imageRect2.left() - m_captionImageMarginMm, imageRect2.top() + m_captionImageMarginMm + captionIndicatorSizeMm);
                    captionFlags3 = Qt::TextWordWrap | Qt::AlignLeft | Qt::AlignTop;

                    captionIndicator3 = doubleArrowUp();
                    captionIndicatorTransform3 = QTransform().scale(captionIndicatorSizeMm, captionIndicatorSizeMm) * QTransform().rotate(90) * QTransform().translate(imageRect2.left() - m_captionImageMarginMm, imageRect2.top() + m_captionImageMarginMm) * (page.transformMmToPaintDevice(device));
                }

                //caption2
                if (!caption(image2).isEmpty())
                {
                    const double captionSize2Height = page.widthMm() - (imageRect2.width() + m_spacingMm + imageRect3.width() + adjustedGutterMm + m_captionImageMarginMm + captionSize3.height());
                    if (captionSize2Height > 0)
                    {
                        captionSize2 = QSizeF(imageRect3.height() - (page.trimMm() + m_captionPageMarginMm + m_captionImageMarginMm + captionIndicatorSizeMm), captionSize2Height);
                        captionTransform2 = QTransform().rotate(90) * QTransform().translate(imageRect2.left() - (m_captionImageMarginMm + captionSize3.height()), imageRect2.top() + m_captionImageMarginMm + captionIndicatorSizeMm);
                        captionFlags2 = Qt::TextWordWrap | Qt::AlignLeft | Qt::AlignTop;

                        captionIndicator2 = singleArrowUp();
                        captionIndicatorTransform2 = QTransform().scale(captionIndicatorSizeMm, captionIndicatorSizeMm) * QTransform().rotate(90) * QTransform().translate(imageRect2.left() - (m_captionImageMarginMm + captionSize3.height()), imageRect2.top() + m_captionImageMarginMm) * (page.transformMmToPaintDevice(device));
                    }
                }
            }

        }
    }
    else if (!isLandscape(image1) && isLandscape(image2) && !isLandscape(image3))
    {
        // same layout as above, should be handled there
        Q_ASSERT(false);
    }
    else if (!isLandscape(image1) && !isLandscape(image2) && isLandscape(image3))
    {
        // landscape at bottom

        // top images same height
        QSizeF scaled2;
        sameHeight(image1->size(), image2->size(), &scaled2);
        const double image1ScaledWidth = image1->size().width();
        const double image2ScaledWidth = scaled2.width();

        // top images and bottom image same width
        const double imagesTopScaledHeight = image1->size().height();
        const double imagesTopScaledWidth = image1ScaledWidth + image2ScaledWidth;// we ignore the m_spacingMm betweeon the two bottom images

        QSizeF scaled3;
        sameWidth(QSizeF(imagesTopScaledWidth, imagesTopScaledHeight), image3->size(), &scaled3);
        const double imageBottomScaledHeight = scaled3.height();

        // to mm
        const double imagesTopHeightMm = (page.heightMm() - m_spacingMm) * (imagesTopScaledHeight / (imagesTopScaledHeight + imageBottomScaledHeight));
        const double imageBottomHeightMm = (page.heightMm() - m_spacingMm) * (imageBottomScaledHeight / (imagesTopScaledHeight + imageBottomScaledHeight));

        const double normalImagesTopWidthMm = imagesTopHeightMm * (imagesTopScaledWidth / imagesTopScaledHeight);
        const double maxImageWidthMm = page.widthMm() - m_gutterMm;

        // respect gutter
        const double imageWidthMm = qMin(normalImagesTopWidthMm, maxImageWidthMm);

        const double image1WidthMm = (imageWidthMm - m_spacingMm) * (image1ScaledWidth / (image1ScaledWidth + image2ScaledWidth));
        const double image2WidthMm = (imageWidthMm - m_spacingMm) * (image2ScaledWidth / (image1ScaledWidth + image2ScaledWidth));

        imageRect1 = QRectF(leftPage(page)?0:(page.widthMm() - imageWidthMm), 0, image1WidthMm, imagesTopHeightMm);
        imageRect2 = QRectF(imageRect1.right() + m_spacingMm, imageRect1.top(), image2WidthMm, imagesTopHeightMm);
        imageRect3 = QRectF(imageRect1.left(), imageRect1.bottom() + m_spacingMm, imageWidthMm, imageBottomHeightMm);

        // captions
        // captions are always drawn horizontal next to the image. If the whitespace is too small
        // whiteSpaceMm < (m_captionMinHorSpaceMm + m_gutterMm), then the gutter is reduced to the 2 *captionPageMargin.
        const double whiteSpaceMm = page.widthMm() - imageWidthMm;
        double adjustedGutterMm = 2 * m_captionPageMarginMm;

        if (whiteSpaceMm < (m_captionImageMarginMm + adjustedGutterMm))
        {
            // cannot do.
        }
        else
        {
            //caption3
            captionSize3 = QSizeF(imageRect3.height() - (page.trimMm() + m_captionPageMarginMm + m_captionImageMarginMm), page.widthMm() - (imageRect3.width() + adjustedGutterMm + m_captionImageMarginMm));

            if (leftPage(page))
                captionTransform3 = QTransform().rotate(-90) * QTransform().translate(imageRect3.right() + m_captionImageMarginMm, imageRect3.bottom() - (page.trimMm() + m_captionPageMarginMm));
            else if (rightPage(page))
                captionTransform3 = QTransform().rotate(90) * QTransform().translate(imageRect3.left() - m_captionImageMarginMm, imageRect3.top() + m_captionImageMarginMm);

            captionFlags3 = Qt::TextWordWrap | Qt::AlignLeft | Qt::AlignTop;

            if (leftPage(page))
            {
                //caption1
                if (!caption(image1).isEmpty())
                {
                    // one line fixed. todo: determine used space?
                    QFontMetrics fm(m_captionFont);
                    captionSize1 = QSizeF(imageRect1.height() - (page.trimMm() + m_captionPageMarginMm + m_captionImageMarginMm + captionIndicatorSizeMm), fm.height() * 0.3527);
                    captionTransform1 = QTransform().rotate(-90) * QTransform().translate(imageRect2.right() + m_captionImageMarginMm, imageRect2.bottom() - (m_captionImageMarginMm + captionIndicatorSizeMm));
                    captionFlags1 = Qt::TextWordWrap | Qt::AlignLeft | Qt::AlignTop;

                    captionIndicator1 = doubleArrowUp();
                    captionIndicatorTransform1 = QTransform().scale(captionIndicatorSizeMm, captionIndicatorSizeMm) * QTransform().rotate(-90) * QTransform().translate(imageRect2.right() + m_captionImageMarginMm, imageRect2.bottom() - m_captionImageMarginMm) * (page.transformMmToPaintDevice(device));
                }

                //caption2
                if (!caption(image2).isEmpty())
                {
                    const double captionSize2Height = page.widthMm() - (imageRect1.width() + m_spacingMm + imageRect2.width() + adjustedGutterMm + m_captionImageMarginMm + captionSize1.height());
                    if (captionSize2Height > 0)
                    {
                        captionSize2 = QSizeF(imageRect2.height() - (page.trimMm() + m_captionPageMarginMm + m_captionImageMarginMm + captionIndicatorSizeMm), captionSize2Height);
                        captionTransform2 = QTransform().rotate(-90) * QTransform().translate(imageRect2.right() + m_captionImageMarginMm + captionSize1.height(), imageRect2.bottom() - (m_captionImageMarginMm + captionIndicatorSizeMm));
                        captionFlags2 = Qt::TextWordWrap | Qt::AlignLeft | Qt::AlignTop;

                        captionIndicator2 = singleArrowUp();
                        captionIndicatorTransform2 = QTransform().scale(captionIndicatorSizeMm, captionIndicatorSizeMm) * QTransform().rotate(-90) * QTransform().translate(imageRect2.right() + m_captionImageMarginMm + captionSize1.height(), imageRect2.bottom() - m_captionImageMarginMm) * (page.transformMmToPaintDevice(device));
                    }
                }
            }
            else if (rightPage(page))
            {
                //caption2
                if (!caption(image2).isEmpty())
                {
                    // one line fixed. todo: determine used space?
                    QFontMetrics fm(m_captionFont);
                    captionSize2 = QSizeF(imageRect2.height() - (page.trimMm() + m_captionPageMarginMm + m_captionImageMarginMm + captionIndicatorSizeMm), fm.height() * 0.3527);
                    captionTransform2 = QTransform().rotate(90) * QTransform().translate(imageRect1.left() - m_captionImageMarginMm, imageRect1.top() + page.trimMm() + m_captionPageMarginMm + captionIndicatorSizeMm);
                    captionFlags2 = Qt::TextWordWrap | Qt::AlignLeft | Qt::AlignTop;

                    captionIndicator2 = doubleArrowUp();
                    captionIndicatorTransform2 = QTransform().scale(captionIndicatorSizeMm, captionIndicatorSizeMm) * QTransform().rotate(90) * QTransform().translate(imageRect1.left() - m_captionImageMarginMm, imageRect1.top() + page.trimMm() + m_captionPageMarginMm) * (page.transformMmToPaintDevice(device));
                }

                //caption1
                if (!caption(image1).isEmpty())
                {
                    const double captionRect1Height = page.widthMm() - (imageRect2.width() + m_spacingMm + imageRect1.width() + adjustedGutterMm + m_captionImageMarginMm + captionSize2.height());
                    if (captionRect1Height > 0)
                    {
                        captionSize1 = QSizeF(imageRect2.height() - (page.trimMm() + m_captionPageMarginMm + m_captionImageMarginMm + captionIndicatorSizeMm), captionRect1Height);
                        captionTransform1 = QTransform().rotate(90) * QTransform().translate(imageRect1.left() - (m_captionImageMarginMm + captionSize2.height()), imageRect2.top() + page.trimMm() + m_captionPageMarginMm + captionIndicatorSizeMm);
                        captionFlags1 = Qt::TextWordWrap | Qt::AlignLeft | Qt::AlignTop;

                        captionIndicator1 = singleArrowUp();
                        captionIndicatorTransform1 = QTransform().scale(captionIndicatorSizeMm, captionIndicatorSizeMm) * QTransform().rotate(90) * QTransform().translate(imageRect1.left() - (m_captionImageMarginMm + captionSize2.height()), imageRect1.top() + page.trimMm() + m_captionPageMarginMm) * (page.transformMmToPaintDevice(device));
                    }
                }
            }
        }
    }

    drawImageMaintainingRatio(m_cropping, page.scaleToPaintDevice(imageRect1, device), image1, painter, nativeResolution);
    drawImageMaintainingRatio(m_cropping, page.scaleToPaintDevice(imageRect2, device), image2, painter, nativeResolution);
    drawImageMaintainingRatio(m_cropping, page.scaleToPaintDevice(imageRect3, device), image3, painter, nativeResolution);

    drawCaption(caption(image1), m_captionFont, m_captionColor, captionSize1, captionFlags1, captionTransform1, page.transformMmToPaintDevice(device), painter, showLayoutLines);
    drawCaption(caption(image2), m_captionFont, m_captionColor, captionSize2, captionFlags2, captionTransform2, page.transformMmToPaintDevice(device), painter, showLayoutLines);
    drawCaption(caption(image3), m_captionFont, m_captionColor, captionSize3, captionFlags3, captionTransform3, page.transformMmToPaintDevice(device), painter, showLayoutLines);

    drawCaptionIndicator(captionIndicator1, m_captionColor, captionIndicatorTransform1, painter, showLayoutLines);
    drawCaptionIndicator(captionIndicator2, m_captionColor, captionIndicatorTransform2, painter, showLayoutLines);
    drawCaptionIndicator(captionIndicator3, m_captionColor, captionIndicatorTransform3, painter, showLayoutLines);
}
