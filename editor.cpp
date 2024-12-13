#include "myeditor.h"

#include <QFileInfoList>
#include <QDir>
#include <QtAlgorithms>
#include "imagelist.h"

namespace{

bool chronologic(Image const* image1, Image const* image2)
{
    return image1->dateTime() < image2->dateTime();
}

bool alphabetic(Image const* image1, Image const* image2)
{
    return image1->fileName() < image2->fileName();
}

} // anonymous

Editor::Editor()
{
    m_defaultPageSharing = 0;
}

Editor::Editor(int defaultPageSharing)
{
    m_defaultPageSharing = defaultPageSharing;
}

Editor::~Editor()
{
}

void Editor::appendImages(const QStringList& fileNames, ImageList* imageList) const
{
    Q_CHECK_PTR(imageList);

    QList<Image*> images;

    foreach(const QString& fileName, fileNames)
    {
        // incredibly slow....
        //        QFileInfo fi(fileName);
        //        Image* image = new Image(fi.absoluteFilePath());
        Image* image = new Image(fileName);
        if (image->init())
        {
            image->setPageSharing(m_defaultPageSharing);
            images.append(image);
        }
        else
            delete image;
    }

    // batch of images is sorted chronologic before appanding to image list
    qSort(images.begin(), images.end(), chronologic);
//    qSort(images.begin(), images.end(), alphabetic);

    imageList->append(images);
}

void Editor::sortByDate(ImageList* imageList) const
{
    qSort(imageList->begin(), imageList->end(), chronologic);
}

void Editor::setDefaultPageSharing(int defaultPageSharing)
{
    m_defaultPageSharing = defaultPageSharing;
}

int Editor::defaultPageSharing() const
{
    return m_defaultPageSharing;
}

void Editor::showDateFirstInMonth(ImageList* imageList) const
{
    QDate lastDate;
    foreach(Image* image, *imageList)
    {
        // only in included images
        if (image->dateTime().isValid() && image->include())
        {
            // only first of new month
            if (image->dateTime().date().month() > lastDate.month())
            {
                image->setShowDate(true);
                lastDate = image->dateTime().date();
            }
            else
                image->setShowDate(false);
        }
    }
}
