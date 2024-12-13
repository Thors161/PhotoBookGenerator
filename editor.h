#ifndef EDITOR_H
#define EDITOR_H

#include <QList>
#include <QString>
#include <QDomElement>

#include "book.h"
#include "image.h"

class ImageList;
class PageLayout;
class QDomDocument;

class Editor
{
public:
    Editor();
    Editor(int defaultPageSharing);
    virtual ~Editor();

    virtual Editor* clone() const = 0;

    virtual void edit(Book* book, QList<Image const* >* images) = 0;
    virtual QDomElement xml(QDomDocument* document) const = 0;

    void appendImages(const QStringList& fileNames, ImageList* imageList) const;
    void sortByDate(ImageList* imageList) const;

    virtual const QList<PageLayout*>& pageLayouts() const = 0;

    void setDefaultPageSharing(int defaultPageSharing);
    int defaultPageSharing() const;

    void showDateFirstInMonth(ImageList* imageList) const;

private:
    int m_defaultPageSharing;
};

#endif // EDITOR_H
