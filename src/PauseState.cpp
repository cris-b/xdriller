#include "PauseState.h"
#include "Gettext.h"
#include "ConfigManager.h"

using namespace Ogre;

PauseState* PauseState::mPauseState;

void PauseState::enter( void ) {
    mRoot            = Root::getSingletonPtr();
    mOverlayMgr      = OverlayManager::getSingletonPtr();
    mViewport        = mRoot->getAutoCreatedWindow()->getViewport( 0 );
    mSceneMgr         = mRoot->getSceneManager( "ST_GENERIC" );


    mPanel = static_cast<OverlayContainer*>(
        mOverlayMgr->createOverlayElement("Panel", "PauseStateOverlayPanel"));
    mPanel->setMetricsMode(Ogre::GMM_RELATIVE);
    mPanel->setPosition(0, 0);
    mPanel->setDimensions(1, 1);

    mTextAreaDepth = static_cast<TextAreaOverlayElement*>(
        mOverlayMgr->createOverlayElement("TextArea", "PauseText"));
    mTextAreaDepth->setMetricsMode(Ogre::GMM_RELATIVE);
    mTextAreaDepth->setPosition(0.5, 0.35);
    mTextAreaDepth->setDimensions(0.6, 0.3);
    mTextAreaDepth->setCaption(_("PAUSE"));
    mTextAreaDepth->setCharHeight(0.3);
    mTextAreaDepth->setFontName("CoolFont");
    mTextAreaDepth->setAlignment(TextAreaOverlayElement::Center);
    //mTextAreaDepth->setColourBottom(ColourValue(0.0, 0.0, 0.0));
    //mTextAreaDepth->setColourTop(ColourValue(1, 0, 0));
    mTextAreaDepth->setColour(ColourValue(0,0,0,0.5));

    mOverlay = mOverlayMgr->create("PauseStateOverlay");
    mOverlay->add2D(mPanel);


    mPanel->addChild(mTextAreaDepth);

    // Show the overlay
    mOverlay->show();


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

    mPanel->removeChild("PauseText");
    mOverlayMgr->destroyOverlayElement(mTextAreaDepth);

    mOverlay->remove2D(mPanel);
    mOverlayMgr->destroyOverlayElement(mPanel);


    mOverlayMgr->destroy(mOverlay);

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
    sleep(10);
    #endif

}

void PauseState::keyPressed( const OIS::KeyEvent &e ) {
}

void PauseState::keyReleased( const OIS::KeyEvent &e ) {
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





