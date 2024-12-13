#include "imagelist.h"

#include <QDomDocument>
#include "image.h"

ImageList::ImageList() : QList<Image*>()
{
}

ImageList::ImageList(const ImageList& imageList) : QList<Image*>(imageList)
{

}

ImageList::ImageList(const QDomElement& imagesElement) : QList<Image*>()
{
    QDomElement imageElement = imagesElement.firstChildElement();

    while(!imageElement.isNull())
    {
        Image* image = new Image(imageElement);
        if (image->init())
        {
            append(image);
        }
        else
        {
            delete image;
        }
        imageElement = imageElement.nextSiblingElement();
    }
}

ImageList::~ImageList()
{
}

ImageList* ImageList::clone() const
{
    return new ImageList(*this);
}

void ImageList::clear()
{
    foreach(Image* image, *this)
        delete image;

    QList<Image*>::clear();
}

QDomElement ImageList::xml(QDomDocument* document) const
{
    QDomElement imagesElement = document->createElement("images");
    QDomElement elements;

    foreach(Image const* image, *this)
        imagesElement.appendChild(image->xml(document));

    return imagesElement;
}
