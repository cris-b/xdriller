#include "RingSwitcher.h"

#include <math.h>

using namespace Ogre;

RingSwitcher::RingSwitcher(float size)
{
    this->size = size;

    selected = 0;

    mNode = Root::getSingletonPtr()->getSceneManager( "ST_GENERIC" )->getRootSceneNode()->createChildSceneNode("RingSwitcher");

    doubleArrowEnt = Root::getSingletonPtr()->getSceneManager( "ST_GENERIC" )->createEntity("RingSwicherDoubleArrowent", "doublearrow.mesh");

    doubleArrowEnt->setRenderQueueGroup(RENDER_QUEUE_OVERLAY-1);

    doubleArrowNode = mNode->createChildSceneNode("RingSwicherDoubleArrowNode");

    doubleArrowNode->attachObject(doubleArrowEnt);
}

RingSwitcher::~RingSwitcher()
{
    while (!objects.empty())
    {
        delete objects.back();  objects.pop_back();
    }

    doubleArrowNode->detachObject(doubleArrowEnt);

    Root::getSingletonPtr()->getSceneManager( "ST_GENERIC" )->destroyEntity(doubleArrowEnt);

    mNode->removeAndDestroyChild("RingSwicherDoubleArrowNode");

    Root::getSingletonPtr()->getSceneManager( "ST_GENERIC" )->getRootSceneNode()->removeAndDestroyChild("RingSwitcher");
}

void RingSwitcher::addObject(const Ogre::String &name,const Ogre::String &mesh)
{
    objects.push_back(new RingSwitcherObject(name,mesh,mNode));
}


void RingSwitcher::setBlocked(const Ogre::String &name,bool _blocked)
{

    for(unsigned int i=0; i<objects.size();i++)
    {
        if(objects[i]->getName() == name) objects[i]->setBlocked(_blocked);
    }
}

bool RingSwitcher::isBlocked(const Ogre::String &name)
{

    for(unsigned int i=0; i<objects.size();i++)
    {
        if(objects[i]->getName() == name) return objects[i]->isBlocked();
    }

    return false;
}

void RingSwitcher::setMaterialName(const Ogre::String &name, const Ogre::String &material)
{
    for(unsigned int i=0; i<objects.size();i++)
    {
        if(objects[i]->getName() == name) objects[i]->setMaterialName(material);
    }
}

void RingSwitcher::update( unsigned long lTimeElapsed )
{
    int num_objects = objects.size();

    if(num_objects == 0) return;
    //if(num_objects == 1) return;

    angle += lTimeElapsed/500.0;

    if(angle > 3.1415926535*2.0) angle -= 3.1415926535*2.0;

    float scale = 2 + sin(angle)*0.3;

    objects[selected]->setScale(scale);

    for(int i=0; i<num_objects;i++)
    {
        Vector3 pos;

        pos.x = ((size/(num_objects+1))*(i+1))-(size/2.0);
        pos.y = 0;
        pos.z = 0;

        objects[i]->setPosition(pos);

        if(i == selected) doubleArrowNode->setPosition(pos);

        if(i != selected) objects[i]->setScale(2.0);
    }
}

void RingSwitcher::next()
{
    int num_objects = objects.size();

    selected++;

    angle = 0;

    if(selected > num_objects-1) selected = 0;

}

void RingSwitcher::prev()
{
    int num_objects = objects.size();

    selected--;

    angle = 0;

    if(selected < 0) selected = num_objects-1;
}

Ogre::String RingSwitcher::getCurrentName()
{
    return objects[selected]->getName();
}

