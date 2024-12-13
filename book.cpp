#include "book.h"


#include <QPrinter>
#include <QPainter>
#include <QDebug>

Book::Book()
{
    m_coverPageWidthMm = 0;
    m_coverPageHeightMm = 0;
    m_pageWidthMm = 0;
    m_pageHeightMm = 0;
    m_trimMm = 0;
}

Book::Book(const QDomElement& imageElement)
{
    QDomElement element;

    element = imageElement.firstChildElement("coverPageWidthMm");
    m_coverPageWidthMm = element.text().toDouble();

    element = imageElement.firstChildElement("coverPageHeightMm");
    m_coverPageHeightMm = element.text().toDouble();

    element = imageElement.firstChildElement("pageWidthMm");
    m_pageWidthMm = element.text().toDouble();

    element = imageElement.firstChildElement("pageHeightMm");
    m_pageHeightMm = element.text().toDouble();

    element = imageElement.firstChildElement("trimMm");
    m_trimMm = element.text().toDouble();
}

Book::~Book()
{
}

Book* Book::clone() const
{
    return new Book(*this);
}

void Book::setCoverPage(const Page& coverPage)
{
    m_coverPage = coverPage;
}

void Book::addPage(const Page& page)
{
    m_pages.append(page);
}

void Book::clear()
{
    m_coverPage = Page();
    m_pages.clear();
}

Page Book::emptyPage() const
{
    Page p;
    p.setProperties(m_pageWidthMm, m_pageHeightMm, m_trimMm);
    return p;
}

void Book::setCoverPageWidthMm(double coverPageWidthMm)
{
    m_coverPageWidthMm = coverPageWidthMm;
    updatePageSizes();
}

double Book::coverPageWidthMm() const
{
    return m_coverPageWidthMm;
}

void Book::setCoverPageHeightMm(double coverPageHeightMm)
{
    m_coverPageHeightMm = coverPageHeightMm;
    updatePageSizes();
}

double Book::coverPageHeightMm() const
{
    return m_coverPageHeightMm;
}

void Book::setPageWidthMm(double pageWidthMm)
{
    m_pageWidthMm = pageWidthMm;
    updatePageSizes();
}

double Book::pageWidthMm() const
{
    return m_pageWidthMm;
}

void Book::setPageHeightMm(double pageHeightMm)
{
    m_pageHeightMm = pageHeightMm;
    updatePageSizes();
}

double Book::pageHeightMm() const
{
    return m_pageHeightMm;
}

void Book::setTrimMm(double trimMm)
{
    m_trimMm = trimMm;
    updatePageSizes();
}

double Book::trimMm() const
{
    return m_trimMm;
}

const Page& Book::coverPage() const
{
    return m_coverPage;
}

const QList<Page>& Book::pages() const
{
    return m_pages;
}

bool Book::renderCover(const QString& fileName) const
{
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFileName(fileName);
    printer.setPageSizeMM(QSizeF(m_coverPageWidthMm, m_coverPageHeightMm));
    printer.setFullPage(true);
    printer.setResolution(1200);
    printer.setPageMargins(0, 0, 0, 0, QPrinter::Millimeter);

    //qDebug() << "Page rect:  " << printer.pageRect();
    //qDebug() << "Paper rect: " << printer.paperRect();

    QPainter painter;
    painter.begin(&printer);
    m_coverPage.paint(painter, &printer, false, false, true);
    painter.end();

    return true;
}

bool Book::render(const QString& fileName) const
{
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFileName(fileName);
    printer.setPageSizeMM(QSizeF(m_pageWidthMm, m_pageHeightMm));
    printer.setFullPage(true);
    printer.setResolution(1200);
    printer.setPageMargins(0, 0, 0, 0, QPrinter::Millimeter);

    //qDebug() << "Page rect:  " << printer.pageRect();
    //qDebug() << "Paper rect: " << printer.paperRect();

    QPainter painter;
    painter.begin(&printer);

    for (int i = 0; i < m_pages.size(); i++)
    {
        m_pages.at(i).paint(painter, &printer, false, false, true);

        if (i < m_pages.size() - 1)
            printer.newPage();
    }

    painter.end();

    return true;
}

QDomElement Book::xml(QDomDocument* document) const
{
    Q_CHECK_PTR(document);

    QDomElement imageElement = document->createElement("book");
    QDomElement element;
    QDomText text;

    element = document->createElement("coverPageWidthMm");
    text = document->createTextNode(QString::number(m_coverPageWidthMm));
    element.appendChild(text);
    imageElement.appendChild(element);

    element = document->createElement("coverPageHeightMm");
    text = document->createTextNode(QString::number(m_coverPageHeightMm));
    element.appendChild(text);
    imageElement.appendChild(element);

    element = document->createElement("pageWidthMm");
    text = document->createTextNode(QString::number(m_pageWidthMm));
    element.appendChild(text);
    imageElement.appendChild(element);

    element = document->createElement("pageHeightMm");
    text = document->createTextNode(QString::number(m_pageHeightMm));
    element.appendChild(text);
    imageElement.appendChild(element);

    element = document->createElement("trimMm");
    text = document->createTextNode(QString::number(m_trimMm));
    element.appendChild(text);
    imageElement.appendChild(element);

    return imageElement;
}

void Book::updatePageSizes()
{
    m_coverPage.setProperties(m_coverPageWidthMm, m_coverPageHeightMm, m_trimMm);

    for (QList<Page>::iterator it = m_pages.begin(); it != m_pages.end(); ++it)
        it->setProperties(m_pageWidthMm, m_pageHeightMm, m_trimMm);
}
