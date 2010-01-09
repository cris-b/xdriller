#include "TextEffector.h"

#include <Ogre.h>


using namespace Ogre;
using namespace std;

TextEffector::TextEffector(Ogre::SceneNode *camNode)
{
    bigMessage = NULL;
    this->camNode = camNode;
}

TextEffector::~TextEffector()
{
    if(bigMessage != NULL) delete bigMessage;
}

void TextEffector::addBigMessage(const String caption)
{
    if(bigMessage != NULL)
    {
        delete bigMessage;
        bigMessage = NULL;
    }

    bigMessage = new TextEffectorBigMessage(camNode, caption);
}


void TextEffector::update( unsigned long lTimeElapsed )
{
    if(bigMessage != NULL)
    {
        bigMessage->update(lTimeElapsed);

        if(bigMessage->isDead())
        {
            delete bigMessage;
            bigMessage = NULL;
        }
    }
}
