
#include "PlayState.h"
#include "DotScene.h"
#include "Tools.h"
#include "SoundManager.h"
#include "LevelLoader.h"
#include "DotScene.h"
#include "ConfigManager.h"

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

    mCamera = mSceneMgr->createCamera( "Camera" );
    mCamera->setNearClipDistance(0.1);
    mCamera->setFarClipDistance(2000);


    mViewport   = mRoot->getAutoCreatedWindow()->addViewport( mCamera );


    mCameraNode = mSceneMgr->getRootSceneNode()->createChildSceneNode( "CameraNode" , Vector3(0,2,0));
    mCameraNode->attachObject(mCamera);
    mCameraDestNode = mSceneMgr->getRootSceneNode()->createChildSceneNode( "CameraDestNode" , Vector3(0,0,0));

    mCam = new Cam(mCamera, mCameraNode,mCameraDestNode);

    mCam->setDistance(10);


    //Setup lights
    //-----------------------------------------
    mSceneMgr->setAmbientLight(ColourValue(0.7,0.7,0.7));

    Light *light = mSceneMgr->createLight("Light1");
    light->setType(Light::LT_DIRECTIONAL);
    light->setDirection(Vector3(0.5, -1, -0.5));


    light->setDiffuseColour(0.9,0.9,0.9);
    light->setSpecularColour(0.5, 0.5, 0.5);
    //light->setAttenuation(200,1.0,0.022,0.0019);


    Light *light2 = mSceneMgr->createLight("Light2");
    light2->setType(Light::LT_POINT);
    light2->setPosition(Vector3(10, 0, 5));


    light2->setDiffuseColour(0.2, 0.2, 0.2);
    light2->setSpecularColour(0.2, 0.2, 0.2);

    //TODO:
    //light2->setAttenuation(200,1.0,0.022,0.0019);

    mCam->getSceneNode()->attachObject(light2);



    //load scene
    //--------------------------------------------

    backgroundSceneNode = mSceneMgr->getRootSceneNode()->createChildSceneNode( "backgroundSceneNode" , Vector3(0,0,0));

    // Load background scene
    CDotScene dotScene;
    //dotScene = new CDotScene();

    String sceneFilename = LevelLoader::getSingleton().getValue("background_scene") + ".xml";

    ConfigManager::getSingleton().setValue("last_bg_scene",LevelLoader::getSingleton().getLevelName());

    dotScene.parseDotScene(sceneFilename,"General",mSceneMgr, backgroundSceneNode, "background_");

    //Setup fog and bg color
    //--------------------------------------------------------------------
    mViewport->setBackgroundColour(StringConverter::parseColourValue(LevelLoader::getSingleton().getValue("background_color")));

    if(LevelLoader::getSingleton().getValue("fog") == "on")
    {
        Real fog_start = 10;
        Real fog_end = 40;

        if(LevelLoader::getSingleton().getValue("fog_start") != "")
            fog_start = StringConverter::parseReal(LevelLoader::getSingleton().getValue("fog_start"));
        if(LevelLoader::getSingleton().getValue("fog_end") != "")
            fog_end = StringConverter::parseReal(LevelLoader::getSingleton().getValue("fog_end"));

        mSceneMgr->setFog(FOG_LINEAR,
        StringConverter::parseColourValue(LevelLoader::getSingleton().getValue("background_color")),
        0.0, fog_start, fog_end);
    }
    else mSceneMgr->setFog(FOG_NONE);


    //Setup skybox
    //--------------------------------------------------------------------

    if(LevelLoader::getSingleton().getValue("skybox") != "")
    {
        //rota el cielo 180 grados
        if(LevelLoader::getSingleton().getValue("skybox_quaternion") != "")
        {
            Quaternion q = StringConverter::parseQuaternion(LevelLoader::getSingleton().getValue("skybox_quaternion"));
            mSceneMgr->setSkyBox(true,LevelLoader::getSingleton().getValue("skybox"),1000,true,q);
        }
        else
        {
            mSceneMgr->setSkyBox(true,LevelLoader::getSingleton().getValue("skybox"),1000,true);
        }
    }

    //-----------------------------------------------------------------------
    //Setup particle_effect
    if(LevelLoader::getSingleton().getValue("particle_effect") != "")
    {
        ParticleSystem* mParticleEffect = mSceneMgr->createParticleSystem("sceneParticleEffect", "snow");
        //SceneNode* mParticleEffectNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("sceneParticleEffectNode");
        //mParticleEffectNode->attachObject(mParticleEffect);

        //attach particle system to camera node

        mCam->getSceneNode()->attachObject(mParticleEffect);

    }
    //-----------------------------------------------------------------------

    mBoard = new Board();

    mPlayer = new Player(mBoard);

    mPlayer->setPosition(0,10,0);
    mCam->setPosition(0,mPlayer->getPosition().y,9);


    SoundManager::getSingleton().loadMusic("music.ogg");
    SoundManager::getSingleton().playMusic(true);

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

    ///////////////////////////////////////////////////////
    #ifdef XDRILLER_DEBUG
    mTextAreaDebug = static_cast<TextAreaOverlayElement*>(
        mOverlayMgr->createOverlayElement("TextArea", "TextAreaDebug"));
    mTextAreaDebug->setMetricsMode(Ogre::GMM_RELATIVE);
    mTextAreaDebug->setPosition(0.01, 0.1);
    mTextAreaDebug->setDimensions(0.2, 0.2);
    mTextAreaDebug->setCaption(" ");
    mTextAreaDebug->setCharHeight(0.03);
    mTextAreaDebug->setFontName("SmallFont");
    mTextAreaDebug->setColour(ColourValue(1,1,1,0.5));

    mPanel->addChild(mTextAreaDebug);
    #endif
    /////////////////////////////////////////////////////////

    mPanel->addChild(mTextAreaDepth);
    mPanel->addChild(mTextAreaPoints);
    mPanel->addChild(mTextAreaClock);

    mOverlay->setZOrder(100);


    // Show the overlay
    mOverlay->show();



    //LogManager::getSingleton().logMessage(DumpNodes(mSceneMgr->getRootSceneNode()).c_str());

}

void PlayState::exit( void ) {

    delete mBoard;
    delete mPlayer;

    destroyOverlayElements();

    #ifdef XDRILLER_DEBUG
    if(mTextAreaDebug != NULL)  mOverlayMgr->destroyOverlayElement(mTextAreaDebug);     mTextAreaDebug = NULL;
    #endif

    mOverlayMgr->destroy(mOverlay);

    mSceneMgr->setShadowTechnique(SHADOWTYPE_NONE);

    mSceneMgr->clearScene();
    mSceneMgr->destroyAllCameras();
    mRoot->getAutoCreatedWindow()->removeAllViewports();

    SoundManager::getSingleton().stopMusic();
    SoundManager::getSingleton().stopAllSounds();


}

void PlayState::pause( void ) {

    mOverlay->hide();
    mSceneMgr->setShadowTechnique(SHADOWTYPE_NONE);
}

void PlayState::resume( void ) {

    nextFramePause = false;
    mOverlay->show();
}

void PlayState::update( unsigned long lTimeElapsed )
{

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

            mPlayer->moveUp();
        }
        else if (mInputDevice->isKeyDown(OIS::KC_DOWN))
        {

            mPlayer->moveDown();
        }
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
    {
        static int lastLives = 0;

        if(mPlayer->isAlive() && lastLives!=mPlayer->getLives())
        {
            mLivesPanel->setPosition(0.98-(mPlayer->getLives())*0.0375*HEARTS_SCALE, 0.02);
            mLivesPanel->setDimensions(0.0375*HEARTS_SCALE*mPlayer->getLives(), 0.05*HEARTS_SCALE);
            mLivesPanel->setUV(0,0,mPlayer->getLives(),1);

            lastLives = mPlayer->getLives();
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

            //mOverlayMgr->destroyAllOverlayElements();

            destroyOverlayElements();

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
            //mOverlayMgr->destroyAllOverlayElements();

            destroyOverlayElements();

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

    #ifdef XDRILLER_DEBUG

    if(mTextAreaDebug != NULL)
    {
        RenderWindow *mRenderWindow = mRoot->getAutoCreatedWindow();

        String debugText = "FPS:   " + StringConverter::toString(mRenderWindow->getLastFPS()) +
                           "\nAvFPS  " + StringConverter::toString(mRenderWindow->getAverageFPS()) +
                           "\nTris:  " + StringConverter::toString(mRenderWindow->getTriangleCount()) +
                           "\nBatch: " + StringConverter::toString(mRenderWindow->getBatchCount()) +
                           "\nFSAA:   " + StringConverter::toString(mRenderWindow->getFSAA());



        mTextAreaDebug->setCaption(debugText);
    }

    #endif

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

        backgroundSceneNode->translate(0,backgroundSceneNode->getPosition().y + distanceUp,0);
    }
    else
    {

        if(!mSceneMgr->hasSceneNode("EndFloor"))
        {
            Entity *endFloorEnt = mSceneMgr->createEntity("EndFloor", "endsupercube.mesh");
            endFloorEnt->setCastShadows(false);
            SceneNode *endFloorNode = mSceneMgr->getRootSceneNode()->createChildSceneNode( "EndFloor" , Vector3(0,mPlayer->getPosition().y-10,0));
            endFloorNode->attachObject(endFloorEnt);

            mPlayer->setEndFloor();

        }

    }

    //en los subsiguentes tableros la escena de fondo de sigue dibujando
    // ¿como hacer que nop se dibujen?
    //if(boardNum == 1) backgroundSceneNode->detachObject(backgroundSceneEnt);


};

void PlayState::keyPressed( const OIS::KeyEvent &e )
{
    mInputDevice->capture();




    if ( e.key == OIS::KC_SPACE)
    {

        mPlayer->breakBlock();
    }


    if( e.key == OIS::KC_UP)
    {
        mCam->setMode(CMODE_FOLLOW_LOOK_UP);
    }
    if( e.key == OIS::KC_DOWN)
    {
        mCam->setMode(CMODE_FOLLOW);
    }

    #ifdef XDRILLER_DEBUG

    if (e.key == OIS::KC_F1)
    {

        if(tiempoBala) tiempoBala = false;
        else tiempoBala = true;
    }
    if( e.key == OIS::KC_F2)
    {
        mCam->setMode(CMODE_FOLLOW_FAR);
    }
    if ( e.key == OIS::KC_0)
    {
        mBoard->printLog();
    }

    if ( e.key == OIS::KC_U)
    {

        mBoard->killUpwards(mPlayer->getPosition());
    }

    /*
    if ( e.key == OIS::KC_I)
    {
        //LogManager::getSingleton().logMessage(StringConverter::toString(GpuProgramManager::getSingleton().getSupportedSyntax()));
        LogManager::getSingleton().logMessage(StringConverter::toString(Root::getSingleton().getAutoCreatedWindow()->getAverageFPS()));
        LogManager::getSingleton().logMessage(StringConverter::toString(Root::getSingleton().getAutoCreatedWindow()->getTriangleCount()));
        LogManager::getSingleton().logMessage(StringConverter::toString(Root::getSingleton().getAutoCreatedWindow()->getBatchCount()));
    }*/

    #endif
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
        this->fadeState( MenuState::getSingletonPtr());
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

void PlayState::destroyOverlayElements()
{

    if(mTextAreaDepth != NULL)  mOverlayMgr->destroyOverlayElement(mTextAreaDepth);     mTextAreaDepth = NULL;
    if(mTextAreaPoints != NULL) mOverlayMgr->destroyOverlayElement(mTextAreaPoints);    mTextAreaPoints = NULL;
    if(mTextAreaClock != NULL)  mOverlayMgr->destroyOverlayElement(mTextAreaClock);     mTextAreaClock = NULL;
    if(mTextAreaLives != NULL)  mOverlayMgr->destroyOverlayElement(mTextAreaLives);     mTextAreaLives = NULL;
    if(mTextAreaTotal != NULL)  mOverlayMgr->destroyOverlayElement(mTextAreaTotal);     mTextAreaTotal = NULL;
    if(mArrow != NULL)          mOverlayMgr->destroyOverlayElement(mArrow);             mArrow = NULL;
    if(mSkull != NULL)          mOverlayMgr->destroyOverlayElement(mSkull);             mSkull = NULL;
    if(mScore != NULL)          mOverlayMgr->destroyOverlayElement(mScore);             mScore = NULL;
    if(mClock != NULL)          mOverlayMgr->destroyOverlayElement(mClock);             mClock = NULL;
    if(mLivesPanel != NULL)     mOverlayMgr->destroyOverlayElement(mLivesPanel);        mLivesPanel = NULL;
    if(mBottle != NULL)         mOverlayMgr->destroyOverlayElement(mBottle);            mBottle = NULL;
    if(mBottleAir != NULL)      mOverlayMgr->destroyOverlayElement(mBottleAir);         mBottleAir = NULL;
    if(mPanel != NULL)          mOverlayMgr->destroyOverlayElement(mPanel);             mPanel = NULL;



}
