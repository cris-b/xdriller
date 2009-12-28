#ifndef MenuButton_H
#define MenuButton_H

#include <Ogre.h>
#include <OgrePanelOverlayElement.h>
#include <OgreTextAreaOverlayElement.h>

#define BSTATE_INACTIVE     0
#define BSTATE_ACTIVE       1

#define ALIGN_LEFT          0
#define ALIGN_CENTER        1
#define ALIGN_RIGHT         2


class MenuButton
{
    public:
        MenuButton(Ogre::UTFString caption, int align = ALIGN_CENTER, bool hasOption = false, bool hasArrows = false);
        ~MenuButton();

        void setPosition(Ogre::Vector2 pos);
        void setPosition(Ogre::Real x, Ogre::Real y);
        void setDest(Ogre::Vector2 dest);
        void setDest(Ogre::Real x,Ogre::Real y);

        void setState(int state);
        void setColor(Ogre::ColourValue col);

        void setOptionCaption(Ogre::String caption);
        Ogre::String getOptionCaption() {return optionCaption;}

        void setCaption(Ogre::String caption);
        Ogre::String getCaption() {return caption;}

        Ogre::Vector2 getScreenPosition();
        Ogre::Vector2 getOptionScreenPosition();

        float getWidth();
        float getOptionWidth();

        bool hasOption() {return _hasOption;}

        void setArrows(bool trueOrFalse) {_hasArrows = trueOrFalse;}
        bool hasArrows() {return _hasArrows;}

        void setBlocked(bool _blocked);
        bool isBlocked();


        void update(unsigned long lTimeElapsed);

    private:

        bool _hasOption;
        bool _hasArrows;
        bool _isBlocked;
        int align;

        Ogre::String name;

        /*Ogre::ManualObject *frame;
        MovableText *text;
        MovableText *optionText;

        Ogre::SceneNode *mTextNode;
        Ogre::SceneNode *mOptionNode;
        Ogre::SceneNode *mNode;
        */

        Ogre::OverlayManager    *mOverlayMgr;
        Ogre::Overlay           *mOverlay;

        Ogre::PanelOverlayElement     *mPanel;
        Ogre::TextAreaOverlayElement  *mText;
        Ogre::TextAreaOverlayElement  *mOptionText;


        Ogre::String caption;
        Ogre::String optionCaption;

        Ogre::Real textWidth;
        Ogre::Real optionTextWidth;


        Ogre::Vector2 pos,dest,speed;
        bool dest_reached;

        int state;

};

#endif // MENUBUTTON_H
