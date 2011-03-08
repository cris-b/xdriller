#include "Planet.h"

using namespace Ogre;

Planet::Planet()
{

    mNode = Root::getSingletonPtr()->getSceneManager( "ST_GENERIC" )->getRootSceneNode()->createChildSceneNode("Planet_Node");

    mEnt = Root::getSingletonPtr()->getSceneManager( "ST_GENERIC" )->createEntity("Planet_Ent", "planeta.mesh");

    mEnt->setRenderQueueGroup(RENDER_QUEUE_OVERLAY-1);

    mNode->attachObject(mEnt);

    mNode->setPosition(0,-0.5,-10);

    mNode->scale(3,3,3);

    from = Quaternion(0,0,1,0);
    to = Quaternion(0,0,1,0);

    rot_progress = 0;

}

Planet::~Planet()
{

    mNode->detachObject(mEnt);

    Root::getSingletonPtr()->getSceneManager( "ST_GENERIC" )->destroyEntity(mEnt);

    Root::getSingletonPtr()->getSceneManager( "ST_GENERIC" )->getRootSceneNode()->removeAndDestroyChild("Planet_Node");
}



void Planet::update( unsigned long lTimeElapsed )
{
    if(mNode->getPosition().z < 0) mNode->translate(0,0,0.01*lTimeElapsed);
    if(mNode->getPosition().z > 0) mNode->setPosition(0,-0.5,0);

    if(from != to)
    {
        rot_progress += 0.001 * lTimeElapsed;

        if(rot_progress >= 1.57079633)
        {
            rot_progress = 0;
            from = to;
            mNode->setOrientation(to);
        }
        else
        {
            mNode->setOrientation(Quaternion::Slerp(atan(rot_progress), from, to,true));
        }

    }

}

void Planet::setOrientation(Ogre::Quaternion q)
{
    rot_progress = 0;
    from = mNode->getOrientation();
    to = q;
}
