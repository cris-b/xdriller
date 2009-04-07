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
    void setAir(float air);
    Real getLives() {return lives;};
    void setLives(int lives);
    Real getDepth() {return depth;}
    Real getLastDepth() {return lastDepth;}

    int orientation;

    private:

    Vector3 speed;

    Real air;
    Real idleTime;
    int lives;
    Real depth;
    Real lastDepth;

    bool _moveLeft;
    bool _moveRight;

    bool _moveJump;

    bool _falling;

    int fallTime;

    bool alive;

	Entity *mEnt;
	SceneNode *mNode;

    AnimationState *mAnimationState;

	AxisAlignedBox mBox;

	Board *mBoard;

};

#endif
