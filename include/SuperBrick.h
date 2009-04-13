#ifndef SuperBrick_H
#define SuperBrick_H

#include <Ogre.h>

#include "EntityMaterialInstance.h"


using namespace Ogre;

class SuperBrick
{
    public:

    SuperBrick(const Vector3& position);
    ~SuperBrick();

    void update(unsigned long lTimeElapsed);
    void kill();

    bool isAlive() { return alive; }

    Vector3 getPosition() {return mNode->getPosition();}

    AxisAlignedBox mBox;

    private:

    Real alpha;

    bool alive;

    SceneManager *mSceneMgr;



	Entity *mEnt;
	SceneNode *mNode;

	EntityMaterialInstance *mEmi;

};



#endif


