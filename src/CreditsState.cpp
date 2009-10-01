#include "CreditsState.h"
#include "SoundManager.h"

using namespace Ogre;

CreditsState* CreditsState::mCreditsState;

void CreditsState::enter( void ) {
    mRoot         = Root::getSingletonPtr();
    mOverlayMgr   = OverlayManager::getSingletonPtr();
    mSceneMgr     = mRoot->getSceneManager( "ST_GENERIC" );
    mCamera       = mSceneMgr->createCamera( "CreditsCamera" );
    mViewport     = mRoot->getAutoCreatedWindow()->addViewport( mCamera );

    mViewport->setBackgroundColour(ColourValue(0.0,0.7,0.8));

    mSceneMgr->setAmbientLight(ColourValue(1.0,1.0,1.0));

    //mSceneMgr->setFog(FOG_LINEAR, ColourValue(1,1,1), 0.0, 30, 100);

    mCamera->setNearClipDistance(1);
    mCamera->setFarClipDistance(10000);

    Light *light = mSceneMgr->createLight("CreditsLight");
    light->setType(Light::LT_DIRECTIONAL);
    light->setDirection(Vector3(0.5, -1.0, -1));
    light->setDiffuseColour(1.0, 1.0, 1.0);
    light->setSpecularColour(0.5, 0.5, 0.5);


    mCamNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("CreditsCamNode",Vector3(0,0,20));

    mCamNode->attachObject(mCamera);

    mCamNode->lookAt(Vector3(0,0,0),Ogre::SceneNode::TS_WORLD);


    mKarolaEnt = mSceneMgr->createEntity("KarolaEnt", "karola.mesh");

    mKarolaNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("KarolaNode");
    mKarolaNode->rotate(Quaternion(Degree(-90),Vector3::UNIT_X));
    mKarolaNode->rotate(Quaternion(Degree(70),Vector3::UNIT_Z));
    mKarolaNode->setPosition(0,-7,0);

    mKarolaNode->attachObject(mKarolaEnt);

    Entity *mNubeEnt = mSceneMgr->createEntity("NubeEnt", "nube.mesh");

    SceneNode *mNubeNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("NubeNode");

    mNubeNode->attachObject(mNubeEnt);

    mNubeNode->setPosition(0,-1.7,0);

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


    mOverlay = mOverlayMgr->create("CreditsOverlay");

    mFadePanel = static_cast<PanelOverlayElement*>(
        mOverlayMgr->createOverlayElement("Panel", "CreditsFadeOverlayPanel"));
    mFadePanel->setMetricsMode(Ogre::GMM_RELATIVE);
    mFadePanel->setPosition(0, 0);
    mFadePanel->setDimensions(1, 1);
    mFadePanel->setMaterialName("fade_material");

    mFadeOverlay = mOverlayMgr->create("CreditsFadeOverlay");
    mFadeOverlay->add2D(mFadePanel);

    fade_alpha = 1.0;


    mOverlay->setZOrder(100);
    mFadeOverlay->setZOrder(101);

    // Show the overlay
    mOverlay->show();
    mFadeOverlay->show();


}

void CreditsState::exit( void )
{
    mOverlayMgr->destroyAllOverlayElements();

    mOverlayMgr->destroy(mOverlay);
    mOverlayMgr->destroy(mFadeOverlay);

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
    if(fade_alpha > 0)
    {
        fade_alpha -= (float) lTimeElapsed / 1000.0;

		Ogre::ResourcePtr resptr = Ogre::MaterialManager::getSingleton().getByName("fade_material");
		Ogre::Material * mat = dynamic_cast<Ogre::Material*>(resptr.getPointer());

		mat->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setAlphaOperation(LBX_MODULATE, LBS_MANUAL, LBS_TEXTURE, fade_alpha);
    }
    else
    {
        fade_alpha = 0;
        mFadeOverlay->hide();
    }
}

void CreditsState::keyPressed( const OIS::KeyEvent &e ) {
}

void CreditsState::keyReleased( const OIS::KeyEvent &e )
{
    if( e.key == OIS::KC_ESCAPE )
    {
        //this->requestShutdown();
        this->changeState( MenuState::getSingletonPtr() );
    }
    if( e.key == OIS::KC_Q )
    {
        this->requestShutdown();
        //this->changeState( MenuState::getSingletonPtr() );
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

