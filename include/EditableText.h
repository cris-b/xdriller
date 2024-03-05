#ifndef EDITABLETEXT_H
#define EDITABLETEXT_H

#include <Ogre.h>

#define OIS_DYNAMIC_LIB
#include <OIS.h>


class EditableText
{
    public:
        EditableText();
        ~EditableText();

        void setText(const Ogre::String &s);
        Ogre::String &getText();

        bool injectKeyPress( const OIS::KeyEvent k);

    private:

        Ogre::String mText;

        unsigned int max_length;

};

#endif // EDITABLETEXT_H
