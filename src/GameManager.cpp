#include "GameManager.h"
#include "OgreWindowEventUtilities.h"
#include "OgreException.h"
#include "OgreOverlaySystem.h"

#include "PlayState.h"
#include "GameState.h"
#include "IntroState.h"
#include "PauseState.h"
#include "MenuState.h"
#include "CreditsState.h"
#include "HighScoreState.h"


#include "SoundManager.h"
#include "ConfigManager.h"
#include "LevelLoader.h"
#include "HighScoreManager.h"
#include "RumbleManager.h"
#include "BackgroundSceneManager.h"

#include "Tools.h"
#include "Globals.h"
#include "Gettext.h"

#include <locale.h>

#include <OgrePlatform.h>

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE

#include "CoreFoundation/CoreFoundation.h" //para los paths en el app bunddle

#endif 


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
    if( mCreditsState ) {
        delete mCreditsState;
        mCreditsState = 0;
    }

    ConfigManager::getSingleton().save();

    HighScoreManager::getSingleton().save();

    LogManager::getSingleton().logMessage("Everage FPS: " + StringConverter::toString(Root::getSingleton().getAutoCreatedWindow()->getStatistics().avgFPS));

    if( mRoot ) {
        delete mRoot;
        mRoot = 0;
    }

    delete LogManager::getSingletonPtr();
}

void GameManager::startGame( GameState *gameState )
{
	

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32 //Puto windows
    configPath = ".";
    String defaultDataPath = ".";
#elif OGRE_PLATFORM == OGRE_PLATFORM_APPLE // Puto mac
		
    char path[1024];
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    assert( mainBundle );
	
    CFURLRef mainBundleURL = CFBundleCopyBundleURL( mainBundle);
    assert( mainBundleURL);
	
    CFStringRef cfStringRef = CFURLCopyFileSystemPath( mainBundleURL, kCFURLPOSIXPathStyle);
    assert( cfStringRef);
	
    CFStringGetCString( cfStringRef, path, 1024, kCFStringEncodingASCII);
	
    CFRelease( mainBundleURL);
    CFRelease( cfStringRef);
	
	configPath = String(path) + "/Contents/Resources/";
	String defaultDataPath = String(path) + "/Contents/Resources/";

#else // Linux, etc..

	configPath = String(getenv("HOME")) + "/.config/xdriller";
    String defaultDataPath = "/usr/share/xdriller";

    if(!fileExists(String(getenv("HOME")) + "/.config"))
    {
        makeDirectory(String(getenv("HOME")) + "/.config");
    }

    if(!fileExists(configPath))
    {
        makeDirectory(configPath);
    }
	
    if(fileExists("resources.cfg"))
    {
        configPath = ".";
    }
	
#endif

	new LogManager;
    LogManager::getSingleton().createLog(configPath + "/xdriller.log");
    LogManager::getSingleton().logMessage("Xdriller v" + StringConverter::toString(XDRILLER_VERSION_STRING));

#if OGRE_PLATFORM != OGRE_PLATFORM_APPLE // Puto mac

    if(!fileExists(configPath))
    {
        makeDirectory(configPath);
    }


    if(!fileExists(configPath + "/plugins.cfg"))
    {
        //std::cout << "Copying default config file: plugins.cfg" << endl;
        LogManager::getSingleton().logMessage("Copying default config file: plugins.cfg");
        copyFile(defaultDataPath + "/default_config/plugins.cfg",configPath + "/plugins.cfg");
    }
    if(!fileExists(configPath + "/resources.cfg"))
    {
        //std::cout << "Copying default config file: resources.cfg" << endl;
        LogManager::getSingleton().logMessage("Copying default config file: resources.cfg");
        copyFile(defaultDataPath + "/default_config/resources.cfg",configPath + "/resources.cfg");
    }
    if(!fileExists(configPath + "/config.cfg"))
    {
        //std::cout << "Copying default config file: config.cfg" << endl;
        LogManager::getSingleton().logMessage("Copying default config file: config.cfg");
        copyFile(defaultDataPath + "/default_config/config.cfg",configPath + "/config.cfg");
    }
#endif





    new ConfigManager(configPath + "/config.cfg");
    if (!ConfigManager::getSingleton().load())
    {
        LogManager::getSingleton().logMessage("Error: could not load config.cfg");
    }

    if(ConfigManager::getSingleton().getString("player_name") == "")
    {
        #if OGRE_PLATFORM == OGRE_PLATFORM_WIN32 //Puto windows
        //averiguar el nombre de usuario en windows???
        #else  //Linux, etc...
        ConfigManager::getSingleton().setValue("player_name",getenv("USER"));
        #endif
    }

    // For gettext support
    //----------------------------------------------------------------

    #if OGRE_PLATFORM == OGRE_PLATFORM_LINUX

    setlocale( LC_ALL, "" );
    bindtextdomain( "xdriller", std::string(ConfigManager::getSingleton().getString("resource_path") + String("/locale")).c_str());
    textdomain( "xdriller" );
    #endif //OGRE_PLATFORM == OGRE_PLATFORM_LINUX

    //----------------------------------------------------------------

    mRoot = new Root(configPath + "/plugins.cfg", "", "");

    {
        Ogre::NameValuePairList opts;
        opts.clear();
        std::string val;
        //Ogre::RenderSystemList *renderSystems = NULL;
        Ogre::RenderSystemList::iterator r_it;

        val = ConfigManager::getSingleton().getString("render_system");

        //renderSystems = mRoot->getAvailableRenderers();

        RenderSystem *tmpRenderSystem = 0;

        //se acabo. solo soporte para opengl y a tomar por culo
        /*bool renderSystemFound = false;
        for (r_it=renderSystems->begin(); r_it!=renderSystems->end(); r_it++) {
            tmpRenderSystem = *r_it;
            std::string rName(tmpRenderSystem->getName());

            if ((int) rName.find(val) >= 0) {
                mRoot->setRenderSystem(*r_it);
                renderSystemFound = true;
                break;
            }
        }*/

        tmpRenderSystem = mRoot->getRenderSystemByName("OpenGL Rendering Subsystem");

        //-----------

        //if (!renderSystemFound) {
        //    LogManager::getSingleton().logMessage("Error: Specified render system (" + val + ") not found");
        //}

        tmpRenderSystem->setConfigOption("Full Screen",ConfigManager::getSingleton().getString("fullscreen"));

        //if(ConfigManager::getSingleton().getString("render_system") == "OpenGL Rendering Subsystem")
        tmpRenderSystem->setConfigOption("Video Mode",ConfigManager::getSingleton().getString("resolution"));
        //else if(ConfigManager::getSingleton().getString("render_system") == "Direct3D9 Rendering Subsystem")
        //{
        //    tmpRenderSystem->setConfigOption("Video Mode",ConfigManager::getSingleton().getString("resolution") + " @ 32-bit colour");
        //}



        //if(ConfigManager::getSingleton().getString("render_system") == "OpenGL Rendering Subsystem")
        tmpRenderSystem->setConfigOption("FSAA",ConfigManager::getSingleton().getString("FSAA"));

        /*else if(ConfigManager::getSingleton().getString("render_system") == "Direct3D9 Rendering Subsystem")
        {
            if(ConfigManager::getSingleton().getString("FSAA") == "0")
                tmpRenderSystem->setConfigOption("Anti aliasing","None");
            if(ConfigManager::getSingleton().getString("FSAA") == "2")
                tmpRenderSystem->setConfigOption("Anti aliasing","Level 2");
            if(ConfigManager::getSingleton().getString("FSAA") == "4")
                tmpRenderSystem->setConfigOption("Anti aliasing","Level 4");
        }*/

        tmpRenderSystem->setConfigOption("RTT Preferred Mode","FBO");
		

        mRoot->setRenderSystem(tmpRenderSystem);

    }

    //LogManager::getSingleton().setLogDetail(LL_LOW);

    // Setup states
    mIntroState = IntroState::getSingletonPtr();
    mPlayState  = PlayState::getSingletonPtr();
    mPauseState = PauseState::getSingletonPtr();
    mMenuState  = MenuState::getSingletonPtr();
    mCreditsState  = CreditsState::getSingletonPtr();
    mHighScoreState  = HighScoreState::getSingletonPtr();

    // Setup and configure game
    this->setupResources();
    if( !this->configureGame() ) {
        // If game can't be configured, throw exception and quit application
        throw Ogre::Exception( Ogre::Exception::ERR_INTERNAL_ERROR,
            "Error - Couldn't Configure Renderwindow",
            "Example - Error" );
        return;
    }

    LogManager::getSingleton().logMessage("Initializing Input Manager...");

    // Setup input //yys
    mInputMgr = InputManager::getSingletonPtr();//mInputMgr = new InputManager();//
	//mRenderWindow = mRoot->getAutoCreatedWindow();
    mInputMgr->initialise( mRenderWindow );
	OgreBites::WindowEventUtilities::addWindowEventListener( mRenderWindow, this );//yys

    LogManager::getSingleton().logMessage("Initializing keyboard listener...");
    mInputMgr->addKeyListener( this, "GameManager" );
    LogManager::getSingleton().logMessage("Initializing mouse listener...");
    mInputMgr->addMouseListener( this, "GameManager" );
    LogManager::getSingleton().logMessage("Initializing joystick listener...");
    mInputMgr->addJoystickListener( this, "GameManager" );
    LogManager::getSingleton().logMessage("Number of joysticks = " + StringConverter::toString(mInputMgr->getNumOfJoysticks()));
    //mInputMgr->getJoystick( 1 );//yys


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

    LogManager::getSingleton().logMessage("Creating BackgroundSceneManager...");

    new BackgroundSceneManager;

    LogManager::getSingleton().logMessage("Creating LevelLoader...");

    new LevelLoader;

    LevelLoader::getSingleton(); ///????

    LogManager::getSingleton().logMessage("Loading High Scores...");

    new HighScoreManager(configPath + "/highscores.xml");

    HighScoreManager::getSingleton().load();

    LogManager::getSingleton().logMessage("Creating Fader...");

    new Fader;

    LogManager::getSingleton().logMessage("Creating RumbleManager...");

    new RumbleManager;






    LogManager::getSingleton().logMessage("Entering IntroState...");

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

        //mStates.back()->updateStateFader();

        Fader::getSingletonPtr()->update(lTimeSinceLastFrame);

		OgreBites::WindowEventUtilities::messagePump();//yys
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
    //if( !mRoot->restoreConfig() ) {
        // If there is no config file, show the configuration dialog
        //if( !mRoot->showConfigDialog() ) {
        //    return false;
        //}
    //}



    /*ConfigManager::getSingleton().setValue("audio_rate","44100");
    ConfigManager::getSingleton().setValue("audio_channels","2");
    ConfigManager::getSingleton().setValue("audio_buffers","4096");
    ConfigManager::getSingleton().setValue("music_volume","64");
    ConfigManager::getSingleton().setValue("sound_volume","100");
    ConfigManager::getSingleton().setValue("resource_path","media/");
    ConfigManager::getSingleton().save();*/

    // Initialise and create a default rendering window
    mRenderWindow = mRoot->initialise( true, "Xdriller v" + String(XDRILLER_VERSION_STRING) );

    // Create needed scenemanagers
    Ogre::SceneManager* sm = mRoot->createSceneManager( Ogre::SMT_DEFAULT, "ST_GENERIC" );
    Ogre::OverlaySystem* os = OGRE_NEW Ogre::OverlaySystem();
    sm->addRenderQueueListener(os);

    // Initialise resources
    ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    return true;
}

void GameManager::setupResources( void ) {
    // Load resource paths from config file
    ConfigFile cf;
    cf.load( configPath + "/resources.cfg" );

    // Go through all settings in the file
    const auto &settings = cf.getSettingsBySection();

    String sSection, sType, sArch;
    for (const auto &sec : settings) {
        sSection = sec.first;

        for (const auto &set : sec.second) {
            sType = set.first;
            sArch = set.second;
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
            ResourceGroupManager::getSingleton().addResourceLocation(
                configPath + sArch, sType, sSection );
#else
            ResourceGroupManager::getSingleton().addResourceLocation(
				sArch, sType, sSection );			
#endif
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

bool GameManager::buttonPressed( const OIS::JoyStickEvent &arg, int button )
{
    OIS::KeyCode kc;

    switch(button)
    {

        case 0:
            kc = OIS::KC_SPACE;
            break;
        case 1:
            kc = OIS::KC_RETURN;
            break;
        case 2:
            kc = OIS::KC_ESCAPE;
            break;
        default:
            return true;

    }

    OIS::KeyEvent e(NULL, kc, 1);

    mStates.back()->keyPressed( e );

    return true;
}
bool GameManager::buttonReleased( const OIS::JoyStickEvent &arg, int button )
{
    OIS::KeyCode kc;

    switch(button)
    {

        case 0:
            kc = OIS::KC_SPACE;
            break;
        case 1:
            kc = OIS::KC_RETURN;
            break;
        case 2:
            kc = OIS::KC_ESCAPE;
            break;
        default:
            return true;

    }

    OIS::KeyEvent e(NULL, kc, 1);

    mStates.back()->keyReleased( e );

    return true;
}
bool GameManager::axisMoved( const OIS::JoyStickEvent &arg, int axis )
{

    OIS::KeyCode kc = OIS::KC_A;

    static bool axisRight = false;
    static bool axisLeft = false;
    static bool axisDown = false;
    static bool axisUp = false;

    if(axis == 0 && arg.state.mAxes[axis].abs < 0)
    {
        if(arg.state.mAxes[axis].abs <= -JOYSTICK_MAX_AXIS*0.6)
        {
            if(axisLeft == false)
            {
                axisLeft = true;
                kc = OIS::KC_LEFT;
            }
        }

    }
    if(axis == 0 && arg.state.mAxes[axis].abs > 0)
    {
        if(arg.state.mAxes[axis].abs >= JOYSTICK_MAX_AXIS*0.6)
        {
            if(axisRight == false)
            {
                axisRight = true;
                kc = OIS::KC_RIGHT;
            }
        }

    }
    if(axis == 1 && arg.state.mAxes[axis].abs < 0)
    {
        if(arg.state.mAxes[axis].abs <= -JOYSTICK_MAX_AXIS*0.6)
        {
            if(axisUp == false)
            {
                axisUp = true;
                kc = OIS::KC_UP;
            }
        }

    }
    if(axis == 1 && arg.state.mAxes[axis].abs > 0)
    {
        if(arg.state.mAxes[axis].abs >= JOYSTICK_MAX_AXIS*0.6)
        {
            if(axisDown == false)
            {
                axisDown = true;
                kc = OIS::KC_DOWN;
            }
        }

    }

    if(axis == 0 && abs(arg.state.mAxes[axis].abs) <= JOYSTICK_MAX_AXIS*0.3)
    {
        axisLeft = false;
        axisRight = false;
    }
    if(axis == 1 && abs(arg.state.mAxes[axis].abs) <= JOYSTICK_MAX_AXIS*0.3)
    {
        axisUp = false;
        axisDown = false;
    }



    if(kc == OIS::KC_LEFT || kc == OIS::KC_RIGHT || kc == OIS::KC_UP || kc == OIS::KC_DOWN)
    {
        OIS::KeyEvent e(NULL, kc, 1);

        mStates.back()->keyPressed( e );
    }
    return true;
}

GameManager* GameManager::getSingletonPtr( void ) {
    if( !mGameManager ) {
        mGameManager = new GameManager();
    }

    return mGameManager;
}
