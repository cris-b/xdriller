#ifndef CreditsState_H
#define CreditsState_H



#ifndef GameState_H
#include "GameState.h"
#endif
#ifndef MenuState_H
#include "MenuState.h"
#endif

class CreditsState : public GameState {
public:
    ~CreditsState( void );

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

    static CreditsState* getSingletonPtr( void );
private:
    CreditsState( void ) { }
    CreditsState( const CreditsState& ) { }
    CreditsState & operator = ( const CreditsState& );

    Ogre::Root           *mRoot;
    Ogre::Camera         *mCamera;
    Ogre::SceneManager   *mSceneMgr;
    Ogre::Viewport       *mViewport;

    Ogre::SceneNode     *mKarolaNode;
    Ogre::Entity        *mKarolaEnt;

    Ogre::SceneNode     *mCamNode;

    Ogre::OverlayManager    *mOverlayMgr;
    Ogre::Overlay           *mOverlay;

    Ogre::Overlay                 *mFadeOverlay;
    Ogre::PanelOverlayElement     *mFadePanel;

    float fade_alpha;

    static CreditsState    *mCreditsState;
};
#endif




