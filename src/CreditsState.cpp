#include "CreditsState.h"
#include "SoundManager.h"
#include "Tools.h"

#ifndef MenuState_H
#include "MenuState.h"
#endif

#include "Gettext.h"

#include <OgreRectangle2D.h>

using namespace Ogre;

CreditsState* CreditsState::mCreditsState;

CreditsState::CreditsState()
{
    mKarolaNode = NULL;
    mKarolaEnt = NULL;
    mKarolaAnimState = NULL;
    mCamNode = NULL;
    mOverlay = NULL;
}

void CreditsState::enter( void ) {
    mRoot         = Root::getSingletonPtr();
    mOverlayMgr   = OverlayManager::getSingletonPtr();
    mSceneMgr     = mRoot->getSceneManager( "ST_GENERIC" );
    mCamera       = mSceneMgr->createCamera( "CreditsCamera" );
    mViewport     = mRoot->getAutoCreatedWindow()->addViewport( mCamera );

    mViewport->setBackgroundColour(ColourValue(0.0,0.7,0.8));

    mSceneMgr->setAmbientLight(ColourValue(0.5,0.5,0.5));

    //mSceneMgr->setFog(FOG_LINEAR, ColourValue(1,1,1), 0.0, 30, 100);

    mCamera->setNearClipDistance(1);
    mCamera->setFarClipDistance(1000);

    Light *light = mSceneMgr->createLight("CreditsLight");
    light->setType(Light::LT_DIRECTIONAL);
    light->setDiffuseColour(0.6, 0.6, 0.6);
    light->setSpecularColour(0.8, 0.8, 0.8);

    SceneNode *lightNode = mSceneMgr->getRootSceneNode()->createChildSceneNode( "LightNode" );
    lightNode->attachObject(light);
    lightNode->setDirection(Vector3(0.5, -1.0, -1));



    mCamNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("CreditsCamNode",Vector3(0,0,15));

    mCamNode->attachObject(mCamera);

    //mCamNode->lookAt(Vector3(0,-10,0),Ogre::SceneNode::TS_WORLD);




    mKarolaEnt = mSceneMgr->createEntity("KarolaEnt", "karola.mesh");

    mKarolaNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("KarolaNode");
    //mKarolaNode->rotate(Quaternion(Degree(-90),Vector3::UNIT_X));
    mKarolaNode->rotate(Quaternion(Degree(-90),Vector3::UNIT_Y));
    mKarolaNode->setPosition(-6,-7,-10);

    mKarolaNode->attachObject(mKarolaEnt);

    mKarolaAnimState = mKarolaEnt->getAnimationState("rotate");
    mKarolaAnimState->setLoop(false);
    mKarolaAnimState->setEnabled(true);

    /*Entity *mNubeEnt = mSceneMgr->createEntity("NubeEnt", "nube.mesh");

    SceneNode *mNubeNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("NubeNode");

    //mNubeNode->attachObject(mNubeEnt);

    mNubeNode->setPosition(0,-1.7,0);*/



    Rectangle2D* fondoNubesRect = new Rectangle2D(true);
    fondoNubesRect->setCorners(-1.0, 1.0, 1.0, -1.0);
    fondoNubesRect->setMaterial(MaterialManager::getSingleton().getByName("fondo_nubes"));

    fondoNubesRect->setRenderQueueGroup(RENDER_QUEUE_BACKGROUND);

    AxisAlignedBox aabInf;
    aabInf.setInfinite();
    fondoNubesRect->setBoundingBox(aabInf);

    SceneNode* fondoNubesNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("FondoNubes");
    fondoNubesNode->attachObject(fondoNubesRect);


    //mOverlay = mOverlayMgr->create("CreditsOverlay");
    //if(mOverlayMgr-> mOverlayMgr->parseScript("credits.overlay","General");
    mOverlay = mOverlayMgr->getByName("CreditsOverlay");
    //mOverlay->scroll(0,0.1);

    String c_title = ""; // "^6"
    String c_text  = ""; // "^0"

    String s_codemusicart = c_title + _("Code, Music and Artwork")  + "\n\n" + c_text + "durmieu" + "\n\n";
    String s_translators  = c_title + _("Translators") + "\n\n" + c_text + _("Spanish") + ": Ana\n" + _("Basque") +  ": Eneko & Maddi\n" + _("French") +  ": Vincent Petry" + "\n\n";
    String s_powered      = c_title + _("Powered by")  + "\n\n\n\n" + c_text + "OIS" + "\n" + "SDL_mixer" + "\n\n";
    String s_artprogs     = c_title + _("Art made with") + "\n\n" + c_text + "Blender" + "\n" + "The Gimp" + "\n\n";
    String s_musicprogs   = c_title + _("Music with") + "\n\n" + c_text + "Acid Pro" + "\n\n";
    String s_thanksto     = c_title + _("Thanks to") + "\n\n" + c_text + "futurist\n" + "ogre3d.org community" + "\n\n";;
    String s_license      = c_title + _("License");

    String s_fullcredits =  s_codemusicart + s_translators + s_powered + s_artprogs + s_musicprogs + s_thanksto + s_license ;

   OverlayElement* mText = OverlayManager::getSingleton().getOverlayElement("CreditsOverlay/Text");
   mText->setCaption(s_fullcredits);

    //mText = OverlayManager::getSingleton().getOverlayElement("CreditsOverlay/Text2");
   //mText->setCaption(findAndReplace(mText->getCaption(),"</br>","\n"));

    OverlayElement* mMobilePanel = OverlayManager::getSingleton().getOverlayElement("CreditsOverlay/MobilePanel");
    mMobilePanel->setTop(1);

    mOverlay->setZOrder(100);


    // Show the overlay
    mOverlay->show();

    SoundManager::getSingleton().loadMusic("credits_music.ogg");
    SoundManager::getSingleton().playMusic(true);



}

void CreditsState::exit( void )
{
    //TODO: Hay que destruir los overlays!!!


    //mOverlayMgr->destroyAllOverlayElements();

    //mOverlayMgr->destroy(mOverlay);

    mOverlay->hide();




    mSceneMgr->destroyAllCameras();
    mSceneMgr->clearScene();
    mRoot->getAutoCreatedWindow()->removeAllViewports();
}

void CreditsState::pause( void ) {

}

void CreditsState::resume( void ) {

}

void CreditsState::update( unsigned long lTimeElapsed )
{
    static float karolaSpeed;

    OverlayElement* mMobilePanel = OverlayManager::getSingleton().getOverlayElement("CreditsOverlay/MobilePanel");
    mMobilePanel->setTop(mMobilePanel->getTop()-0.00005*lTimeElapsed);
    //if(mMobilePanel->getTop() < -2) mMobilePanel->setTop(-2);


    mKarolaAnimState->addTime(0.00002*lTimeElapsed);

    if(!mKarolaAnimState->hasEnded())
    {
        mKarolaAnimState->addTime(0.00004*lTimeElapsed);
        karolaSpeed = 0;
    }
    else if (mKarolaNode->getPosition().z == -10)
    {
        if(karolaSpeed < 0.0003) karolaSpeed += 0.0003*lTimeElapsed/1000.0;
        else karolaSpeed = 0.0003;

        if(mKarolaNode->getPosition().x > 20)
        {
            mKarolaNode->translate(0,0,8);
            karolaSpeed = -karolaSpeed;
            mKarolaAnimState->setEnabled(false);
        }
    }
    else if (mKarolaNode->getPosition().z == -2)
    {
        if(mKarolaNode->getPosition().x < -20)
        {
            mKarolaNode->translate(0,0,-8);
            karolaSpeed = -karolaSpeed;
            mKarolaAnimState->setEnabled(true);
        }

    }

    mKarolaNode->translate(lTimeElapsed*karolaSpeed,0,0);


    //esto pa cuando tengo una nuve volumetrica de esas
    //mCamNode->setPosition(mCamNode->getPosition().x,mCamNode->getPosition().y-((mCamNode->getPosition().y/2.0)*(lTimeElapsed/1000.0)),mCamNode->getPosition().z);

}

void CreditsState::keyPressed( const OIS::KeyEvent &e ) {
}

void CreditsState::keyReleased( const OIS::KeyEvent &e )
{
    if( e.key == OIS::KC_ESCAPE )
    {
        //this->requestShutdown();
        //this->changeState( GAMESTATE_MENU );
        fadeState(MenuState::getSingletonPtr());
    }
    if( e.key == OIS::KC_Q )
    {
        this->requestShutdown();
        //this->fadeState( MenuState::getSingletonPtr() );
    }
}

void CreditsState::mouseMoved( const OIS::MouseEvent &e ) {
}

void CreditsState::mousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id ) {
}

void CreditsState::mouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id ) {

}

CreditsState* CreditsState::getSingletonPtr( void )
{
    if( !mCreditsState ) {
        mCreditsState = new CreditsState();
    }

    return mCreditsState;
}

