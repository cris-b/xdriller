#include "Arrows.h"


using namespace Ogre;



Arrows::Arrows()
{

    x = 0;
    y = 0;
    s=0.5;
    s2=0.5;
    h = 0.05;
    w = 0.0375;
    min_alpha = 0.2;
    ar = 1;
    al = 1;

    mOverlayMgr   = OverlayManager::getSingletonPtr();
    mOverlay = mOverlayMgr->create("ArrowsOverlay");

    arrowLeft = static_cast<PanelOverlayElement*>(
        mOverlayMgr->createOverlayElement("Panel", "arrowLeftPanel"));
    arrowLeft->setMetricsMode(Ogre::GMM_RELATIVE);
    arrowLeft->setPosition(0, 0);
    arrowLeft->setDimensions(w, h);
    arrowLeft->setHorizontalAlignment(Ogre::GHA_CENTER);
    arrowLeft->setVerticalAlignment(Ogre::GVA_CENTER);
    arrowLeft->setMaterialName("arrow_left");

    arrowRight = static_cast<PanelOverlayElement*>(
        mOverlayMgr->createOverlayElement("Panel", "arrowRightPanel"));
    arrowRight->setMetricsMode(Ogre::GMM_RELATIVE);
    arrowRight->setPosition(0, 0);
    arrowRight->setDimensions(w, h);
    arrowRight->setHorizontalAlignment(Ogre::GHA_CENTER);
    arrowRight->setVerticalAlignment(Ogre::GVA_CENTER);
    arrowRight->setMaterialName("arrow_right");

    mOverlay->add2D(arrowLeft);
    mOverlay->add2D(arrowRight);



    update(1000);

}

Arrows::~Arrows()
{

    mOverlay->remove2D(arrowLeft);
    mOverlay->remove2D(arrowRight);
    mOverlayMgr->destroy(mOverlay);

}

void Arrows::setPosition(float x, float y)
{
    this->x = x;
    this->y = y;
}

void Arrows::setSize(float s)
{
    this->s2 = s;
}

void Arrows::update(unsigned long lTimeElapsed)
{
    arrowLeft->setPosition(x-(w/2.0)-s,y-(h/2.0));
    arrowRight->setPosition(x-(w/2.0)+s,y-(h/2.0));

    s += (s2-s)*(lTimeElapsed/100.0);

    {
        if( al > min_alpha )
        {
            al -= lTimeElapsed/1000.0;
            if(al<min_alpha) al = min_alpha;
            Ogre::ResourcePtr resptr = Ogre::MaterialManager::getSingleton().getByName("arrow_left");
            Ogre::Material * mat = dynamic_cast<Ogre::Material*>(resptr.getPointer());

            mat->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setAlphaOperation(LBX_MODULATE, LBS_MANUAL, LBS_TEXTURE, al);
        }
    }
    {
        if( ar > min_alpha )
        {
            ar -= lTimeElapsed/1000.0;
            if(ar<min_alpha) ar = min_alpha;
            Ogre::ResourcePtr resptr = Ogre::MaterialManager::getSingleton().getByName("arrow_right");
            Ogre::Material * mat = dynamic_cast<Ogre::Material*>(resptr.getPointer());

            mat->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setAlphaOperation(LBX_MODULATE, LBS_MANUAL, LBS_TEXTURE, ar);
        }
    }
}

void Arrows::show()
{
    mOverlay->show();
}

void Arrows::hide()
{
    mOverlay->hide();
}

void Arrows::left()
{
    al = 1.0;
}

void Arrows::right()
{
    ar = 1.0;
}
