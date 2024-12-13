#ifndef BOOK_H
#define BOOK_H

#include <QDomElement>
#include "page.h"

class PageLayout;
class QDomDocument;

class Book
{
public:
    Book();
    Book(const QDomElement& imageElement);
    ~Book();

    Book* clone() const;
    
    void setCoverPage(const Page& page);
    void addPage(const Page& page);

    void clear();

    Page emptyPage() const;

    void setCoverPageWidthMm(double coverPageWidthMm);
    double coverPageWidthMm() const;
    void setCoverPageHeightMm(double coverPageHeightMm);
    double coverPageHeightMm() const;
    void setPageWidthMm(double pageWidthMm);
    double pageWidthMm() const;
    void setPageHeightMm(double pageHeightMm);
    double pageHeightMm() const;
    void setTrimMm(double trimMm);
    double trimMm() const;

    const Page& coverPage() const;
    const QList<Page>& pages() const;

    bool renderCover(const QString& fileName) const;
    bool render(const QString& fileName) const;

    QDomElement xml(QDomDocument* document) const;

private:
    void updatePageSizes();

    Page m_coverPage;
    QList<Page> m_pages;

    double m_coverPageWidthMm;
    double m_coverPageHeightMm;
    double m_pageWidthMm;
    double m_pageHeightMm;
    double m_trimMm;
};

#endif // BOOK_H
