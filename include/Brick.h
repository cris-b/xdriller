#ifndef Brick_H
#define Brick_H

#include <Ogre.h>

#include "EntityMaterialInstance.h"

#define FSTATE_STILL    0
#define FSTATE_PREFALL  1
#define FSTATE_FALLING  2
#define FSTATE_PREDYE   3

#define BRICK_SUPERBRICK   -1
#define BRICK_NONE          0
#define BRICK_GREEN         1
#define BRICK_RED           2
#define BRICK_BLUE          3
#define BRICK_ROCK          4
#define BRICK_YELLOW        5
#define BRICK_AIR           6
#define BRICK_HEART         7
#define BRICK_FIXED         8
#define BRICK_TUTORIAL      9

class Brick
{
    public:

    Brick();
    ~Brick();
    void create (const Ogre::String&  name, Ogre::SceneManager *mSceneMgr, int type, const Ogre::Vector3& position);

    int getType() {return type;}

    Ogre::Vector3 getPosition()
    {
        return mNode->getPosition();
    }

    int isAlive() {return alive;}
    void kill();
    void kick();
    void setPredye();


    float getX() {return mNode->getPosition().x+4.0;}
    float getY() {return -(mNode->getPosition().y);}

    void setY(Ogre::Real pos) { mNode->setPosition(mNode->getPosition().x,pos,0);}

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

    Ogre::AxisAlignedBox mBox;

    private:

    int type;

    int lives;

    int fallstate;
    bool alive;


    long dyeTime;
    long predyeTime;

    Ogre::Vector3 speed;

    Ogre::SceneManager *mSceneMgr;

	Ogre::Entity *mEnt;
	Ogre::SceneNode *mNode;

	EntityMaterialInstance *mEmi;

	Ogre::AnimationState *mAnimationState;



};



#endif

