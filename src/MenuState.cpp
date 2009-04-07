#include "MenuState.h"


#include <OgreTextAreaOverlayElement.h>
#include <OgreFontManager.h>

#include "Tools.h"



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

    mViewport->setBackgroundColour(ColourValue(0.8,0.8,0.8));

    mSceneMgr->setAmbientLight(ColourValue(0.8,0.8,0.8));

    mCamera->setPosition(0,0,10);
    mCamera->setNearClipDistance(0.1);
    mCamera->setFarClipDistance(100);

    Light *light = mSceneMgr->createLight("Menu_Light");
    light->setType(Light::LT_POINT);
    light->setPosition(Vector3(10, 0, 5));
    light->setDiffuseColour(1.0, 1.0, 1.0);
    light->setSpecularColour(1.0, 1.0, 1.0);

    mSceneMgr->setFog(FOG_LINEAR, ColourValue(0.8,0.8,0.8), 0.0, 10, 30);


    /*MaterialPtr material = MaterialManager::getSingleton().create("Background", "General");
    material->getTechnique(0)->getPass(0)->createTextureUnitState("fondomenu.jpg");
    material->getTechnique(0)->getPass(0)->setDepthCheckEnabled(false);
    material->getTechnique(0)->getPass(0)->setDepthWriteEnabled(false);
    material->getTechnique(0)->getPass(0)->setLightingEnabled(false);

    Rectangle2D* rect = new Rectangle2D(true);
    rect->setCorners(-1.0, 1.0, 1.0, -1.0);
    rect->setMaterial("Background");

    rect->setRenderQueueGroup(RENDER_QUEUE_BACKGROUND);


    rect->setBoundingBox(AxisAlignedBox(-100000.0*Vector3::UNIT_SCALE, 100000.0*Vector3::UNIT_SCALE));

    SceneNode* node = mSceneMgr->getRootSceneNode()->createChildSceneNode("Background");
    node->attachObject(rect);
*/
    Rectangle2D* toprect = new Rectangle2D(true);
    toprect->setCorners(-1.0, 1.0, 1.0, -1.0);
    toprect->setMaterial("foreground");
    toprect->setRenderQueueGroup(RENDER_QUEUE_6);

    SceneNode* foregorund_node = mSceneMgr->getRootSceneNode()->createChildSceneNode("foreground");
    foregorund_node->attachObject(toprect);


    for(int i = 0; i < NUM_MENU_BRICKS; i++)
    {

        String n = "Menu_Brick_" + StringConverter::toString(i);
        Vector3 v = Vector3((rand()%400)/10.0 - 20 , (rand() % 400)/10.0 - 20 , -(rand() % 200)/10.0-10);
        //Vector3 v = Vector3(0,0,0);
        mBrickEnt[i] = mSceneMgr->createEntity(n, "cube.mesh");

        if(i>3) mBrickEnt[i]->setMaterialName("gris");
        else if(i==0) mBrickEnt[i]->setMaterialName("rojo");
        else if(i==1) mBrickEnt[i]->setMaterialName("verde");
        else if(i==2) mBrickEnt[i]->setMaterialName("azul");
        else if(i==3) mBrickEnt[i]->setMaterialName("amarillo");


        mBrickNode[i] = mSceneMgr->getRootSceneNode()->createChildSceneNode( n , v);

        mBrickNode[i]->attachObject(mBrickEnt[i]);

        mBrickNode[i]->rotate(Quaternion(Degree(-90),Vector3::UNIT_X));

        mBrickSpeed[i] = Vector3(0,-(rand() % 20+10)*0.0001,0);

    }





    //mLogoXDriller->setScroll(0,1.5);

    mLogoXDriller = static_cast<OverlayContainer*>(
        mOverlayMgr->createOverlayElement("Panel", "PlayStateOverlayPanel"));
    mLogoXDriller->setMetricsMode(Ogre::GMM_RELATIVE);
    mLogoXDriller->setHorizontalAlignment(Ogre::GHA_CENTER);
    mLogoXDriller->setVerticalAlignment(Ogre::GVA_CENTER);
    mLogoXDriller->setPosition(-0.24375, -0.48);
    mLogoXDriller->setDimensions(0.4875, 0.254166667);
    mLogoXDriller->setMaterialName("LogoXDriller");

    //mLogoXDriller->show();

    mOverlay = mOverlayMgr->create("MenuOverlay");
    mOverlay->add2D(mLogoXDriller);

    mOverlay->show();

    changePage(MENU_PAGE_MAIN);


    //mGUI = new BetaGUI::GUI("MenuFont",32);

    //BetaGUI::Window* window = mGUI->createWindow(Vector4(100,100,300,100), "button", BetaGUI::NONE, "Magical Doubler");
    //window->createStaticText(Vector4(4,22,198,40), "Type in a number and I'll double it!");
    //BetaGUI::Button* mDoubleIt = window->createButton(Vector4(108,50,104,24), "button", "Go on then!", BetaGUI::Callback(myCallback));
    //BetaGUI::TextInput* mAmount = window->createTextInput(Vector4(4,50,104,24), "button", "1", 3);


}

void MenuState::exit( void )
{

    mOverlayMgr->destroyAllOverlayElements();

    mOverlayMgr->destroy(mOverlay);


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

void MenuState::update( unsigned long lTimeElapsed ) {

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

    if(menuPage == MENU_PAGE_MAIN);
    {
        /*Real scrollY;


        scrollY = mLogoXDriller->getScrollY();

        scrollY = scrollY - lTimeElapsed*0.001;

        if(scrollY < 0.7)
        {
            scrollY = 0.7;
        }


        mLogoXDriller->setScroll(0,scrollY);*/


    }

    if(!buttons.empty())
    {
       for(unsigned int i=0; i < buttons.size(); i++)
       {
          buttons[i]->update(lTimeElapsed);
       }

    }


}

void MenuState::keyPressed( const OIS::KeyEvent &e )
{

    if(e.key == OIS::KC_R)
    {


        //TexturePtr Texture = TextureManager::getSingleton().getByName("MenuFontTexture");
        //RenderTexture* pRenderTexture = Texture->getBuffer()->getRenderTarget();
        //pRenderTexture->update();
        //SaveImage(Texture,"font.png");


    }
    if(e.key == OIS::KC_S)
    {


        TexturePtr tp = static_cast<Ogre::TexturePtr> (TextureManager::getSingleton().getByName("MenuFontTexture"));
        const size_t buffSize = (tp->getWidth() * tp->getHeight() * 4);
        unsigned char *data = OGRE_ALLOC_T(unsigned char,buffSize,Ogre::MEMCATEGORY_GENERAL);
        memset(data, 0, buffSize);
        Ogre::Image i;
        i.loadDynamicImage(data, tp->getWidth(), tp->getHeight(), 1, Ogre::PF_R8G8B8A8, true);

        Ogre::HardwarePixelBufferSharedPtr buf = tp->getBuffer();
        const Ogre::PixelBox destBox = i.getPixelBox();
        buf->blitToMemory(destBox);

        // Save to disk!
        i.save("waka.png");


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
    }
    if( e.key == OIS::KC_RETURN)
    {
        switch(menuPage)
        {
            case MENU_PAGE_MAIN:
            {
                if(menuCursor == 0) this->changeState( PlayState::getSingletonPtr() );
                if(menuCursor == 3) changePage(MENU_PAGE_QUIT);

                break;
            }
            case MENU_PAGE_QUIT:
            {
                if(menuCursor == 0) changePage(MENU_PAGE_MAIN);
                if(menuCursor == 1) this->requestShutdown();

                break;
            }




        }

    }

}

void MenuState::keyReleased( const OIS::KeyEvent &e ) {

}

void MenuState::mouseMoved( const OIS::MouseEvent &e )
{
    //btn->setPosition(Vector3((e.state.X.abs / 640.0)+0.5,(e.state.Y.abs / 480.0)+0.5,0));
}

void MenuState::mousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id ) {
}

void MenuState::mouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id ) {
    this->changeState( PlayState::getSingletonPtr() );
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

    while (!buttons.empty())
    {
        delete buttons.back();  buttons.pop_back();
    }

    switch(menuPage)
    {
        case MENU_PAGE_MAIN:
        {
            buttons.push_back(new MenuButton("Start random level"));

            buttons[0]->setPosition(-10,1,0);
            buttons[0]->setState(BSTATE_ACTIVE);
            buttons[0]->setDest(Vector3(0,1,0));

            buttons.push_back(new MenuButton("Options"));

            buttons[1]->setPosition(-15,0,0);
            buttons[1]->setDest(Vector3(0,0,0));

            buttons.push_back(new MenuButton("Credits"));

            buttons[2]->setPosition(-20,-1,0);
            buttons[2]->setDest(Vector3(0,-1,0));

            buttons.push_back(new MenuButton("Exit"));

            buttons[3]->setPosition(-25,-2,0);
            buttons[3]->setDest(Vector3(0,-2,0));

            menuCursor = 0;

            break;
        }

        case MENU_PAGE_QUIT:
        {
            buttons.push_back(new MenuButton("Go Back!"));

            buttons[0]->setPosition(-10,0,0);
            buttons[0]->setState(BSTATE_ACTIVE);
            buttons[0]->setDest(Vector3(0,0,0));

            buttons.push_back(new MenuButton("Quit"));

            buttons[1]->setPosition(-15,-1,0);
            buttons[1]->setDest(Vector3(0,-1,0));

            menuCursor = 0;

            break;

        }

    }

}

