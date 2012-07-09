#ifndef FCN_ICONPROGRESSBAR_HPP
#define FCN_ICONPROGRESSBAR_HPP

#include <fifechan/widget.hpp>

#include <vector>

namespace fcn
{
    class Container;
    class Graphics;
    class Icon;
    class Image;
    
    class FCN_CORE_DECLSPEC IconProgressBar : public Widget
    {
    public:
        
        enum Orientation 
        {
            HORIZONTAL = 0,
            VERTICAL
        };
        
        /**
         * Default constructor.
         */
        IconProgressBar();

        /**
         * Constructor. The image passed is not owned by this object.
         * 
         * @param image Image used by the progress bar's icons.
         * @param maxIcons Count of icons when the progress bar is full.
         */
        IconProgressBar(Image* image, int maxIcons);
        
        /**
         * Constructor. The image indicated by filename is opened and it's
         * owned by this object.
         * 
         * @param filename Filename of the image to be used by the progress bar's icons.
         * @param maxIcons Count of icons when the progress bar is full.
         */
        IconProgressBar(const std::string& filename, int maxIcons);
        
        /**
         * Destructor.
         */
        virtual ~IconProgressBar();

        /**
         * Draws this IconProgressBar.
         */
        virtual void draw(Graphics* graphics);
        
        /**
         * Returns children area of this IconProgressBar.
         */
        virtual Rectangle getChildrenArea();
        
        /**
         * Sets the IconProgressBar's image. The image passed is not owned by
         * this object.
         * 
         * @param image Image used by the progress bar's icons. 
         * @see setMaxIcons
         */
        void setImage(Image* image);

        /**
         * @return The image used by this IconProgressBar.
         */
        const Image* getImage() const;
        
        /**
         * Sets count of icons when the progress bar is full.
         * Also creates enough icons to represent that.
         * 
         * @param maxIcons desired maximum count of icons.
         */
        void setMaxIcons(int maxIcons);
        
        /**
         * @return Maximum count of icons for this IconProgressBar.
         */
        int getMaxIcons() const;
        
        /**
         * Sets the IconProgressBar's orientation.
         * 
         * @param orientation Desired orientation.
         */
        void setOrientation(Orientation orientation);
        
        /**
         * @return Orientation of this IconProgressBar.
         */
        Orientation getOrientation() const;
        
        /**
         * Advances the progress bar to use one more icon. If there can be no advancement
         * (maxIcons is reached), then the counter is reset to 0, useful if client wishes
         * to make the IconProgressBar looping.
         */
        void advance();
        
        /**
         * Resets the progress bar.
         */
        void reset();
        
    protected:
        
        /**
         * Creates icons needed by this progress bar.
         * Makes them invisible.
         * 
         * @param iconCount How many icons to create.
         */
        void createIcons(int iconCount);
     
        /**
         * Destroys icons created for this progress bar.
         */
        void destroyIcons();
     
        /**
         * Arranges icon positions in their container and
         * resizes the container to fit them.
         */
        void arrangeIcons();
        
        /**
         * Image used by the progress bar.
         */
        const Image* mImage;
        
        /**
         * True if the image has been loaded internally, false otherwise.
         * An image not loaded internally should not be deleted in the
         * destructor.
         */
        bool mInternalImage;
        
        /**
         * Count of icons when progress bar is full.
         */
        int mMaxIcons;
        
        /**
         * Holds how many icons are currently displayed.
         */
        int mIconCounter;
        
        /**
         * IconProgressBar's orientation.
         */
        Orientation mOrientation;
        
        /**
         * Holds if the progress bar should be reset during the next advancement.
         */
        bool mShouldReset;
        
        /**
         * Container holding the icon objects.
         */
        Container* mContents;
        
        /**
         * Vector holding the icons used for fast access.
         */
        std::vector<Icon*> mIcons;
    };
};

#endif //FCN_ICONPROGRESSBAR_HPP