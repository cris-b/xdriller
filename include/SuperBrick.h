#ifndef SuperBrick_H
#define SuperBrick_H

#include <Ogre.h>

#include "EntityMaterialInstance.h"




class SuperBrick
{
    public:

    SuperBrick(const Ogre::Vector3& position);
    ~SuperBrick();

    void update(unsigned long lTimeElapsed);
    void kill();

    bool isAlive() { return alive; }

    Ogre::Vector3 getPosition() {return mNode->getPosition();}

    Ogre::AxisAlignedBox mBox;

    private:

    Ogre::Real alpha;

    bool alive;

    Ogre::SceneManager *mSceneMgr;



	Ogre::Entity *mEnt;
	Ogre::SceneNode *mNode;

	EntityMaterialInstance *mEmi;

};



#endif


