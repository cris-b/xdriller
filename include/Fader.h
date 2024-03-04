#ifndef Fader_H
#define Fader_H

#include <Ogre.h>
#include <OgreSingleton.h>
#include <OgreOverlay.h>
#include <OgreOverlayManager.h>
#include <OgrePanelOverlayElement.h>

#include "GameState.h"

#define FADE_NONE   0
#define FADE_FULL   1
#define FADE_IN     2
#define FADE_OUT    3


class Fader : public Ogre::Singleton<Fader>
{
    public:

    Fader();
    ~Fader();

    void enable();
    void disable();

    void fadeIn();
    void fadeOut();

    void setColor(float r, float g, float b);


    void update(unsigned long lTimeElapsed);

    static Fader& getSingleton(void);
    static Fader* getSingletonPtr(void);

    int getFadeState() {return fadeState;}

    void fadeToState(GameState *fadeTo) {this->fadeTo = fadeTo;}

    private:

    GameState *fadeTo;

    Ogre::OverlayManager    *mOverlayMgr;

    Ogre::Overlay                 *mFadeOverlay;
    Ogre::PanelOverlayElement     *mFadePanel;

    int fadeState;

    bool enabled;
    float r,g,b;
    float alpha;
    float speed;


};

#endif
