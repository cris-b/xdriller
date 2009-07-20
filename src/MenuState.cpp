#include "MenuState.h"
#include <OgreTextAreaOverlayElement.h>
#include <OgreFontManager.h>

#include "ConfigManager.h"
#include "Tools.h"
#include "LevelLoader.h"
#include "SoundManager.h"
#include "Arrows.h"

#define MENU_PAGE_MAIN                  0
#define MENU_PAGE_OPTIONS               1
#define MENU_PAGE_CREDITS               2
#define MENU_PAGE_QUIT                  3
#define MENU_PAGE_LEVELSELECT           4
#define MENU_PAGE_GRAPHIC_OPTIONS       5
#define MENU_PAGE_AUDIO_OPTIONS         6

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

    //mCamera->setPolygonMode(PM_WIREFRAME);

    mViewport->setBackgroundColour(ColourValue(1,1,1));

    mSceneMgr->setAmbientLight(ColourValue(1,1,1));



    mCamera->setPosition(0,0,10);
    mCamera->setNearClipDistance(0.1);
    mCamera->setFarClipDistance(100);

    Light *light = mSceneMgr->createLight("Menu_Light");
    light->setType(Light::LT_POINT);
    light->setPosition(Vector3(0, 0, 0));
    light->setDiffuseColour(1.0, 1.0, 1.0);
    light->setSpecularColour(1.0, 1.0, 1.0);

    mSceneMgr->setFog(FOG_LINEAR, ColourValue(1,1,1), 0.0, 20, 50);

    Rectangle2D* toprect = new Rectangle2D(true);
    toprect->setCorners(-1.0, 1.0, 1.0, -1.0);
    toprect->setMaterial("foreground");
    toprect->setRenderQueueGroup(RENDER_QUEUE_6);

    SceneNode* foregorund_node = mSceneMgr->getRootSceneNode()->createChildSceneNode("foreground");
    foregorund_node->attachObject(toprect);


    for(int i = 0; i < NUM_MENU_BRICKS; i++)
    {

        String n = "Menu_Brick_" + StringConverter::toString(i);
        Vector3 v = Vector3((rand()%400)/10.0 - 20 , (rand() % 400)/10.0 - 200 , -(rand() % 200)/10.0-10);
        //Vector3 v = Vector3(0,0,0);
        mBrickEnt[i] = mSceneMgr->createEntity(n, "cube.mesh");

        if(i<NUM_MENU_BRICKS*0.2) mBrickEnt[i]->setMaterialName("gris");
        else if(i<NUM_MENU_BRICKS*0.4) mBrickEnt[i]->setMaterialName("rojo");
        else if(i<NUM_MENU_BRICKS*0.6) mBrickEnt[i]->setMaterialName("verde");
        else if(i<NUM_MENU_BRICKS*0.8) mBrickEnt[i]->setMaterialName("azul");
        else if(i<=NUM_MENU_BRICKS) mBrickEnt[i]->setMaterialName("amarillo");


        mBrickNode[i] = mSceneMgr->getRootSceneNode()->createChildSceneNode( n , v);

        mBrickNode[i]->attachObject(mBrickEnt[i]);

        mBrickNode[i]->rotate(Quaternion(Degree(-90),Vector3::UNIT_X));

        mBrickSpeed[i] = Vector3(0,-(rand() % 20+10)*0.0001,0);

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
    mInfoTextArea->setColourBottom(ColourValue(1.0, 1, 1.0));
    mInfoTextArea->setColourTop(ColourValue(0, 0, 0));
    //mInfoTextArea->setColour(ColourValue(0,0,0));
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

    mLevelInfo = static_cast<TextAreaOverlayElement*>(
        mOverlayMgr->createOverlayElement("TextArea", "LevelInfoTextArea"));
    mLevelInfo->setMetricsMode(Ogre::GMM_RELATIVE);
    mLevelInfo->setPosition(0.05, 0.3);
    mLevelInfo->setDimensions(0.5, 0.5);
    mLevelInfo->setCaption("Test text 213 enj afj ajfdsajfoajs pofj asjfd iaJSF IJASPOFJIJADSFJPOIGJ POJPAJGPOIAG FA PIGJ");
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

    mPanel->addChild(mInfoTextArea);
    mPanel->addChild(mLevelInfo);

    mFadePanel = static_cast<PanelOverlayElement*>(
        mOverlayMgr->createOverlayElement("Panel", "MenuFadeOverlayPanel"));
    mFadePanel->setMetricsMode(Ogre::GMM_RELATIVE);
    mFadePanel->setPosition(0, 0);
    mFadePanel->setDimensions(1, 1);
    mFadePanel->setMaterialName("fade_material");

    mFadeOverlay = mOverlayMgr->create("MenuFadeOverlay");
    mFadeOverlay->add2D(mFadePanel);

    fade_alpha = 1.0;


    mOverlay->setZOrder(100);
    mFadeOverlay->setZOrder(101);

    // Show the overlay
    mOverlay->show();
    mFadeOverlay->show();

    arrows = new Arrows;

    arrows->hide();


    changePage(MENU_PAGE_MAIN);

    SoundManager::getSingleton().loadMusic("menu_music.ogg");
    SoundManager::getSingleton().playMusic(true);

}

void MenuState::exit( void )
{
    delete arrows;

    mOverlayMgr->destroyAllOverlayElements();

    mOverlayMgr->destroy(mOverlay);
    mOverlayMgr->destroy(mFadeOverlay);


    while (!buttons.empty())
    {
        delete buttons.back();  buttons.pop_back();
    }

    //delete mGUI;

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




    for(int i = 0; i < NUM_MENU_BRICKS; i++)
    {
            //mBrickNode[i]->roll(Radian(0.0002*lTimeElapsed));
            //mBrickNode[i]->pitch(Radian(0.0003*lTimeElapsed));
            //mBrickNode[i]->yaw(Radian(0.0001*lTimeElapsed));
            mBrickNode[i]->translate(mBrickSpeed[i]*lTimeElapsed);

            if(mBrickNode[i]->getPosition().y<-20)
            {
                mBrickNode[i]->setPosition((rand()%400)/10.0 - 20 , 20, -(rand() % 200)/10.0-10);
            }

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

        SoundManager::getSingleton().playSound(SOUND_MENU1);

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

        SoundManager::getSingleton().playSound(SOUND_MENU1);

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
                    if(buttons[0]->getOptionCaption() == "800 x 600") ConfigManager::getSingleton().setValue("resolution","640 x 480");
                    if(buttons[0]->getOptionCaption() == "640 x 480") ConfigManager::getSingleton().setValue("resolution","1024 x 768");

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
                    if(buttons[2]->getOptionCaption() == "Yes") ConfigManager::getSingleton().setValue("fullscreen","No");
                    if(buttons[2]->getOptionCaption() == "No") ConfigManager::getSingleton().setValue("fullscreen","Yes");

                    buttons[2]->setOptionCaption(ConfigManager::getSingleton().getString("fullscreen"));
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

                }

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
                    if(buttons[2]->getOptionCaption() == "Yes") ConfigManager::getSingleton().setValue("fullscreen","No");
                    if(buttons[2]->getOptionCaption() == "No") ConfigManager::getSingleton().setValue("fullscreen","Yes");

                    buttons[2]->setOptionCaption(ConfigManager::getSingleton().getString("fullscreen"));
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


                }

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
                    changePage(MENU_PAGE_LEVELSELECT);
                    SoundManager::getSingleton().playSound(SOUND_MENU2);

                }
                if(menuCursor == 1)
                {
                    changePage(MENU_PAGE_OPTIONS);
                    SoundManager::getSingleton().playSound(SOUND_MENU2);
                }
                if(menuCursor == 3)
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
                if(menuCursor == 3)
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
            case MENU_PAGE_LEVELSELECT:
            {

                if(menuCursor == 0)
                {
                    //LevelLoader::getSingleton().setLevelName("test");
                    this->changeState( PlayState::getSingletonPtr() );
                    SoundManager::getSingleton().playSound(SOUND_MENU2);
                    return;
                }

                break;
            }




        }
        _updateArrows();
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
            case MENU_PAGE_LEVELSELECT:
            {
                changePage(MENU_PAGE_MAIN);
                SoundManager::getSingleton().playSound(SOUND_MENU4);
                break;
            }
            case MENU_PAGE_OPTIONS:
            {
                changePage(MENU_PAGE_MAIN);
                SoundManager::getSingleton().playSound(SOUND_MENU4);
                break;
            }
            case MENU_PAGE_GRAPHIC_OPTIONS:
            {

                changePage(MENU_PAGE_OPTIONS);
                SoundManager::getSingleton().playSound(SOUND_MENU4);
                break;
            }
            case MENU_PAGE_AUDIO_OPTIONS:
            {
                changePage(MENU_PAGE_OPTIONS);
                SoundManager::getSingleton().playSound(SOUND_MENU4);
                break;
            }
        }
        _updateArrows();
    }

}

void MenuState::keyReleased( const OIS::KeyEvent &e ) {

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

    while (!buttons.empty())
    {
        delete buttons.back();  buttons.pop_back();
    }
    if(titleButton != NULL)
    {
        delete titleButton;
        titleButton = NULL;
    }

    mOverlay->remove2D(mLogoXDriller);

    switch(menuPage)
    {
        case MENU_PAGE_MAIN:
        {
            mOverlay->add2D(mLogoXDriller);

            buttons.push_back(new MenuButton("Play Game"));

            buttons[0]->setPosition(0,1,0);
            buttons[0]->setState(BSTATE_ACTIVE);
            buttons[0]->setDest(Vector3(0,1,0));

            buttons.push_back(new MenuButton("Options"));

            buttons[1]->setPosition(0,0,0);
            buttons[1]->setDest(Vector3(0,0,0));

            buttons.push_back(new MenuButton("Credits"));

            buttons[2]->setPosition(0,-1,0);
            buttons[2]->setDest(Vector3(0,-1,0));

            buttons.push_back(new MenuButton("Exit"));

            buttons[3]->setPosition(0,-2,0);
            buttons[3]->setDest(Vector3(0,-2,0));

            menuCursor = 0;

            mInfoTextArea->setCaption("Fixi Studios makes cool games for you");

            break;
        }

        case MENU_PAGE_AUDIO_OPTIONS:
        {
            //mOverlay->add2D(mLogoXDriller);
            titleButton = new MenuButton("Audio Options");
            titleButton->setPosition(0,3.5,0);
            titleButton->setDest(Vector3(0,3.5,0));
            titleButton->setColor(ColourValue(1,0,0));

            buttons.push_back(new MenuButton("Music Volume",ALIGN_LEFT,true));

            buttons[0]->setPosition(-4,1,0);
            buttons[0]->setState(BSTATE_ACTIVE);
            buttons[0]->setDest(Vector3(-4,1,0));
            buttons[0]->setOptionCaption(ConfigManager::getSingleton().getString("music_volume"));

            buttons.push_back(new MenuButton("FX Volume",ALIGN_LEFT,true));

            buttons[1]->setPosition(-4,0,0);
            buttons[1]->setDest(Vector3(-4,0,0));
            buttons[1]->setOptionCaption(ConfigManager::getSingleton().getString("sound_volume"));


            buttons.push_back(new MenuButton("Back",ALIGN_LEFT));

            buttons[2]->setPosition(-4,-1,0);
            buttons[2]->setDest(Vector3(-4,-1,0));

            menuCursor = 0;

            mInfoTextArea->setCaption("We are electric...");

            break;
        }

        case MENU_PAGE_GRAPHIC_OPTIONS:
        {
            //mOverlay->add2D(mLogoXDriller);
            titleButton = new MenuButton("Graphic Options");
            titleButton->setPosition(0,3.5,0);
            titleButton->setDest(Vector3(0,3.5,0));
            titleButton->setColor(ColourValue(1,0,0));

            buttons.push_back(new MenuButton("Resolution",ALIGN_LEFT,true));

            buttons[0]->setPosition(-4,1,0);
            buttons[0]->setState(BSTATE_ACTIVE);
            buttons[0]->setDest(Vector3(-4,1,0));
            buttons[0]->setOptionCaption(ConfigManager::getSingleton().getString("resolution"));

            buttons.push_back(new MenuButton("Anti-aliasing",ALIGN_LEFT,true));

            buttons[1]->setPosition(-4,0,0);
            buttons[1]->setDest(Vector3(-4,0,0));
            buttons[1]->setOptionCaption(ConfigManager::getSingleton().getString("FSAA"));

            buttons.push_back(new MenuButton("Fullscreen",ALIGN_LEFT,true));

            buttons[2]->setPosition(-4,-1,0);
            buttons[2]->setDest(Vector3(-4,-1,0));
            buttons[2]->setOptionCaption(ConfigManager::getSingleton().getString("fullscreen"));

            buttons.push_back(new MenuButton("Back",ALIGN_LEFT));

            buttons[3]->setPosition(-4,-2,0);
            buttons[3]->setDest(Vector3(-4,-2,0));

            menuCursor = 0;

            mInfoTextArea->setCaption("ATENTION: restart the game for new settings to apply");



            break;
        }
        case MENU_PAGE_OPTIONS:
        {
            //mOverlay->add2D(mLogoXDriller);

            titleButton = new MenuButton("Options");
            titleButton->setPosition(0,3.5,0);
            titleButton->setDest(Vector3(0,3.5,0));
            titleButton->setColor(ColourValue(1,0,0));

            buttons.push_back(new MenuButton("Graphic Options"));

            buttons[0]->setPosition(0,1,0);
            buttons[0]->setState(BSTATE_ACTIVE);
            buttons[0]->setDest(Vector3(0,1,0));


            buttons.push_back(new MenuButton("Audio Options"));

            buttons[1]->setPosition(0,0,0);
            buttons[1]->setDest(Vector3(0,0,0));


            buttons.push_back(new MenuButton("Controls"));

            buttons[2]->setPosition(0,-1,0);
            buttons[2]->setDest(Vector3(0,-1,0));


            buttons.push_back(new MenuButton("Back"));

            buttons[3]->setPosition(0,-2,0);
            buttons[3]->setDest(Vector3(0,-2,0));

            menuCursor = 0;

            mInfoTextArea->setCaption("It's good to have options");

            break;
        }

        case MENU_PAGE_LEVELSELECT:
        {
            buttons.push_back(new MenuButton(LevelLoader::getSingleton().getLongName(LevelLoader::getSingleton().getLevelName())));

            buttons[0]->setPosition(0,4.5,0);
            buttons[0]->setState(BSTATE_ACTIVE);
            buttons[0]->setDest(Vector3(0,3.5,0));
            buttons[0]->setArrows(true);


            mLevelScreenshot->show();
            mLevelScreenshot_shadow->show();
            mLevelInfo->show();

            _updateLevelSelect();

            menuCursor = 0;

            mInfoTextArea->setCaption("Select Level & Press ENTER");

            break;
        }

        case MENU_PAGE_QUIT:
        {
            titleButton = new MenuButton("Are you sure you want to quit?");
            titleButton->setPosition(0,3.5,0);
            titleButton->setDest(Vector3(0,3.5,0));
            titleButton->setColor(ColourValue(1,0,0));

            buttons.push_back(new MenuButton("No!"));

            buttons[0]->setPosition(0,0.5,0);
            buttons[0]->setState(BSTATE_ACTIVE);
            buttons[0]->setDest(Vector3(0,0.5,0));

            buttons.push_back(new MenuButton("Yes, quit!"));

            buttons[1]->setPosition(0,-0.5,0);
            buttons[1]->setDest(Vector3(0,-0.5,0));

            menuCursor = 0;

            mInfoTextArea->setCaption("C'mon! One more game can't hurt...");

            break;

        }

    }

    _updateArrows(true);

}

void MenuState::_updateLevelSelect()
{

            buttons[0]->setCaption(LevelLoader::getSingleton().getLongName(LevelLoader::getSingleton().getLevelName()));
            buttons[0]->setPosition(0,4.5,0);
            buttons[0]->setDest(Vector3(0,3.5,0));

            Ogre::ResourcePtr resptr = Ogre::MaterialManager::getSingleton().getByName("level_screenshot");
            Ogre::Material * mat = dynamic_cast<Ogre::Material*>(resptr.getPointer());

            mat->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setTextureName(
                LevelLoader::getSingleton().getLevelName() + ".jpg");



            Ogre::String caption = findAndReplace(
                    LevelLoader::getSingleton().getLevelInfo(LevelLoader::getSingleton().getLevelName()),
                    "</br>",
                    "\n"
                    );

            mLevelInfo->setCaption(caption);



}

void MenuState::_updateArrows(bool jump)
{
    if(buttons[menuCursor]->hasOption())
    {

        Vector2 pos = buttons[menuCursor]->getOptionScreenPosition();

        /*Ogre::LogManager::getSingleton().logMessage("ARROWS_POS X = " + StringConverter::toString(pos.x)
                                                    + " Y = " + StringConverter::toString(pos.y));*/

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
