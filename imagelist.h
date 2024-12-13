#ifndef IMAGELIST_H
#define IMAGELIST_H

#include <QList>
#include <QDomElement>

class Image;
class QDomDocument;

class ImageList : public QList<Image*>
{
public:
    ImageList();
    ImageList(const ImageList& imageList);
    ImageList(const QDomElement& imagesElement);
    ~ImageList();

    ImageList* clone() const;

    void clear();
    QDomElement xml(QDomDocument* document) const;
};

#endif // IMAGELIST_H
