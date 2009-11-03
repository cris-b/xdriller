#include "RingSwitcher.h"

#include <math.h>

using namespace Ogre;

RingSwitcher::RingSwitcher(float radius)
{
    this->radius = radius;

    selected = 0;

    mNode = Root::getSingletonPtr()->getSceneManager( "ST_GENERIC" )->getRootSceneNode()->createChildSceneNode("RingSwitcher");
}

RingSwitcher::~RingSwitcher()
{
    while (!objects.empty())
    {
        delete objects.back();  objects.pop_back();
    }

    Root::getSingletonPtr()->getSceneManager( "ST_GENERIC" )->getRootSceneNode()->removeAndDestroyChild("RingSwitcher");
}

void RingSwitcher::addObject(const Ogre::String &name,const Ogre::String &mesh)
{
    objects.push_back(new RingSwitcherObject(name,mesh,mNode));
}

void RingSwitcher::update( unsigned long lTimeElapsed )
{
    int num_objects = objects.size();

    if(num_objects == 0) return;
    if(num_objects == 1) return;

    for(int i=0; i<num_objects;i++)
    {
        float angle = ((M_PI*2.0) / (float) num_objects) * (float) i;

        Vector3 pos;

        pos.x = radius * sin(angle);
        pos.y = radius * cos(angle);
        pos.z = 0;

        objects[i]->setPosition(pos);

        if(i == selected) objects[i]->setScale(1.5);
        else objects[i]->setScale(1);
    }
}

void RingSwitcher::next()
{
    int num_objects = objects.size();

    selected++;

    if(selected > num_objects-1) selected = 0;

}

void RingSwitcher::prev()
{
    int num_objects = objects.size();

    selected--;

    if(selected < 0) selected = num_objects-1;
}

Ogre::String RingSwitcher::getCurrentName()
{
    return objects[selected]->getName();
}
