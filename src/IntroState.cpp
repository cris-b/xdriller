#include "IntroState.h"
#include "SoundManager.h"


using namespace Ogre;

IntroState* IntroState::mIntroState;

void IntroState::enter( void )
{


    mRoot         = Root::getSingletonPtr();
    mOverlayMgr    = OverlayManager::getSingletonPtr();
    mSceneMgr     = mRoot->getSceneManager( "ST_GENERIC" );
    mCamera       = mSceneMgr->createCamera( "IntroCamera" );
    mViewport     = mRoot->getAutoCreatedWindow()->addViewport( mCamera );

    mViewport->setBackgroundColour(ColourValue(1,1,1));

    mSceneMgr->setAmbientLight(ColourValue(1.0,1.0,1.0));

    mCamera->setNearClipDistance(0.1);
    mCamera->setFarClipDistance(100);




    mCamNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("IntroCamNode",Vector3(0,-3,25));

    mCamNode->attachObject(mCamera);


    mLogoEnt = mSceneMgr->createEntity("LogoFS3d_Ent", "logofs3d.mesh");
    //mLogoEnt->setRenderQueueGroup(RENDER_QUEUE_MAX);

    mLogoNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("LogoFS3d_Node");
    mLogoNode->rotate(Quaternion(Degree(-90),Vector3::UNIT_X));
    mLogoNode->setPosition(0,0,36);

    mLogoNode->attachObject(mLogoEnt);

    //mCamera->setAutoTracking(true,mLogoNode);


    // set up spline animation of node
    Animation* anim = mSceneMgr->createAnimation("CameraTrack", 10);
    // Spline it for nice curves
    anim->setInterpolationMode(Animation::IM_SPLINE);
    // Create a track to animate the camera's node
    NodeAnimationTrack* track = anim->createNodeTrack(0, mCamNode);
    // Setup keyframes
    TransformKeyFrame* key = track->createNodeKeyFrame(0); // startposition
    key = track->createNodeKeyFrame(0.0);
    key->setTranslate(Vector3(-10,0,0));
    /*key = track->createNodeKeyFrame(5);
    key->setTranslate(Vector3(-10,10,-6));*/
    key = track->createNodeKeyFrame(7.5);
    key->setTranslate(Vector3(-5,-1,0));
    key = track->createNodeKeyFrame(10);
    key->setTranslate(Vector3(0,-3,26));
    // Create a new animation state to track this
    mAnimState = mSceneMgr->createAnimationState("CameraTrack");
    //mAnimState->setEnabled(true);
    mAnimState->setLoop(false);








    mLogoFSPanel = static_cast<OverlayContainer*>(
        mOverlayMgr->createOverlayElement("Panel", "LogoFSPanel"));
    mLogoFSPanel->setMetricsMode(Ogre::GMM_RELATIVE);
    mLogoFSPanel->setPosition(0.26796875, 0.65);
    mLogoFSPanel->setDimensions(0.4640625, 0.139583333);
    mLogoFSPanel->setMaterialName("logoFS");


    mOverlay = mOverlayMgr->create("IntroStateOverlay");
    mOverlay->add2D(mLogoFSPanel);

    SoundManager::getSingleton().loadMusic("intro_music.ogg");
    SoundManager::getSingleton().playMusic(false);





}

void IntroState::exit( void )
{

    mOverlay->remove2D(mLogoFSPanel);

    mOverlayMgr->destroyOverlayElement(mLogoFSPanel);


    mOverlayMgr->destroy(mOverlay);

    mSceneMgr->clearScene();
    mSceneMgr->destroyAllCameras();
    mRoot->getAutoCreatedWindow()->removeAllViewports();
}

void IntroState::pause( void ) {

}

void IntroState::resume( void ) {

}

void IntroState::update( unsigned long lTimeElapsed )
{
    static float alpha = 0;


    mAnimState->addTime(lTimeElapsed*0.001);

    if(mLogoNode->getPosition().z > 0)
    {

        mLogoNode->setPosition(mLogoNode->getPosition().x,mLogoNode->getPosition().y,mLogoNode->getPosition().z-0.01*lTimeElapsed);
        if(mLogoNode->getPosition().z < 0)
        {
            mLogoNode->setPosition(0,0,0);

        }

        mLogoNode->resetOrientation();
        mLogoNode->rotate(Quaternion(Degree(-90),Vector3::UNIT_X));
        mLogoNode->roll(Degree(360 - mLogoNode->getPosition().z*10.0));
    }
    else if(alpha < 1)
    {




        mOverlay->show();

        MaterialPtr FadeMaterial = MaterialManager::getSingleton().getByName("logoFS");
        TextureUnitState *FadeTextureLayer = FadeMaterial->getTechnique(0)->getPass(0)->getTextureUnitState(0);
        FadeMaterial->getTechnique(0)->getPass(0)->setSceneBlending(SBT_TRANSPARENT_ALPHA);

        FadeTextureLayer->setAlphaOperation(LBX_MODULATE, LBS_MANUAL, LBS_TEXTURE, alpha);

        alpha+=lTimeElapsed*0.001;
        if(alpha > 1)
        {
            alpha = 1;
        }
    }
    else if(alpha == 1)
    {


        fadeState(MenuState::getSingletonPtr());
    }


}

void IntroState::keyPressed( const OIS::KeyEvent &e ) {
    if( e.key == OIS::KC_RETURN )
    {
        //this->requestShutdown();
        //this->changeState( GAMESTATE_MENU );
        fadeState(MenuState::getSingletonPtr());
    }
}

void IntroState::keyReleased( const OIS::KeyEvent &e ) {
    if( e.key == OIS::KC_ESCAPE)
    {
        //this->requestShutdown();

        //changingState = GAMESTATE_MENU;
        fadeState(MenuState::getSingletonPtr());
    }

}

void IntroState::mouseMoved( const OIS::MouseEvent &e ) {
}

void IntroState::mousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id ) {
}

void IntroState::mouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id ) {
    //this->fadeState( MenuState::getSingletonPtr() );
}

IntroState* IntroState::getSingletonPtr( void ) {
    if( !mIntroState ) {
        mIntroState = new IntroState();
    }

    return mIntroState;
}
