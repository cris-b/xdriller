#include "SuperBrick.h"




using namespace Ogre;

SuperBrick::SuperBrick (const Vector3& position )
{

    alive = true;

    mSceneMgr = Root::getSingletonPtr()->getSceneManager( "ST_GENERIC" );

    mEnt = mSceneMgr->createEntity("SuperBrick", "endsupercube.mesh");

    //mEnt->setMaterialName("supercubo");

    mEnt->setCastShadows(false);

    if(mSceneMgr->hasSceneNode("BoardSceneNode") == false)
    {
        mSceneMgr->getRootSceneNode()->createChildSceneNode("BoardSceneNode");
    }

    mNode = mSceneMgr->getSceneNode("BoardSceneNode")->createChildSceneNode( "SuperBrick" , position);
    mNode->attachObject(mEnt);

    //mNode->rotate(Quaternion(Degree(-90),Vector3::UNIT_X));

    mBox = AxisAlignedBox(position.x-4.5,position.y-0.5,position.z-0.5,
                          position.x+4.5,position.y+0.5,position.z+0.5);

    mEmi = new EntityMaterialInstance (mEnt);
    mEmi->setSceneBlending (SBT_TRANSPARENT_ALPHA);

    alpha = 0.1;

    mEmi->setTransparency (alpha);
}


SuperBrick::~SuperBrick()
{
	mNode->detachAllObjects();
	mNode->getParentSceneNode()->removeAndDestroyChild(mNode->getName());

    delete mEmi;

	if (mEnt)
		mSceneMgr->destroyEntity(mEnt);


}






void SuperBrick::update(unsigned long lTimeElapsed)
{


    if(!alive && alpha < 1.0)
    {
        alpha += 0.005*lTimeElapsed;

        if(alpha > 1) alpha = 1;

        mEmi->setTransparency (alpha);
    }

}



void SuperBrick::kill()
{

    alive = false;

}



