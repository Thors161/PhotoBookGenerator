#include "testeditor.h"

#include "mypagelayout.h"

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
}

TestEditor::TestEditor() : Editor()
{
    m_myPageLayout = createPageLayout();
    // only one layout
    m_pageLayouts.append(m_myPageLayout);
}

TestEditor::TestEditor(int defaultPageSharing) : Editor (defaultPageSharing)
{
    m_myPageLayout = createPageLayout();
    // only one layout
    m_pageLayouts.append(m_myPageLayout);
}

TestEditor::TestEditor(const TestEditor& myEditor) : Editor(myEditor)
{
    m_myPageLayout = myEditor.m_myPageLayout->clone();

    // only one layout
    m_pageLayouts.append(m_myPageLayout);
}

TestEditor::TestEditor(const QDomElement& editorElement)
{
    QDomElement myEditorElement = editorElement.firstChildElement("myEditor");
    QDomElement myPageLayoutElement = myEditorElement.firstChildElement("myPageLayout");

    m_myPageLayout = new MyPageLayout(myPageLayoutElement);

    // only one layout
    m_pageLayouts.append(m_myPageLayout);
}

TestEditor::~TestEditor()
{
}

TestEditor* TestEditor::clone() const
{
    return new TestEditor(*this);
}

const QList<PageLayout*>& TestEditor::pageLayouts() const
{
    return m_pageLayouts;
}

void TestEditor::edit(Book* book, Image const* front, QList<Image const*>* images, Image const*  back)
{
    Q_CHECK_PTR(book);
    Q_CHECK_PTR(images);

    book->clear();

    // sort chronologic
//    qSort(images->begin(), images->end(), qLess<Image>());


//    book->setFront(book->emptyPage().addImage(front));

    Page::PageSide currentSide = Page::LeftPage;
    Page page;
    page = book->emptyPage();
    page.setPageSide(currentSide);
    page.setPagelayout(m_myPageLayout);

    /*

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
                // problem! put first portrait on one page
                Image const* takenImage = page.takeImage(1);

                book->addPage(page);
                page = book->emptyPage();
                currentSide = nextSide(currentSide);
                page.setPageSide(currentSide);
                page.setPagelayout(m_myPageLayout);

                // now maybe the next is a landscape again
                page.addImage(takenImage);
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
        book->addPage(page);

    // todo: remove last page if empty
*/

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



//    book->setBack(book->emptyPage().addImage(back));
}

QDomElement TestEditor::xml(QDomDocument* document) const
{
    Q_CHECK_PTR(document);

    QDomElement myEditorElement = document->createElement("myEditor");
    myEditorElement.appendChild(m_myPageLayout->xml(document));

    QDomElement editorElement = document->createElement("editor");
    editorElement.appendChild(myEditorElement);

    return editorElement;
}
