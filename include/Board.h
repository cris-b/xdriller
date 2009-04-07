#ifndef Board_H
#define Board_H

#include <Ogre.h>
#include "Brick.h"
#include "SuperBrick.h"


using namespace Ogre;


class Board
{
    public:

    Board(int width, int height, const String&  level);
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

    void update(unsigned long lTimeElapsed);


    void rKillUp(Vector3 pos);
    void rKillDown(Vector3 pos);
    void rKillLeft(Vector3 pos);
    void rKillRight(Vector3 pos);

    Brick *detectCollision(AxisAlignedBox &b2);

    void printLog();

    private:

    SceneManager *mSceneMgr;
    int height, width;
    void _rKill(int x, int y, int type);
    void _rSetPredye(int x, int y, int type);
    void _rSetFallState(int x, int y, int state, int type);
    void _rClearDone(int x, int y, int type);

    bool _checkFall(int x, int y, int type);
    void _checkNum (int x, int y, int type,int &num);

    void clearDone();


};



#endif
