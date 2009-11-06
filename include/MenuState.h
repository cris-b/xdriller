#ifndef MenuState_H
#define MenuState_H

#ifndef PlayState_H
#include "PlayState.h"
#endif
#ifndef CreditsState_H
#include "CreditsState.h"
#endif
#ifndef GameState_H
#include "GameState.h"
#endif

#include "MenuButton.h"
#include "Arrows.h"

#include "RingSwitcher.h"

#include <vector>




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

    void _updateLevelSelect();
    void _updateArrows(bool jump = false);

    Ogre::Root           *mRoot;
    Ogre::Camera         *mCamera;
    Ogre::SceneManager   *mSceneMgr;
    Ogre::Viewport       *mViewport;

    Ogre::OverlayManager    *mOverlayMgr;
    Ogre::Overlay           *mOverlay;
    Ogre::OverlayContainer  *mLogoXDriller;
    PanelOverlayElement     *mPanel;

    TextAreaOverlayElement  *mInfoTextArea;

    PanelOverlayElement     *mLevelScreenshot;
    PanelOverlayElement     *mLevelScreenshot_shadow;
    TextAreaOverlayElement  *mLevelInfo;

    static MenuState    *mMenuState;

    std::vector<MenuButton*> buttons;

    MenuButton *titleButton;

    float tittle_alpha;

    Arrows *arrows;
    RingSwitcher *ringSwitcher;

    unsigned int menuPage;
    int menuCursor;

    int changingState;

};
#endif



