#include "GameManager.h"
#include "OgreWindowEventUtilities.h"
#include "OgreException.h"

#ifndef PlayState_H
#include "PlayState.h"
#endif
#ifndef GameState_H
#include "GameState.h"
#endif
#ifndef IntroState_H
#include "IntroState.h"
#endif
#ifndef PauseState_H
#include "PauseState.h"
#endif
#ifndef MenuState_H
#include "MenuState.h"
#endif
#include "SoundManager.h"
#include "ConfigManager.h"

using namespace Ogre;

GameManager* GameManager::mGameManager;

GameManager::GameManager( void ) :
    mRoot( 0 ),
    mInputMgr( 0 ),
    mIntroState( 0 ),
    mPlayState( 0 ),
    mPauseState( 0 ),
    mMenuState( 0 ),
    bShutdown( false ) {
}

GameManager::~GameManager( void ) {
    // Clean up all the states
    while( !mStates.empty() ) {
        mStates.back()->exit();
        mStates.pop_back();
    }

    if( mInputMgr ) {
        delete mInputMgr;
        mInputMgr = 0;
    }

    if( mIntroState ) {
        delete mIntroState;
        mIntroState = 0;
    }

    if( mPlayState ) {
        delete mPlayState;
        mPlayState  = 0;
    }

    if( mPauseState ) {
        delete mPauseState;
        mPauseState = 0;
    }

    if( mMenuState ) {
        delete mMenuState;
        mMenuState = 0;
    }

    if( mRoot ) {
        delete mRoot;
        mRoot = 0;
    }
}

void GameManager::startGame( GameState *gameState ) {
    mRoot = new Root("plugins.cfg", "ogre.cfg", "xdriller.log");

    // Setup states
    mIntroState = IntroState::getSingletonPtr();
    mPlayState  = PlayState::getSingletonPtr();
    mPauseState = PauseState::getSingletonPtr();
    mMenuState  = MenuState::getSingletonPtr();

    // Setup and configure game
    this->setupResources();
    if( !this->configureGame() ) {
        // If game can't be configured, throw exception and quit application
        throw Ogre::Exception( Ogre::Exception::ERR_INTERNAL_ERROR,
            "Error - Couldn't Configure Renderwindow",
            "Example - Error" );
        return;
    }

    // Setup input //yys
    mInputMgr = InputManager::getSingletonPtr();//mInputMgr = new InputManager();//
	//mRenderWindow = mRoot->getAutoCreatedWindow();
    mInputMgr->initialise( mRenderWindow );
	WindowEventUtilities::addWindowEventListener( mRenderWindow, this );//yys

    mInputMgr->addKeyListener( this, "GameManager" );
    mInputMgr->addMouseListener( this, "GameManager" );
//    mInputMgr->getJoystick( 1 );//yys

    new SoundManager();

    SoundManager::getSingleton().loadSounds();

    //Textura donde renderizar la pantalla para modo pausa
    TexturePtr screenshotTexturePtr = Ogre::TextureManager::getSingleton().createManual("ScreenShot_texture",
        ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, TEX_TYPE_2D,
        //mRenderWindow->getWidth(), mRenderWindow->getHeight(), 0, PF_R8G8B8, TU_RENDERTARGET);
        512, 512, 0, PF_R8G8B8, TU_RENDERTARGET);
    screenshotRenderTexture = screenshotTexturePtr->getBuffer()->getRenderTarget();


    //MaterialPtr material = MaterialManager::getSingleton().create("ScreenShot", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    MaterialPtr material = MaterialManager::getSingleton().getByName("ScreenShot");

    //Ogre::Technique *technique = material->createTechnique();
    //technique->createPass();
    //material->getTechnique(0)->getPass(0)->setLightingEnabled(false);
    //material->getTechnique(0)->getPass(0)->createTextureUnitState("ScreenShot_texture");


    // Change to first state
    this->changeState( gameState );

    // lTimeLastFrame remembers the last time that it was checked
    // We use it to calculate the time since last frame
    unsigned long lTimeLastFrame = 0;

    // Main while-loop
    while( !bShutdown ) {
        // Calculate time since last frame and remember current time for next frame
        unsigned long lTimeCurrentFrame = mRoot->getTimer()->getMilliseconds();
        unsigned long lTimeSinceLastFrame = lTimeCurrentFrame - lTimeLastFrame;
        lTimeLastFrame = lTimeCurrentFrame;

        // Update inputmanager
        mInputMgr->capture();

        // Update current state
        mStates.back()->update( lTimeSinceLastFrame );

		WindowEventUtilities::messagePump();//yys
		// Render next frame
        mRoot->renderOneFrame();

        // Deal with platform specific issues
        //PlatformManager::getSingletonPtr()->messagePump( mRenderWindow );
		//MYmessagePump( mRenderWindow );//yys

		//WindowEventUtilities::addWindowEventListener(mRenderWindow, this);
    }
}


bool GameManager::configureGame( void ) {
    // Load config settings from ogre.cfg
    if( !mRoot->restoreConfig() ) {
        // If there is no config file, show the configuration dialog
        if( !mRoot->showConfigDialog() ) {
            return false;
        }
    }

    new ConfigManager("config.cfg");

    ConfigManager::getSingleton().load();

    /*ConfigManager::getSingleton().setValue("audio_rate","44100");
    ConfigManager::getSingleton().setValue("audio_channels","2");
    ConfigManager::getSingleton().setValue("audio_buffers","4096");
    ConfigManager::getSingleton().setValue("music_volume","64");
    ConfigManager::getSingleton().setValue("sound_volume","100");
    ConfigManager::getSingleton().setValue("resource_path","media/");
    ConfigManager::getSingleton().save();*/

    // Initialise and create a default rendering window
    mRenderWindow = mRoot->initialise( true, "XDriller" );

    // Initialise resources
    ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    // Create needed scenemanagers
    mRoot->createSceneManager( ST_GENERIC, "ST_GENERIC" );

    return true;
}

void GameManager::setupResources( void ) {
    // Load resource paths from config file
    ConfigFile cf;
    cf.load( "resources.cfg" );

    // Go through all settings in the file
    ConfigFile::SectionIterator itSection = cf.getSectionIterator();

    String sSection, sType, sArch;
    while( itSection.hasMoreElements() ) {
        sSection = itSection.peekNextKey();

        ConfigFile::SettingsMultiMap *mapSettings = itSection.getNext();
        ConfigFile::SettingsMultiMap::iterator itSetting = mapSettings->begin();
        while( itSetting != mapSettings->end() ) {
            sType = itSetting->first;
            sArch = itSetting->second;

            ResourceGroupManager::getSingleton().addResourceLocation(
                sArch, sType, sSection );

            ++itSetting;
        }
    }
}

void GameManager::changeState( GameState *gameState ) {
    // Cleanup the current state
    if( !mStates.empty() ) {
        mStates.back()->exit();
        mStates.pop_back();
    }

    // Store and init the new state
    mStates.push_back( gameState );
    mStates.back()->enter();
}

void GameManager::pushState( GameState *gameState ) {
    // Pause current state
    if( !mStates.empty() ) {
        mStates.back()->pause();
    }

    // Store and init the new state
    mStates.push_back( gameState );
    mStates.back()->enter();
}

void GameManager::popState() {
    // Cleanup the current state
    if( !mStates.empty() ) {
        mStates.back()->exit();
        mStates.pop_back();
    }

    // Resume previous state
    if( !mStates.empty() ) {
        mStates.back()->resume();
    }
}

void GameManager::requestShutdown( void ) {
    bShutdown = true;
}

bool GameManager::keyPressed( const OIS::KeyEvent &e ) {
    // Call keyPressed of current state
    mStates.back()->keyPressed( e );

    return true;
}

bool GameManager::keyReleased( const OIS::KeyEvent &e ) {
    // Call keyReleased of current state
    mStates.back()->keyReleased( e );

    return true;
}

bool GameManager::mouseMoved( const OIS::MouseEvent &e ) {
    // Call mouseMoved of current state
    mStates.back()->mouseMoved( e );

    return true;
}

bool GameManager::mousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id ) {
    // Call mousePressed of current state
    mStates.back()->mousePressed( e, id );

    return true;
}

bool GameManager::mouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id ) {
    // Call mouseReleased of current state
    mStates.back()->mouseReleased( e, id );

    return true;
}

GameManager* GameManager::getSingletonPtr( void ) {
    if( !mGameManager ) {
        mGameManager = new GameManager();
    }

    return mGameManager;
}
