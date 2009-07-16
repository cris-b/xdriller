#ifndef Brick_H
#define Brick_H

#include <Ogre.h>

#include "EntityMaterialInstance.h"

#define FSTATE_STILL    0
#define FSTATE_PREFALL  1
#define FSTATE_FALLING  2
#define FSTATE_PREDYE   3

#define BRICK_NONE      0
#define BRICK_GREEN     1
#define BRICK_RED       2
#define BRICK_BLUE      3
#define BRICK_ROCK      4
#define BRICK_YELLOW    5
#define BRICK_AIR       6
#define BRICK_HEART     7






using namespace Ogre;

class Brick
{
    public:

    Brick() {}
    ~Brick();
    void create (const String&  name, SceneManager *mSceneMgr, int type, const Vector3& position);

    int getType() {return type;}

    Vector3 getPosition()
    {
        return mNode->getPosition();
    }

    int isAlive() {return alive;}
    void kill();
    void kick();
    void setPredye();


    float getX() {return mNode->getPosition().x+4;}
    float getY() {return -(mNode->getPosition().y);}

    void setY(Real pos) { mNode->setPosition(mNode->getPosition().x,pos,0);}

    bool isFalling()
    {
        if(fallstate != FSTATE_FALLING) return false;
        else return true;

    }
    bool isPreFalling()
    {
        if(fallstate != FSTATE_PREFALL) return false;
        else return true;

    }
    bool isPreOrFalling()
    {
        if(fallstate == FSTATE_PREFALL) return true;
        else if(fallstate == FSTATE_FALLING) return true;
        else return false;

    }
    bool isPreDying()
    {
        if(predyeTime>0) return true;
        else return false;

    }
    void setFalling(int newState);


    void update(unsigned long lTimeElapsed);



    bool done;

    long prefallTime;

    AxisAlignedBox mBox;

    private:

    int type;

    int lives;

    int fallstate;
    bool alive;


    long dyeTime;
    long predyeTime;

    Vector3 speed;

    SceneManager *mSceneMgr;

	Entity *mEnt;
	SceneNode *mNode;

	EntityMaterialInstance *mEmi;

	AnimationState *mAnimationState;



};



#endif

