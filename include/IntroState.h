#ifndef IntroState_H
#define IntroState_H

#ifndef MenuState_H
#include "MenuState.h"
#endif
#ifndef GameState_H
#include "GameState.h"
#endif

class IntroState : public GameState{
public:
    ~IntroState( void );

    void enter( void );
    void exit( void );

    void pause( void );
    void resume( void );
    void update( unsigned long lTimeElapsed );

    void keyPressed( const OIS::KeyEvent &e );
    void keyReleased( const OIS::KeyEvent &e );

    void mouseMoved( const OIS::MouseEvent &e );
    void mousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id );
    void mouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id );

    static IntroState* getSingletonPtr( void );
private:
    IntroState( void ) { }
    IntroState( const IntroState& ) { }
    IntroState & operator = ( const IntroState& );

    Ogre::Root           *mRoot;
    Ogre::Camera         *mCamera;
    Ogre::SceneManager   *mSceneMgr;
    Ogre::Viewport       *mViewport;

    Ogre::SceneNode     *mLogoNode;
    Ogre::Entity        *mLogoEnt;

    Ogre::SceneNode     *mCamNode;

    Ogre::OverlayManager    *mOverlayMgr;
    Ogre::Overlay           *mOverlay;
    Ogre::OverlayContainer  *mLogoFSPanel;

    Ogre::AnimationState *mAnimState;


    static IntroState    *mIntroState;

};
#endif


