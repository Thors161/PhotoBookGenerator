#-------------------------------------------------
#
# Project created by QtCreator 2014-06-20T15:41:16
#
#-------------------------------------------------

QT       += core gui xml printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = photoBookGenerator
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    image.cpp \
    qexifimageheader.cpp \
    page.cpp \
    book.cpp \
    mypagelayout.cpp \
    myeditor.cpp \
    editor.cpp \
    pagelayout.cpp \
    imagewidget.cpp \
    verticalscrollarea.cpp \
    pagewidget.cpp \
    pagelistwidget.cpp \
    mypagelayoutoptionswidget.cpp \
    pagelayoutoptionswidget.cpp \
    imagelist.cpp \
    project.cpp \
    myproject.cpp \
    imagelistwidget.cpp \
    mycoverpagelayout.cpp \
    bookpropertiesdialog.cpp \
    pagelayoutpropertiesdialog.cpp \
    mycoverpagepropertieswidget.cpp

HEADERS  += mainwindow.h \
    page.h \
    image.h \
    qexifimageheader.h \
    book.h \
    pagelayout.h \
    mypagelayout.h \
    editor.h \
    myeditor.h \
    imagewidget.h \
    verticalscrollarea.h \
    pagewidget.h \
    pagelistwidget.h \
    pagelayoutoptionswidget.h \
    mypagelayoutoptionswidget.h \
    imagelist.h \
    project.h \
    myproject.h \
    imagelistwidget.h \
    mycoverpagelayout.h \
    bookpropertiesdialog.h \
    pagelayoutpropertiesdialog.h \
    mycoverpagepropertieswidget.h

FORMS    += mainwindow.ui \
    imagewidget.ui \
    pagewidget.ui \
    pagelistwidget.ui \
    mypagelayoutoptionswidget.ui \
    imagelistwidget.ui \
    bookpropertiesdialog.ui \
    pagelayoutpropertiesdialog.ui \
    mycoverpagepropertieswidget.ui

OTHER_FILES += \
    resources/ruit.png \
    pdfConverter/Blurb_ICC_Profile.icc \
    pdfConverter/convert.sh \
    pdfConverter/myPDFX_def.ps \
    pdfConverter/myPDFX_def_profile.ps

RESOURCES += \
    photobookgenerator.qrc
