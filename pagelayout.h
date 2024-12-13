#ifndef PAGELAYOUT_H
#define PAGELAYOUT_H

class Page;
class QPainter;
class QPaintDevice;
class QRectF;
class Image;
class QWidget;
class PageLayoutOptionsWidget;

class PageLayout
{
public:
    PageLayout();
    virtual ~PageLayout();

    virtual PageLayout* clone() const = 0;

    virtual PageLayoutOptionsWidget* createPageLayoutOptionsWidget(QWidget* parent) = 0;

    virtual void paint(const Page& page, QPainter& painter, QPaintDevice* device, bool showLayoutLines, bool nativeResolution) const = 0;

protected:
    void drawImageMaintainingRatio(bool cropping, const QRectF& rectF, Image const* image, QPainter& painter, bool nativeResolution) const;
};

#endif // PAGELAYOUT_H
