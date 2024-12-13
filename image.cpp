#include "image.h"

#include "qexifimageheader.h"
#include <QFileInfo>
#include <QImage>
#include <QDomDocument>
#include <QDebug>

namespace
{
QImage generatePreview(const QImage& image, const QExifImageHeader& exif, int width)
{
    if (exif.contains(QExifImageHeader::Orientation))
    {
        QImage preview;

        switch (exif.value(QExifImageHeader::Orientation).toShort())
        {
        case 1:
        case 2:
        case 3:
        case 4:
            preview = image.scaledToWidth(width);;
            break;
        case 5:
        case 6:
        case 7:
        case 8:
            // will be rotated later
            preview = image.scaledToHeight(width);
            break;
        }

        switch (exif.value(QExifImageHeader::Orientation).toShort())
        {
        case 1:
            return preview;
        case 2:
            return preview.transformed(QTransform().rotate(180, Qt::YAxis));
        case 3:
            return preview.transformed(QTransform().rotate(180, Qt::ZAxis));
        case 4:
            return preview.transformed(QTransform().rotate(180, Qt::XAxis));
        case 5:
            return preview.transformed(QTransform().rotate(180, Qt::YAxis).rotate(90, Qt::ZAxis));
        case 6:
            return preview.transformed(QTransform().rotate(90, Qt::ZAxis));
        case 7:
            return preview.transformed(QTransform().rotate(180, Qt::XAxis).rotate(90, Qt::ZAxis));
        case 8:
            return preview.transformed(QTransform().rotate(270, Qt::ZAxis));
        }
    }

    // fallback
    return image.scaledToWidth(width);
}
} // namespace

Image::Image()
{
    m_isNull = true;
    m_include = true;
    m_pageSharing = 0;
    m_firstOnPage = false;
    m_showDate = false;
}

Image::Image(const QString& fileName) : m_fileName(fileName)
{
    m_isNull = true;
    m_include = true;
    m_pageSharing = 0;
    m_firstOnPage = false;
    m_showDate = false;
}

Image::Image(const QDomElement& imageElement)
{
    QDomElement element;

    element = imageElement.firstChildElement("fileName");
    m_fileName = element.text();

    element = imageElement.firstChildElement("include");
    m_include = element.text() == "true";

    element = imageElement.firstChildElement("caption");
    m_caption = element.text();

    element = imageElement.firstChildElement("pageSharing");
    m_pageSharing = element.text().toInt();

    element = imageElement.firstChildElement("firstOnPage");
    m_firstOnPage = element.text() == "true";

    element = imageElement.firstChildElement("showDate");
    m_showDate = element.text() == "true";

    element = imageElement.firstChildElement("fileName");
    m_fileName = element.text();
}

Image::~Image()
{
}

bool Image::init()
{
    QImage image(m_fileName);
    if (image.isNull())
        return false;

    QExifImageHeader exif;
    exif.loadFromJpeg(m_fileName);

    if (exif.contains(QExifImageHeader::DateTimeOriginal))
    {
        m_dateTime = exif.value(QExifImageHeader::DateTimeOriginal).toDateTime();
    }
    else if (exif.contains(QExifImageHeader::DateTime))
    {
        m_dateTime = exif.value(QExifImageHeader::DateTime).toDateTime();
    }
    else
    {
        // see if we can grab from filename
        const QString filename = QFileInfo(m_fileName).fileName();

        if (m_dateTime.isNull())
        {
            // whatsapp src: IMG-20160723-WA0019
            QStringList parts = filename.split("-", QString::SkipEmptyParts);
            if (parts.size() >=2)
            {
                if (parts.at(1).size() == 8)
                {
                    QDate date = QDate::fromString(parts.at(1), "yyyyMMdd");
                    if (date.isValid())
                    {
                        m_dateTime = QDateTime(date);
                    }
                }
            }
        }

        if (m_dateTime.isNull())
        {
            // generic: 20160723-blds.jpg
            QStringList parts = filename.split("-", QString::SkipEmptyParts);
            if (parts.size() >=1)
            {
                if (parts.at(0).size() == 8)
                {
                    QDate date = QDate::fromString(parts.at(0), "yyyyMMdd");
                    if (date.isValid())
                    {
                        m_dateTime = QDateTime(date);
                    }
                }
            }
        }

        if (m_dateTime.isNull())
        {
            // generic: 20160723_blds.jpg
            QStringList parts = filename.split("_", QString::SkipEmptyParts);
            if (parts.size() >=1)
            {
                if (parts.at(0).size() == 8)
                {
                    QDate date = QDate::fromString(parts.at(0), "yyyyMMdd");
                    if (date.isValid())
                    {
                        m_dateTime = QDateTime(date);
                    }
                }
            }
        }

        if (m_dateTime.isNull())
        {
            // generic: 20160723_blds.jpg
            QStringList parts = filename.split(" ", QString::SkipEmptyParts);
            if (parts.size() >=1)
            {
                if (parts.at(0).size() == 8)
                {
                    QDate date = QDate::fromString(parts.at(0), "yyyyMMdd");
                    if (date.isValid())
                    {
                        m_dateTime = QDateTime(date);
                    }
                }
            }
        }

        if (m_dateTime.isNull())
        {
            // generic: 20160723_blds.jpg
            QStringList parts = filename.split("_", QString::SkipEmptyParts);
            if (parts.size() >=1)
            {
                if (parts.at(0).size() == 8)
                {
                    QDate date = QDate::fromString(parts.at(0), "yyyyMMdd");
                    if (date.isValid())
                    {
                        m_dateTime = QDateTime(date);
                    }
                }
            }
        }

        if (m_dateTime.isNull())
        {
            // generic: img_20160723_blds.jpg
            QStringList parts = filename.split("_", QString::SkipEmptyParts);
            if (parts.size() >=2)
            {
                if (parts.at(1).size() == 8)
                {
                    QDate date = QDate::fromString(parts.at(1), "yyyyMMdd");
                    if (date.isValid())
                    {
                        m_dateTime = QDateTime(date);
                    }
                }
            }
        }

        if (m_dateTime.isNull())
        {
            // generic: 20160723.jpg
            QStringList parts = filename.split(".", QString::SkipEmptyParts);
            if (parts.size() >=1)
            {
                if (parts.at(0).size() == 8)
                {
                    QDate date = QDate::fromString(parts.at(0), "yyyyMMdd");
                    if (date.isValid())
                    {
                        m_dateTime = QDateTime(date);
                    }
                }
            }
        }

        // null dateTime

    }

    // size
    if (exif.contains(QExifImageHeader::Orientation))
    {
        if (exif.contains(QExifImageHeader::ImageWidth) && exif.contains(QExifImageHeader::ImageLength))
        {
            // from exif data, include orientation

            switch (exif.value(QExifImageHeader::Orientation).toShort())
            {
            case 1:
            case 2:
            case 3:
            case 4:
                // trust image size over exif ImageWidth and ImageLength
                // as an image might be rotated by other software
                m_size = QSize(image.size());
                break;
            case 5:
            case 6:
            case 7:
            case 8:
                m_size = QSize(
                            exif.value(QExifImageHeader::ImageLength).toLong(),
                            exif.value(QExifImageHeader::ImageWidth).toLong());
                break;
            }
        }
        else
        {
            // from exif data, include orientation

            switch (exif.value(QExifImageHeader::Orientation).toShort())
            {
            case 1:
            case 2:
            case 3:
            case 4:
                // trust image size over exif ImageWidth and ImageLength
                // as an image might be rotated by other software
                m_size = QSize(image.size());
                break;
            case 5:
            case 6:
            case 7:
            case 8:
                m_size = QSize(image.height(), image.width());
                break;
            default:
                // SD 20201228 some google photos donwloaded images have a different orientation value
                // from file
                m_size = image.size();
                break;
            }
        }
    }
    else
    {
        // from file
        m_size = image.size();
    }

    m_preview = generatePreview(image, exif, m_previewWidth);

    m_isNull = false;

    // for testing:
    //m_caption = "Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod tempor incididunt ut labore et dolore";

    qDebug() << "loaded:  " << m_fileName;

    return true;
}

bool Image::isNull() const
{
    return m_isNull;
}

double Image::ratio() const
{
    return (static_cast<double>(m_size.width()) / static_cast<double>(m_size.height()));
}

bool Image::portait() const
{
    return (m_size.height() >= m_size.width());
}

const QDateTime& Image::dateTime() const
{
    return m_dateTime;
}

const QSize& Image::size() const
{
    return m_size;
}

const QString& Image::fileName() const
{
    return m_fileName;
}

void Image::setCaption(const QString& caption)
{
    m_caption = caption;
}

const QString& Image::caption() const
{
    return m_caption;
}

void Image::setPageSharing(int pageSharing)
{
    m_pageSharing = pageSharing;
}

int Image::pageSharing() const
{
    return m_pageSharing;
}

void Image::setFirstOnPage(bool firstOnPage)
{
    m_firstOnPage = firstOnPage;
}

bool Image::firstOnPage() const
{
    return m_firstOnPage;
}

void Image::setShowDate(bool showDate)
{
    m_showDate = showDate;
}

bool Image::showDate() const
{
    return m_showDate;
}

void Image::setInclude(bool include)
{
    m_include = include;
}

bool Image::include() const
{
    return m_include;
}

const QImage& Image::preview() const
{
    return m_preview;
}

QImage Image::image() const
{
    QImage image(m_fileName);

    // respect exif orientation when loading
    QExifImageHeader exif;
    exif.loadFromJpeg(m_fileName);
    if (exif.contains(QExifImageHeader::Orientation))
    {
        switch (exif.value(QExifImageHeader::Orientation).toShort())
        {
        case 1:
            return image;
        case 2:
            return image.transformed(QTransform().rotate(180, Qt::YAxis));
        case 3:
            return image.transformed(QTransform().rotate(180, Qt::ZAxis));
        case 4:
            return image.transformed(QTransform().rotate(180, Qt::XAxis));
        case 5:
            return image.transformed(QTransform().rotate(180, Qt::YAxis).rotate(90, Qt::ZAxis));
        case 6:
            return image.transformed(QTransform().rotate(90, Qt::ZAxis));
        case 7:
            return image.transformed(QTransform().rotate(180, Qt::XAxis).rotate(90, Qt::ZAxis));
        case 8:
            return image.transformed(QTransform().rotate(270, Qt::ZAxis));
        }
    }

    return image;
}

QDomElement Image::xml(QDomDocument* document) const
{
    Q_CHECK_PTR(document);

    QDomElement imageElement = document->createElement("image");
    QDomElement element;
    QDomText text;

    element = document->createElement("fileName");
    text = document->createTextNode(m_fileName);
    element.appendChild(text);
    imageElement.appendChild(element);

    element = document->createElement("include");
    text = document->createTextNode(m_include?"true":"false");
    element.appendChild(text);
    imageElement.appendChild(element);

    element = document->createElement("caption");
    text = document->createTextNode(m_caption);
    element.appendChild(text);
    imageElement.appendChild(element);

    element = document->createElement("pageSharing");
    text = document->createTextNode(QString::number(m_pageSharing));
    element.appendChild(text);
    imageElement.appendChild(element);

    element = document->createElement("firstOnPage");
    text = document->createTextNode(m_firstOnPage?"true":"false");
    element.appendChild(text);
    imageElement.appendChild(element);

    element = document->createElement("showDate");
    text = document->createTextNode(m_showDate?"true":"false");
    element.appendChild(text);
    imageElement.appendChild(element);

    return imageElement;
}
