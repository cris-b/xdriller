#ifndef GameManager_H
#define GameManager_H

#include <Ogre.h>//#include <OgreRoot.h>
#include <OgreConfigFile.h>
//#include <OgreRenderWindow.h>
//#include <OgrePlatformManager.h>//yys
#include <OgreWindowEventUtilities.h>

#define OIS_DYNAMIC_LIB
#include <OIS.h>
//#include <OISMouse.h>
//#include <OISKeyboard.h>


#ifndef InputManager_H
#include "InputManager.h"
#endif



class GameState;

class GameManager : public OIS::KeyListener, OIS::MouseListener, OIS::JoyStickListener,  public OgreBites::WindowEventListener {
public:
    ~GameManager( void );

    void startGame( GameState *gameState );

    void changeState( GameState *gameState );
    void pushState( GameState *gameState );
    void popState( void );
    void requestShutdown( void );

    static GameManager* getSingletonPtr( void );

    Ogre::RenderTexture *screenshotRenderTexture;

private:
    GameManager( void );
    GameManager( const GameManager& ) { }
    GameManager & operator = ( const GameManager& );

    bool configureGame( void );
    void setupResources( void );

    bool keyPressed( const OIS::KeyEvent &e );
    bool keyReleased( const OIS::KeyEvent &e );

    bool mouseMoved( const OIS::MouseEvent &e );
    bool mousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id );
    bool mouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id );

    bool buttonPressed( const OIS::JoyStickEvent &arg, int button );
    bool buttonReleased( const OIS::JoyStickEvent &arg, int button );
    bool axisMoved( const OIS::JoyStickEvent &arg, int axis );

    Ogre::Root         *mRoot;
    Ogre::RenderWindow *mRenderWindow;
    InputManager       *mInputMgr;

    Ogre::String        configPath;


    GameState          *mIntroState;
    GameState          *mPlayState;
    GameState          *mPauseState;
    GameState          *mMenuState;
    GameState          *mCreditsState;
    GameState          *mHighScoreState;

    bool bShutdown;
    std::vector<GameState*> mStates;
    static GameManager *mGameManager;
};
#endif


