
#include "PlayState.h"
#include "DotScene.h"
#include "Tools.h"
#include "SoundManager.h"
#include "LevelLoader.h"



#include <string>
#include <vector>

#define HEARTS_SCALE 1.0



using namespace Ogre;

PlayState* PlayState::mPlayState;



void PlayState::enter( void ) {
    mRoot             = Root::getSingletonPtr();
    mOverlayMgr       = OverlayManager::getSingletonPtr();
    mInputDevice      = InputManager::getSingletonPtr()->getKeyboard();
    mSceneMgr         = mRoot->getSceneManager( "ST_GENERIC" );
    //mSceneMgr = mRoot->createSceneManager("DotSceneOctreeManager");
    tiempoBala = false;
    nextFramePause = false;
    depthAccumulation = 0;
    depth = 0;
    boardNum = 0;
    points = 0;
    pointsAccumulation = 0;
    gameTime = 0;
    gameSeconds = 0;
    finished = false;

    fade_alpha = 1.0;

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

    backgroundSceneNode = mSceneMgr->getRootSceneNode()->createChildSceneNode( "backgroundScene_Node" , Vector3(0,0,0));
    backgroundSceneEnt = mSceneMgr->createEntity("backGroundScene_Ent", "fondo_polo.mesh");
    backgroundSceneNode->attachObject(backgroundSceneEnt);

    backgroundSceneNode->rotate(Quaternion(Degree(-90),Vector3::UNIT_X));


    mBoard = new Board();

    mPlayer = new Player(mBoard);

    mPlayer->setPosition(0,10,0);
    mCam->setPosition(0,mPlayer->getPosition().y,9);


    //prueba de sonido

    SoundManager::getSingleton().loadMusic("music.ogg");
    SoundManager::getSingleton().playMusic(true);

     //LogManager::getSingleton().logMessage(DumpNodes(mSceneMgr->getRootSceneNode()).c_str());

    mPanel = static_cast<PanelOverlayElement*>(
        mOverlayMgr->createOverlayElement("Panel", "PlayStateOverlayPanel"));
    mPanel->setMetricsMode(Ogre::GMM_RELATIVE);
    mPanel->setPosition(0, 0);
    mPanel->setDimensions(1, 1);

    mArrow =  static_cast<PanelOverlayElement*>(
        mOverlayMgr->createOverlayElement("Panel", "Arrow"));
    mArrow->setMetricsMode(Ogre::GMM_RELATIVE);
    mArrow->setPosition(0.01,0.88);
    mArrow->setDimensions(0.075,0.1);
    mArrow->setMaterialName("arrow");

    mScore =  static_cast<PanelOverlayElement*>(
        mOverlayMgr->createOverlayElement("Panel", "Score"));
    mScore->setMetricsMode(Ogre::GMM_RELATIVE);
    mScore->setPosition(0.01,0.01);
    mScore->setDimensions(0.0375,0.05);
    mScore->setMaterialName("score");

    mClock =  static_cast<PanelOverlayElement*>(
        mOverlayMgr->createOverlayElement("Panel", "Clock"));
    mClock->setMetricsMode(Ogre::GMM_RELATIVE);
    mClock->setPosition(0.4,0.01);
    mClock->setDimensions(0.0375,0.05);
    mClock->setMaterialName("clock");




    mTextAreaDepth = static_cast<TextAreaOverlayElement*>(
        mOverlayMgr->createOverlayElement("TextArea", "TextAreaDepth"));
    mTextAreaDepth->setMetricsMode(Ogre::GMM_RELATIVE);
    mTextAreaDepth->setPosition(0.0475, 0.88);
    mTextAreaDepth->setDimensions(0.1, 0.1);
    mTextAreaDepth->setCaption("0");
    mTextAreaDepth->setCharHeight(0.07);
    mTextAreaDepth->setFontName("CoolFont");
    //mTextAreaDepth->setColourBottom(ColourValue(0.0, 0.0, 0.0));
    //mTextAreaDepth->setColourTop(ColourValue(1, 0, 0));
    mTextAreaDepth->setColour(ColourValue(1,1,1));
    mTextAreaDepth->setAlignment(TextAreaOverlayElement::Center);

    mTextAreaPoints = static_cast<TextAreaOverlayElement*>(
        mOverlayMgr->createOverlayElement("TextArea", "TextAreaPoints"));
    mTextAreaPoints->setMetricsMode(Ogre::GMM_RELATIVE);
    mTextAreaPoints->setPosition(0.05, 0.0);
    mTextAreaPoints->setDimensions(0.2, 0.1);
    mTextAreaPoints->setCaption("x0");
    mTextAreaPoints->setCharHeight(0.07);
    mTextAreaPoints->setFontName("CoolFont");
    mTextAreaPoints->setColour(ColourValue(1,1,1));
    mTextAreaPoints->setAlignment(TextAreaOverlayElement::Left);

    mTextAreaClock = static_cast<TextAreaOverlayElement*>(
        mOverlayMgr->createOverlayElement("TextArea", "TextAreaClock"));
    mTextAreaClock->setMetricsMode(Ogre::GMM_RELATIVE);
    mTextAreaClock->setPosition(0.45, 0.0);
    mTextAreaClock->setDimensions(0.2, 0.1);
    mTextAreaClock->setCaption("00:00");
    mTextAreaClock->setCharHeight(0.07);
    mTextAreaClock->setFontName("CoolFont");
    mTextAreaClock->setColour(ColourValue(1,1,1));
    mTextAreaClock->setAlignment(TextAreaOverlayElement::Left);

    mLivesPanel =  static_cast<PanelOverlayElement*>(
        mOverlayMgr->createOverlayElement("Panel", "LivesPanel"));
    mLivesPanel->setMetricsMode(Ogre::GMM_RELATIVE);
    mLivesPanel->setPosition(0.98-(mPlayer->getLives())*0.0375*HEARTS_SCALE, 0.02);
    mLivesPanel->setDimensions(0.0375*HEARTS_SCALE*mPlayer->getLives(), 0.05*HEARTS_SCALE);
    mLivesPanel->setUV(0,0,mPlayer->getLives(),1);
    mLivesPanel->setMaterialName("heart");



    mBottleAir =  static_cast<PanelOverlayElement*>(
        mOverlayMgr->createOverlayElement("Panel", "BottlePanelAir"));
    mBottleAir->setMetricsMode(Ogre::GMM_RELATIVE);
    mBottleAir->setPosition(0.9,0.72);
    mBottleAir->setDimensions(0.1,0.25);
    mBottleAir->setUV(0,0,1,1);
    mBottleAir->setMaterialName("bottle_air");

    mBottle =  static_cast<PanelOverlayElement*>(
        mOverlayMgr->createOverlayElement("Panel", "BottlePanel"));
    mBottle->setMetricsMode(Ogre::GMM_RELATIVE);
    mBottle->setPosition(0.9,0.72);
    mBottle->setDimensions(0.1,0.25);
    //mBottle->setUV(0,0,mPlayer->getLives(),1);
    mBottle->setMaterialName("bottle");

    mOverlay = mOverlayMgr->create("PlayStateOverlay");


    mOverlay->add2D(mArrow);
    mOverlay->add2D(mScore);
    mOverlay->add2D(mClock);
    mOverlay->add2D(mPanel);
    mOverlay->add2D(mLivesPanel);
    mOverlay->add2D(mBottleAir);
    mOverlay->add2D(mBottle);



    mPanel->addChild(mTextAreaDepth);
    mPanel->addChild(mTextAreaPoints);
    mPanel->addChild(mTextAreaClock);


    mFadePanel = static_cast<PanelOverlayElement*>(
        mOverlayMgr->createOverlayElement("Panel", "FadeOverlayPanel"));
    mFadePanel->setMetricsMode(Ogre::GMM_RELATIVE);
    mFadePanel->setPosition(0, 0);
    mFadePanel->setDimensions(1, 1);
    mFadePanel->setMaterialName("fade_material");

    mFadeOverlay = mOverlayMgr->create("FadeOverlay");
    mFadeOverlay->add2D(mFadePanel);


    mOverlay->setZOrder(100);
    mFadeOverlay->setZOrder(101);

    // Show the overlay
    mOverlay->show();
    mFadeOverlay->show();

}

void PlayState::exit( void ) {

    delete mBoard;
    delete mPlayer;

    mOverlayMgr->destroyAllOverlayElements();

    mTextAreaLives = NULL;
    mTextAreaTotal = NULL;
    mSkull         = NULL;

    mOverlayMgr->destroy(mOverlay);
    mOverlayMgr->destroy(mFadeOverlay);


    mSceneMgr->clearScene();
    mSceneMgr->destroyAllCameras();
    mRoot->getAutoCreatedWindow()->removeAllViewports();

    SoundManager::getSingleton().stopMusic();
    SoundManager::getSingleton().stopAllSounds();
}

void PlayState::pause( void ) {

    mOverlay->hide();
}

void PlayState::resume( void ) {

    nextFramePause = false;
    mOverlay->show();
}

void PlayState::update( unsigned long lTimeElapsed )
{

    if(fade_alpha > 0)
    {
        fade_alpha -= (float) lTimeElapsed / 1000.0;

		Ogre::ResourcePtr resptr = Ogre::MaterialManager::getSingleton().getByName("fade_material");
		Ogre::Material * mat = dynamic_cast<Ogre::Material*>(resptr.getPointer());

		mat->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setAlphaOperation(LBX_MODULATE, LBS_MANUAL, LBS_TEXTURE, fade_alpha);;
    }
    else
    {
        fade_alpha = 0;
        mFadeOverlay->hide();
    }

    if(lTimeElapsed <= 0) return;
    if(lTimeElapsed > 100) lTimeElapsed = 100;

    if(!finished && mPlayer->getFinished())
    {
        finished = true;
    }

    if(tiempoBala) lTimeElapsed = 1;

    gameTime += lTimeElapsed;

    mInputDevice->capture();

    mBoard->update(lTimeElapsed);

    if(!finished)
    {
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
    }
    else
    {
       mCam->setMode(CMODE_FOLLOW_ABOVE);
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
            if(mPlayer->isAlive())
            {
                mLivesPanel->setPosition(0.98-(mPlayer->getLives())*0.0375*HEARTS_SCALE, 0.02);
                mLivesPanel->setDimensions(0.0375*HEARTS_SCALE*mPlayer->getLives(), 0.05*HEARTS_SCALE);
                mLivesPanel->setUV(0,0,mPlayer->getLives(),1);
            }

        }
    }

    mCam->setParentPos(mPlayer->getPosition());
    mCam->update(lTimeElapsed);

    //String s;

    //s = StringConverter::toString(mBoard->collisionDistanceLeft(mPlayer->getPosition()));
    //s = "mPlayer->getPosition().x "
    //    + StringConverter::toString(mPlayer->getPosition().x);
    //s = "FPS: " + StringConverter::toString(Root::getSingleton().getAutoCreatedWindow()->getAverageFPS());



    if(!finished)
    {
        mBottleAir->setPosition(0.9,0.72+0.25*(1-mPlayer->getAir()*0.8));
        mBottleAir->setDimensions(0.1,0.25*(mPlayer->getAir()*0.8));
        mBottleAir->setUV(0,1-mPlayer->getAir()*0.8,1,1);

        if(mPlayer->getLastDepth() != mPlayer->getDepth())
        {
            depth = depthAccumulation+mPlayer->getDepth();
            mTextAreaDepth->setCaption( StringConverter::toString(depth));
        }

        if(points < pointsAccumulation + mPlayer->getPoints()*5 + mBoard->getPoints())
        {
            points = pointsAccumulation + mPlayer->getPoints()*5 + mBoard->getPoints();
            mTextAreaPoints->setCaption( String("x") + StringConverter::toString(points) );
        }
        if(gameSeconds != int(gameTime / 1000))
        {
            gameSeconds = int(gameTime / 1000);

            int minutes = int(gameSeconds/60);
            int seconds = gameSeconds - minutes*60;

            if(minutes < 10 && seconds < 10)
                mTextAreaClock->setCaption( String("0") + StringConverter::toString(minutes) + ":0" +  StringConverter::toString(seconds));
            else if(minutes < 10)
                mTextAreaClock->setCaption( String("0") + StringConverter::toString(minutes) + ":" +  StringConverter::toString(seconds));
            else if(seconds < 10)
                mTextAreaClock->setCaption(StringConverter::toString(minutes) + ":0" +  StringConverter::toString(seconds));
            else mTextAreaClock->setCaption( StringConverter::toString(minutes) + ":" +  StringConverter::toString(seconds));

        }
    }

    if(nextFramePause == true)
    {
        this->pushState( PauseState::getSingletonPtr() );
    }

    if(mPlayer->getDepth() >= mBoard-> getHeight() + 10)
    {
        nextBoard();
    }

    static float count = 0;

    if(finished)
    {


        const float countTime = 1000;


        if(mTextAreaLives == NULL && mPlayer->isAlive())
        {

            mOverlayMgr->destroyAllOverlayElements();

            mPanel = static_cast<PanelOverlayElement*>(
                mOverlayMgr->createOverlayElement("Panel", "PlayStateOverlayPanel"));
            mPanel->setMetricsMode(Ogre::GMM_RELATIVE);
            mPanel->setPosition(0, 0);
            mPanel->setDimensions(1, 1);

            mArrow =  static_cast<PanelOverlayElement*>(
                mOverlayMgr->createOverlayElement("Panel", "Arrow"));
            mArrow->setMetricsMode(Ogre::GMM_RELATIVE);
            mArrow->setPosition(0.1,0.05);
            mArrow->setDimensions(0.0375,0.05);
            mArrow->setMaterialName("arrow");

            mScore =  static_cast<PanelOverlayElement*>(
                mOverlayMgr->createOverlayElement("Panel", "Score"));
            mScore->setMetricsMode(Ogre::GMM_RELATIVE);
            mScore->setPosition(0.1,0.15);
            mScore->setDimensions(0.0375,0.05);
            mScore->setMaterialName("score");

            mClock =  static_cast<PanelOverlayElement*>(
                mOverlayMgr->createOverlayElement("Panel", "Clock"));
            mClock->setMetricsMode(Ogre::GMM_RELATIVE);
            mClock->setPosition(0.1,0.25);
            mClock->setDimensions(0.0375,0.05);
            mClock->setMaterialName("clock");

            mLivesPanel =  static_cast<PanelOverlayElement*>(
            mOverlayMgr->createOverlayElement("Panel", "LivesPanel"));
            mLivesPanel->setMetricsMode(Ogre::GMM_RELATIVE);
            mLivesPanel->setPosition(0.1,0.35);
            mLivesPanel->setDimensions(0.0375,0.05);
            mLivesPanel->setMaterialName("heart");

            mTextAreaDepth = static_cast<TextAreaOverlayElement*>(
                mOverlayMgr->createOverlayElement("TextArea", "TextAreaDepth"));
            mTextAreaDepth->setMetricsMode(Ogre::GMM_RELATIVE);
            mTextAreaDepth->setPosition(0.15, 0.035);
            mTextAreaDepth->setDimensions(0.1, 0.1);
            mTextAreaDepth->setCaption("x = ");
            mTextAreaDepth->setCharHeight(0.07);
            mTextAreaDepth->setFontName("CoolFont");
            mTextAreaDepth->setColour(ColourValue(1,1,1));
            mTextAreaDepth->setAlignment(TextAreaOverlayElement::Left);

            mTextAreaPoints = static_cast<TextAreaOverlayElement*>(
                mOverlayMgr->createOverlayElement("TextArea", "mTextAreaPoints"));
            mTextAreaPoints->setMetricsMode(Ogre::GMM_RELATIVE);
            mTextAreaPoints->setPosition(0.15, 0.135);
            mTextAreaPoints->setDimensions(0.1, 0.1);
            mTextAreaPoints->setCaption("x = ");
            mTextAreaPoints->setCharHeight(0.07);
            mTextAreaPoints->setFontName("CoolFont");
            mTextAreaPoints->setColour(ColourValue(1,1,1));
            mTextAreaPoints->setAlignment(TextAreaOverlayElement::Left);

            mTextAreaClock = static_cast<TextAreaOverlayElement*>(
                mOverlayMgr->createOverlayElement("TextArea", "mTextAreaClock"));
            mTextAreaClock->setMetricsMode(Ogre::GMM_RELATIVE);
            mTextAreaClock->setPosition(0.15, 0.235);
            mTextAreaClock->setDimensions(0.1, 0.1);
            mTextAreaClock->setCaption("x = ");
            mTextAreaClock->setCharHeight(0.07);
            mTextAreaClock->setFontName("CoolFont");
            mTextAreaClock->setColour(ColourValue(1,1,1));
            mTextAreaClock->setAlignment(TextAreaOverlayElement::Left);

            mTextAreaLives = static_cast<TextAreaOverlayElement*>(
                mOverlayMgr->createOverlayElement("TextArea", "mTextAreaLives"));
            mTextAreaLives->setMetricsMode(Ogre::GMM_RELATIVE);
            mTextAreaLives->setPosition(0.15, 0.335);
            mTextAreaLives->setDimensions(0.1, 0.1);
            mTextAreaLives->setCaption("x = ");
            mTextAreaLives->setCharHeight(0.07);
            mTextAreaLives->setFontName("CoolFont");
            mTextAreaLives->setColour(ColourValue(1,1,1));
            mTextAreaLives->setAlignment(TextAreaOverlayElement::Left);



            mOverlay->add2D(mArrow);
            mOverlay->add2D(mScore);
            mOverlay->add2D(mClock);
            mOverlay->add2D(mPanel);
            mOverlay->add2D(mLivesPanel);

            mPanel->addChild(mTextAreaDepth);
            mPanel->addChild(mTextAreaPoints);
            mPanel->addChild(mTextAreaClock);
            mPanel->addChild(mTextAreaLives);

        }
        else if(mSkull == NULL && !mPlayer->isAlive())
        {
            mOverlayMgr->destroyAllOverlayElements();

            mPanel = static_cast<PanelOverlayElement*>(
                mOverlayMgr->createOverlayElement("Panel", "PlayStateOverlayPanel"));
            mPanel->setMetricsMode(Ogre::GMM_RELATIVE);
            mPanel->setPosition(0, 0);
            mPanel->setDimensions(1, 1);

            mSkull =  static_cast<PanelOverlayElement*>(
                mOverlayMgr->createOverlayElement("Panel", "Arrow"));
            mSkull->setMetricsMode(Ogre::GMM_RELATIVE);
            mSkull->setPosition(0.425,0.2);
            mSkull->setDimensions(0.150,0.2);
            mSkull->setMaterialName("skull");

            mTextAreaTotal = static_cast<TextAreaOverlayElement*>(
                mOverlayMgr->createOverlayElement("TextArea", "mTextAreaTotal"));
            mTextAreaTotal->setMetricsMode(Ogre::GMM_RELATIVE);
            mTextAreaTotal->setPosition(0.5, 0.45);
            mTextAreaTotal->setDimensions(1.0, 0.1);
            mTextAreaTotal->setCaption("Press ESC");
            mTextAreaTotal->setCharHeight(0.07);
            mTextAreaTotal->setFontName("CoolFont");
            mTextAreaTotal->setColour(ColourValue(1,1,1));
            mTextAreaTotal->setAlignment(TextAreaOverlayElement::Center);

            mPanel->addChild(mTextAreaTotal);
            mOverlay->add2D(mPanel);

            mOverlay->add2D(mSkull);






        }

        if(count<countTime && mPlayer->isAlive())
        {

            count += lTimeElapsed;
            if(count>countTime) count = countTime;

            mTextAreaDepth->setCaption(String("+ ") + StringConverter::toString((int)(depth*(count/countTime))));
            mTextAreaPoints->setCaption(String("+ ") + StringConverter::toString((int)(points*(count/countTime))));
            mTextAreaClock->setCaption(String("- ") + StringConverter::toString((int)(gameSeconds*(count/countTime))));
            mTextAreaLives->setCaption(String("x ") + StringConverter::toString((int)(mPlayer->getLives()*(count/countTime))));


        }
        else if (mPlayer->isAlive())
        {

            if(mTextAreaTotal == NULL)
            {
                int total = (depth + points - gameSeconds) * mPlayer->getLives();

                mTextAreaTotal = static_cast<TextAreaOverlayElement*>(
                    mOverlayMgr->createOverlayElement("TextArea", "mTextAreaTotal"));
                mTextAreaTotal->setMetricsMode(Ogre::GMM_RELATIVE);
                mTextAreaTotal->setPosition(0.15, 0.435);
                mTextAreaTotal->setDimensions(0.1, 0.1);
                mTextAreaTotal->setCaption("= " + StringConverter::toString(total) + "");
                mTextAreaTotal->setCharHeight(0.07);
                mTextAreaTotal->setFontName("CoolFont");
                mTextAreaTotal->setColour(ColourValue(1,1,1));
                mTextAreaTotal->setAlignment(TextAreaOverlayElement::Left);


                mPanel->addChild(mTextAreaTotal);
            }


        }




    }
    else count = 0;

}

void PlayState::nextBoard()
{

    boardNum ++;

    if(boardNum<LevelLoader::getSingleton().getNumBoards())
    {
        LevelLoader::getSingleton().setBoardNum(boardNum);

        Real distanceUp = -mPlayer->getPosition().y + 10;

        depthAccumulation += depth;

        pointsAccumulation += points;

        delete mBoard;
        mBoard = new Board();
        mPlayer->setBoard(mBoard);

        mPlayer->setPosition(mPlayer->getPosition().x,mPlayer->getPosition().y + distanceUp,mPlayer->getPosition().z);
        mCam->setPosition(0,mCam->getPosition().y + distanceUp,9);
        mCam->setParentPos(mPlayer->getPosition());
        mCam->update(0);
    }
    else
    {

        if(!mSceneMgr->hasSceneNode("EndFloor"))
        {
            Entity *endFloorEnt = mSceneMgr->createEntity("EndFloor", "endsupercube.mesh");
            SceneNode *endFloorNode = mSceneMgr->getRootSceneNode()->createChildSceneNode( "EndFloor" , Vector3(0,mPlayer->getPosition().y-10,0));
            endFloorNode->attachObject(endFloorEnt);

            mPlayer->setEndFloor();

        }

    }

    if(boardNum == 1) backgroundSceneNode->detachObject(backgroundSceneEnt);


};

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
    /*if ( e.key == OIS::KC_0)
    {
        mBoard->printLog();
    }

    if ( e.key == OIS::KC_R)
    {
        delete mBoard;
        mBoard = new Board();

        delete mPlayer;
        mPlayer = new Player(mBoard);
    }

    if ( e.key == OIS::KC_I)
    {
        //LogManager::getSingleton().logMessage(StringConverter::toString(GpuProgramManager::getSingleton().getSupportedSyntax()));
        LogManager::getSingleton().logMessage(StringConverter::toString(Root::getSingleton().getAutoCreatedWindow()->getAverageFPS()));
        LogManager::getSingleton().logMessage(StringConverter::toString(Root::getSingleton().getAutoCreatedWindow()->getTriangleCount()));
        LogManager::getSingleton().logMessage(StringConverter::toString(Root::getSingleton().getAutoCreatedWindow()->getBatchCount()));
    }*/
}

void PlayState::keyReleased( const OIS::KeyEvent &e ) {
    if( e.key == OIS::KC_P)
    {

        //actualiza la textura de escrinchot

        GameManager::getSingletonPtr()->screenshotRenderTexture->removeAllViewports();
        GameManager::getSingletonPtr()->screenshotRenderTexture->addViewport(mCamera);
        GameManager::getSingletonPtr()->screenshotRenderTexture->getViewport(0)->setClearEveryFrame(true);
        GameManager::getSingletonPtr()->screenshotRenderTexture->getViewport(0)->setBackgroundColour(mViewport->getBackgroundColour());
        GameManager::getSingletonPtr()->screenshotRenderTexture->getViewport(0)->setOverlaysEnabled(false);
        GameManager::getSingletonPtr()->screenshotRenderTexture->setAutoUpdated(false);
        GameManager::getSingletonPtr()->screenshotRenderTexture->update();



        nextFramePause = true;
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
