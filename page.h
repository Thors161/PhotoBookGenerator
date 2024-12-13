#ifndef PAGE_H
#define PAGE_H

#include <QList>
#include <QString>
#include <QRectF>
#include <QTransform>

#include "image.h"

class Book;
class PageLayout;
class QPaintDevice;
class QPainter;

class Page
{
public:
    Page();
    ~Page();

    enum PageSide
    {
        UnknownPage,
        CoverPage,
        LeftPage,
        RightPage
    };

    Page& addImage(Image const* image);
    Image const* takeImage(int at);
    void setProperties(double widthMm, double heightMm, double trimMm);
    double widthMm() const;
    double heightMm() const;
    double trimMm() const;
    void setPageSide(PageSide pageSide);
    PageSide pageSide() const;
    void setPagelayout(PageLayout const * pageLayout);
    const QList<Image const*>& images() const;
    int imagesCount() const;
    int portraitImagesCount() const;
    int landscapeImagesCount() const;
    double ratioWH() const;
    void paint(QPainter& painter, QPaintDevice* device, bool showLayoutLines, bool eraseTrim, bool nativeResolution) const;

    QTransform transformMmToPaintDevice(QPaintDevice* device) const;
    QRectF scaleToPaintDevice(const QRectF& s, QPaintDevice* device) const;
    template<typename T> T scaleToPaintDevice(const T& s, QPaintDevice* device) const;

private:
    void drawTrimLines(QPainter& painter, QPaintDevice* device) const;
    void removeTrim(QPainter& painter, QPaintDevice* device) const;

    QList<Image const*> m_images; // image properties can be modified by the editor, not by the book of course
    PageSide m_pageSide;
    PageLayout const * m_pageLayout; // page layout properties can be modified by the editor, not by the book of course

    double m_widthMm;
    double m_heightMm;
    double m_trimMm;
};

template<typename T> T Page::scaleToPaintDevice(const T& s, QPaintDevice* device) const
{
    Q_CHECK_PTR(device);
    return transformMmToPaintDevice(device).map(s);
}

#endif // PAGE_H
