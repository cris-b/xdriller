#include "TextGlobe.h"

#define CHAR_HEIGHT 0.04
#define TEXT_SPEED  0.02

using namespace Ogre;

TextGlobe::TextGlobe()
{
    mOverlayMgr   = OverlayManager::getSingletonPtr();

    if(mOverlayMgr->getByName("TextGlobe") == 0)
    {
        mOverlay = mOverlayMgr->create("TextGlobe");
    }
    else
    {
        mOverlay = mOverlayMgr->getByName("TextGlobe");
    }

    mPanel = static_cast<PanelOverlayElement*>(
                 mOverlayMgr->createOverlayElement("Panel", "textGlobe_panel"));
    mPanel->setMetricsMode(Ogre::GMM_RELATIVE);
    mPanel->setHorizontalAlignment(Ogre::GHA_CENTER);
    mPanel->setVerticalAlignment(Ogre::GVA_CENTER);
    mPanel->setPosition(-0.3, 0.27);
    mPanel->setDimensions(0.60, 0.20);
    mPanel->setMaterialName("text_globe");

    mRedX = static_cast<PanelOverlayElement*>(
                 mOverlayMgr->createOverlayElement("Panel", "textGlobe_redx"));
    mRedX->setMetricsMode(Ogre::GMM_RELATIVE);
    mRedX->setPosition(0.55, 0.13);
    mRedX->setDimensions(0.0375,0.05);
    mRedX->setMaterialName("red_x");

    mText = static_cast<TextAreaOverlayElement*>(
                mOverlayMgr->createOverlayElement("TextArea", "textGlobe_text"));
    mText->setMetricsMode(Ogre::GMM_RELATIVE);
    mText->setPosition(0.01, 0.01);
    mText->setDimensions(0.59, 0.19);
    mText->setCharHeight(CHAR_HEIGHT);
    mText->setFontName("SmallFont");
    mText->setColour(ColourValue(0,0,0));
	mText->setSpaceWidth(0.017);
    /*if(align == ALIGN_CENTER) mText->setAlignment(TextAreaOverlayElement::Center);
    else if(align == ALIGN_LEFT) mText->setAlignment(TextAreaOverlayElement::Left);
    else if(align == ALIGN_RIGHT) mText->setAlignment(TextAreaOverlayElement::Right);*/



    mPanel->addChild(mText);
    mPanel->addChild(mRedX);

    mOverlay->add2D(mPanel);

    mRedX->hide();
    mOverlay->hide();

    string_length = 0;
    time_since_reset = 0;
}

TextGlobe::~TextGlobe()
{
    mPanel->removeChild("textGlobe_text");
    mPanel->removeChild("textGlobe_redx");

    mOverlay->remove2D(mPanel);

    mOverlayMgr->destroyOverlayElement("textGlobe_text");
    mOverlayMgr->destroyOverlayElement("textGlobe_redx");

    mOverlayMgr->destroyOverlayElement("textGlobe_panel");
}

void TextGlobe::update(unsigned long lTimeElapsed)
{
    time_since_reset += lTimeElapsed;

    if(string_length < text.size())
    {

        String tmpText;
        tmpText.clear();

        string_length = time_since_reset * TEXT_SPEED;

        for(unsigned int i = 0; i<string_length; i++)
        {
            if(text[i] == '^')
            {
                i++; 
                //setTextColour(text[i]);
            }
            else {
                tmpText.push_back(text[i]);
            }
        }
        mText->setCaption(tmpText);

        if(string_length >= text.size()) mRedX->show();
    }
}

void TextGlobe::setText(Ogre::String text)
{
    time_since_reset = 0;
    string_length = 0;
    this->text = text;
    mText->setCaption("");
    mOverlay->show();
    mRedX->hide();
}

void TextGlobe::setTextColour(char colour) {
    if(colour == '0') mText->setColour(ColourValue::Black);
    if(colour == '1') mText->setColour(ColourValue::Green);
    if(colour == '4') mText->setColour(ColourValue::Blue);
    if(colour == '5') mText->setColour(ColourValue::Red);
}

void TextGlobe::show()
{
    mOverlay->show();
}

void TextGlobe::hide()
{
    mOverlay->hide();
}

bool TextGlobe::isVisible()
{
    return mOverlay->isVisible();
}

bool TextGlobe::isWritten()
{
    if(string_length < text.size()) return false;
    else return true;
}

void TextGlobe::more()
{
    string_length = text.size();

    String tmpText;
        for(unsigned int i = 0; i<string_length; i++)
        {
            if(text[i] == '^')
            {
                i++;
            }
            else {
                tmpText.push_back(text[i]);
            }
        }

    mText->setCaption(tmpText);
    mRedX->show();
}

void TextGlobe::close()
{
    hide();
}








