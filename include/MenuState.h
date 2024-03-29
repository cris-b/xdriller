#ifndef MenuState_H
#define MenuState_H


#ifndef GameState_H
#include "GameState.h"
#endif

#include "MenuButton.h"
#include "Arrows.h"

#include "RingSwitcher.h"
#include "Planet.h"
#include "PlayerModelSelector.h"
#include "HighScoreTable.h"

#include "EditableText.h"


#include <vector>

class MenuState : public GameState {
public:
    virtual ~MenuState( void ) {};

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
    MenuState( void );
    MenuState( const MenuState& ) { }
    MenuState & operator = ( const MenuState& );

    void _updateLevelSelect();
    void _updateArrows(bool jump = false);

    Ogre::Root           *mRoot;
    Ogre::Camera         *mCamera;
    Ogre::SceneNode      *mCameraNode;

    Ogre::SceneManager   *mSceneMgr;
    Ogre::Viewport       *mViewport;

    Ogre::OverlayManager    *mOverlayMgr;
    Ogre::Overlay           *mOverlay;
    Ogre::OverlayContainer  *mLogoXDriller;
    Ogre::PanelOverlayElement     *mPanel;

    //Ogre::TextAreaOverlayElement  *mInfoTextArea;

    Ogre::OverlayContainer     *mLevelScreenshot;
    Ogre::OverlayContainer     *mLevelScreenshot_shadow;
    Ogre::TextAreaOverlayElement  *mLevelInfo;

    Ogre::OverlayContainer     *mBigImage; //para el teclado por el momento


    static MenuState    *mMenuState;

    std::vector<MenuButton*> buttons;

    MenuButton *titleButton;

    float tittle_alpha;

    Arrows *arrows;
    RingSwitcher *ringSwitcher;
    Planet *planet;
    PlayerModelSelector *playerModelSelector;
    HighScoreTable *highScoreTable;

    EditableText *mEditableText;


    unsigned int menuPage;
    int menuCursor;

};
#endif



