#ifndef MenuState_H
#define MenuState_H

#ifndef PlayState_H
#include "PlayState.h"
#endif
#ifndef GameState_H
#include "GameState.h"
#endif

#include "MenuButton.h"

#include <vector>


#define NUM_MENU_BRICKS         10

#define MENU_PAGE_MAIN                  0
#define MENU_PAGE_OPTIONS               1
#define MENU_PAGE_CREDITS               2
#define MENU_PAGE_QUIT                  3
#define MENU_PAGE_GAMESELECT            4
#define MENU_PAGE_GRAPHIC_OPTIONS       5
#define MENU_PAGE_AUDIO_OPTIONS         6

class MenuState : public GameState {
public:
    ~MenuState( void );

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

    void changePage(unsigned int page);

    static MenuState* getSingletonPtr( void );
private:
    MenuState( void ) { }
    MenuState( const MenuState& ) { }
    MenuState & operator = ( const MenuState& );

    Ogre::Root           *mRoot;
    Ogre::Camera         *mCamera;
    Ogre::SceneManager   *mSceneMgr;
    Ogre::Viewport       *mViewport;

    Ogre::OverlayManager    *mOverlayMgr;
    Ogre::Overlay           *mOverlay;
    Ogre::OverlayContainer  *mLogoXDriller;
    PanelOverlayElement     *mPanel;
    TextAreaOverlayElement  *mInfoTextArea;

    Overlay                 *mFadeOverlay;
    PanelOverlayElement     *mFadePanel;

    float fade_alpha;


    Ogre::SceneNode *mBrickNode[NUM_MENU_BRICKS];
    Ogre::Entity *mBrickEnt[NUM_MENU_BRICKS];
    Vector3 mBrickSpeed[NUM_MENU_BRICKS];

    static MenuState    *mMenuState;

    std::vector<MenuButton*> buttons;

    MenuButton *titleButton;

    unsigned int menuPage;
    int menuCursor;

};
#endif



