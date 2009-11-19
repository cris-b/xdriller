#ifndef Player_H
#define Player_H

#include <Ogre.h>
#include "Board.h"

#define LOOK_UP     0
#define LOOK_DOWN   1
#define LOOK_LEFT   2
#define LOOK_RIGHT  3

using namespace Ogre;

class Player
{
    public:

    Player(Board *mBoard);
    ~Player();

    void update(unsigned long lTimeElapsed);

    void setPosition(Real x, Real y, Real z) {mNode->setPosition(x,y,z);}
    Vector3 getPosition()
    {
        return mNode->getPosition();
    }


    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();

    bool movingLeft() {return _moveLeft;}
    bool movingRight(){return _moveRight;}

    bool isAlive() { return alive;}

    void breakBlock();

    Real getAir() {return air;};
    void setAir(Real air);
    Real getLives() {return lives;};
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

    SceneManager *mSceneMgr;
    Vector3 speed;
    Real orientationAngle;



    Real air;
    Real idleTime;
    int lives;
    int depth;
    int points;
    int lastDepth;
    Real scale;

    bool _moveLeft;
    bool _moveRight;

    bool _moveJump;

    bool _falling;

    bool _isAnimated;

    int fallTime;

    bool alive;
    bool finished;

    bool endFloor;
    Real endFloorPosY;

	Entity *mEnt;
	SceneNode *mNode;
	SceneNode *mScaleNode;
	ParticleSystem *starsParticle;
	ParticleSystem *heartsParticle;
	ParticleSystem *plus20Particle;

    AnimationState *mAnimationState;

	AxisAlignedBox mBox;

	Board *mBoard;

    void setAnimationState(const String& anim);
    String getAnimationName();
    bool getAnimationEnded();
};

#endif
