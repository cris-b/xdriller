#include "RingSwitcher.h"

#include <math.h>

using namespace Ogre;

RingSwitcher::RingSwitcher(float radius)
{
    this->radius = radius;

    selected = 0;
    desired_angle = 0;
    angle = 0;

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

    if(angle < desired_angle)
    {
        angle += lTimeElapsed/100.0;

        if(angle > desired_angle) angle = desired_angle;
    }
    else if(angle > desired_angle)
    {
        angle -= lTimeElapsed/100.0;

        if(angle < desired_angle) angle = desired_angle;
    }

    for(int i=0; i<num_objects;i++)
    {
        float a = ((M_PI*2.0) / (float) num_objects) * (float) i;

        a += angle;

        Vector3 pos;

        pos.x = radius * sin(a);
        pos.y = radius * cos(a);
        pos.z = 0;

        objects[i]->setPosition(pos);

        float scale = 1 + cos(a)*2.0;

        if(scale < 1) scale = 1;

        objects[i]->setScale(scale);

    }
}

void RingSwitcher::next()
{
    int num_objects = objects.size();

    selected++;

    desired_angle -= (M_PI*2.0) / (float) num_objects;

    if(selected > num_objects-1) selected = 0;

}

void RingSwitcher::prev()
{
    int num_objects = objects.size();

    selected--;

    desired_angle += (M_PI*2.0) / (float) num_objects;

    if(selected < 0) selected = num_objects-1;
}

Ogre::String RingSwitcher::getCurrentName()
{
    return objects[selected]->getName();
}
