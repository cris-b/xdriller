#include "TextEffectorBigMessage.h"

#include <Ogre.h>

#define DYE_TIME 1.5
#define FONT_NAME "CoolFont"
#define CHAR_HEIGHT 0.5
#define START_SCALE 1.0
#define END_SCALE   2.0
#define COLOR_R 1.0
#define COLOR_G 1.0
#define COLOR_B 0.0
#define COLOR_A 1.0

using namespace Ogre;

TextEffectorBigMessage::TextEffectorBigMessage(Ogre::SceneNode *camNode, const Ogre::String caption)
{
    dead_flag = false;
    timeAlive = 0;

    String name = caption + "_bigMessage";

    //this->camNode = camNode;

    mNode = camNode->createChildSceneNode(name + "_Node");

    ColourValue c = ColourValue(COLOR_R,COLOR_G,COLOR_B,COLOR_A);
    movableText = new MovableText(name + "_MovableText",caption,FONT_NAME,CHAR_HEIGHT,c);


    mNode->attachObject(movableText);

    mNode->translate(0,0,-5);
}

TextEffectorBigMessage::~TextEffectorBigMessage()
{
    mNode->detachObject(movableText);

    delete movableText;

    //mNode->removeAndDestroyAllChildren();

    mNode->getParentSceneNode()->removeAndDestroyChild(mNode->getName());


}

void TextEffectorBigMessage::update( unsigned long lTimeElapsed )
{
    timeAlive += (float) lTimeElapsed / 1000.0;

    Real scale = START_SCALE+(END_SCALE-START_SCALE)*timeAlive/DYE_TIME;

    mNode->setScale(scale,scale,scale);

    if(timeAlive > DYE_TIME) dead_flag = true;


}
