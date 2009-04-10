#include "PauseState.h"

using namespace Ogre;

PauseState* PauseState::mPauseState;

void PauseState::enter( void ) {
    mRoot            = Root::getSingletonPtr();
    mOverlayMgr      = OverlayManager::getSingletonPtr();
    mViewport        = mRoot->getAutoCreatedWindow()->getViewport( 0 );





    mPanel = static_cast<OverlayContainer*>(
        mOverlayMgr->createOverlayElement("Panel", "PauseStateOverlayPanel"));
    mPanel->setMetricsMode(Ogre::GMM_RELATIVE);
    mPanel->setPosition(0, 0);
    mPanel->setDimensions(1, 1);
    mPanel->setMaterialName("ScreenShot");

    mTextAreaDepth = static_cast<TextAreaOverlayElement*>(
        mOverlayMgr->createOverlayElement("TextArea", "PauseText"));
    mTextAreaDepth->setMetricsMode(Ogre::GMM_RELATIVE);
    mTextAreaDepth->setPosition(0.01, 0);
    mTextAreaDepth->setDimensions(0.2, 0.2);
    mTextAreaDepth->setCaption("PAUSE");
    mTextAreaDepth->setCharHeight(0.07);
    mTextAreaDepth->setFontName("CoolFont");
    //mTextAreaDepth->setColourBottom(ColourValue(0.0, 0.0, 0.0));
    //mTextAreaDepth->setColourTop(ColourValue(1, 0, 0));
    mTextAreaDepth->setColour(ColourValue(1,0,0));

    mOverlay = mOverlayMgr->create("PauseStateOverlay");
    mOverlay->add2D(mPanel);


    mPanel->addChild(mTextAreaDepth);

    // Show the overlay
    mOverlay->show();

}

void PauseState::exit( void )
{

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

void PauseState::update( unsigned long lTimeElapsed ) {
    // Update wat je moet updaten
}

void PauseState::keyPressed( const OIS::KeyEvent &e ) {
}

void PauseState::keyReleased( const OIS::KeyEvent &e ) {
    if( e.key == OIS::KC_P ) {
        this->popState();
    }
    else if( e.key == OIS::KC_ESCAPE ) {
        this->requestShutdown();
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





