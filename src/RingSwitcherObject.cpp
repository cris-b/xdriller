#include "RingSwitcherObject.h"

using namespace Ogre;

RingSwitcherObject::RingSwitcherObject(const Ogre::String &name,const Ogre::String &mesh, Ogre::SceneNode *parent)
{
    this->name=name;

    mEnt = Root::getSingletonPtr()->getSceneManager( "ST_GENERIC" )->createEntity(name + "_RingSwicherObjectEntity", mesh);

    mEnt->setRenderQueueGroup(RENDER_QUEUE_OVERLAY-1);

    mEnt->setCastShadows(false);

    mNode = parent->createChildSceneNode(name + "_RingSwicherObjectSceneNode");
    mNode->attachObject(mEnt);
}

RingSwitcherObject::~RingSwitcherObject()
{

    mNode->detachObject(mEnt);
    Root::getSingletonPtr()->getSceneManager( "ST_GENERIC" )->destroyEntity(mEnt);

    mNode->getParentSceneNode()->removeAndDestroyChild(mNode->getName());

}

void RingSwitcherObject::setPosition(const Ogre::Vector3 &pos)
{
    mNode->setPosition(pos);
}
