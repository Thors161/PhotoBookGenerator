#ifndef PROJECT_H
#define PROJECT_H

#include <QString>
#include <QFileInfo>

class QStringList;
class QDomElement;

class ImageList;
class Editor;
class Book;

class Project
{
public:
    Project();
    virtual ~Project();

    bool load(const QString& fileName, QString* error);
    bool save(const QString& fileName, QString* error);

    QString projectFileName() const;
    QString projectFilePath() const;

    ImageList* images() const;
    Editor* editor() const;
    Book* book() const;

    void appendImages(const QStringList& fileNames) const;
    void edit();
    void clear();
    void close();

    bool render(const QString& fileName) const;

    void showDateFirstInMonth();
    void sortByDate();

protected:
    Project(Editor* editor);
    virtual Editor* createEditor(const QDomElement& editorElement) const = 0;

private:
    QFileInfo m_projectFileInfo;

    ImageList* m_images;
    Editor* m_editor;
    Book* m_book;
};

#endif // PROJECT_H
