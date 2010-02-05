#include "RingSwitcherObject.h"

using namespace Ogre;

RingSwitcherObject::RingSwitcherObject(const Ogre::String &name,const Ogre::String &mesh, Ogre::SceneNode *parent)
{
    this->name=name;

    blocked = false;

    mEnt = Root::getSingletonPtr()->getSceneManager( "ST_GENERIC" )->createEntity(name + "_RingSwicherObjectEntity", mesh);

    mEnt->setRenderQueueGroup(RENDER_QUEUE_OVERLAY-1);

    mEnt->setCastShadows(false);

    mNode = parent->createChildSceneNode(name + "_RingSwicherObjectSceneNode");
    mNode->attachObject(mEnt);

    mEmi = new EntityMaterialInstance (mEnt);

    mEmi->setSceneBlending (SBT_TRANSPARENT_ALPHA);
}

RingSwitcherObject::~RingSwitcherObject()
{
    delete mEmi;

    mNode->detachObject(mEnt);
    Root::getSingletonPtr()->getSceneManager( "ST_GENERIC" )->destroyEntity(mEnt);

    mNode->getParentSceneNode()->removeAndDestroyChild(mNode->getName());



}

void RingSwitcherObject::setPosition(const Ogre::Vector3 &pos)
{
    mNode->setPosition(pos);
}

void RingSwitcherObject::setBlocked(bool _blocked)
{
    blocked = _blocked;

    if(blocked) mEmi->setTransparency (0.7f);
    else mEmi->setTransparency (0.0f);
}

bool RingSwitcherObject::isBlocked()
{
    return blocked;
}

void RingSwitcherObject::setMaterialName(const Ogre::String &material)
{
    mEnt->setMaterialName(material);
}
