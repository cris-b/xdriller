#include "PauseState.h"
#include "Gettext.h"
#include "ConfigManager.h"
#include "SoundManager.h"

using namespace Ogre;

PauseState* PauseState::mPauseState;

void PauseState::enter( void ) {
    mRoot            = Root::getSingletonPtr();
    //mOverlayMgr      = OverlayManager::getSingletonPtr();
    mViewport        = mRoot->getAutoCreatedWindow()->getViewport( 0 );
    mSceneMgr         = mRoot->getSceneManager( "ST_GENERIC" );





    // Create background rectangle covering the whole screen
    backgroundRect = new Rectangle2D(true);
    backgroundRect->setCorners(-1.0, 1.0, 1.0, -1.0);
    backgroundRect->setMaterial("ScreenShot");

    // Render the background before everything else
    backgroundRect->setRenderQueueGroup(RENDER_QUEUE_BACKGROUND);

    // Use infinite AAB to always stay visible
    AxisAlignedBox aabInf;
    aabInf.setInfinite();
    backgroundRect->setBoundingBox(aabInf);

    // Attach background to the scene
    backgroundNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("BackgroundNode");
    backgroundNode->attachObject(backgroundRect);


    if(ConfigManager::getSingleton().getInt("compositors"))
    {
        CompositorManager::getSingleton().addCompositor(mViewport, "gaussian_blur");
        CompositorManager::getSingleton().setCompositorEnabled(mViewport, "gaussian_blur", true);
    }

    titleButton = new MenuButton(_("Pause"));
    titleButton->setPosition(0,-0.45);
    titleButton->setColor(ColourValue(1,0,0));


    buttons.push_back(new MenuButton(_("Continue")));

    buttons[0]->setPosition(0,-0.10);
    buttons[0]->setState(BSTATE_ACTIVE);

    buttons.push_back(new MenuButton(_("Quit")));

    buttons[1]->setPosition(0,0);

    menuCursor = 0;

    SoundManager::getSingleton().setMusicVolume(SoundManager::getSingleton().getMusicVolume() / 3);


}

void PauseState::exit( void )
{
    if(ConfigManager::getSingleton().getInt("compositors"))
    {
        CompositorManager::getSingleton().setCompositorEnabled(mViewport, "gaussian_blur", false);
    }
    backgroundNode->detachAllObjects();

    mSceneMgr->destroySceneNode(backgroundNode);

    delete backgroundRect;

    while (!buttons.empty())
    {
        delete buttons.back();  buttons.pop_back();
    }
    if(titleButton != NULL)
    {
        delete titleButton;
        titleButton = NULL;
    }

    SoundManager::getSingleton().setMusicVolume(SoundManager::getSingleton().getMusicVolume() * 3);


}

void PauseState::pause( void ) {

}

void PauseState::resume( void ) {

}

void PauseState::update( unsigned long lTimeElapsed )
{

    #if defined OIS_WIN32_PLATFORM
    // como se sleepea en windows???
    #elif defined OIS_LINUX_PLATFORM
    //sleep(10);
    #endif

}

void PauseState::keyPressed( const OIS::KeyEvent &e )
{

    if(e.key == OIS::KC_DOWN)
    {

        buttons[menuCursor]->setState(BSTATE_INACTIVE);
        menuCursor++;
        if(menuCursor >= (int)buttons.size())
        {
            menuCursor = 0;

        }
        buttons[menuCursor]->setState(BSTATE_ACTIVE);

        if(buttons.size() > 1)SoundManager::getSingleton().playSound(SOUND_MENU1);



    }
    if(e.key == OIS::KC_UP)
    {

        buttons[menuCursor]->setState(BSTATE_INACTIVE);
        menuCursor--;
        if(menuCursor < 0)
        {
            menuCursor = buttons.size()-1;

        }
        buttons[menuCursor]->setState(BSTATE_ACTIVE);

        if(buttons.size() > 1)SoundManager::getSingleton().playSound(SOUND_MENU1);

    }

    if(e.key == OIS::KC_RETURN)
    {
        if(menuCursor == 0) this->popState();
        if(menuCursor == 1)
        {
            this->popState();
            this->fadeState( MenuState::getSingletonPtr() );
        }
    }
}

void PauseState::keyReleased( const OIS::KeyEvent &e )
{
    if( e.key == OIS::KC_P ) {
        this->popState();
    }
    else if( e.key == OIS::KC_ESCAPE ) {
        this->popState();
    }
}

void PauseState::mouseMoved( const OIS::MouseEvent &e ) {

}

void PauseState::mousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id ) {
}

void PauseState::mouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id ) {
}

PauseState* PauseState::getSingletonPtr( void ) {
    if( !mPauseState ) {
        mPauseState = new PauseState();
    }

    return mPauseState;
}





