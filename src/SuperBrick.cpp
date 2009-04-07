#include "SuperBrick.h"




using namespace Ogre;

SuperBrick::SuperBrick (const Vector3& position )
{

    alive = true;

    mSceneMgr = Root::getSingletonPtr()->getSceneManager( "ST_GENERIC" );

    mEnt = mSceneMgr->createEntity("superbrick", "endsupercube.mesh");


    mNode = mSceneMgr->getRootSceneNode()->createChildSceneNode( "superbrick" , position);
    mNode->attachObject(mEnt);

    mNode->rotate(Quaternion(Degree(-90),Vector3::UNIT_X));

    mBox = AxisAlignedBox(position.x-4.5,position.y-0.5,position.z-0.5,
                          position.x+4.5,position.y+0.5,position.z+0.5);

    mEmi = new EntityMaterialInstance (mEnt);
    mEmi->setSceneBlending (SBT_TRANSPARENT_ALPHA);
}


SuperBrick::~SuperBrick()
{
	mNode->detachAllObjects();
	mNode->getParentSceneNode()->removeAndDestroyChild(mNode->getName());

	if (mEnt)
		mSceneMgr->destroyEntity(mEnt);

}






void SuperBrick::update(unsigned long lTimeElapsed)
{

}



void SuperBrick::kill()
{

    alive = false;
    mEmi->setTransparency (0.5);

}



