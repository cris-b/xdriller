#include "HighScoreState.h"

#ifndef MenuState_H
#include "MenuState.h"
#endif

using namespace Ogre;

HighScoreState* HighScoreState::mHighScoreState;

void HighScoreState::enter( void ) {
    mRoot         = Root::getSingletonPtr();
    mOverlayMgr   = OverlayManager::getSingletonPtr();
    mSceneMgr     = mRoot->getSceneManager( "ST_GENERIC" );
    mCamera       = mSceneMgr->createCamera( "HighScoreCamera" );
    mViewport     = mRoot->getAutoCreatedWindow()->addViewport( mCamera );

    mViewport->setBackgroundColour(ColourValue(0.0,0.7,0.8));

    mSceneMgr->setAmbientLight(ColourValue(0.5,0.5,0.5));

    //mSceneMgr->setFog(FOG_LINEAR, ColourValue(1,1,1), 0.0, 30, 100);

    mCamera->setNearClipDistance(1);
    mCamera->setFarClipDistance(1000);

    Light *light = mSceneMgr->createLight("HighScoreLight");
    light->setType(Light::LT_DIRECTIONAL);
    light->setDirection(Vector3(0.5, -1.0, -1));
    light->setDiffuseColour(0.6, 0.6, 0.6);
    light->setSpecularColour(0.8, 0.8, 0.8);



    mCamNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("HighScoreCamNode",Vector3(0,0,15));

    mCamNode->attachObject(mCamera);

    //mCamNode->lookAt(Vector3(0,-10,0),Ogre::SceneNode::TS_WORLD);


    //mOverlay = mOverlayMgr->getByName("HighScoreStateOverlay");


    //mOverlay->setZOrder(100);


    // Show the overlay
    //mOverlay->show();



}

void HighScoreState::exit( void )
{
    mSceneMgr->destroyAllCameras();
    mSceneMgr->clearScene();
    mRoot->getAutoCreatedWindow()->removeAllViewports();
}

void HighScoreState::pause( void )
{

}

void HighScoreState::resume( void )
{

}

void HighScoreState::update( unsigned long lTimeElapsed )
{

}

void HighScoreState::keyPressed( const OIS::KeyEvent &e )
{

}

void HighScoreState::keyReleased( const OIS::KeyEvent &e )
{

    if( e.key == OIS::KC_ESCAPE || e.key == OIS::KC_RETURN)
    {
        fadeState(MenuState::getSingletonPtr());
    }

}

void HighScoreState::mouseMoved( const OIS::MouseEvent &e )
{

}

void HighScoreState::mousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id )
{
}

void HighScoreState::mouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id )
{

}

HighScoreState* HighScoreState::getSingletonPtr( void )
{
    if( !mHighScoreState ) {
        mHighScoreState = new HighScoreState();
    }

    return mHighScoreState;
}
