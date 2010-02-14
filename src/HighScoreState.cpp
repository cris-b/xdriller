#include "HighScoreState.h"

#ifndef MenuState_H
#include "MenuState.h"
#endif

#include "Gettext.h"
#include "ConfigManager.h"
#include "HighScoreManager.h"


using namespace Ogre;

HighScoreState* HighScoreState::mHighScoreState;

HighScoreState::HighScoreState()
{

    mPanel = NULL;
    mOverlay = NULL;
    highScoreTable = NULL;
    mEditableText = NULL;
    mTextAreaTitle = NULL;
    mTextAreaName = NULL;
}

void HighScoreState::enter( void ) {
    mRoot         = Root::getSingletonPtr();
    mOverlayMgr   = OverlayManager::getSingletonPtr();
    mSceneMgr     = mRoot->getSceneManager( "ST_GENERIC" );
    mCamera       = mSceneMgr->createCamera( "HighScoreCamera" );
    mViewport     = mRoot->getAutoCreatedWindow()->addViewport( mCamera );

    mViewport->setBackgroundColour(ColourValue(0.5,0.5,0.5));

    mSceneMgr->setAmbientLight(ColourValue(0.5,0.5,0.5));

    //mSceneMgr->setFog(FOG_LINEAR, ColourValue(1,1,1), 0.0, 30, 100);

    mCamera->setNearClipDistance(1);
    mCamera->setFarClipDistance(1000);

    Light *light = mSceneMgr->createLight("HighScoreLight");
    light->setType(Light::LT_DIRECTIONAL);
    light->setDirection(Vector3(0.5, -1.0, -1));
    light->setDiffuseColour(0.6, 0.6, 0.6);
    light->setSpecularColour(0.8, 0.8, 0.8);





    mPanel = static_cast<PanelOverlayElement*>(
        mOverlayMgr->createOverlayElement("Panel", "HighScoreStateOverlayPanel"));
    mPanel->setMetricsMode(Ogre::GMM_RELATIVE);
    mPanel->setPosition(0, 0);
    mPanel->setDimensions(1, 1);

    mTextAreaTitle = static_cast<TextAreaOverlayElement*>(
        mOverlayMgr->createOverlayElement("TextArea", "TitleTextArea"));
    mTextAreaTitle->setMetricsMode(Ogre::GMM_RELATIVE);
    mTextAreaTitle->setPosition(0.5, 0.05);
    mTextAreaTitle->setDimensions(0.5, 0.5);
    mTextAreaTitle->setCaption(_("Enter your name"));
    mTextAreaTitle->setCharHeight(0.07);
    mTextAreaTitle->setFontName("CoolFont");
    mTextAreaTitle->setColour(ColourValue(1,1,1));
    mTextAreaTitle->setAlignment(TextAreaOverlayElement::Center);

    mTextAreaName = static_cast<TextAreaOverlayElement*>(
        mOverlayMgr->createOverlayElement("TextArea", "NameTextArea"));
    mTextAreaName->setMetricsMode(Ogre::GMM_RELATIVE);
    mTextAreaName->setPosition(0.5, 0.15);
    mTextAreaName->setDimensions(0.5, 0.5);
    mTextAreaName->setCaption("_");
    mTextAreaName->setCharHeight(0.14);
    mTextAreaName->setFontName("CoolFont");
    mTextAreaName->setColourBottom(ColourValue(0.7, 0.7, 0.7));
    mTextAreaName->setColourTop(ColourValue(1,1,1));
    //mTextAreaName->setColour(ColourValue(1,1,1));
    mTextAreaName->setAlignment(TextAreaOverlayElement::Center);

    mOverlay = mOverlayMgr->create("HighScoreStateOverlay");


    mOverlay->add2D(mPanel);
    mPanel->addChild(mTextAreaTitle);
    mPanel->addChild(mTextAreaName);

    mOverlay->setZOrder(100);
    mOverlay->show();

    mEditableText = new EditableText;

    highScoreTable = new HighScoreTable;



}

void HighScoreState::exit( void )
{
    mSceneMgr->destroyAllCameras();
    mSceneMgr->clearScene();
    mRoot->getAutoCreatedWindow()->removeAllViewports();

    if(mPanel != NULL) { mOverlayMgr->destroyOverlayElement(mPanel); mPanel = NULL;}
    if(mTextAreaTitle != NULL) { mOverlayMgr->destroyOverlayElement(mTextAreaTitle); mTextAreaTitle = NULL;}
    if(mTextAreaName != NULL) { mOverlayMgr->destroyOverlayElement(mTextAreaName); mTextAreaName = NULL;}

    if(mOverlay != NULL)
    {
        mOverlayMgr->destroy(mOverlay);
        mOverlay = NULL;
    }

    if(highScoreTable != NULL)
    {
        delete highScoreTable;
        highScoreTable = NULL;
    }
    if(mEditableText != NULL)
    {
        delete mEditableText;
        mEditableText = NULL;
    }
}

void HighScoreState::pause( void )
{

}

void HighScoreState::resume( void )
{

}

void HighScoreState::update( unsigned long lTimeElapsed )
{

}

void HighScoreState::keyPressed( const OIS::KeyEvent &e )
{

    if( e.key == OIS::KC_RETURN)
    {
        ConfigManager::getSingleton().setValue("player_name",mTextAreaName->getCaption());
        HighScoreManager::getSingleton().addScore(
            HighScoreManager::getSingleton().lastScore.mode,
            HighScoreManager::getSingleton().lastScore.level,
            mEditableText->getText(),
            HighScoreManager::getSingleton().lastScore.score.time,
            HighScoreManager::getSingleton().lastScore.score.lives,
            HighScoreManager::getSingleton().lastScore.score.depth);

        highScoreTable->updateTable(
            HighScoreManager::getSingleton().lastScore.mode,
            HighScoreManager::getSingleton().lastScore.level);

    }
    else if(mEditableText->injectKeyPress(e))
    {
        if(mEditableText->getText().size() < 10) mTextAreaName->setCaption(mEditableText->getText() + "_");
        else mTextAreaName->setCaption(mEditableText->getText());
    }
}

void HighScoreState::keyReleased( const OIS::KeyEvent &e )
{

    if( e.key == OIS::KC_ESCAPE)
    {
        fadeState(MenuState::getSingletonPtr());
    }

}

void HighScoreState::mouseMoved( const OIS::MouseEvent &e )
{

}

void HighScoreState::mousePressed( const OIS::MouseEvent &e, OIS::MouseButtonID id )
{
}

void HighScoreState::mouseReleased( const OIS::MouseEvent &e, OIS::MouseButtonID id )
{

}

HighScoreState* HighScoreState::getSingletonPtr( void )
{
    if( !mHighScoreState ) {
        mHighScoreState = new HighScoreState();
    }

    return mHighScoreState;
}
