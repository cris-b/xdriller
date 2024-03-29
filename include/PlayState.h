#ifndef PlayState_H
#define PlayState_H

#include <Ogre.h>//#include <OgreCamera.h>
#include <OgreTextAreaOverlayElement.h>
#include <OgrePanelOverlayElement.h>
#include "Cam.h"
#include "Board.h"
#include "Player.h"
#include "Globals.h"
#include "TextEffector.h"
#include "TextGlobe.h"

#ifndef GameState_H
#include "GameState.h"
#endif
#ifndef MenuState_H
#include "MenuState.h"
#endif



class PlayState : public GameState {
public:

    virtual ~PlayState( void ) {};

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

    void nextBoard();

    static PlayState* getSingletonPtr( void );
private:
    PlayState( void );
    PlayState( const PlayState& ) { }
    PlayState & operator = ( const PlayState& );

    Ogre::Root           *mRoot;
    Ogre::Camera         *mCamera;
    Ogre::SceneNode      *mCameraNode;
    Ogre::SceneNode      *mCameraDestNode;

    Ogre::SceneManager   *mSceneMgr;
    Ogre::Viewport       *mViewport;

    Ogre::SceneNode *backgroundSceneNode;


    Ogre::OverlayManager    *mOverlayMgr;
    Ogre::Overlay                 *mOverlay;
    Ogre::PanelOverlayElement     *mPanel;
    Ogre::TextAreaOverlayElement  *mTextAreaDepth;
    //Ogre::TextAreaOverlayElement  *mTextAreaPoints;
    Ogre::TextAreaOverlayElement  *mTextAreaClock;
    Ogre::TextAreaOverlayElement  *mTextAreaLives;
    Ogre::TextAreaOverlayElement  *mTextAreaTotal;
    Ogre::PanelOverlayElement     *mArrow;
    Ogre::PanelOverlayElement     *mSkull;
    Ogre::PanelOverlayElement     *mScore;
    Ogre::PanelOverlayElement     *mClock;
    Ogre::PanelOverlayElement     *mLivesPanel;
    Ogre::PanelOverlayElement     *mBottle;
    Ogre::PanelOverlayElement     *mBottleAir;
    Ogre::PanelOverlayElement     *mBottleAlarm;

    #if XDRILLER_DEBUG == 1
    Ogre::TextAreaOverlayElement  *mTextAreaDebug;
    #endif

    int boardNum;
    bool tiempoBala;
    bool nextFramePause;
    int depthAccumulation;
    int depth;
    //int pointsAccumulation;
    //int points;
    unsigned int tutorial_page_number;

    long int gameTime;

    long int gameSeconds;

    int finished;

    OIS::Keyboard        *mKeyboard;
    OIS::JoyStick        *mJoystick;

    Cam *mCam;

    Board *mBoard;
    Player *mPlayer;

    TextEffector *textEffector;

    TextGlobe *textGlobe;

    static PlayState *mPlayState;

    void destroyOverlayElements();
};
#endif


