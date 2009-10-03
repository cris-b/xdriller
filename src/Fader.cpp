#include "Fader.h"

using namespace Ogre;

template<> Fader* Ogre::Singleton<Fader>::ms_Singleton = 0;
Fader* Fader::getSingletonPtr(void)
{
    return ms_Singleton;
}
Fader& Fader::getSingleton(void)
{
    assert( ms_Singleton );  return ( *ms_Singleton );
}


Fader::Fader()
{
    mOverlayMgr   = OverlayManager::getSingletonPtr();

    mFadePanel = static_cast<PanelOverlayElement*>(
        mOverlayMgr->createOverlayElement("Panel", "MenuFadeOverlayPanel"));
    mFadePanel->setMetricsMode(Ogre::GMM_RELATIVE);
    mFadePanel->setPosition(0, 0);
    mFadePanel->setDimensions(1, 1);
    mFadePanel->setMaterialName("fade_material");

    mFadeOverlay = mOverlayMgr->create("MenuFadeOverlay");
    mFadeOverlay->add2D(mFadePanel);

    mFadeOverlay->setZOrder(300);

    speed = 0.5;
    r = 1;
    g = 1;
    b = 1;
    alpha = 0;

    setColor(r,g,b);

    fadeState = FADE_NONE;

    fadeTo = NULL;
}

void Fader::enable()
{

}
void Fader::disable()
{

}

void Fader::fadeIn()
{
    if(alpha > 0) fadeState = FADE_IN;
}
void Fader::fadeOut()
{
    if(alpha < 1) fadeState = FADE_OUT;
}

void Fader::setColor(float r, float g, float b)
{
    this->r=r;
    this->g=g;
    this->b=b;

    Ogre::ResourcePtr resptr = Ogre::MaterialManager::getSingleton().getByName("fade_material");
    Ogre::Material * mat = dynamic_cast<Ogre::Material*>(resptr.getPointer());

    mat->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setColourOperationEx(LBX_SOURCE1,LBS_MANUAL,LBS_TEXTURE, ColourValue(r,g,b,1.0));
    mat->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setAlphaOperation(LBX_MODULATE, LBS_MANUAL, LBS_TEXTURE, alpha);

}

void Fader::update(unsigned long lTimeElapsed)
{

    switch(fadeState)
    {
        case FADE_IN:
        {
            alpha -= (float) lTimeElapsed / 1000.0;

            if(alpha < 0)
            {
                alpha = 0;
                fadeState = FADE_NONE;
            }

            break;
        }
        case FADE_OUT:
        {
            alpha += (float) lTimeElapsed / 1000.0;

            if(alpha > 1)
            {
                alpha = 1;
                fadeState = FADE_FULL;
            }

            break;

        }

    }

    if(fadeTo != NULL && getFadeState() != FADE_OUT)
        fadeOut();

    if(fadeTo != NULL && getFadeState() == FADE_FULL)
    {
        GameState *_fadeTo = fadeTo;
        fadeTo = NULL;
        fadeIn();
        GameManager::getSingletonPtr()->changeState( _fadeTo );
    }


    Ogre::ResourcePtr resptr = Ogre::MaterialManager::getSingleton().getByName("fade_material");
    Ogre::Material * mat = dynamic_cast<Ogre::Material*>(resptr.getPointer());

    mat->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setAlphaOperation(LBX_MODULATE, LBS_MANUAL, LBS_TEXTURE, alpha);

    if(alpha <= 0 && mFadeOverlay->isVisible())
    {
        mFadeOverlay->hide();
    }
    else if(!mFadeOverlay->isVisible())
    {
        mFadeOverlay->show();
    }
}
