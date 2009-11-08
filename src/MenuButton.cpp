#include "MenuButton.h"
#include "Tools.h"


int menuButtonCount = 0;

using namespace Ogre;

#define CHAR_HEIGHT 0.07

MenuButton::MenuButton(UTFString caption, int align, bool hasOption, bool hasArrows)
{
    this->_hasOption = hasOption;
    this->_hasArrows = hasArrows;
    this->align = align;
    this->caption = caption;

    name = "MenuButton_" + StringConverter::toString(menuButtonCount);
    menuButtonCount++;

    mOverlayMgr   = OverlayManager::getSingletonPtr();

    if(mOverlayMgr->getByName("MenuButtonsOverlay") == 0)
    {
        mOverlay = mOverlayMgr->create("MenuButtonsOverlay");
    }
    else
    {
        mOverlay = mOverlayMgr->getByName("MenuButtonsOverlay");
    }

    mPanel = static_cast<PanelOverlayElement*>(
        mOverlayMgr->createOverlayElement("Panel", name + "_panel"));
    mPanel->setMetricsMode(Ogre::GMM_RELATIVE);

    mPanel->setVerticalAlignment(Ogre::GVA_CENTER);
    mPanel->setPosition(0, 0);
    mPanel->setDimensions(1, 0.2);
    mPanel->setTransparent(true);

    mText = static_cast<TextAreaOverlayElement*>(
        mOverlayMgr->createOverlayElement("TextArea", name + "_text"));
    mText->setMetricsMode(Ogre::GMM_RELATIVE);
    mText->setHorizontalAlignment(Ogre::GHA_CENTER);
    mText->setVerticalAlignment(Ogre::GVA_CENTER);
    mText->setPosition(0, 0);
    mText->setDimensions(1, 0.2);
    mText->setCharHeight(CHAR_HEIGHT);
    mText->setFontName("CoolFont");
    mText->setColour(ColourValue(1,1,1));
    if(align == ALIGN_CENTER) mText->setAlignment(TextAreaOverlayElement::Center);
    else if(align == ALIGN_LEFT) mText->setAlignment(TextAreaOverlayElement::Left);
    else if(align == ALIGN_RIGHT) mText->setAlignment(TextAreaOverlayElement::Right);

    setCaption(caption);

    mPanel->addChild(mText);

    if(_hasOption)
    {
        mOptionText = static_cast<TextAreaOverlayElement*>(
            mOverlayMgr->createOverlayElement("TextArea", name + "_optionText"));
        mOptionText->setMetricsMode(Ogre::GMM_RELATIVE);
        mOptionText->setHorizontalAlignment(Ogre::GHA_CENTER);
        mOptionText->setVerticalAlignment(Ogre::GVA_CENTER);
        mOptionText->setPosition(0.8, 0);
        mOptionText->setDimensions(0.5, 0.2);
        mOptionText->setCharHeight(CHAR_HEIGHT);
        mOptionText->setFontName("CoolFont");
        mOptionText->setColour(ColourValue(1,0,0));
        mOptionText->setAlignment(TextAreaOverlayElement::Right);

        mPanel->addChild(mOptionText);
    }


    mOverlay->add2D(mPanel);

    mOverlay->show();

    state = BSTATE_INACTIVE;
    dest_reached = false;

    speed = Vector2(0,0);

}

float MenuButton::getWidth()
{

    return textWidth;
}

float MenuButton::getOptionWidth()
{
    return optionTextWidth;

}

Ogre::Vector2 MenuButton::getScreenPosition()
{
    return Vector2(pos.x,pos.y+CHAR_HEIGHT/2.0);
}

Ogre::Vector2 MenuButton::getOptionScreenPosition()
{

    return Vector2(0.4-optionTextWidth/2.0,pos.y+CHAR_HEIGHT/2.0);
}

void MenuButton::setCaption(String caption)
{
    this->caption = caption;
    mText->setCaption(caption);

    textWidth = getStringWidth(caption,"CoolFont",CHAR_HEIGHT);
}


void MenuButton::setOptionCaption(String caption)
{
    this->optionCaption = caption;
    mOptionText->setCaption(caption);

    optionTextWidth = getStringWidth(optionCaption,"CoolFont",CHAR_HEIGHT);
}

void MenuButton::setPosition(Ogre::Vector2 pos)
{
    this->pos = pos;
}

void MenuButton::setPosition(Ogre::Real x, Ogre::Real y)
{

    pos.x = x;
    pos.y = y;

    mPanel->setPosition(x,y-0.1);

}

void MenuButton::setColor(Ogre::ColourValue col)
{
    mText->setColour(col);
}

MenuButton::~MenuButton()
{
    mPanel->removeChild(name + "_text");
    if(_hasOption) mPanel->removeChild(name + "_optionText");
    mOverlay->remove2D(mPanel);

    mOverlayMgr->destroyOverlayElement(name + "_text");
    if(_hasOption) mOverlayMgr->destroyOverlayElement(name + "_optionText");
    mOverlayMgr->destroyOverlayElement(name + "_panel");

    menuButtonCount--;
}

void MenuButton::setState(int state)
{
    this->state = state;

    if(state == BSTATE_ACTIVE)
    {
        mText->setColour(ColourValue(1,1,0));
    }
    else if(state == BSTATE_INACTIVE)
    {
        mText->setColour(ColourValue(1,1,1));
    }

}

void MenuButton::update(unsigned long lTimeElapsed)
{
    if(dest_reached == false)
    {
        /*Vector3 pos = mNode->getPosition();
        Vector3 dir = dest-pos;

        dir.normalise();

        speed += dir*lTimeElapsed*0.0001;

        if(speed.length() > Vector3(pos-dest).length())
        {
            speed = Vector3(0,0,0);
            dest_reached = true;
            //mNode->setPosition(dest);
        }
        else
        {
            //mNode->setPosition(pos+speed);
        }
        */
    }

}

void MenuButton::setDest(Vector2 dest)
{
    this->dest = dest;
    dest_reached = false;
}

void MenuButton::setDest(Real x,Real y)
{
    Vector2 dest(x,y);
    this->dest = dest;
    dest_reached = false;
}




