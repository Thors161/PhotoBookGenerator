#include "pagelayout.h"

#include <QRectF>
#include "image.h"
#include <QPainter>
#include <QTransform>

PageLayout::PageLayout()
{
}

PageLayout::~PageLayout()
{
}

void PageLayout::drawImageMaintainingRatio(bool cropping, const QRectF& rectF, Image const* image, QPainter& painter, bool nativeResolution) const
{
    Q_CHECK_PTR(image);

    bool scaleToHeight = false;

    if (cropping)
        scaleToHeight = image->ratio() > rectF.width() / rectF.height();
    else
        scaleToHeight = image->ratio() < rectF.width() / rectF.height();

    QRectF targetRect;
    QRectF sourceRect;

    if (scaleToHeight)
    {
        // scale to height
        if (cropping)
        {
            targetRect = rectF;
            double originalWidth = image->size().width();
            double newWidth = image->size().height() * (rectF.width() / rectF.height());
            sourceRect = QRectF((originalWidth - newWidth) * 0.5, 0, newWidth, image->size().height());
        }
        else
        {
            const double originalWidth = rectF.width();
            const double newWidth = rectF.height() * image->ratio();
            targetRect = QRectF(rectF.left() + (originalWidth - newWidth) * 0.5, rectF.top(), newWidth, rectF.height());
            sourceRect = QRectF(QPoint(0, 0), image->size());
        }

    }
    else
    {
        // scale to width
        if (cropping)
        {
            targetRect = rectF;
            double originalHeight = image->size().height();
            double newHeight = image->size().width() / (rectF.width() / rectF.height());
            sourceRect = QRectF(0, (originalHeight - newHeight) * 0.5, image->size().width(), newHeight);
        }
        else
        {
            double originalHeight = rectF.height();
            double newHeight = rectF.width() / image->ratio();
            targetRect = QRectF(rectF.left(), rectF.top() + (originalHeight - newHeight) * 0.5, rectF.width(), newHeight);
            sourceRect = QRectF(QPoint(0, 0), image->size());
        }
    }

    if (nativeResolution)
    {
        painter.drawImage(targetRect, image->image(), sourceRect);
    }
    else
    {
        const double scale = static_cast<double>(image->preview().width()) / static_cast<double>(image->size().width());
        QTransform transform = QTransform().scale(scale, scale);
        sourceRect = transform.mapRect(sourceRect);
        painter.drawImage(targetRect, image->preview(), sourceRect);
    }
}
