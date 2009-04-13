#include "Board.h"
#include <string>
#include "SoundManager.h"



#define FALL_SPEED      -0.004
#define FALL_ACC        -0.01
#define PREFALL_TIME    750
#define PREDYE_TIME     200

#define DYE_TIME        500.0

using namespace Ogre;

Brick::~Brick()
{
	mNode->detachAllObjects();
	mNode->getParentSceneNode()->removeAndDestroyChild(mNode->getName());

    delete mEmi;

	if (mEnt)
		Root::getSingletonPtr()->getSceneManager( "ST_GENERIC" )->destroyEntity(mEnt);

}


void Brick::create (const String&  name, SceneManager *mSceneMgr, int type, const Vector3& position)
{

    this->type = type;

    done = false;
    fallstate = FSTATE_STILL;
    alive = true;
    this->mSceneMgr = mSceneMgr;
    predyeTime = 0;
    dyeTime = 0;
    lives = 1;

    speed = Vector3(0,0,0);

    if(type == 4)
    {
        mEnt = mSceneMgr->createEntity(name, "rock.mesh");
        lives = 4;
    }
    else if(type == 6)
    {
        mEnt = mSceneMgr->createEntity(name, "O2.mesh");
    }
    else mEnt = mSceneMgr->createEntity(name, "cube.mesh");

    mNode = mSceneMgr->getRootSceneNode()->createChildSceneNode( name , position);
    mNode->attachObject(mEnt);

    mNode->rotate(Quaternion(Degree(-90),Vector3::UNIT_X));

    mBox = AxisAlignedBox(position.x-0.5,position.y-0.5,position.z-0.5,
                          position.x+0.5,position.y+0.5,position.z+0.5);

    mEmi = new EntityMaterialInstance (mEnt);
    mEmi->setSceneBlending (SBT_TRANSPARENT_ALPHA);

    if(type == BRICK_NONE) kill();
    else if(type == BRICK_GREEN) mEmi->setMaterialName("verde");
    else if(type == BRICK_RED) mEmi->setMaterialName("rojo");
    else if(type == BRICK_BLUE) mEmi->setMaterialName("azul");
    else if(type == BRICK_ROCK) mEmi->setMaterialName("roca");
    else if(type == BRICK_YELLOW) mEmi->setMaterialName("amarillo");
    else if(type == BRICK_AIR);
    else kill();


    mAnimationState = mEnt->getAnimationState("Shake");
    mAnimationState->setLoop(true);
    mAnimationState->setEnabled(false);




}

void Brick::setFalling(int newState)
{

    if(fallstate == FSTATE_STILL && newState == FSTATE_PREFALL)
    {
        prefallTime = PREFALL_TIME;
        fallstate = FSTATE_PREFALL;
        mAnimationState->setEnabled(true);

    }

    if(fallstate == FSTATE_FALLING && newState == FSTATE_STILL)
    {
        fallstate = FSTATE_STILL;
        mAnimationState->setEnabled(false);
    }
    if(fallstate == FSTATE_PREFALL && newState == FSTATE_STILL)
    {
        fallstate = FSTATE_STILL;
        mAnimationState->setEnabled(false);
    }
    if(fallstate == FSTATE_FALLING && newState == FSTATE_PREFALL)
    {
        fallstate = FSTATE_PREFALL;
        mAnimationState->setEnabled(true);
    }

}

void Brick::setPredye()
{
    predyeTime = PREDYE_TIME;

}

void Brick::update(unsigned long lTimeElapsed)
{
    static Vector3 pos;

    if(type == 6)
    {

        mNode->roll(Radian(0.001*lTimeElapsed));
    }

    if(isAlive())
    {
        pos =  mNode->getPosition();
        if(fallstate == FSTATE_STILL)
        {
            if(getY() - (int)(getY()) >= 0.5)
            {
                speed += Vector3(0,FALL_ACC*lTimeElapsed,0);
                if(speed.y < FALL_SPEED) speed.y=FALL_SPEED;
                mNode->setPosition(pos+speed*lTimeElapsed);
                if( getY() - (int)getY() < 0.5)
                    mNode->setPosition(Vector3(getX()-4,-(int)(getY()),0));
            }

            mNode->setPosition(Vector3(getX()-4,(int)-getY(),0));
        }
        else if(fallstate == FSTATE_PREFALL)
        {
            prefallTime -= lTimeElapsed;

            if(prefallTime <=0)
            {
                fallstate = FSTATE_FALLING;
                mAnimationState->setEnabled(false);
            }

        }

        else if(fallstate == FSTATE_FALLING)
        {

            speed += Vector3(0,FALL_ACC*lTimeElapsed,0);
            if(speed.y < FALL_SPEED) speed.y=FALL_SPEED;
            mNode->setPosition(pos+speed*lTimeElapsed);

        }


        else speed = Vector3(0,0,0);

    }
    else if(dyeTime>0)
    {
        Real scale = dyeTime/DYE_TIME;
        mNode->setScale(scale,scale,scale);

        dyeTime -= lTimeElapsed;

        mEmi->setTransparency (1-scale);

        if(dyeTime <= 0)
        {
            mNode->detachObject(mEnt);
            mEnt->setVisible(false);
        }
    }

    if(predyeTime>0)
    {
        mEmi->setTransparency(0.8);
        predyeTime -= lTimeElapsed;
        if(predyeTime <= 0) kill();
    }

    pos = mNode->getPosition();

    mBox.setExtents(pos.x-0.5,pos.y-0.5,pos.z-0.5,
                     pos.x+0.5,pos.y+0.5,pos.z+0.5);

    mAnimationState->addTime(0.001*lTimeElapsed);

}

void Brick::kick()
{
    lives--;

    if(type == BRICK_ROCK)
    {
        if(lives == 3)
        {
            mEmi->setMaterialName("roca2");
        }
        if(lives == 2)
        {
            mEmi->setMaterialName("roca3");
        }
        if(lives == 1)
        {
            mEmi->setMaterialName("roca4");
        }

    }

    if(lives <= 0)
    {
        SoundManager::getSingleton().playSound(SOUND_BREAK);
        kill();
    }
    else
    {
        SoundManager::getSingleton().playSound(SOUND_KICK);
    }


}

void Brick::kill()
{

    if(type == BRICK_AIR) SoundManager::getSingleton().playSound(SOUND_AIR);

    alive = false;

    dyeTime = DYE_TIME;

}


