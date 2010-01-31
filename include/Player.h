#ifndef Player_H
#define Player_H

#include <Ogre.h>
#include "Board.h"

#define LOOK_UP     0
#define LOOK_DOWN   1
#define LOOK_LEFT   2
#define LOOK_RIGHT  3


class Player
{
    public:

    Player(Board *mBoard);
    ~Player();

    void update(unsigned long lTimeElapsed);

    void setPosition(Ogre::Real x, Ogre::Real y, Ogre::Real z) {mNode->setPosition(x,y,z);}
    Ogre::Vector3 getPosition()
    {
        return mNode->getPosition();
    }


    void moveLeft(Ogre::Real speed = 1);
    void moveRight(Ogre::Real speed = 1);
    void moveUp();
    void moveDown();

    bool movingLeft() {return _moveLeft;}
    bool movingRight(){return _moveRight;}

    bool isAlive() { return alive;}

    void breakBlock();

    Ogre::Real getAir() {return air;};
    void setAir(Ogre::Real air);
    Ogre::Real getLives() {return lives;};
    void setLives(int lives);
    int getDepth() {return depth;}
    int getLastDepth() {return lastDepth;}
    void resurrect();
    int getPoints() { return points; }
    bool getFinished() {return finished;}

    void livesUp(int n);

    void setBoard(Board *board) {mBoard = board;};
    void setEndFloor();

    int orientation;

    private:

    Ogre::SceneManager *mSceneMgr;
    Ogre::Vector3 speed;
    Ogre::Real orientationAngle;



    Ogre::Real air;
    Ogre::Real idleTime;
    int lives;
    int depth;
    int points;
    int lastDepth;
    Ogre::Real scale;

    bool _moveLeft;
    bool _moveRight;

    Ogre::Real _moveSpeed;

    Ogre::Real _jumpCharger;

    bool _moveJump;

    bool _falling;

    bool _isAnimated;

    int fallTime;

    bool alive;
    bool finished;

    bool endFloor;
    Ogre::Real endFloorPosY;

	Ogre::Entity *mEnt;
	Ogre::SceneNode *mNode;
	Ogre::SceneNode *mScaleNode;
	Ogre::ParticleSystem *starsParticle;
	Ogre::ParticleSystem *heartsParticle;
	Ogre::ParticleSystem *plus20Particle;

    Ogre::AnimationState *mAnimationState;

	Ogre::AxisAlignedBox mBox;

	Board *mBoard;

    void setAnimationState(const Ogre::String& anim);
    Ogre::String getAnimationName();
    bool getAnimationEnded();
};

#endif
