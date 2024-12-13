#include "myproject.h"
#include "myeditor.h"

#define DEFAULT_PAGE_SHARING 3

MyProject::MyProject() : Project(new MyEditor(DEFAULT_PAGE_SHARING))
{
}

MyProject::~MyProject()
{
}

Editor* MyProject::createEditor(const QDomElement& editorElement) const
{
    MyEditor* myEditor = new MyEditor(editorElement);
    myEditor->setDefaultPageSharing(DEFAULT_PAGE_SHARING);
    return myEditor;
}


