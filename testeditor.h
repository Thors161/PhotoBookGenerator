#ifndef TESTEDITOR_H
#define TESTEDITOR_H

#include "editor.h"

class MyPageLayout;

class TestEditor : public Editor
{
public:
    TestEditor();
    TestEditor(int defaultPageSharing);
    TestEditor(const QDomElement& myEditorElement);
    virtual ~TestEditor();

    virtual TestEditor* clone() const;

    virtual void edit(Book* book, Image const* front, QList<Image const* >* images, Image const* back);
    virtual QDomElement xml(QDomDocument* document) const;

    virtual const QList<PageLayout*>& pageLayouts() const;

private:
    QList<PageLayout*> m_pageLayouts;
    MyPageLayout* m_myPageLayout;
};

#endif // MYEDITOR_H
