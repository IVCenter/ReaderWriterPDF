#ifndef PDFREADER
#define PDFREADER

#include <osg/Image>
#include <osg/Geode>


/** Hints structure that can be passed to PdfReader and VncClient classes to help guide them on what geometry to build.*/
struct GeometryHints
{
    enum AspectRatioPolicy
    {
        RESIZE_HEIGHT_TO_MAINTAINCE_ASPECT_RATIO,
        RESIZE_WIDTH_TO_MAINTAINCE_ASPECT_RATIO,
        IGNORE_DOCUMENT_ASPECT_RATIO
    };

    GeometryHints():
        position(0.0f,0.0f,0.0f),
        widthVec(1.0f,0.0f,0.0f),
        heightVec(0.0f,1.0f,0.0f),
        backgroundColor(1.0f,1.0f,1.0f,1.0f),
        aspectRatioPolicy(RESIZE_HEIGHT_TO_MAINTAINCE_ASPECT_RATIO),
        widthResolution(1024),
        heightResolution(1024) {}

    GeometryHints(const osg::Vec3& pos,
                  const osg::Vec3& wVec,
                  const osg::Vec3& hVec, 
                  const osg::Vec4& bColor,
                  AspectRatioPolicy asp=RESIZE_HEIGHT_TO_MAINTAINCE_ASPECT_RATIO,
                  unsigned int wRes=1024,
                  unsigned int hRes=1024):
        position(pos),
        widthVec(wVec),
        heightVec(hVec),
        backgroundColor(bColor),
        aspectRatioPolicy(asp),
        widthResolution(wRes),
        heightResolution(hRes) {}

    osg::Vec3           position;
    osg::Vec3           widthVec;
    osg::Vec3           heightVec;

    osg::Vec4           backgroundColor;

    AspectRatioPolicy   aspectRatioPolicy;

    unsigned int        widthResolution;
    unsigned int        heightResolution;

};

/** Pure virtual base class for interfacing with implementation of PDF reader.*/
class PdfImage : public osg::Image
{
    public:
    
        PdfImage():
            _backgroundColor(1.0f,1.0f,1.0f,1.0f),
            _pageNum(0) {}

        void setBackgroundColor(const osg::Vec4& backgroundColor) { _backgroundColor = backgroundColor; }
        const osg::Vec4& getBackgroundColor() const { return _backgroundColor; }

        int getPageNum() const { return _pageNum; }

        virtual int getNumOfPages() = 0;

        virtual bool page(int pageNum) = 0;

        bool previous()
        {
            return page(_pageNum-1);
        }

        bool next()
        { 
             return page(_pageNum+1);
        }

    protected:

        virtual ~PdfImage() {}

        osg::Vec4 _backgroundColor;

        int _pageNum;
};


/** Convinience class that provides a interactive quad that can be placed directly in the scene.*/
class PdfReader : public osg::Geode
{
    public:

        PdfReader() {}

        PdfReader(const std::string& filename, const GeometryHints& hints = GeometryHints());

        bool assign(PdfImage* pdfImage, const GeometryHints& hints = GeometryHints());

        bool open(const std::string& filename, const GeometryHints& hints = GeometryHints());

        bool page(int pageNum);

        bool previous();

        bool next();

    protected:

        osg::ref_ptr<PdfImage> _pdfImage;
};

#endif
