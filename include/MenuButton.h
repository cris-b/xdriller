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
        MenuButton(UTFString caption, int align = ALIGN_CENTER, bool hasOption = false);
        ~MenuButton();

        void setPosition(Vector3 pos) {mNode->setPosition(pos);}
        void setPosition(Real x, Real y, Real z) {mNode->setPosition(x,y,z);}
        void setDest(Vector3 dest);
        void setDest(Real x,Real y ,Real z);

        void setState(int state);
        void setColor(Ogre::ColourValue col) {text->setColor(col);}

        void setOptionCaption(Ogre::String caption);
        String getOptionCaption() {return optionText->getCaption();}

        void setCaption(Ogre::String caption);

        Vector3 getPosition() {return mNode->getPosition();}

        float getWidth();


        void update(unsigned long lTimeElapsed);

    private:

        int hasOption;
        int align;

        ManualObject *frame;
        MovableText *text;
        MovableText *optionText;

        SceneNode *mTextNode;
        SceneNode *mOptionNode;
        SceneNode *mNode;

        String optionCaption;


        Vector3 dest,speed;
        bool dest_reached;

        int state;

};

#endif // MENUBUTTON_H
