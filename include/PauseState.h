#ifndef PauseState_H
#define PauseState_H


#ifndef GameState_H
#include "GameState.h"
#endif


#include "MenuButton.h"

class PauseState : public GameState {
public:
    ~PauseState( void ) { }

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

    static PauseState* getSingletonPtr( void );
private:
    PauseState( void ) { }
    PauseState( const PauseState& ) { }
    PauseState & operator = ( const PauseState& );

    Ogre::Root           *mRoot;
    Ogre::Camera         *mCamera;
    Ogre::SceneManager   *mSceneMgr;
    Ogre::Viewport       *mViewport;

    Ogre::Rectangle2D* backgroundRect;
    Ogre::SceneNode* backgroundNode;

    std::vector<MenuButton*> buttons;

    MenuButton *titleButton;

    int menuCursor;

    static PauseState    *mPauseState;
};
#endif


