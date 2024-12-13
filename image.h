#ifndef IMAGE_H
#define IMAGE_H

#include <QSize>
#include <QDateTime>
#include <QString>
#include <QImage>
#include <QDomElement>

#include "image.h"

class QDomDocument;

class Image
{
public:
    Image();
    Image(const QString& fileName);
    Image(const QDomElement& imageElement);
    ~Image();

    bool init();

    bool isNull() const;
    double ratio() const;
    bool portait() const;
    const QDateTime& dateTime() const;
    const QSize& size() const;
    const QString& fileName() const;

    void setCaption(const QString& caption);
    const QString& caption() const;

    void setPageSharing(int pageSharing);
    int pageSharing() const;

    void setFirstOnPage(bool firstOnPage);
    bool firstOnPage() const;

    void setShowDate(bool showDate);
    bool showDate() const;

    void setInclude(bool include);
    bool include() const;

    const QImage& preview() const;

    QImage image() const;

    QDomElement xml(QDomDocument* document) const;

private:
    QSize m_size;
    QString m_fileName;
    QDateTime m_dateTime;

    bool m_include;
    QString m_caption;
    int m_pageSharing; // with how may others we want on a page
    bool m_firstOnPage; // on new page
    bool m_showDate;
    QImage m_preview;
    const static int m_previewWidth = 480;

    bool m_isNull;
};

#endif // IMAGE_H
