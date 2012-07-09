#include <fifechan/image.hpp>
#include <fifechan/exception.hpp>
#include <fifechan/graphics.hpp>
#include <fifechan/widgets/container.hpp>
#include <fifechan/widgets/icon.hpp>
#include <fifechan/widgets/iconprogressbar.hpp>

namespace fcn
{
    IconProgressBar::IconProgressBar()
    :
    mImage(NULL),
    mInternalImage(false),
    mMaxIcons(0),
    mIconCounter(0),
    mOrientation(HORIZONTAL),
    mShouldReset(false)
    {
        mContents = new Container;
        mContents->setPosition(0, 0);
        add(mContents);
    }
    
    IconProgressBar::IconProgressBar(Image* image, int maxIcons)
    :
    mImage(image),
    mInternalImage(false),
    mMaxIcons(maxIcons),
    mIconCounter(0),
    mOrientation(HORIZONTAL),
    mShouldReset(false)
    {
        mContents = new Container;
        mContents->setPosition(0, 0);
        add(mContents);
        
        mIcons.reserve(maxIcons);
        
        createIcons(maxIcons);
        arrangeIcons();
    }
    
    IconProgressBar::IconProgressBar(const std::string& filename, int maxIcons)
    :
    mImage(NULL),
    mInternalImage(true),
    mMaxIcons(maxIcons),
    mOrientation(HORIZONTAL),
    mShouldReset(false)
    {
        mImage = Image::load(filename);
        
        mContents = new Container;
        mContents->setPosition(0, 0);
        add(mContents);
        
        mIcons.reserve(maxIcons);
        
        createIcons(maxIcons);
        arrangeIcons();
    }
    
    IconProgressBar::~IconProgressBar()
    {
        delete mContents;
        destroyIcons();
        
        if(mInternalImage)
        {
            delete mImage;
        }
    }
    
    void IconProgressBar::draw(Graphics* graphics)
    {
        mContents->draw(graphics);
    }
    
    Rectangle IconProgressBar::getChildrenArea()
    {
        return mContents->getChildrenArea();
    }
    
    void IconProgressBar::setOpaque(bool opaque)
    {
        mContents->setOpaque(opaque);
    }
    
    bool IconProgressBar::isOpaque() const
    {
        return mContents->isOpaque();
    }
    
    void IconProgressBar::setImage(Image* image)
    {
        if(mInternalImage)
        {
            delete mImage;
        }
        
        mInternalImage = false;
        mImage = image;
        
        for(int i = 0; i < mMaxIcons; i++)
        {
            mIcons[i]->setImage(mImage);
        }
        
        arrangeIcons();
    }
    
    const Image* IconProgressBar::getImage() const
    {
        return mImage;
    }
    
    void IconProgressBar::setMaxIcons(int maxIcons)
    {
        mMaxIcons = maxIcons;
        
        destroyIcons();
        createIcons(maxIcons);
        arrangeIcons();
    }
    
    int IconProgressBar::getMaxIcons() const
    {
        return mMaxIcons;
    }
    
    void IconProgressBar::setOrientation(Orientation orientation)
    {
        mOrientation = orientation;
        
        arrangeIcons();
    }
    
    IconProgressBar::Orientation IconProgressBar::getOrientation() const
    {
        return mOrientation;
    }
    
    void IconProgressBar::advance()
    {
        if(mMaxIcons == 0)
            return;
        
        if(mShouldReset)
        {
            reset();
            mShouldReset = false;
        }
        
        mIcons[mIconCounter]->setVisible(true);
        
        mIconCounter = (mIconCounter + 1) % (mMaxIcons);
        
        if(mIconCounter == 0)
        {
            mShouldReset = true;
        }
    }
    
    void IconProgressBar::reset()
    {
        mIconCounter = 0;
        
        for(int i = 0; i < mMaxIcons; i++)
        {
            mIcons[i]->setVisible(false);
        }
    }
    
    void IconProgressBar::createIcons(int iconCount)
    {
        for(int i = 0; i < iconCount; i++)
        {
            Icon *icon = mImage? new Icon(mImage) : new Icon;
            icon->setVisible(false);
            mIcons.push_back(icon);
            mContents->add(mIcons[i]);
        }
        
        arrangeIcons();
    }
    
    void IconProgressBar::destroyIcons()
    {   
        int totalIcons = mIcons.size();
        for(int i = 0; i < totalIcons; i++)
        {
            mContents->remove(mIcons[i]);
            delete mIcons[i];
        }
        
        mIcons.clear();
    }
    
    void IconProgressBar::arrangeIcons()
    {
        int lastPos = 0;
        
        for(int i = 0; i < mMaxIcons; i++)
        {            
            if(mOrientation == HORIZONTAL)
            {
                mIcons[i]->setPosition(lastPos, 0);
                lastPos = lastPos + mIcons[i]->getWidth();
            }
            else if(mOrientation == VERTICAL)
            {
                mIcons[i]->setPosition(0, lastPos);
                lastPos = lastPos + mIcons[i]->getHeight();
            }
            else
            {
                throw FCN_EXCEPTION("Unknown orientation type in IconProgressBar object");
            }
        }
        
        mContents->resizeToContent();
        resizeToChildren();
    }
};