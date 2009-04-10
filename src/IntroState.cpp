#include "IntroState.h"

using namespace Ogre;

IntroState* IntroState::mIntroState;

void IntroState::enter( void ) {
    mRoot         = Root::getSingletonPtr();

    mSceneMgr     = mRoot->getSceneManager( "ST_GENERIC" );
    mCamera       = mSceneMgr->createCamera( "IntroCamera" );
    mViewport     = mRoot->getAutoCreatedWindow()->addViewport( mCamera );


}

void IntroState::exit( void ) {


    mSceneMgr->clearScene();
    mSceneMgr->destroyAllCameras();
    mRoot->getAutoCreatedWindow()->removeAllViewports();
}

void IntroState::pause( void ) {

}

void IntroState::resume( void ) {

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
