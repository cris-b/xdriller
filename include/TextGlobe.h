#ifndef TextGlobe_H
#define TextGlobe_H

#include <Ogre.h>
#include <OgrePanelOverlayElement.h>
#include <OgreTextAreaOverlayElement.h>

/*#define ALIGN_LEFT          0
#define ALIGN_CENTER        1
#define ALIGN_RIGHT         2*/

class TextGlobe
{
    public:
        TextGlobe();
        virtual ~TextGlobe();

        void update(unsigned long lTimeElapsed);

        void setText(Ogre::UTFString text);

        bool isVisible();
        bool isWritten();

        void show();
        void hide();
        void more();
        void close();

    private:

        Ogre::OverlayManager    *mOverlayMgr;
        Ogre::Overlay           *mOverlay;

        Ogre::PanelOverlayElement     *mPanel;
        Ogre::PanelOverlayElement     *mRedX;
        Ogre::TextAreaOverlayElement  *mText;

        Ogre::UTFString text;

        unsigned long string_lenght;

        Ogre::Real time_since_reset;

};

#endif // TextGlobe_H