#ifndef MenuButton_H
#define MenuButton_H

#include "MovableText.h"

#define BSTATE_INACTIVE     0
#define BSTATE_ACTIVE       1


class MenuButton
{
    public:
        MenuButton(UTFString caption);
        ~MenuButton();

        void setPosition(Vector3 pos) {mNode->setPosition(pos);}
        void setPosition(Real x, Real y, Real z) {mNode->setPosition(x,y,z);}
        void setDest(Vector3 dest);
        void setDest(Real x,Real y ,Real z);

        void setState(int state);


        void update(unsigned long lTimeElapsed);

    private:

    ManualObject *frame;
    MovableText *text;

    SceneNode *mTextNode;
    SceneNode *mNode;

    Vector3 dest,speed;
    bool dest_reached;

    int state;

};

#endif // MENUBUTTON_H
