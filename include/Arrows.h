#ifndef Arrows_H
#define Arrows_H


#include <Ogre.h>
#include <OgrePanelOverlayElement.h>


class Arrows
{
    public:
    Arrows();
    ~Arrows();

    void setPosition(float x, float y);
    void setSize(float s);

    void show();
    void hide();

    void left();
    void right();

    void update(unsigned long lTimeElapsed);

    private:

    float x,y,w,h,s;
    float s2;
    float ar,al;
    float min_alpha;

    Ogre::OverlayManager    *mOverlayMgr;
    Ogre::Overlay           *mOverlay;
    Ogre::PanelOverlayElement     *arrowLeft;
    Ogre::PanelOverlayElement     *arrowRight;

};

#endif
