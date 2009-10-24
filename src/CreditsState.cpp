#include "CreditsState.h"
#include "SoundManager.h"
#include "Tools.h"


using namespace Ogre;

CreditsState* CreditsState::mCreditsState;

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
    light->setDirection(Vector3(0.5, -1.0, -1));
    light->setDiffuseColour(0.6, 0.6, 0.6);
    light->setSpecularColour(0.8, 0.8, 0.8);



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

    MaterialPtr fondoNubesMat = MaterialManager::getSingleton().create("FondoNubes", "General");
    fondoNubesMat->getTechnique(0)->getPass(0)->createTextureUnitState("fondonubes.jpg");
    fondoNubesMat->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    fondoNubesMat->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
    fondoNubesMat->getTechnique(0)->getPass(0)->setLightingEnabled(false);
    fondoNubesMat->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setScrollAnimation(-0.01, 0.0);


    Rectangle2D* fondoNubesRect = new Rectangle2D(true);
    fondoNubesRect->setCorners(-1.0, 1.0, 1.0, -1.0);
    fondoNubesRect->setMaterial("FondoNubes");

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

   OverlayElement* mText = OverlayManager::getSingleton().getOverlayElement("CreditsOverlay/Text");
   mText->setCaption(findAndReplace(mText->getCaption(),"</br>","\n"));

    mText = OverlayManager::getSingleton().getOverlayElement("CreditsOverlay/Text2");
   mText->setCaption(findAndReplace(mText->getCaption(),"</br>","\n"));

    OverlayElement* mMobilePanel = OverlayManager::getSingleton().getOverlayElement("CreditsOverlay/MobilePanel");
    mMobilePanel->setTop(1);


    /*mPanel = static_cast<PanelOverlayElement*>(
        mOverlayMgr->createOverlayElement("Panel", "CreditsOverlayPanel"));
    mPanel->setMetricsMode(Ogre::GMM_RELATIVE);
    mPanel->setPosition(0.0, 0.0);
    mPanel->setDimensions(1.0, 1.0);

    mTextArea = static_cast<TextAreaOverlayElement*>(
        mOverlayMgr->createOverlayElement("TextArea", "CreditsTextArea"));
    mTextArea->setMetricsMode(Ogre::GMM_RELATIVE);
    mTextArea->setPosition(0.5, 0.0);
    mTextArea->setDimensions(0.5, 0.5);
    mTextArea->setCaption("Credits:\n\nThis game was brouht to you\nby Fixi Studios.\n\nPowered by:");
    mTextArea->setCharHeight(0.050);
    mTextArea->setFontName("CoolFont");
    //mTextArea->setColourTop(ColourValue(1, 0.5, 0.0));
    //mTextArea->setColourBottom(ColourValue(1.0, 1.0, 1.0,0.9));
    mTextArea->setColour(ColourValue(1.0,1.0,1.0));
    mTextArea->setAlignment(TextAreaOverlayElement::Center);



    mOverlay->add2D(mPanel);

    mPanel->addChild(mTextArea);
*/




    mOverlay->setZOrder(100);


    // Show the overlay
    mOverlay->show();



}

void CreditsState::exit( void )
{
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

