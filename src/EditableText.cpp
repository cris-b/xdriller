#include "EditableText.h"

using namespace Ogre;

EditableText::EditableText()
{
    max_length = 10;
}

EditableText::~EditableText()
{
}


void EditableText::setText(const Ogre::String &s)
{
    mText = s;
}

String& EditableText::getText()
{
    return mText;
}

bool EditableText::injectKeyPress( const OIS::KeyEvent k)
{

    if( isgraph( k.text ) || isspace( k.text ) )
    {
        if(mText.size() < max_length) mText += k.text;
    }
    else if(k.key == OIS::KC_BACK || k.key == OIS::KC_LEFT)
    {
        if(mText.size() > 0) mText.erase(mText.end()-1);
    }
    else return false;

    return true;
}
