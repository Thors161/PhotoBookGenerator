#include "page.h"
#include "pagelayout.h"
#include <QPainter>
#include <QLineF>

Page::Page()
{
    m_pageSide = UnknownPage;

    m_widthMm = 0;
    m_heightMm = 0;
    m_trimMm = 0;

    m_pageLayout = 0;
}

Page::~Page()
{
}

Page& Page::addImage(Image const* image)
{
    Q_CHECK_PTR(image);

    m_images.append(image);
    return *this;
}

Image const* Page::takeImage(int at)
{
    // todo: check bounds
    return m_images.takeAt(at);
}

void Page::setProperties(double widthMm, double heightMm, double trimMm)
{
    m_widthMm = widthMm;
    m_heightMm = heightMm;
    m_trimMm = trimMm;
}

double Page::widthMm() const
{
    return m_widthMm;
}

double Page::heightMm() const
{
    return m_heightMm;
}

double Page::trimMm() const
{
    return m_trimMm;
}

void Page::setPageSide(PageSide pageSide)
{
    m_pageSide = pageSide;
}

Page::PageSide Page::pageSide() const
{
    return m_pageSide;
}

void Page::setPagelayout(PageLayout const * pageLayout)
{
    m_pageLayout = pageLayout;
}

const QList<Image const*>& Page::images() const
{
    return m_images;
}

int Page::imagesCount() const
{
    return m_images.size();
}

int Page::portraitImagesCount() const
{
    int portraitCount = 0;
    foreach(Image const* image, m_images)
    {
        if (image->portait())
            portraitCount++;
    }

    return portraitCount;
}

int Page::landscapeImagesCount() const
{
    int landscapeCount = 0;
    foreach(Image const* image, m_images)
    {
        if (!image->portait())
            landscapeCount++;
    }

    return landscapeCount;
}

double Page::ratioWH() const
{
    return (m_widthMm / m_heightMm);
}

void Page::paint(QPainter& painter, QPaintDevice* device, bool showLayoutLines, bool eraseTrim, bool nativeResolution) const
{
    Q_CHECK_PTR(device);
    Q_CHECK_PTR(m_pageLayout);
    Q_ASSERT(m_pageSide != UnknownPage);

    m_pageLayout->paint(*this, painter, device, showLayoutLines, nativeResolution);

    if (showLayoutLines)
        drawTrimLines(painter, device);

    if (eraseTrim)
        removeTrim(painter, device);
}

QTransform Page::transformMmToPaintDevice(QPaintDevice* device) const
{
    double xScale = device->width() / m_widthMm;
    double yScale = device->height() / m_heightMm;
    return QTransform().scale(xScale, yScale);
}

QRectF Page::scaleToPaintDevice(const QRectF& s, QPaintDevice* device) const
{
    Q_CHECK_PTR(device);
    return transformMmToPaintDevice(device).mapRect(s);
}

void Page::drawTrimLines(QPainter& painter, QPaintDevice* device) const
{
    painter.save();

    // paint trim lines
    QPen trimLinePen(Qt::green, 1, Qt::DashLine);
    painter.setPen(trimLinePen);

    QPainterPath trimPath;
    trimPath.moveTo(0, m_trimMm);
    trimPath.lineTo(m_widthMm, m_trimMm);

    trimPath.moveTo(0, m_heightMm - m_trimMm);
    trimPath.lineTo(m_widthMm, m_heightMm - m_trimMm);

    if ((m_pageSide == LeftPage) || (m_pageSide == CoverPage))
    {
        trimPath.moveTo(m_trimMm, 0);
        trimPath.lineTo(m_trimMm, m_heightMm);
    }

    if ((m_pageSide == RightPage) || (m_pageSide == CoverPage))
    {
        trimPath.moveTo(m_widthMm - m_trimMm, 0);
        trimPath.lineTo(m_widthMm - m_trimMm, m_heightMm);
    }

    painter.drawPath(scaleToPaintDevice(trimPath, device));

    painter.restore();
}

void Page::removeTrim(QPainter& painter, QPaintDevice* device) const
{
    QRectF top(0, 0, m_widthMm, m_trimMm);
    painter.eraseRect(scaleToPaintDevice(top, device));

    QRectF bottom(0, m_heightMm - m_trimMm, m_widthMm, m_trimMm);
    painter.eraseRect(scaleToPaintDevice(bottom, device));

    if ((m_pageSide == LeftPage) || (m_pageSide == CoverPage))
    {
        QRectF side(0, 0, m_trimMm, m_heightMm);
        painter.eraseRect(scaleToPaintDevice(side, device));
    }

    if ((m_pageSide == RightPage) || (m_pageSide == CoverPage))
    {
        QRectF side(m_widthMm - m_trimMm, 0, m_trimMm, m_heightMm);
        painter.eraseRect(scaleToPaintDevice(side, device));
    }

}

