#ifndef TEXTEFFECTORBIGMESSAGE_H
#define TEXTEFFECTORBIGMESSAGE_H

#include <Ogre.h>
#include "MovableText.h"

class TextEffectorBigMessage
{
    public:
        TextEffectorBigMessage(Ogre::SceneNode *camNode, const Ogre::String caption);
        ~TextEffectorBigMessage();

        void update( unsigned long lTimeElapsed );

        bool isDead() {return dead_flag;}

    private:


        Ogre::SceneNode *mNode;

        Ogre::Real timeAlive;
        bool dead_flag;

        MovableText *movableText;

};

#endif // TEXTEFFECTORBIGMESSAGE_H
