#ifndef MYEDITOR_H
#define MYEDITOR_H

#include "editor.h"

class MyPageLayout;
class MyCoverPageLayout;

class MyEditor : public Editor
{
public:
    MyEditor();
    MyEditor(int defaultPageSharing);
    MyEditor(const MyEditor& myEditor);
    MyEditor(const QDomElement& myEditorElement);
    virtual ~MyEditor();

    virtual MyEditor* clone() const;

    virtual void edit(Book* book, QList<Image const* >* images);
    virtual QDomElement xml(QDomDocument* document) const;

    virtual const QList<PageLayout*>& pageLayouts() const;

private:
    QList<PageLayout*> m_pageLayouts;
    MyPageLayout* m_myPageLayout;
    MyCoverPageLayout* m_myCoverPageLayout;
};

#endif // MYEDITOR_H
