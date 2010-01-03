#ifndef Board_H
#define Board_H

#include <Ogre.h>
#include "Brick.h"
#include "SuperBrick.h"






class Board
{
    public:

    Board();
    ~Board();

    Brick *mBricks;
    Brick **mBricksPtr;
    SuperBrick *mSuperBrick;

    int getHeight() {return height;}
    int getWidth() {return width;}

    void rKill(int x, int y);
    void rSetPredye(int x, int y);
    void rSetFallState(int x, int y, int state);
    void rClearDone(int x, int y);


    bool checkFall (int x, int y);
    int  checkNum  (int x, int y);



    int  rKillUp(Ogre::Vector3 pos);
    int  rKillDown(Ogre::Vector3 pos);
    int  rKillLeft(Ogre::Vector3 pos);
    int  rKillRight(Ogre::Vector3 pos);

    void killUpwards(Ogre::Vector3 pos);

    Brick *detectCollision(Ogre::AxisAlignedBox &b2);

    void update(unsigned long lTimeElapsed);

    void printLog();

    int getPoints() {return points;}

    private:

    Ogre::SceneManager *mSceneMgr;
    int height, width;

    int firstToCheck;

    void _rKill(int x, int y, int type);
    void _rSetPredye(int x, int y, int type);
    void _rSetFallState(int x, int y, int state, int type);
    void _rClearDone(int x, int y, int type);

    bool _checkFall(int x, int y, int type);
    void _checkNum (int x, int y, int type,int &num);

    void clearDone();

    int points;

    /*ParticleSystem *spiralsParticle;

    void emitOneParticle(Real x, Real y);*/


};



#endif
