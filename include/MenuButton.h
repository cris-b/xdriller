#ifndef MenuButton_H
#define MenuButton_H

#include "MovableText.h"

#define BSTATE_INACTIVE     0
#define BSTATE_ACTIVE       1

#define ALIGN_LEFT          0
#define ALIGN_CENTER        1


class MenuButton
{
    public:
        MenuButton(Ogre::UTFString caption, int align = ALIGN_CENTER, bool hasOption = false, bool hasArrows = false);
        ~MenuButton();

        void setPosition(Ogre::Vector3 pos) {mNode->setPosition(pos);}
        void setPosition(Ogre::Real x, Ogre::Real y, Ogre::Real z) {mNode->setPosition(x,y,z);}
        void setDest(Ogre::Vector3 dest);
        void setDest(Ogre::Real x,Ogre::Real y ,Ogre::Real z);

        void setState(int state);
        void setColor(Ogre::ColourValue col) {text->setColor(col);}

        void setOptionCaption(Ogre::String caption);
        Ogre::String getOptionCaption() {return optionText->getCaption();}

        void setCaption(Ogre::String caption);

        Ogre::Vector3 getPosition() {return mNode->getPosition();}
        Ogre::Vector2 getScreenPosition();
        Ogre::Vector2 getOptionScreenPosition();

        float getWidth();
        float getOptionWidth();

        bool hasOption() {return _hasOption;}

        void setArrows(bool trueOrFalse) {_hasArrows = trueOrFalse;}
        bool hasArrows() {return _hasArrows;}


        void update(unsigned long lTimeElapsed);

    private:

        bool _hasOption;
        bool _hasArrows;
        int align;

        Ogre::ManualObject *frame;
        MovableText *text;
        MovableText *optionText;

        Ogre::SceneNode *mTextNode;
        Ogre::SceneNode *mOptionNode;
        Ogre::SceneNode *mNode;

        Ogre::String optionCaption;


        Ogre::Vector3 dest,speed;
        bool dest_reached;

        int state;

};

#endif // MENUBUTTON_H
