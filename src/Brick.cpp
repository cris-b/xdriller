#include "Board.h"
#include <string>
#include "SoundManager.h"



#define FALL_SPEED      -0.004
#define FALL_ACC        -0.01
#define PREFALL_TIME    750
#define PREDYE_TIME     200

//tiene que ser de coma flotante aunque se queje el compilador
#define DYE_TIME        500.0


using namespace Ogre;

Brick::Brick()
{
    type = BRICK_NONE;
}

Brick::~Brick()
{
    if(type != BRICK_NONE)
    {

        if(mNode->numAttachedObjects()>0) mNode->detachAllObjects();
        mNode->getParentSceneNode()->removeAndDestroyChild(mNode->getName());

        delete mEmi;

        if (mEnt)
            Root::getSingletonPtr()->getSceneManager( "ST_GENERIC" )->destroyEntity(mEnt);
    }
}


void Brick::create (const String&  name, SceneManager *mSceneMgr, int type, const Vector3& position)
{

    this->type = type;

    if(type == BRICK_NONE)
    {
        alive=false;
        return;
    }

    done = false;
    fallstate = FSTATE_STILL;
    alive = true;
    this->mSceneMgr = mSceneMgr;
    predyeTime = 0;
    dyeTime = 0;
    lives = 1;
    mEmi = NULL;

    speed = Vector3(0,0,0);

    if(type == BRICK_ROCK)
    {
        mEnt = mSceneMgr->createEntity(name, "rock.mesh");
        lives = 4;
    }
    else if(type == BRICK_AIR)
    {
        mEnt = mSceneMgr->createEntity(name, "O2.mesh");
    }
    else if(type == BRICK_HEART)
    {
        mEnt = mSceneMgr->createEntity(name, "corazon.mesh");
    }
    else if(type == BRICK_FIXED)
    {
        mEnt = mSceneMgr->createEntity(name, "fijo.mesh");
    }
    else mEnt = mSceneMgr->createEntity(name, "cube.mesh");

    #if OGRE_VERSION_MINOR < 6
        mEnt->setNormaliseNormals(true);
    #endif

    if(mSceneMgr->hasSceneNode("BoardSceneNode") == false)
    {
        mSceneMgr->getRootSceneNode()->createChildSceneNode("BoardSceneNode");
    }

    mNode = mSceneMgr->getSceneNode("BoardSceneNode")->createChildSceneNode( name , position);

    mNode->attachObject(mEnt);

    //mNode->rotate(Quaternion(Degree(-90),Vector3::UNIT_X));

    mBox = AxisAlignedBox(position.x-0.5,position.y-0.5,position.z-0.5,
                          position.x+0.5,position.y+0.5,position.z+0.5);




    if(type == BRICK_GREEN) mEnt->setMaterialName("verde");
    else if(type == BRICK_RED) mEnt->setMaterialName("rojo");
    else if(type == BRICK_BLUE) mEnt->setMaterialName("azul");
    else if(type == BRICK_ROCK) mEnt->setMaterialName("roca");
    else if(type == BRICK_YELLOW) mEnt->setMaterialName("amarillo");
    else if(type == BRICK_HEART) mEnt->setMaterialName("corazon");
    else if(type == BRICK_AIR) mEnt->setMaterialName("O2");
    else if(type == BRICK_FIXED) mEnt->setMaterialName("fijo");
    else kill();

    if(mEnt->hasSkeleton())
    {
        mAnimationState = mEnt->getAnimationState("Shake");
        mAnimationState->setLoop(true);
        mAnimationState->setEnabled(false);
    }
    mEnt->setCastShadows(false);


}

void Brick::setFalling(int newState)
{
    if(type == BRICK_FIXED) return;


    if(fallstate == FSTATE_STILL && newState == FSTATE_PREFALL)
    {
        prefallTime = PREFALL_TIME;
        fallstate = FSTATE_PREFALL;
        if(mEnt->hasSkeleton()) mAnimationState->setEnabled(true);

    }

    if(fallstate == FSTATE_FALLING && newState == FSTATE_STILL)
    {
        fallstate = FSTATE_STILL;
        if(mEnt->hasSkeleton()) mAnimationState->setEnabled(false);
    }
    if(fallstate == FSTATE_PREFALL && newState == FSTATE_STILL)
    {
        fallstate = FSTATE_STILL;
        if(mEnt->hasSkeleton()) mAnimationState->setEnabled(false);
    }
    if(fallstate == FSTATE_FALLING && newState == FSTATE_PREFALL)
    {
        //prefallTime = PREFALL_TIME;
        fallstate = FSTATE_PREFALL;
        if(mEnt->hasSkeleton()) mAnimationState->setEnabled(true);
    }


}

void Brick::setPredye()
{
    predyeTime = PREDYE_TIME;

}

void Brick::update(unsigned long lTimeElapsed)
{
    if(lTimeElapsed == 0) return;

    if(type == BRICK_NONE) return;

    static Vector3 pos;

    if(type == BRICK_AIR)
    {

        mNode->yaw(Radian(0.001*lTimeElapsed));
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
                if(mEnt->hasSkeleton()) mAnimationState->setEnabled(false);
            }

        }

        else if(fallstate == FSTATE_FALLING)
        {

            speed += Vector3(0,FALL_ACC*lTimeElapsed,0);
            if(speed.y < FALL_SPEED) speed.y=FALL_SPEED;
            mNode->setPosition(pos+speed*lTimeElapsed);

        }


        else
        {
            speed.x=0;
            speed.y=0;
        }
    }
    else if(dyeTime>0)
    {
        Real scale = dyeTime/DYE_TIME;
        mNode->setScale(scale,scale,scale);

        dyeTime -= lTimeElapsed;

        if(mEmi == NULL)
        {
            mEmi = new EntityMaterialInstance (mEnt);
            mEmi->setSceneBlending (SBT_TRANSPARENT_ALPHA);
        }

        mEmi->setTransparency (1-scale);

        if(dyeTime <= 0)
        {
            mNode->detachObject(mEnt);
            mEnt->setVisible(false);
        }
    }

    if(predyeTime>0)
    {
        /*
        if(mEmi == NULL)
        {
            mEmi = new EntityMaterialInstance (mEnt);
            mEmi->setSceneBlending (SBT_TRANSPARENT_ALPHA);
        }

        mEmi->setTransparency(0.8);*/

        predyeTime -= lTimeElapsed;
        if(predyeTime <= 0) kill();
    }

    pos = mNode->getPosition();

    mBox.setExtents(pos.x-0.5,pos.y-0.5,pos.z-0.5,
                     pos.x+0.5,pos.y+0.5,pos.z+0.5);

    if(mEnt->hasSkeleton()) mAnimationState->addTime(0.001*lTimeElapsed);

}

void Brick::kick()
{
    lives--;

    if(type == BRICK_ROCK)
    {
        if(lives == 3)
        {
            mEnt->setMaterialName("roca2");
        }
        if(lives == 2)
        {
            mEnt->setMaterialName("roca3");
        }
        if(lives == 1)
        {
            mEnt->setMaterialName("roca4");
        }

    }

    if(lives <= 0 && type != BRICK_FIXED)
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
    if(type == BRICK_HEART) SoundManager::getSingleton().playSound(SOUND_AIR);

    alive = false;

    dyeTime = DYE_TIME;

}


