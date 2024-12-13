#include "project.h"

#include <QObject>
#include <QDomElement>
#include <QDomDocument>
#include <QStringList>
#include <QFile>
#include <QTextStream>
#include <QFileInfo>

#include "imagelist.h"
#include "book.h"
#include "editor.h"

Project::Project()
{
    m_editor = NULL;
    m_images = NULL;
    m_book = NULL;
}

Project::Project(Editor* editor)
{
    m_editor = editor;
    m_images = new ImageList();
    m_book = new Book();
}

// will invalidate all previous valid pointers
Project::~Project()
{
    close();
}

// will invalidate all previous valid pointers
bool Project::load(const QString& fileName, QString* error)
{
    Q_CHECK_PTR(error);

    // make sure everything is empty
    close();

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
    {
        *error = QObject::tr("Cannot open file %1 to open project.").arg(fileName);
        return false;
    }

    QDomDocument document;
    if (!document.setContent(&file))
    {
        *error = QObject::tr("Invalid file %1.").arg(fileName);
        file.close();
        return false;
    }

    file.close();

    // set images
    QDomElement projectElement = document.firstChildElement("project");

    // check version
    if (projectElement.attribute("version", 0).toInt() != 1)
    {
        *error = QObject::tr("Invalid version in file %1.").arg(fileName);
        return false;
    }

    m_images = new ImageList(projectElement.firstChildElement("images"));
    m_book = new Book(projectElement.firstChildElement("book"));
    m_editor = createEditor(projectElement.firstChildElement("editor"));

    m_projectFileInfo = QFileInfo(fileName);
    return true;
}

bool Project::save(const QString& fileName, QString* error)
{
    Q_CHECK_PTR(m_editor);
    Q_CHECK_PTR(m_book);
    Q_CHECK_PTR(m_images);
    Q_CHECK_PTR(error);

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        *error = QObject::tr("Cannot open file %1 to save project.").arg(fileName);
        return false;
    }

    QDomDocument document;
    QDomElement projectElement = document.createElement("project");
    projectElement.setAttribute("version", 1);

    projectElement.appendChild(m_book->xml(&document));
    projectElement.appendChild(m_editor->xml(&document));
    projectElement.appendChild(m_images->xml(&document));

    document.appendChild(projectElement);

    QTextStream textStream(&file);
    textStream << document.toString() ;
    file.close();

    m_projectFileInfo = QFileInfo(fileName);
    return true;
}

QString Project::projectFileName() const
{
    return m_projectFileInfo.fileName();
}

QString Project::projectFilePath() const
{
    return m_projectFileInfo.filePath();
}

ImageList* Project::images() const
{
    return m_images;
}

Editor* Project::editor() const
{
    return m_editor;
}

Book* Project::book() const
{
    return m_book;
}

void Project::edit()
{
    Q_CHECK_PTR(m_editor);
    Q_CHECK_PTR(m_book);
    Q_CHECK_PTR(m_images);

    QList<Image const*> images;

    foreach(Image* image, *m_images)
        images.append(image);

    m_editor->edit(m_book, &images);
}

void Project::clear()
{
    Q_CHECK_PTR(m_editor);
    Q_CHECK_PTR(m_book);

    m_images->clear();
    m_book->clear();

    // do not clear layout settings
}

void Project::close()
{
    if (m_editor != NULL)
    {
        delete m_editor;
        m_editor = NULL;
    }

    if (m_images != NULL)
    {
        m_images->clear();
        delete m_images;
        m_images = NULL;
    }

    if (m_book != NULL)
    {
        delete m_book;
        m_book = NULL;
    }

    m_projectFileInfo = QFileInfo();
}

bool Project::render(const QString& fileName) const
{
    Q_CHECK_PTR(m_book);
    bool ok = m_book->render(fileName);

    ok &= m_book->renderCover(fileName + ".cover.pdf");

    return ok;
}

void Project::showDateFirstInMonth()
{
    Q_CHECK_PTR(m_editor);
    Q_CHECK_PTR(m_images);
    m_editor->showDateFirstInMonth(m_images);
}

void Project::appendImages(const QStringList& fileNames) const
{
    Q_CHECK_PTR(m_editor);
    Q_CHECK_PTR(m_images);
    m_editor->appendImages(fileNames, m_images);
}

void Project::sortByDate()
{
    Q_CHECK_PTR(m_images);
    Q_CHECK_PTR(m_images);
    m_editor->sortByDate(m_images);
}
