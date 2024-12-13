#ifndef MYPROJECT_H
#define MYPROJECT_H

#include <project.h>

class MyProject : public Project
{
public:
    MyProject();
    ~MyProject();

protected:
    Editor* createEditor(const QDomElement& editorElement) const;
};

#endif // MYPROJECT_H
