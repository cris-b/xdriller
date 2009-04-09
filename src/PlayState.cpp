
#include "PlayState.h"
#include "DotScene.h"
#include "Tools.h"
#include "SoundManager.h"



#include <string>
#include <vector>





using namespace Ogre;

PlayState* PlayState::mPlayState;



void PlayState::enter( void ) {
    mRoot             = Root::getSingletonPtr();
    mOverlayMgr       = OverlayManager::getSingletonPtr();
    mInputDevice      = InputManager::getSingletonPtr()->getKeyboard();
    mSceneMgr         = mRoot->getSceneManager( "ST_GENERIC" );
    //mSceneMgr = mRoot->createSceneManager("DotSceneOctreeManager");
    tiempoBala = false;

    /*CDotScene *dotScene;
    dotScene = new CDotScene();
    dotScene->parseDotScene("Scene.xml","General",mSceneMgr, NULL, "");
*/


    mCamera           = mSceneMgr->createCamera( "Camera" );
    mCamera->setNearClipDistance(0.1);
    mCamera->setFarClipDistance(1000);


    mViewport   = mRoot->getAutoCreatedWindow()->addViewport( mCamera );
    mViewport->setBackgroundColour(ColourValue(0.95,0.95,1));
    mSceneMgr->setFog(FOG_LINEAR, ColourValue(0.95,0.95,1), 0.0, 10, 40);

    mCameraNode = mSceneMgr->getRootSceneNode()->createChildSceneNode( "CameraNode" , Vector3(0,2,0));
    mCameraNode->attachObject(mCamera);
    mCameraDestNode = mSceneMgr->getRootSceneNode()->createChildSceneNode( "CameraDestNode" , Vector3(0,0,0));

    mCam = new Cam(mCamera, mCameraNode,mCameraDestNode);





    mSceneMgr->setAmbientLight(ColourValue(0.5,0.5,0.5));

    Light *light = mSceneMgr->createLight("Light1");
    light->setType(Light::LT_POINT);
    light->setPosition(Vector3(10, 0, 5));


    light->setDiffuseColour(1.0, 1.0, 1.0);
    light->setSpecularColour(1.0, 1.0, 1.0);
    light->setAttenuation(200,1.0,0.022,0.0019);

    mCam->getSceneNode()->attachObject(light);

    SceneNode *fondoNode = mSceneMgr->getRootSceneNode()->createChildSceneNode( "fondo" , Vector3(0,0,0));
    Entity *fondoEnt = mSceneMgr->createEntity("fondo", "fondo_roca.mesh");
    fondoNode->attachObject(fondoEnt);

    fondoNode->rotate(Quaternion(Degree(-90),Vector3::UNIT_X));


    mBoard = new Board(0,0,"test");

    mPlayer = new Player(mBoard);

    mPlayer->setPosition(0,10,0);
    mCam->setPosition(0,mPlayer->getPosition().y,0);


    //prueba de sonido

    SoundManager::getSingleton().loadMusic("music.ogg");
    SoundManager::getSingleton().playMusic();

     //LogManager::getSingleton().logMessage(DumpNodes(mSceneMgr->getRootSceneNode()).c_str());

    mPanel = static_cast<OverlayContainer*>(
        mOverlayMgr->createOverlayElement("Panel", "PlayStateOverlayPanel"));
    mPanel->setMetricsMode(Ogre::GMM_RELATIVE);
    mPanel->setPosition(0, 0);
    mPanel->setDimensions(1, 1);

    mTextAreaDepth = static_cast<TextAreaOverlayElement*>(
        mOverlayMgr->createOverlayElement("TextArea", "TextAreaDepth"));
    mTextAreaDepth->setMetricsMode(Ogre::GMM_RELATIVE);
    mTextAreaDepth->setPosition(0.01, 0);
    mTextAreaDepth->setDimensions(0.2, 0.2);
    mTextAreaDepth->setCaption("0");
    mTextAreaDepth->setCharHeight(0.07);
    mTextAreaDepth->setFontName("MenuFont");
    //mTextAreaDepth->setColourBottom(ColourValue(0.0, 0.0, 0.0));
    //mTextAreaDepth->setColourTop(ColourValue(1, 0, 0));
    mTextAreaDepth->setColour(ColourValue(1,1,1));

    mOverlay = mOverlayMgr->create("PlayStateOverlay");
    mOverlay->add2D(mPanel);


    mPanel->addChild(mTextAreaDepth);

    // Show the overlay
    mOverlay->show();

}

void PlayState::exit( void ) {

    delete mBoard;
    delete mPlayer;



    mOverlayMgr->destroyAllOverlayElements();

    mOverlayMgr->destroy(mOverlay);


    mSceneMgr->clearScene();
    mSceneMgr->destroyAllCameras();
    mRoot->getAutoCreatedWindow()->removeAllViewports();
}

void PlayState::pause( void ) {

    mOverlay->hide();
}

void PlayState::resume( void ) {


    mOverlay->show();
}

void PlayState::update( unsigned long lTimeElapsed )
{

    if(lTimeElapsed <= 0) return;
    if(lTimeElapsed > 100) lTimeElapsed = 100;

    if(tiempoBala) lTimeElapsed = 1;



    mInputDevice->capture();

    mBoard->update(lTimeElapsed);

	if (mInputDevice->isKeyDown(OIS::KC_F1))
	{

	    if(tiempoBala) tiempoBala = false;
	    else tiempoBala = true;
	}

	if (mInputDevice->isKeyDown(OIS::KC_LEFT))
	{

	    mPlayer->moveLeft();
	}
	else if (mInputDevice->isKeyDown(OIS::KC_RIGHT))
	{

	    mPlayer->moveRight();
	}
	else if (mInputDevice->isKeyDown(OIS::KC_UP))
	{
	    mCam->setMode(CMODE_FOLLOW_LOOK_UP);
	    mPlayer->moveUp();
	}
	else if (mInputDevice->isKeyDown(OIS::KC_DOWN))
	{
	    mCam->setMode(CMODE_FOLLOW);
	    mPlayer->moveDown();
	}

    mPlayer->update(lTimeElapsed);

    {
        static int timeDead = 0;

        if(!mPlayer->isAlive())
        {
            timeDead += lTimeElapsed;
        }
        else
        {
            timeDead=0;
        }

        if(timeDead > 3000)
        {

            mBoard->killUpwards(mPlayer->getPosition());
            mPlayer->resurrect();


        }
    }

    mCam->setParentPos(mPlayer->getPosition());
    mCam->update(lTimeElapsed);

    //String s;

    //s = StringConverter::toString(mBoard->collisionDistanceLeft(mPlayer->getPosition()));
    //s = "mPlayer->getPosition().x "
    //    + StringConverter::toString(mPlayer->getPosition().x);
    //s = "FPS: " + StringConverter::toString(Root::getSingleton().getAutoCreatedWindow()->getAverageFPS());




    mTextAreaDepth->setCaption( StringConverter::toString(mPlayer->getDepth()));




}

void PlayState::keyPressed( const OIS::KeyEvent &e )
{
    mInputDevice->capture();




    if ( e.key == OIS::KC_SPACE)
    {

        mPlayer->breakBlock();
    }
    if ( e.key == OIS::KC_U)
    {

        mBoard->killUpwards(mPlayer->getPosition());
    }
    if ( e.key == OIS::KC_0)
    {
        mBoard->printLog();
    }

    if ( e.key == OIS::KC_R)
    {
        delete mBoard;
        mBoard = new Board(9,80,"random");

        delete mPlayer;
        mPlayer = new Player(mBoard);
    }
    if ( e.key == OIS::KC_T)
    {
        delete mBoard;
        mBoard = new Board(9,500,"random");

        delete mPlayer;
        mPlayer = new Player(mBoard);
    }
    if ( e.key == OIS::KC_I)
    {
        LogManager::getSingleton().logMessage(StringConverter::toString(Root::getSingleton().getAutoCreatedWindow()->getAverageFPS()));
        LogManager::getSingleton().logMessage(StringConverter::toString(Root::getSingleton().getAutoCreatedWindow()->getTriangleCount()));
        LogManager::getSingleton().logMessage(StringConverter::toString(Root::getSingleton().getAutoCreatedWindow()->getBatchCount()));
    }
}

void PlayState::keyReleased( const OIS::KeyEvent &e ) {
    if( e.key == OIS::KC_P)
    {

        //actualiza la textura de escrinchot

        GameManager::getSingletonPtr()->screenshotRenderTexture->removeAllViewports();
        GameManager::getSingletonPtr()->screenshotRenderTexture->addViewport(mCamera);
        GameManager::getSingletonPtr()->screenshotRenderTexture->getViewport(0)->setClearEveryFrame(true);
        GameManager::getSingletonPtr()->screenshotRenderTexture->getViewport(0)->setBackgroundColour(mViewport->getBackgroundColour());
        GameManager::getSingletonPtr()->screenshotRenderTexture->getViewport(0)->setOverlaysEnabled(true);
        GameManager::getSingletonPtr()->screenshotRenderTexture->update();



        this->pushState( PauseState::getSingletonPtr() );
    }
    else if( e.key == OIS::KC_ESCAPE ) {
        this->changeState( MenuState::getSingletonPtr());
    }
}

void PlayState::mouseMoved( const OIS::MouseEvent &e )
{

}

void PlayState::mousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id ) {
}

void PlayState::mouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id ) {
}

PlayState* PlayState::getSingletonPtr( void ) {
    if( !mPlayState ) {
        mPlayState = new PlayState();
    }

    return mPlayState;
}
