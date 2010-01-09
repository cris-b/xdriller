#ifndef TEXTEFFECTOR_H
#define TEXTEFFECTOR_H

#include <Ogre.h>
#include <vector>

#include "TextEffectorBigMessage.h"

class TextEffector
{
    public:
        TextEffector(Ogre::SceneNode *camNode);
        ~TextEffector();

        void addBigMessage(const Ogre::String caption);


        void update( unsigned long lTimeElapsed );

    private:

        Ogre::SceneNode *camNode;

        //std::vector<TextEffectorBigMessage*> bigMessages;
        TextEffectorBigMessage *bigMessage;

};

#endif // TEXTEFFECTOR_H
