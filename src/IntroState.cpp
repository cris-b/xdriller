#include "IntroState.h"

using namespace Ogre;

IntroState* IntroState::mIntroState;

void IntroState::enter( void ) {
    mRoot         = Root::getSingletonPtr();
    mOverlayMgr   = OverlayManager::getSingletonPtr();
    mSceneMgr     = mRoot->getSceneManager( "ST_GENERIC" );
    mCamera       = mSceneMgr->createCamera( "IntroCamera" );
    mViewport     = mRoot->getAutoCreatedWindow()->addViewport( mCamera );

    mIntroOverlay = mOverlayMgr->getByName( "Overlay/IntroState" );

    mIntroOverlay->show();
}

void IntroState::exit( void ) {
    mIntroOverlay->hide();

    mSceneMgr->clearScene();
    mSceneMgr->destroyAllCameras();
    mRoot->getAutoCreatedWindow()->removeAllViewports();
}

void IntroState::pause( void ) {
    mIntroOverlay->hide();
}

void IntroState::resume( void ) {
    mIntroOverlay->show();
}

void IntroState::update( unsigned long lTimeElapsed ) {
    // Update wat je moet updaten
}

void IntroState::keyPressed( const OIS::KeyEvent &e ) {
}

void IntroState::keyReleased( const OIS::KeyEvent &e ) {
    if( e.key == OIS::KC_ESCAPE ) {
        this->requestShutdown();
    }
    else {
        this->changeState( MenuState::getSingletonPtr() );
    }
}

void IntroState::mouseMoved( const OIS::MouseEvent &e ) {
}

void IntroState::mousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id ) {
}

void IntroState::mouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id ) {
    this->changeState( MenuState::getSingletonPtr() );
}

IntroState* IntroState::getSingletonPtr( void ) {
    if( !mIntroState ) {
        mIntroState = new IntroState();
    }

    return mIntroState;
}
