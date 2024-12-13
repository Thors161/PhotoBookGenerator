#include "myeditor.h"

#include "mypagelayout.h"
#include "mycoverpagelayout.h"

#include <QImage>

namespace
{
Page::PageSide nextSide(Page::PageSide pageSide)
{
    if (pageSide == Page::LeftPage)
        return Page::RightPage;
    else if (pageSide == Page::RightPage)
        return Page::LeftPage;

    Q_ASSERT(false);
    return Page::UnknownPage;
}

MyPageLayout* createPageLayout()
{
    MyPageLayout* myPageLayout = new MyPageLayout();
    myPageLayout->setBgColor(QColor(40, 40, 40));
    myPageLayout->setBgColor(Qt::white);
    myPageLayout->setCropping(true);
    myPageLayout->setGutterMm(30);
    myPageLayout->setSpacingMm(2);

    return myPageLayout;
}

MyCoverPageLayout* createCoverPageLayout()
{
    MyCoverPageLayout* myCoverPageLayout = new MyCoverPageLayout();

    return myCoverPageLayout;
}

}

MyEditor::MyEditor() : Editor()
{
    m_myPageLayout = createPageLayout();
    m_myCoverPageLayout = createCoverPageLayout();

    m_pageLayouts.append(m_myPageLayout);
    m_pageLayouts.append(m_myCoverPageLayout);
}

MyEditor::MyEditor(int defaultPageSharing) : Editor (defaultPageSharing)
{
    m_myPageLayout = createPageLayout();
    m_myCoverPageLayout = createCoverPageLayout();

    m_pageLayouts.append(m_myPageLayout);
    m_pageLayouts.append(m_myCoverPageLayout);
}

MyEditor::MyEditor(const MyEditor& myEditor) : Editor(myEditor)
{
    m_myPageLayout = createPageLayout();
    m_myCoverPageLayout = createCoverPageLayout();

    m_pageLayouts.append(m_myPageLayout);
    m_pageLayouts.append(m_myCoverPageLayout);
}

MyEditor::MyEditor(const QDomElement& editorElement)
{
    QDomElement myEditorElement = editorElement.firstChildElement("myEditor");
    QDomElement myPageLayoutElement = myEditorElement.firstChildElement("myPageLayout");
    QDomElement myCoverPageLayoutElement = myEditorElement.firstChildElement("myCoverPageLayout");

    m_myPageLayout = new MyPageLayout(myPageLayoutElement);
    m_myCoverPageLayout = new MyCoverPageLayout(myCoverPageLayoutElement);

    m_pageLayouts.append(m_myPageLayout);
    m_pageLayouts.append(m_myCoverPageLayout);
}

MyEditor::~MyEditor()
{
}

MyEditor* MyEditor::clone() const
{
    return new MyEditor(*this);
}

const QList<PageLayout*>& MyEditor::pageLayouts() const
{
    return m_pageLayouts;
}

void MyEditor::edit(Book* book, QList<Image const*>* images)
{
    Q_CHECK_PTR(book);
    Q_CHECK_PTR(images);

    book->clear();

    // create cover page
    Page coverPage;
    coverPage.setProperties(book->coverPageWidthMm(), book->coverPageHeightMm(), book->trimMm());
    coverPage.setPagelayout(m_myCoverPageLayout);
    coverPage.setPageSide(Page::CoverPage);
    book->setCoverPage(coverPage);

    // fill pages of book
    Page::PageSide currentSide = Page::RightPage;
    Page page;
    page = book->emptyPage();
    page.setPageSide(currentSide);
    page.setPagelayout(m_myPageLayout);

    foreach(Image const* image, *images)
    {
        Q_CHECK_PTR(image);

        if (!image->include())
            continue;

        // we are not allowed two or three portrait images on a page
        if ((page.portraitImagesCount() == 2) && page.imagesCount() == 2)
        {
            if (image->portait())
            {
                // already two portrait images on page, put next on a new page
                book->addPage(page);
                page = book->emptyPage();
                currentSide = nextSide(currentSide);
                page.setPageSide(currentSide);
                page.setPagelayout(m_myPageLayout);
            }
        }

        // we are not allowed two landscape and one portrait on a page
        if ((page.landscapeImagesCount() == 2) && page.imagesCount() == 2)
        {
            if (image->portait())
            {
                book->addPage(page);
                page = book->emptyPage();
                currentSide = nextSide(currentSide);
                page.setPageSide(currentSide);
                page.setPagelayout(m_myPageLayout);
            }
        }

        // we are not allowed two landscape and one portrait on a page
        if ((page.landscapeImagesCount() == 1) && (page.portraitImagesCount() == 1) && page.imagesCount() == 2)
        {
            if (!image->portait())
            {
                book->addPage(page);
                page = book->emptyPage();
                currentSide = nextSide(currentSide);
                page.setPageSide(currentSide);
                page.setPagelayout(m_myPageLayout);
            }
        }

        // check sharing
        if (image->pageSharing() < page.imagesCount())
        {
            if ((page.portraitImagesCount() == 2) && page.imagesCount() == 2)
            {
                // problem, two portrait images remained on page! put first portrait on one page
                Image const* takenImage = page.takeImage(1);

                book->addPage(page);
                page = book->emptyPage();
                currentSide = nextSide(currentSide);
                page.setPageSide(currentSide);
                page.setPagelayout(m_myPageLayout);

                // now maybe the next is a landscape again
                page.addImage(takenImage);
            }

            // new page, current page is too crowded for the new image
            book->addPage(page);
            page = book->emptyPage();
            currentSide = nextSide(currentSide);
            page.setPageSide(currentSide);
            page.setPagelayout(m_myPageLayout);
        }

        if (image->firstOnPage() && page.imagesCount() > 0)
        {
            // new page, requested new page
            if ((page.portraitImagesCount() == 2) && page.imagesCount() == 2)
            {
                // problem, two portrait images remained on page! put first portrait on one page
                Image const* takenImage = page.takeImage(1);

                book->addPage(page);
                page = book->emptyPage();
                currentSide = nextSide(currentSide);
                page.setPageSide(currentSide);
                page.setPagelayout(m_myPageLayout);

                // now maybe the next is a landscape again
                page.addImage(takenImage);
            }

            book->addPage(page);
            page = book->emptyPage();
            currentSide = nextSide(currentSide);
            page.setPageSide(currentSide);
            page.setPagelayout(m_myPageLayout);
        }

        page.addImage(image);

        if (page.imagesCount() > image->pageSharing())
        {
            // new page, we cannot share more images
            if ((page.portraitImagesCount() == 2) && page.imagesCount() == 2)
            {
                // problem, two portrait images remained on page! put first portrait on one page
                Image const* takenImage = page.takeImage(1);

                book->addPage(page);
                page = book->emptyPage();
                currentSide = nextSide(currentSide);
                page.setPageSide(currentSide);
                page.setPagelayout(m_myPageLayout);

                // now maybe the next is a landscape again
                page.addImage(takenImage);
            }

            book->addPage(page);
            page = book->emptyPage();
            currentSide = nextSide(currentSide);
            page.setPageSide(currentSide);
            page.setPagelayout(m_myPageLayout);
        }
        else if (page.imagesCount() >= 3)
        {
            // new page, max n images per page
            book->addPage(page);
            page = book->emptyPage();
            currentSide = nextSide(currentSide);
            page.setPageSide(currentSide);
            page.setPagelayout(m_myPageLayout);
        }
    }

    if (page.imagesCount() > 0)
    {
        book->addPage(page);
        page = book->emptyPage();
        currentSide = nextSide(currentSide);
        page.setPageSide(currentSide);
        page.setPagelayout(m_myPageLayout);
    }

    // now the current page is always blank.
    Q_ASSERT(page.imagesCount() == 0);

    // always end with left page
    if (currentSide == Page::LeftPage)
    {
        // when the current page is a left page: add it, it is the last page.
        // when it is a right page, we have already ended with the left page
        book->addPage(page);
    }


/*
    // testing
    // single portrait
    page = book->emptyPage();
    page.addImage(images->at(0));
    page.setPageSide(Page::LeftPage);
    page.setPagelayout(m_myPageLayout);
    book->addPage(page);

    page = book->emptyPage();
    page.addImage(images->at(0));
    page.setPageSide(Page::RightPage);
    page.setPagelayout(m_myPageLayout);
    book->addPage(page);

    // single landscape
    page = book->emptyPage();
    page.addImage(images->at(2));
    page.setPageSide(Page::LeftPage);
    page.setPagelayout(m_myPageLayout);
    book->addPage(page);

    page = book->emptyPage();
    page.addImage(images->at(2));
    page.setPageSide(Page::RightPage);
    page.setPagelayout(m_myPageLayout);
    book->addPage(page);

    // double landscape
    page = book->emptyPage();
    page.addImage(images->at(2));
    page.addImage(images->at(2));
    page.setPageSide(Page::LeftPage);
    page.setPagelayout(m_myPageLayout);
    book->addPage(page);

    page = book->emptyPage();
    page.addImage(images->at(2));
    page.addImage(images->at(2));
    page.setPageSide(Page::RightPage);
    page.setPagelayout(m_myPageLayout);
    book->addPage(page);

    // double potrait
    page = book->emptyPage();
    page.addImage(images->at(0));
    page.addImage(images->at(0));
    page.setPageSide(Page::LeftPage);
    page.setPagelayout(m_myPageLayout);
    book->addPage(page);

    page = book->emptyPage();
    page.addImage(images->at(0));
    page.addImage(images->at(0));
    page.setPageSide(Page::RightPage);
    page.setPagelayout(m_myPageLayout);
    book->addPage(page);

    // lanscape and portrait
    page = book->emptyPage();
    page.addImage(images->at(0));
    page.addImage(images->at(2));
    page.setPageSide(Page::LeftPage);
    page.setPagelayout(m_myPageLayout);
    book->addPage(page);

    page = book->emptyPage();
    page.addImage(images->at(0));
    page.addImage(images->at(2));
    page.setPageSide(Page::RightPage);
    page.setPagelayout(m_myPageLayout);
    book->addPage(page);

    page = book->emptyPage();
    page.addImage(images->at(2));
    page.addImage(images->at(0));
    page.setPageSide(Page::LeftPage);
    page.setPagelayout(m_myPageLayout);
    book->addPage(page);

    page = book->emptyPage();
    page.addImage(images->at(2));
    page.addImage(images->at(0));
    page.setPageSide(Page::RightPage);
    page.setPagelayout(m_myPageLayout);
    book->addPage(page);

    // triple landscape
    page = book->emptyPage();
    page.addImage(images->at(2));
    page.addImage(images->at(2));
    page.addImage(images->at(2));
    page.setPageSide(Page::LeftPage);
    page.setPagelayout(m_myPageLayout);
    book->addPage(page);

    page = book->emptyPage();
    page.addImage(images->at(2));
    page.addImage(images->at(2));
    page.addImage(images->at(2));
    page.setPageSide(Page::RightPage);
    page.setPagelayout(m_myPageLayout);
    book->addPage(page);

    // 1 landscape 2 portrait
    page = book->emptyPage();
    page.addImage(images->at(2));
    page.addImage(images->at(0));
    page.addImage(images->at(0));
    page.setPageSide(Page::LeftPage);
    page.setPagelayout(m_myPageLayout);
    book->addPage(page);

    page = book->emptyPage();
    page.addImage(images->at(2));
    page.addImage(images->at(0));
    page.addImage(images->at(0));
    page.setPageSide(Page::RightPage);
    page.setPagelayout(m_myPageLayout);
    book->addPage(page);

    // 1 landscape 2 portrait
    page = book->emptyPage();
    page.addImage(images->at(0));
    page.addImage(images->at(2));
    page.addImage(images->at(0));
    page.setPageSide(Page::LeftPage);
    page.setPagelayout(m_myPageLayout);
    book->addPage(page);

    page = book->emptyPage();
    page.addImage(images->at(0));
    page.addImage(images->at(2));
    page.addImage(images->at(0));
    page.setPageSide(Page::RightPage);
    page.setPagelayout(m_myPageLayout);
    book->addPage(page);

    // 1 landscape 2 portrait
    page = book->emptyPage();
    page.addImage(images->at(0));
    page.addImage(images->at(0));
    page.addImage(images->at(2));
    page.setPageSide(Page::LeftPage);
    page.setPagelayout(m_myPageLayout);
    book->addPage(page);

    page = book->emptyPage();
    page.addImage(images->at(0));
    page.addImage(images->at(0));
    page.addImage(images->at(2));
    page.setPageSide(Page::RightPage);
    page.setPagelayout(m_myPageLayout);
    book->addPage(page);


*/
//    book->setBack(book->emptyPage().addImage(back));
}

QDomElement MyEditor::xml(QDomDocument* document) const
{
    Q_CHECK_PTR(document);

    QDomElement myEditorElement = document->createElement("myEditor");
    myEditorElement.appendChild(m_myPageLayout->xml(document));
    myEditorElement.appendChild(m_myCoverPageLayout->xml(document));

    QDomElement editorElement = document->createElement("editor");
    editorElement.appendChild(myEditorElement);

    return editorElement;
}
