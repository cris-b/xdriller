#include "MenuState.h"
#include <OgreTextAreaOverlayElement.h>
#include <OgreFontManager.h>

#include "ConfigManager.h"
#include "Tools.h"
#include "LevelLoader.h"
#include "SoundManager.h"
#include "Arrows.h"
#include "DotScene.h"

#include "Gettext.h"


#ifndef PlayState_H
#include "PlayState.h"
#endif
#ifndef CreditsState_H
#include "CreditsState.h"
#endif

//BORRAR cuando no se acaben las pruebas
#ifndef HighScoreState_H
#include "HighScoreState.h"
#endif

#define MENU_PAGE_MAIN                  0
#define MENU_PAGE_OPTIONS               1
#define MENU_PAGE_CREDITS               2
#define MENU_PAGE_QUIT                  3
#define MENU_PAGE_LEVELSELECT           4
#define MENU_PAGE_GRAPHIC_OPTIONS       5
#define MENU_PAGE_AUDIO_OPTIONS         6
#define MENU_PAGE_PLAYER_OPTIONS        7
#define MENU_PAGE_GAME_MODE             8
#define MENU_PAGE_CONTROLS              9
#define MENU_PAGE_HIGHSCORES           10


using namespace Ogre;


MenuState* MenuState::mMenuState;



void MenuState::enter( void )
{
    menuPage = MENU_PAGE_MAIN;
    menuCursor = 0;



    mRoot         = Root::getSingletonPtr();
    mOverlayMgr   = OverlayManager::getSingletonPtr();
    mSceneMgr     = mRoot->getSceneManager( "ST_GENERIC" );
    mCamera       = mSceneMgr->createCamera( "MenuCamera" );
    mViewport     = mRoot->getAutoCreatedWindow()->addViewport( mCamera );

    mCamera->setNearClipDistance(0.1);
    mCamera->setFarClipDistance(2000);

    //load scene
    //--------------------------------------------

    LevelLoader::getSingleton().setLevelName(ConfigManager::getSingleton().getString("last_bg_scene"));

    SceneNode *backgroundSceneNode = mSceneMgr->getRootSceneNode()->createChildSceneNode( "backgroundSceneNode" , Vector3(0,0,0));

    // Load background scene
    CDotScene dotScene;
    //dotScene = new CDotScene();

    String sceneFilename = LevelLoader::getSingleton().getValue("background_scene") + ".xml";

    dotScene.parseDotScene(sceneFilename,"General",mSceneMgr, backgroundSceneNode, "background_");

    //Setup fog and bg color
    //--------------------------------------------------------------------
    ColourValue c = StringConverter::parseColourValue(LevelLoader::getSingleton().getValue("background_color"));
    String s = LevelLoader::getSingleton().getValue("background_color");
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
     SceneNode* mParticleEffectNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("sceneParticleEffectNode");
     mParticleEffectNode->attachObject(mParticleEffect);

     //set particle node to camera pos

     mParticleEffectNode->setPosition(0,0,10);

    }
    //-----------------------------------------------------------------------



    mSceneMgr->setAmbientLight(ColourValue(0.7,0.7,0.7));

    mCamera->setPosition(0,0,10);
    mCamera->setNearClipDistance(1);
    mCamera->setFarClipDistance(10000);

    Light *light = mSceneMgr->createLight("MenuLight1");
    light->setType(Light::LT_DIRECTIONAL);
    light->setDirection(Vector3(0.5, -0.8, -0.5));


    light->setDiffuseColour(0.9,0.9,0.9);
    light->setSpecularColour(0.5, 0.5, 0.5);
    //light->setAttenuation(200,1.0,0.022,0.0019);
    light->setCastShadows(false);


    Light *light2 = mSceneMgr->createLight("MenuLight2");
    light2->setType(Light::LT_POINT);
    light2->setPosition(Vector3(0, 0, 5));


    light2->setDiffuseColour(0.1, 0.1, 0.1);
    light2->setSpecularColour(0.2, 0.2, 0.2);
    light2->setAttenuation(200,1.0,0.022,0.0019);

    light2->setCastShadows(false);

    //mSceneMgr->setFog(FOG_LINEAR, ColourValue(1,1,1), 0.0, 20, 50);

    /*Rectangle2D* toprect = new Rectangle2D(true);
    toprect->setCorners(-1.0, 1.0, 1.0, -1.0);
    toprect->setMaterial("foreground");
    toprect->setRenderQueueGroup(RENDER_QUEUE_6);

    SceneNode* foregorund_node = mSceneMgr->getRootSceneNode()->createChildSceneNode("foreground_node");
    foregorund_node->attachObject(toprect);*/



    if(ConfigManager::getSingleton().getInt("compositors"))
    {
        CompositorManager::getSingleton().addCompositor(mViewport, "gaussian_blur");
        CompositorManager::getSingleton().setCompositorEnabled(mViewport, "gaussian_blur", true);
    }

    mPanel = static_cast<PanelOverlayElement*>(
        mOverlayMgr->createOverlayElement("Panel", "PlayStateOverlayPanel"));
    mPanel->setMetricsMode(Ogre::GMM_RELATIVE);
    mPanel->setPosition(0, 0);
    mPanel->setDimensions(1, 1);

    mInfoTextArea = static_cast<TextAreaOverlayElement*>(
        mOverlayMgr->createOverlayElement("TextArea", "InfoTextArea"));
    mInfoTextArea->setMetricsMode(Ogre::GMM_RELATIVE);
    mInfoTextArea->setPosition(0.98, 0.95);
    mInfoTextArea->setDimensions(1, 0.1);
    mInfoTextArea->setCaption("{ Fixi Studios }");
    mInfoTextArea->setCharHeight(0.035);
    mInfoTextArea->setFontName("SmallFont");
    //mInfoTextArea->setColourBottom(ColourValue(1.0, 1, 1.0));
    //mInfoTextArea->setColourTop(ColourValue(0, 0, 0));
    mInfoTextArea->setColour(ColourValue(0,0,0));
    mInfoTextArea->setAlignment(TextAreaOverlayElement::Right);


    //mLogoXDriller->setScroll(0,1.5);

    mLogoXDriller = static_cast<OverlayContainer*>(
        mOverlayMgr->createOverlayElement("Panel", "LogoOverlay"));
    mLogoXDriller->setMetricsMode(Ogre::GMM_RELATIVE);
    mLogoXDriller->setHorizontalAlignment(Ogre::GHA_CENTER);
    mLogoXDriller->setVerticalAlignment(Ogre::GVA_CENTER);
    mLogoXDriller->setPosition(-0.24375, -0.48);
    mLogoXDriller->setDimensions(0.4875, 0.254166667);
    mLogoXDriller->setMaterialName("LogoXDriller");


    mLevelScreenshot_shadow = static_cast<PanelOverlayElement*>(
        mOverlayMgr->createOverlayElement("Panel", "LevelScreenshot_shadow"));
    mLevelScreenshot_shadow->setMetricsMode(Ogre::GMM_RELATIVE);
    mLevelScreenshot_shadow->setHorizontalAlignment(Ogre::GHA_CENTER);
    mLevelScreenshot_shadow->setVerticalAlignment(Ogre::GVA_CENTER);
    mLevelScreenshot_shadow->setPosition(-0.02, -0.23);
    mLevelScreenshot_shadow->setDimensions(0.44, 0.46);
    mLevelScreenshot_shadow->setMaterialName("level_screenshot_shadow");

    mLevelScreenshot = static_cast<PanelOverlayElement*>(
        mOverlayMgr->createOverlayElement("Panel", "LevelScreenshot"));
    mLevelScreenshot->setMetricsMode(Ogre::GMM_RELATIVE);
    mLevelScreenshot->setHorizontalAlignment(Ogre::GHA_CENTER);
    mLevelScreenshot->setVerticalAlignment(Ogre::GVA_CENTER);
    mLevelScreenshot->setPosition(0.0, -0.2);
    mLevelScreenshot->setDimensions(0.4, 0.4);
    mLevelScreenshot->setMaterialName("level_screenshot");

    mBigImage = static_cast<PanelOverlayElement*>(
        mOverlayMgr->createOverlayElement("Panel", "mBigImage"));
    mBigImage->setMetricsMode(Ogre::GMM_RELATIVE);
    mBigImage->setHorizontalAlignment(Ogre::GHA_CENTER);
    mBigImage->setVerticalAlignment(Ogre::GVA_CENTER);
    mBigImage->setPosition(-0.3, 0.2);
    mBigImage->setDimensions(0.6, 0.2);
    mBigImage->setMaterialName("keyboard_layout");


    mLevelInfo = static_cast<TextAreaOverlayElement*>(
        mOverlayMgr->createOverlayElement("TextArea", "LevelInfoTextArea"));
    mLevelInfo->setMetricsMode(Ogre::GMM_RELATIVE);
    mLevelInfo->setPosition(0.05, 0.3);
    mLevelInfo->setDimensions(0.5, 0.5);
    mLevelInfo->setCaption("...");
    mLevelInfo->setCharHeight(0.035);
    mLevelInfo->setFontName("SmallFont");
    //mLevelInfo->setColourBottom(ColourValue(1.0, 1, 1.0));
    //mLevelInfo->setColourTop(ColourValue(0, 0, 0));
    mLevelInfo->setColour(ColourValue(0,0,0));
    mLevelInfo->setAlignment(TextAreaOverlayElement::Left);

    mOverlay = mOverlayMgr->create("MenuOverlay");

    mOverlay->add2D(mPanel);

    mOverlay->add2D(mLevelScreenshot_shadow);
    mOverlay->add2D(mLevelScreenshot);
    mOverlay->add2D(mBigImage);

    mPanel->addChild(mInfoTextArea);
    mPanel->addChild(mLevelInfo);






    mOverlay->setZOrder(100);

    // Show the overlay
    mOverlay->show();

    arrows = new Arrows;

    arrows->hide();

    titleButton = NULL;


    changePage(menuPage);
    //changePage(MENU_PAGE_GAME_MODE);

    SoundManager::getSingleton().loadMusic("menu_music.ogg");
    SoundManager::getSingleton().playMusic(true);

    Fader::getSingletonPtr()->fadeIn();




}

void MenuState::exit( void )
{
    if(ConfigManager::getSingleton().getInt("compositors"))
    {
        CompositorManager::getSingleton().setCompositorEnabled(mViewport, "gaussian_blur", false);
    }

    delete arrows;

    mOverlayMgr->destroyOverlayElement(mLogoXDriller);
    mOverlayMgr->destroyOverlayElement(mLevelScreenshot);
    mOverlayMgr->destroyOverlayElement(mLevelScreenshot_shadow);
    mOverlayMgr->destroyOverlayElement(mBigImage);
    mOverlayMgr->destroyOverlayElement(mLevelInfo);
    mOverlayMgr->destroyOverlayElement(mInfoTextArea);
    mOverlayMgr->destroyOverlayElement(mPanel);


    mOverlayMgr->destroy(mOverlay);

    if(titleButton != NULL)
    {
        delete titleButton;
        titleButton = NULL;
    }

    while (!buttons.empty())
    {
        delete buttons.back();
        buttons.pop_back();
    }

    if(ringSwitcher != NULL)
    {
        delete ringSwitcher;
        ringSwitcher = NULL;
    }
    if(playerModelSelector != NULL)
    {
        delete playerModelSelector;
        playerModelSelector = NULL;
    }

    mSceneMgr->setShadowTechnique(SHADOWTYPE_NONE);

    mSceneMgr->clearScene();
    mSceneMgr->destroyAllCameras();
    mRoot->getAutoCreatedWindow()->removeAllViewports();



}

void MenuState::pause( void ) {

}

void MenuState::resume( void ) {

}

void MenuState::update( unsigned long lTimeElapsed )
{

    arrows->update(lTimeElapsed);

    if(ringSwitcher != NULL)
    {
        ringSwitcher->update(lTimeElapsed);
    }
    if(playerModelSelector != NULL)
    {
        playerModelSelector->update(lTimeElapsed);
    }




    if(!buttons.empty())
    {
       for(unsigned int i=0; i < buttons.size(); i++)
       {
          buttons[i]->update(lTimeElapsed);
       }

    }

    if(titleButton != NULL)
    {
        titleButton->update(lTimeElapsed);


    }




}

void MenuState::keyPressed( const OIS::KeyEvent &e )
{

    if( e.key == OIS::KC_2)
    {
        //prueba
        fadeState(HighScoreState::getSingletonPtr());
    }

    //captura de pantalla
    if(e.key == OIS::KC_S)
    {

        //solo funciona en ogre 1.6 +
        #if OGRE_VERSION_MINOR >= 6
        TexturePtr tp = static_cast<Ogre::TexturePtr> (TextureManager::getSingleton().getByName("SmallFontTexture"));
        const size_t buffSize = (tp->getWidth() * tp->getHeight() * 4);
        unsigned char *data = OGRE_ALLOC_T(unsigned char,buffSize,Ogre::MEMCATEGORY_GENERAL);
        memset(data, 0, buffSize);
        Ogre::Image i;
        i.loadDynamicImage(data, tp->getWidth(), tp->getHeight(), 1, Ogre::PF_R8G8B8A8, true);

        Ogre::HardwarePixelBufferSharedPtr buf = tp->getBuffer();
        const Ogre::PixelBox destBox = i.getPixelBox();
        buf->blitToMemory(destBox);

        // Save to disk!
        i.save("screenshoot.png");
        #endif


    }

    if(e.key == OIS::KC_DOWN)
    {

        buttons[menuCursor]->setState(BSTATE_INACTIVE);
        menuCursor++;
        if(menuCursor >= (int)buttons.size())
        {
            menuCursor = 0;

        }
        buttons[menuCursor]->setState(BSTATE_ACTIVE);

        if(buttons.size() > 1)SoundManager::getSingleton().playSound(SOUND_MENU1);

        _updateArrows();


    }
    if(e.key == OIS::KC_UP)
    {

        buttons[menuCursor]->setState(BSTATE_INACTIVE);
        menuCursor--;
        if(menuCursor < 0)
        {
            menuCursor = buttons.size()-1;

        }
        buttons[menuCursor]->setState(BSTATE_ACTIVE);

        if(buttons.size() > 1)SoundManager::getSingleton().playSound(SOUND_MENU1);

        _updateArrows();

    }
    if(e.key == OIS::KC_LEFT)
    {
        arrows->left();


       switch(menuPage)
        {
            case MENU_PAGE_GRAPHIC_OPTIONS:
            {

                if(menuCursor == 0)
                {
                    if(buttons[0]->getOptionCaption() == "1024 x 768") ConfigManager::getSingleton().setValue("resolution","800 x 600");
                    else if(buttons[0]->getOptionCaption() == "800 x 600") ConfigManager::getSingleton().setValue("resolution","640 x 480");
                    else if(buttons[0]->getOptionCaption() == "640 x 480") ConfigManager::getSingleton().setValue("resolution","1024 x 768");
                    else ConfigManager::getSingleton().setValue("resolution","1024 x 768");

                    buttons[0]->setOptionCaption(ConfigManager::getSingleton().getString("resolution"));
                    SoundManager::getSingleton().playSound(SOUND_MENU3);
                }
                if(menuCursor == 1)
                {
                    if(buttons[1]->getOptionCaption() == "0") ConfigManager::getSingleton().setValue("FSAA","4");
                    if(buttons[1]->getOptionCaption() == "2") ConfigManager::getSingleton().setValue("FSAA","0");
                    if(buttons[1]->getOptionCaption() == "4") ConfigManager::getSingleton().setValue("FSAA","2");

                    buttons[1]->setOptionCaption(ConfigManager::getSingleton().getString("FSAA"));
                    SoundManager::getSingleton().playSound(SOUND_MENU3);
                }
                if(menuCursor == 2)
                {
                    if(buttons[2]->getOptionCaption() == "Yes")
                    {
                        ConfigManager::getSingleton().setValue("fullscreen","No");
                        buttons[2]->setOptionCaption(_("No"));
                    }
                    else if(buttons[2]->getOptionCaption() == "No")
                    {
                        ConfigManager::getSingleton().setValue("fullscreen","Yes");
                        buttons[2]->setOptionCaption(_("Yes"));
                    }


                    SoundManager::getSingleton().playSound(SOUND_MENU3);
                }


                break;
            }
            case MENU_PAGE_AUDIO_OPTIONS:
            {

                if(menuCursor == 0)
                {
                    SoundManager::getSingleton().setMusicVolume(SoundManager::getSingleton().getMusicVolume()-10);

                    ConfigManager::getSingleton().setValue("music_volume",StringConverter::toString(SoundManager::getSingleton().getMusicVolume()));

                    buttons[0]->setOptionCaption(StringConverter::toString(SoundManager::getSingleton().getMusicVolume()));
                    SoundManager::getSingleton().playSound(SOUND_MENU3);
                }
                if(menuCursor == 1)
                {
                    SoundManager::getSingleton().setSoundVolume(SoundManager::getSingleton().getSoundVolume()-10);

                    ConfigManager::getSingleton().setValue("sound_volume",StringConverter::toString(SoundManager::getSingleton().getSoundVolume()));

                    buttons[1]->setOptionCaption(StringConverter::toString(SoundManager::getSingleton().getSoundVolume()));
                    SoundManager::getSingleton().playSound(SOUND_AIR);
                }

                break;
            }
            case MENU_PAGE_LEVELSELECT:
            {

                if(menuCursor == 0)
                {
                    LevelLoader::getSingleton().prevLevel();

                    _updateLevelSelect();
                    SoundManager::getSingleton().playSound(SOUND_MENU3);
                }

                break;
            }
            case MENU_PAGE_GAME_MODE:
            {

                ringSwitcher->prev();

                buttons[0]->setCaption(ringSwitcher->getCurrentName());
                SoundManager::getSingleton().playSound(SOUND_MENU3);

                break;
            }
            case MENU_PAGE_PLAYER_OPTIONS:
            {
                if(menuCursor == 0)
                {
                }
                if(menuCursor == 1)
                {
                    playerModelSelector->prev();

                    buttons[1]->setOptionCaption(playerModelSelector->getName());
                    SoundManager::getSingleton().playSound(SOUND_MENU3);
                }
                break;
            }
            case MENU_PAGE_HIGHSCORES:
            {
                if(menuCursor == 0)
                {
                    if(buttons[0]->getOptionCaption() == _("Survive")) buttons[0]->setOptionCaption(_("Adventure"));
                    else if(buttons[0]->getOptionCaption() == _("Adventure")) buttons[0]->setOptionCaption(_("Time Attack"));
                    else if(buttons[0]->getOptionCaption() == _("Time Attack")) buttons[0]->setOptionCaption(_("Survive"));

                    SoundManager::getSingleton().playSound(SOUND_MENU3);
                }
                if(menuCursor == 1)
                {
                    int level_num = LevelLoader::getSingleton().getLevelNum();
                    int num_levels = LevelLoader::getSingleton().getNumLevels();

                    level_num--;

                    if(level_num < 0) level_num = num_levels-1;

                    LevelLoader::getSingleton().setLevelNum(level_num);



                    buttons[1]->setOptionCaption(LevelLoader::getSingleton().getLongName());
                    SoundManager::getSingleton().playSound(SOUND_MENU3);
                }

                if(buttons[0]->getOptionCaption() == _("Survive"))
                    highScoreTable->updateTable("Survive", LevelLoader::getSingleton().getLevelName());
                else if(buttons[0]->getOptionCaption() == _("Adventure"))
                    highScoreTable->updateTable("Adventure", LevelLoader::getSingleton().getLevelName());
                else if(buttons[0]->getOptionCaption() == _("Time Attack"))
                    highScoreTable->updateTable("Time Attack", LevelLoader::getSingleton().getLevelName());

                break;
            }
        }
        _updateArrows();
    }
    if(e.key == OIS::KC_RIGHT)
    {
        arrows->right();

       switch(menuPage)
        {
            case MENU_PAGE_GRAPHIC_OPTIONS:
            {

                if(menuCursor == 0)
                {
                    if(buttons[0]->getOptionCaption() == "1024 x 768") ConfigManager::getSingleton().setValue("resolution","640 x 480");
                    if(buttons[0]->getOptionCaption() == "800 x 600") ConfigManager::getSingleton().setValue("resolution","1024 x 768");
                    if(buttons[0]->getOptionCaption() == "640 x 480") ConfigManager::getSingleton().setValue("resolution","800 x 600");

                    buttons[0]->setOptionCaption(ConfigManager::getSingleton().getString("resolution"));

                    SoundManager::getSingleton().playSound(SOUND_MENU3);
                }
                if(menuCursor == 1)
                {
                    if(buttons[1]->getOptionCaption() == "0") ConfigManager::getSingleton().setValue("FSAA","2");
                    if(buttons[1]->getOptionCaption() == "2") ConfigManager::getSingleton().setValue("FSAA","4");
                    if(buttons[1]->getOptionCaption() == "4") ConfigManager::getSingleton().setValue("FSAA","0");

                    buttons[1]->setOptionCaption(ConfigManager::getSingleton().getString("FSAA"));
                    SoundManager::getSingleton().playSound(SOUND_MENU3);
                }
                if(menuCursor == 2)
                {
                    if(buttons[2]->getOptionCaption() == "Yes")
                    {
                        ConfigManager::getSingleton().setValue("fullscreen","No");
                        buttons[2]->setOptionCaption(_("No"));
                    }
                    else if(buttons[2]->getOptionCaption() == "No")
                    {
                        ConfigManager::getSingleton().setValue("fullscreen","Yes");
                        buttons[2]->setOptionCaption(_("Yes"));
                    }


                    SoundManager::getSingleton().playSound(SOUND_MENU3);
                }

                break;
            }
            case MENU_PAGE_AUDIO_OPTIONS:
            {

                if(menuCursor == 0)
                {
                    SoundManager::getSingleton().setMusicVolume(SoundManager::getSingleton().getMusicVolume()+10);

                    ConfigManager::getSingleton().setValue("music_volume",StringConverter::toString(SoundManager::getSingleton().getMusicVolume()));

                    buttons[0]->setOptionCaption(StringConverter::toString(SoundManager::getSingleton().getMusicVolume()));
                    SoundManager::getSingleton().playSound(SOUND_MENU3);
                }
                if(menuCursor == 1)
                {
                    SoundManager::getSingleton().setSoundVolume(SoundManager::getSingleton().getSoundVolume()+10);

                    ConfigManager::getSingleton().setValue("sound_volume",StringConverter::toString(SoundManager::getSingleton().getSoundVolume()));

                    buttons[1]->setOptionCaption(StringConverter::toString(SoundManager::getSingleton().getSoundVolume()));
                    SoundManager::getSingleton().playSound(SOUND_AIR);
                }

                break;
            }
            case MENU_PAGE_LEVELSELECT:
            {

                if(menuCursor == 0)
                {
                    LevelLoader::getSingleton().nextLevel();

                    _updateLevelSelect();
                    SoundManager::getSingleton().playSound(SOUND_MENU3);
                }

                break;
            }
            case MENU_PAGE_GAME_MODE:
            {

                ringSwitcher->next();

                buttons[0]->setCaption(ringSwitcher->getCurrentName());
                SoundManager::getSingleton().playSound(SOUND_MENU3);

                break;
            }
            case MENU_PAGE_PLAYER_OPTIONS:
            {
                if(menuCursor == 0)
                {
                }
                if(menuCursor == 1)
                {
                    playerModelSelector->next();

                    buttons[1]->setOptionCaption(playerModelSelector->getName());
                    SoundManager::getSingleton().playSound(SOUND_MENU3);
                }

                break;
            }
            case MENU_PAGE_HIGHSCORES:
            {
                if(menuCursor == 0)
                {
                    if(buttons[0]->getOptionCaption() == _("Survive")) buttons[0]->setOptionCaption(_("Adventure"));
                    else if(buttons[0]->getOptionCaption() == _("Adventure")) buttons[0]->setOptionCaption(_("Time Attack"));
                    else if(buttons[0]->getOptionCaption() == _("Time Attack")) buttons[0]->setOptionCaption(_("Survive"));

                    SoundManager::getSingleton().playSound(SOUND_MENU3);
                }
                if(menuCursor == 1)
                {
                    int level_num = LevelLoader::getSingleton().getLevelNum();
                    int num_levels = LevelLoader::getSingleton().getNumLevels();

                    level_num++;

                    if(level_num > num_levels-1) level_num = 0;

                    LevelLoader::getSingleton().setLevelNum(level_num);



                    buttons[1]->setOptionCaption(LevelLoader::getSingleton().getLongName());
                    SoundManager::getSingleton().playSound(SOUND_MENU3);
                }

                if(buttons[0]->getOptionCaption() == _("Survive"))
                    highScoreTable->updateTable("Survive", LevelLoader::getSingleton().getLevelName());
                else if(buttons[0]->getOptionCaption() == _("Adventure"))
                    highScoreTable->updateTable("Adventure", LevelLoader::getSingleton().getLevelName());
                else if(buttons[0]->getOptionCaption() == _("Time Attack"))
                    highScoreTable->updateTable("Time Attack", LevelLoader::getSingleton().getLevelName());


                break;
            }
        }
        _updateArrows();
    }
    if( e.key == OIS::KC_RETURN)
    {

        switch(menuPage)
        {
            case MENU_PAGE_MAIN:
            {
                if(menuCursor == 0)
                {
                    changePage(MENU_PAGE_GAME_MODE);
                    SoundManager::getSingleton().playSound(SOUND_MENU2);

                }
                if(menuCursor == 1)
                {
                    changePage(MENU_PAGE_OPTIONS);
                    SoundManager::getSingleton().playSound(SOUND_MENU2);
                }
                if(menuCursor == 2)
                {
                    fadeState( CreditsState::getSingletonPtr() );
                    SoundManager::getSingleton().playSound(SOUND_MENU2);
                }
                if(menuCursor == 3)
                {
                    changePage(MENU_PAGE_HIGHSCORES);
                    SoundManager::getSingleton().playSound(SOUND_MENU2);
                }
                if(menuCursor == 4)
                {
                    changePage(MENU_PAGE_QUIT);
                    SoundManager::getSingleton().playSound(SOUND_MENU2);
                }

                break;
            }
            case MENU_PAGE_QUIT:
            {
                if(menuCursor == 0)
                {
                    changePage(MENU_PAGE_MAIN);
                    SoundManager::getSingleton().playSound(SOUND_MENU4);
                }
                if(menuCursor == 1) this->requestShutdown();

                break;
            }
            case MENU_PAGE_OPTIONS:
            {

                if(menuCursor == 0)
                {
                    changePage(MENU_PAGE_GRAPHIC_OPTIONS);
                    SoundManager::getSingleton().playSound(SOUND_MENU2);
                }
                if(menuCursor == 1)
                {
                    changePage(MENU_PAGE_AUDIO_OPTIONS);
                    SoundManager::getSingleton().playSound(SOUND_MENU2);
                }
                if(menuCursor == 2)
                {
                    changePage(MENU_PAGE_CONTROLS);
                    SoundManager::getSingleton().playSound(SOUND_MENU2);
                }
                if(menuCursor == 3)
                {
                    changePage(MENU_PAGE_PLAYER_OPTIONS);
                    SoundManager::getSingleton().playSound(SOUND_MENU2);
                }
                if(menuCursor == 4)
                {
                    changePage(MENU_PAGE_MAIN);
                    SoundManager::getSingleton().playSound(SOUND_MENU4);
                }

                break;
            }
            case MENU_PAGE_GRAPHIC_OPTIONS:
            {
                if(menuCursor == 3)
                {
                    bool _fs;
                    unsigned int _w, _h;

                    if(ConfigManager::getSingleton().getString("fullscreen") == "Yes")
                    {
                        _fs = true;
                    }
                    else
                    {
                        _fs = false;
                    }

                    //extrae la W y H de de cadena tipo "640 x 480"
                    std::stringstream strm;
                    String word;

                    strm << ConfigManager::getSingleton().getString("resolution");

                    strm >> word;

                    _w = StringConverter::parseUnsignedInt(word);

                    strm >> word; //extrae la "x"

                    strm >> word;

                    _h = StringConverter::parseUnsignedInt(word);

                    mRoot->getAutoCreatedWindow()->setFullscreen(_fs,_w,_h);

                    //SoundManager::getSingleton().playSound(SOUND_MENU2);
                }
                if(menuCursor == 4)
                {
                    changePage(MENU_PAGE_OPTIONS);
                    SoundManager::getSingleton().playSound(SOUND_MENU4);
                }

                break;
            }
            case MENU_PAGE_AUDIO_OPTIONS:
            {

                if(menuCursor == 2)
                {
                    changePage(MENU_PAGE_OPTIONS);
                    SoundManager::getSingleton().playSound(SOUND_MENU4);
                }

                break;
            }
            case MENU_PAGE_GAME_MODE:
            {

                if(menuCursor == 0)
                {
                    if(ringSwitcher->getCurrentName() == _("Survive"))
                    {
                        changePage(MENU_PAGE_LEVELSELECT);
                    }
                    SoundManager::getSingleton().playSound(SOUND_MENU2);
                    return;
                }

                break;
            }

            case MENU_PAGE_LEVELSELECT:
            {

                if(menuCursor == 0)
                {
                    //LevelLoader::getSingleton().setLevelName("test");
                    fadeState( PlayState::getSingletonPtr() );
                    //changingState = GAMESTATE_PLAY;
                    SoundManager::getSingleton().playSound(SOUND_MENU2);
                    return;
                }

                break;
            }




        }

    }
    if( e.key == OIS::KC_ESCAPE)
    {

        switch(menuPage)
        {
            case MENU_PAGE_MAIN:
            {
                changePage(MENU_PAGE_QUIT);
                SoundManager::getSingleton().playSound(SOUND_MENU4);
                break;
            }
            case MENU_PAGE_GAME_MODE:
            case MENU_PAGE_OPTIONS:
            case MENU_PAGE_HIGHSCORES:
            {
                changePage(MENU_PAGE_MAIN);
                SoundManager::getSingleton().playSound(SOUND_MENU4);
                break;
            }
            case MENU_PAGE_LEVELSELECT:
            {
                changePage(MENU_PAGE_GAME_MODE);
                SoundManager::getSingleton().playSound(SOUND_MENU4);
                break;
            }
            case MENU_PAGE_AUDIO_OPTIONS:
            case MENU_PAGE_CONTROLS:
            case MENU_PAGE_PLAYER_OPTIONS:
            case MENU_PAGE_GRAPHIC_OPTIONS:
            {

                changePage(MENU_PAGE_OPTIONS);
                SoundManager::getSingleton().playSound(SOUND_MENU4);
                break;
            }

        }

    }

}

void MenuState::keyReleased( const OIS::KeyEvent &e )
{
    if(e.key == OIS::KC_K)
    {
        Fader::getSingletonPtr()->enable();
    }
    else if(e.key == OIS::KC_L)
    {
        Fader::getSingletonPtr()->disable();
    }


}

void MenuState::mouseMoved( const OIS::MouseEvent &e )
{
    //btn->setPosition(Vector3((e.state.X.abs / 640.0)+0.5,(e.state.Y.abs / 480.0)+0.5,0));
}

void MenuState::mousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id )
{

}

void MenuState::mouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id )
{

}

MenuState* MenuState::getSingletonPtr( void ) {
    if( !mMenuState ) {
        mMenuState = new MenuState();
    }

    return mMenuState;
}

void MenuState::changePage(unsigned int page)
{
    menuPage = page;

    arrows->hide();
    mLevelScreenshot->hide();
    mLevelScreenshot_shadow->hide();
    mLevelInfo->hide();
    mBigImage->hide();

    while (!buttons.empty())
    {
        delete buttons.back();  buttons.pop_back();
    }
    if(titleButton != NULL)
    {
        delete titleButton;
        titleButton = NULL;
    }

    if(ringSwitcher != NULL)
    {
        delete ringSwitcher;
        ringSwitcher = NULL;
    }

    if(playerModelSelector != NULL)
    {
        delete playerModelSelector;
        playerModelSelector = NULL;
    }

    if(highScoreTable != NULL)
    {
        delete highScoreTable;
        highScoreTable = NULL;
    }

    mOverlay->remove2D(mLogoXDriller);

    switch(menuPage)
    {
        case MENU_PAGE_MAIN:
        {
            mOverlay->add2D(mLogoXDriller);

            buttons.push_back(new MenuButton(_("Play Game")));

            buttons[0]->setPosition(0,-0.15);
            buttons[0]->setState(BSTATE_ACTIVE);


            buttons.push_back(new MenuButton(_("Options")));

            buttons[1]->setPosition(0,-0.05);


            buttons.push_back(new MenuButton(_("Credits")));

            buttons[2]->setPosition(0,0.05);

            buttons.push_back(new MenuButton(_("High Scores")));

            buttons[3]->setPosition(0,0.15);
            //buttons[3]->setBlocked(true);


            buttons.push_back(new MenuButton(_("Exit")));

            buttons[4]->setPosition(0,0.25);


            menuCursor = 0;

            mInfoTextArea->setCaption(_("Fixi Studios makes cool games for you"));

            break;
        }

        case MENU_PAGE_AUDIO_OPTIONS:
        {
            //mOverlay->add2D(mLogoXDriller);
            titleButton = new MenuButton(_("Audio Options"));
            titleButton->setPosition(0,-0.45);

            titleButton->setColor(ColourValue(1,0,0));

            buttons.push_back(new MenuButton(_("Music Volume"),ALIGN_LEFT,true,true));

            buttons[0]->setPosition(-0.4,-0.1);
            buttons[0]->setState(BSTATE_ACTIVE);

            buttons[0]->setOptionCaption(ConfigManager::getSingleton().getString("music_volume"));

            buttons.push_back(new MenuButton(_("FX Volume"),ALIGN_LEFT,true,true));

            buttons[1]->setPosition(-0.4,0);

            buttons[1]->setOptionCaption(ConfigManager::getSingleton().getString("sound_volume"));


            buttons.push_back(new MenuButton(_("Back"),ALIGN_LEFT));

            buttons[2]->setPosition(-0.4,0.1);


            menuCursor = 0;

            mInfoTextArea->setCaption(_("We are electric..."));

            break;
        }

        case MENU_PAGE_GRAPHIC_OPTIONS:
        {
            //mOverlay->add2D(mLogoXDriller);
            titleButton = new MenuButton(_("Graphic Options"));
            titleButton->setPosition(0,-0.45);

            titleButton->setColor(ColourValue(1,0,0));

            buttons.push_back(new MenuButton(_("Resolution"),ALIGN_LEFT,true,true));

            buttons[0]->setPosition(-0.4,-0.2);
            buttons[0]->setState(BSTATE_ACTIVE);

            buttons[0]->setOptionCaption(ConfigManager::getSingleton().getString("resolution"));

            buttons.push_back(new MenuButton(_("Anti-aliasing"),ALIGN_LEFT,true,true));

            buttons[1]->setPosition(-0.4,-0.1);
            buttons[1]->setOptionCaption(ConfigManager::getSingleton().getString("FSAA"));

            buttons.push_back(new MenuButton(_("Fullscreen"),ALIGN_LEFT,true,true));

            buttons[2]->setPosition(-0.4,0);
            buttons[2]->setOptionCaption(ConfigManager::getSingleton().getString("fullscreen"));


            buttons.push_back(new MenuButton(_("Apply"),ALIGN_LEFT));

            buttons[3]->setPosition(-0.4,0.1);

            buttons.push_back(new MenuButton(_("Back"),ALIGN_LEFT));

            buttons[4]->setPosition(-0.4,0.2);


            menuCursor = 0;

            mInfoTextArea->setCaption(_("ATENTION: restart the game for new settings to apply correctly"));



            break;
        }
        case MENU_PAGE_OPTIONS:
        {
            //mOverlay->add2D(mLogoXDriller);

            titleButton = new MenuButton(_("Options"));
            titleButton->setPosition(0,-0.45);
            titleButton->setColor(ColourValue(1,0,0));

            buttons.push_back(new MenuButton(_("Graphic Options")));

            buttons[0]->setPosition(0,-0.2);
            buttons[0]->setState(BSTATE_ACTIVE);



            buttons.push_back(new MenuButton(_("Audio Options")));

            buttons[1]->setPosition(0,-0.1);



            buttons.push_back(new MenuButton(_("Controls")));

            buttons[2]->setPosition(0,0);
            //buttons[2]->setBlocked(true);

            buttons.push_back(new MenuButton(_("Player")));

            buttons[3]->setPosition(0,0.1);


            buttons.push_back(new MenuButton(_("Back")));

            buttons[4]->setPosition(0,0.2);


            menuCursor = 0;

            mInfoTextArea->setCaption(_("It's good to have options"));

            break;
        }

        case MENU_PAGE_PLAYER_OPTIONS:
        {
            playerModelSelector = new PlayerModelSelector();

            playerModelSelector->setPosition(0,0,0);

            titleButton = new MenuButton(_("Player Options"));
            titleButton->setPosition(0,-0.45);
            titleButton->setColor(ColourValue(1,0,0));

            buttons.push_back(new MenuButton(_("Player name"),ALIGN_LEFT,true,false));

            buttons[0]->setPosition(-0.4,-0.35);
            buttons[0]->setState(BSTATE_ACTIVE);

            buttons[0]->setOptionCaption(ConfigManager::getSingleton().getString("player_name"));


            buttons.push_back(new MenuButton(_("Model"),ALIGN_LEFT,true,true));

            buttons[1]->setPosition(-0.4,-0.25);

            buttons[1]->setOptionCaption(playerModelSelector->getName());

            menuCursor = 0;

            mInfoTextArea->setCaption(_("Customize your player"));

            break;
        }

        case MENU_PAGE_GAME_MODE:
        {
            ringSwitcher = new RingSwitcher(2);


            //Los nombres de los modos deberian ser las variables que estan
            //en Globals.h (GAME_MODE_1...) pero si lo pongo asi
            //el Gettext no las traduce

            ringSwitcher->addObject(_("Survive"),"corazon.mesh");
            ringSwitcher->setBlocked(_("Survive"),false);
            ringSwitcher->addObject(_("Time Attack"),"reloj.mesh");
            ringSwitcher->setBlocked(_("Time Attack"),false);
            ringSwitcher->addObject(_("Infinite"),"infinito.mesh");
            ringSwitcher->setBlocked(_("Infinite"),false);


            ringSwitcher->setPosition(0,-1,0);

            buttons.push_back(new MenuButton(ringSwitcher->getCurrentName()));

            buttons[0]->setPosition(0,-0.45);
            buttons[0]->setState(BSTATE_ACTIVE);
            buttons[0]->setArrows(true);

            menuCursor = 0;

            mInfoTextArea->setCaption(_("Select Game Mode & Press ENTER"));

            break;



        }

        case MENU_PAGE_LEVELSELECT:
        {
            buttons.push_back(new MenuButton(LevelLoader::getSingleton().getLongName()));

            buttons[0]->setPosition(0,-0.45);
            buttons[0]->setState(BSTATE_ACTIVE);

            buttons[0]->setArrows(true);


            _updateLevelSelect();
            mLevelScreenshot->show();
            mLevelScreenshot_shadow->show();
            mLevelInfo->show();



            menuCursor = 0;

            mInfoTextArea->setCaption(_("Select Level & Press ENTER"));

            break;
        }

        case MENU_PAGE_HIGHSCORES:
        {

            titleButton = new MenuButton(_("HighScores"));
            titleButton->setPosition(0,-0.45);
            titleButton->setColor(ColourValue(1,0,0));

            buttons.push_back(new MenuButton(_("Mode"),ALIGN_LEFT,true,true));

            buttons[0]->setPosition(-0.4,-0.35);
            buttons[0]->setState(BSTATE_ACTIVE);

            buttons[0]->setOptionCaption(_("Survive"));


            buttons.push_back(new MenuButton(_("Level"),ALIGN_LEFT,true,true));

            buttons[1]->setPosition(-0.4,-0.25);

            LevelLoader::getSingleton().setLevelNum(0);

            buttons[1]->setOptionCaption(LevelLoader::getSingleton().getLongName());



            menuCursor = 0;

            mInfoTextArea->setCaption("");


            highScoreTable = new HighScoreTable();

            highScoreTable->updateTable("Survive", LevelLoader::getSingleton().getLevelName());

            break;
        }

        case MENU_PAGE_CONTROLS:
        {

            titleButton = new MenuButton(_("Controls"));
            titleButton->setPosition(0,-0.45);
            titleButton->setColor(ColourValue(1,0,0));

            buttons.push_back(new MenuButton(_("Not configurable")));

            buttons[0]->setPosition(0,-0.05);
            buttons[0]->setState(BSTATE_ACTIVE);

            buttons[0]->setArrows(false);

            mBigImage->setMaterialName("keyboard_layout");
            mBigImage->show();


            menuCursor = 0;



            mInfoTextArea->setCaption("");

            break;
        }


        case MENU_PAGE_QUIT:
        {
            titleButton = new MenuButton(_("Are you sure you want to quit?"));
            titleButton->setPosition(0,-0.45);

            titleButton->setColor(ColourValue(1,0,0));

            buttons.push_back(new MenuButton(_("No!")));

            buttons[0]->setPosition(0,0.0);
            buttons[0]->setState(BSTATE_ACTIVE);


            buttons.push_back(new MenuButton(_("Yes, quit!")));

            buttons[1]->setPosition(0,0.1);


            menuCursor = 0;

            mInfoTextArea->setCaption(_("C'mon! One more game can't hurt..."));

            break;

        }

    }

    _updateArrows(true);

}

void MenuState::_updateLevelSelect()
{

            buttons[0]->setCaption(LevelLoader::getSingleton().getLongName());
            buttons[0]->setPosition(0,-0.45);


            Ogre::ResourcePtr resptr = Ogre::MaterialManager::getSingleton().getByName("level_screenshot");
            Ogre::Material * mat = dynamic_cast<Ogre::Material*>(resptr.getPointer());

            mat->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setTextureName(
                LevelLoader::getSingleton().getLevelName() + ".png");



            /*Ogre::String caption = findAndReplace(
                    LevelLoader::getSingleton().getLevelInfo(),
                    "</br>",
                    "\n"
                    );
            */

            Ogre::String caption;

            caption += _("Number of boards");

            caption += ": ";

            caption += LevelLoader::getSingleton().getValue("num_boards");

            caption += "\n\n";

            caption +=_("Difficulty");

            caption += ": ";

            Ogre::String difficulty = LevelLoader::getSingleton().getValue("difficulty");

            if(difficulty == "easy") caption += _("Easy");
            if(difficulty == "normal") caption += _("Normal");
            if(difficulty == "hard") caption += _("Hard");

            caption += "\n\n";

            mLevelInfo->setCaption(caption);



}

void MenuState::_updateArrows(bool jump)
{

    if(buttons[menuCursor]->hasOption() && buttons[menuCursor]->hasArrows())
    {
        Vector2 pos = buttons[menuCursor]->getOptionScreenPosition();

        if(jump || !arrows->isVisible()) arrows->setPosition(pos.x,pos.y);
        else arrows->setDest(pos.x,pos.y);

        if(jump || !arrows->isVisible()) arrows->setSize(buttons[menuCursor]->getOptionWidth());
        else arrows->setDestSize(buttons[menuCursor]->getOptionWidth());

        //Ogre::LogManager::getSingleton().logMessage("ARROWS_SIZE = " + StringConverter::toString(buttons[menuCursor]->getOptionWidth()));

        arrows->show();
    }
    else if(buttons[menuCursor]->hasArrows())
    {


        Vector2 pos = buttons[menuCursor]->getScreenPosition();


        if(jump || !arrows->isVisible()) arrows->setPosition(pos.x,pos.y);
        else arrows->setDest(pos.x,pos.y);

         if(jump || !arrows->isVisible()) arrows->setSize(buttons[menuCursor]->getWidth());
         else arrows->setDestSize(buttons[menuCursor]->getWidth());

         arrows->show();
    }
    else
    {
       arrows->hide();
    }


}
