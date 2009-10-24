#include "Board.h"
#include <string>
#include "Brick.h"
#include "SoundManager.h"
#include "LevelLoader.h"


#define FALL_SPEED  -0.008
#define FALL_ACC    -0.00007
#define PREFALL_TIME  500

using namespace Ogre;


Board::Board()
{

    mSceneMgr = Root::getSingletonPtr()->getSceneManager( "ST_GENERIC" );



    /*if(levelName == "random")
    {

        this->width = w;
        this->height = h;



        mBricks = new Brick[width*height];
        mBricksPtr = new Brick *[width*height];

        for(int j = 0; j < height; j++)
            for(int i = 0; i < width; i++)
            {
                int t = rand() % 6 +1;
                String n = "Brick" + StringConverter::toString(i) + ":" + StringConverter::toString(j);
                Vector3 v = Vector3(i-4,-j,0);

                mBricks[j*width+i].create(n,mSceneMgr,t,v);

            }
    }*/

    LevelLoader *mLevel = LevelLoader::getSingletonPtr();

    mLevel->loadBoard();

    this->height = mLevel->getHeight();
    this->width = mLevel->getWidth();

    mBricks = new Brick[width*height];
    mBricksPtr = new Brick *[width*height];

    for(int j = 0; j < this->height; j++)
        for(int i = 0; i < this->width; i++)
        {
            int t = mLevel->getBrickType(j*width+i);
            String n = "Brick_" + StringConverter::toString(i) + ":" + StringConverter::toString(j);
            Vector3 v = Vector3(i-4,-j,0);
            mBricks[j*width+i].create(n,mSceneMgr,t,v);
        }


    mSuperBrick = new SuperBrick(Vector3(0,-height,0));

    firstToCheck = 0;
    points = 0;


}

Board::~Board()
{
    delete [] mBricks;

    delete [] mBricksPtr;

    delete mSuperBrick;
}

void Board::rKill(int x,int y)
{
    int type;



    if(mBricksPtr[x+y*width] == NULL) return;
    if(mBricksPtr[x+y*width]->isFalling()) return;

    points += 10;

    type = mBricksPtr[x+y*width]->getType();

    SoundManager::getSingleton().playSound(SOUND_BREAK);

    _rKill(x,y,type);

}

void Board::_rKill(int x,int y, int type)
{
    if(y > firstToCheck) firstToCheck = y;

    if(mBricksPtr[x+y*width] == NULL) return;
    else if(mBricksPtr[x+y*width]->getType() != type) return;

    mBricksPtr[x+y*width]->kill();
    mBricksPtr[x+y*width] = NULL;

    if(x > 0)
        if(mBricksPtr[x-1+y*width] != NULL && mBricksPtr[x-1+y*width]->isAlive())
            _rKill(x-1,y,type);

    if(x < width -1)
        if(mBricksPtr[x+1+y*width] != NULL && mBricksPtr[x+1+y*width]->isAlive())
            _rKill(x+1,y,type);

    if(y > 0)
        if(mBricksPtr[x+(y-1)*width] != NULL && mBricksPtr[x+(y-1)*width]->isAlive())
            _rKill(x,y-1,type);

    if(y < height -1)
        if(mBricksPtr[x+(y+1)*width] != NULL && mBricksPtr[x+(y+1)*width]->isAlive())
            _rKill(x,y+1,type);


}

void Board::rSetPredye(int x,int y)
{
    int type;



    if(mBricksPtr[x+y*width] == NULL) return;
    if(mBricksPtr[x+y*width]->isPreDying()) return;
    type = mBricksPtr[x+y*width]->getType();

    points += 5;

    SoundManager::getSingleton().playSound(SOUND_JOIN);

    _rSetPredye(x,y,type);

    rClearDone(x,y);

}

void Board::_rSetPredye(int x,int y, int type)
{
    if(y > firstToCheck) firstToCheck = y;

    if(mBricksPtr[x+y*width] == NULL) return;
    else if(mBricksPtr[x+y*width]->getType() != type) return;

    mBricksPtr[x+y*width]->setPredye();
    mBricksPtr[x+y*width]->done = true;

    if(x > 0)
        if(mBricksPtr[x-1+y*width] != NULL && !mBricksPtr[x-1+y*width]->done)
            _rSetPredye(x-1,y,type);

    if(x < width -1)
        if(mBricksPtr[x+1+y*width] != NULL && !mBricksPtr[x+1+y*width]->done)
            _rSetPredye(x+1,y,type);

    if(y > 0)
        if(mBricksPtr[x+(y-1)*width] != NULL && !mBricksPtr[x+(y-1)*width]->done)
            _rSetPredye(x,y-1,type);

    if(y < height -1)
        if(mBricksPtr[x+(y+1)*width] != NULL && !mBricksPtr[x+(y+1)*width]->done)
            _rSetPredye(x,y+1,type);



    return;

}

void Board::rSetFallState(int x,int y,int state)
{
    int type;

    if(mBricksPtr[x+y*width] == NULL) return;
    type = mBricksPtr[x+y*width]->getType();



    _rSetFallState(x,y,state,type);

    rClearDone(x,y);


}

void Board::_rSetFallState(int x,int y, int state, int type)
{
    if(y > firstToCheck) firstToCheck = y;

    if(mBricksPtr[x+y*width] == NULL) return;
    else if(mBricksPtr[x+y*width]->getType() != type) return;

    mBricksPtr[x+y*width]->setFalling(state);
    mBricksPtr[x+y*width]->done = true;

    if(x > 0)
        if(mBricksPtr[x-1+y*width] != NULL && !mBricksPtr[x-1+y*width]->done)
            _rSetFallState(x-1,y,state,type);

    if(x < width -1)
        if(mBricksPtr[x+1+y*width] != NULL && !mBricksPtr[x+1+y*width]->done)
            _rSetFallState(x+1,y,state,type);

    if(y > 0)
        if(mBricksPtr[x+(y-1)*width] != NULL && !mBricksPtr[x+(y-1)*width]->done)
            _rSetFallState(x,y-1,state,type);

    if(y < height -1)
        if(mBricksPtr[x+(y+1)*width] != NULL && !mBricksPtr[x+(y+1)*width]->done)
            _rSetFallState(x,y+1,state,type);


}



bool Board::checkFall(int x, int y)
{

    int type;
    bool fall = true;

    if(mBricksPtr[x+y*width] == NULL) return true;

    if(y == height -1) return false;

    if(mBricksPtr[x+y*width]->done == true) return mBricksPtr[x+y*width]->isFalling();

    type = mBricksPtr[x+y*width]->getType();

    fall = _checkFall(x,y,type);

    //clearDone();
    rClearDone(x,y);

    return fall;
}

bool Board::_checkFall(int x,int y, int type)
{

    if(y > firstToCheck) firstToCheck = y;

    if(y == height -1) return false;




    if(mBricksPtr[x+y*width]->getType() == type)
    {
        mBricksPtr[x+y*width]->done = true;

        if(mBricksPtr[x+(y+1)*width] != NULL)
        {
            if(mBricksPtr[x+(y+1)*width]->getType() != type)
            {
                if(!mBricksPtr[x+(y+1)*width]->isPreOrFalling())
                {
                    return false;
                }
                else
                {

                 int locomotora = 1;

                 locomotora++;

                }
            }
        }

        if(x > 0)
            if(mBricksPtr[x-1+y*width] != NULL && !mBricksPtr[x-1+y*width]->done && mBricksPtr[x-1+y*width]->getType() != 6)
                if(_checkFall(x-1,y,type) == false)
                    return false;

        if(x < width -1)
            if(mBricksPtr[x+1+y*width] != NULL && !mBricksPtr[x+1+y*width]->done && mBricksPtr[x+1+y*width]->getType() != 6)
                if(_checkFall(x+1,y,type) == false)
                    return false;

        if(y > 0)
            if(mBricksPtr[x+(y-1)*width] != NULL && !mBricksPtr[x+(y-1)*width]->done && mBricksPtr[x+(y-1)*width]->getType() != 6)
                if(_checkFall(x,y-1,type) == false)
                    return false;

        if(y < height -1)
        {
            if(mBricksPtr[x+(y+1)*width] != NULL && !mBricksPtr[x+(y+1)*width]->done)
                if(_checkFall(x,y+1,type) == false)
                    return false;
        }
        else return false;
    }
    //else return true;

    return true;


}

int Board::checkNum(int x, int y)
{

  int type;
  int num = 0;

  if(mBricksPtr[x+y*width] == NULL) return 0;
  else type = mBricksPtr[x+y*width]->getType();

  if(type == BRICK_AIR || type == BRICK_HEART) return 1;

  _checkNum(x,y,type,num);

  rClearDone(x,y);


  return num;
}

void Board::_checkNum(int x,int y, int type,int &num)
{
    if(y > firstToCheck) firstToCheck = y;

    if(y == height -1) return;

    mBricksPtr[x+y*width]->done = true;


    if(mBricksPtr[x+y*width]->getType() == type)
    {

        num++;

        if(x > 0)
            if(mBricksPtr[x-1+y*width] != NULL && !mBricksPtr[x-1+y*width]->done)
                _checkNum(x-1,y,type,num);

        if(x < width -1)
            if(mBricksPtr[x+1+y*width] != NULL && !mBricksPtr[x+1+y*width]->done)
                _checkNum(x+1,y,type,num);

        if(y > 0)
            if(mBricksPtr[x+(y-1)*width] != NULL && !mBricksPtr[x+(y-1)*width]->done)
                _checkNum(x,y-1,type,num);

        if(y < height -1)
            if(mBricksPtr[x+(y+1)*width] != NULL && !mBricksPtr[x+(y+1)*width]->done)
                _checkNum(x,y+1,type,num);
    }

}



void Board::rClearDone(int x,int y)
{
    int type;

    if(mBricksPtr[x+y*width] == NULL) return;
    type = mBricksPtr[x+y*width]->getType();



    _rClearDone(x,y,type);
}

void Board::_rClearDone(int x,int y, int type)
{
    if(mBricksPtr[x+y*width] == NULL) return;
    //else if(mBricksPtr[x+y*width]->getType() != type) return;
    else if(mBricksPtr[x+y*width]->done      != true) return;

    mBricksPtr[x+y*width]->done = false;

    if(x > 0)
        if(mBricksPtr[x-1+y*width] != NULL && mBricksPtr[x-1+y*width]->done)
            _rClearDone(x-1,y,type);

    if(x < width -1)
        if(mBricksPtr[x+1+y*width] != NULL && mBricksPtr[x+1+y*width]->done)
            _rClearDone(x+1,y,type);

    if(y > 0)
        if(mBricksPtr[x+(y-1)*width] != NULL && mBricksPtr[x+(y-1)*width]->done)
            _rClearDone(x,y-1,type);

    if(y < height -1)
        if(mBricksPtr[x+(y+1)*width] != NULL && mBricksPtr[x+(y+1)*width]->done)
            _rClearDone(x,y+1,type);


}




void Board::clearDone()
{
    for(int i = 0; i < height*width; i++)
    {
        if( mBricksPtr[i] != NULL) mBricksPtr[i]->done = false;
    }
}

void Board::update(unsigned long lTimeElapsed)
{

    for(int i = 0; i < height*width; i++)
    {
        mBricks[i].update(lTimeElapsed);
    }

    for(int i = 0; i < height*width; i++) mBricksPtr[i] = NULL;

    for(int i = 0; i < height*width; i++)
    {
        if(mBricks[i].isAlive())
        {
            mBricksPtr[(int)(mBricks[i].getX())+((int)(mBricks[i].getY()))*width] = &mBricks[i];
        }
    }

    for(int i = 0; i < width; i++)
    {
        bool fall = false;

        for(int j = height-2; j >= 0; j--)
        {
            if( mBricksPtr[i+j*width] == NULL)
            {
                fall = true;

            }

            else if(mBricksPtr[i+j*width] != NULL &&
                    !mBricksPtr[i+j*width]->isPreOrFalling() && fall == true)
            {
                mBricksPtr[i+j*width]->setFalling(FSTATE_PREFALL);
                if(j > firstToCheck) firstToCheck = j;
            }
        }
    }

    for(int j = firstToCheck; j >= 0; j--)
    {
        for(int i = 0; i < width; i++)
        {
            if( mBricksPtr[i+j*width] != NULL)
            {
                if(!mBricksPtr[i+j*width]->done)
                {
                    bool doFall = checkFall(i,j);




                    if(mBricksPtr[i+j*width]->isPreOrFalling() && !doFall)
                    {
                        if(checkNum(i,j) >= 4 && mBricksPtr[i+j*width]->isFalling()) rSetPredye(i,j);

                        mBricksPtr[i+j*width]->setFalling(FSTATE_STILL);

                    }
                    else if(!mBricksPtr[i+j*width]->isFalling() && doFall)
                    {
                        mBricksPtr[i+j*width]->setFalling(FSTATE_PREFALL);
                    }
                    //else if(mBricksPtr[i+j*width]->isFalling() && doFall)
                    //{
                    //    mBricksPtr[i+j*width]->setFalling(FSTATE_PREFALL);
                    //}

                }

                if(j<height-1 && mBricksPtr[i+(j+1)*width] != NULL)
                {
                    if(mBricksPtr[i+j*width]->isFalling() && mBricksPtr[i+(j+1)*width]->isPreFalling())
                    {
                        //if(mBricksPtr[i+j*width]->getType() == mBricksPtr[i+(j+1)*width]->getType())
                        {
                            //if(mBricksPtr[i+j*width]->getPosition().y-mBricksPtr[i+(j+1)*width]->getPosition().y < 1)
                            {
                                //mBricksPtr[i+j*width]->setY((int)-mBricksPtr[i+j*width]->getY());
                                          rSetFallState(i,j,FSTATE_PREFALL);
                            }
                        }
                    }
                }
            }
        }
    }
    //clearDone();



    mSuperBrick->update(lTimeElapsed);

}

void Board::printLog()
{



    for(int j = 0; j < height; j++)
    {
        String s = "Board state:";
        for(int i = 0; i < width; i++)
        {
            if( mBricksPtr[i+j*width] != NULL)
            {
                s += StringConverter::toString(mBricksPtr[i+j*width]->getType()) + ",";
            }
            else s += "N,";
        }

        s += "\n";
        LogManager::getSingleton().logMessage(s);
    }

}

Brick *Board::detectCollision(AxisAlignedBox &b2)
{
    //metodo bestia
    /*for(int i = 0; i < height*width; i++)
    {
        if(mBricks[i].isAlive())
        {
            if(mBricks[i].mBox.intersects(b2))
            return &mBricks[i];
        }
    }*/

    //metodo alternativo
    //( en principio no se nota gran subida en FPS )

    int j1 = (int)-b2.getCenter().y-2;
    int i1 = (int)b2.getCenter().x+4-2;
    int j2 = j1+4;
    int i2 = i1+4;

    if(j1 < 0) j1 = 0; if(j1 > height-1) j1 = height -1;
    if(j2 < 0) j2 = 0; if(j2 > height-1) j2 = height -1;
    if(i1 < 0) i1 = 0; if(i1 > width-1)  i1 = width -1;
    if(i2 < 0) i2 = 0; if(i2 > width-1)  i2 = width -1;




    for(int j = j1; j <= j2; j++)
        for(int i = i1; i <= i2; i++)
        {
            if( mBricksPtr[i+j*width] != NULL)
            {
                if(mBricksPtr[i+j*width]->mBox.intersects(b2))
                return mBricksPtr[i+j*width];
            }
        }



    return NULL;
}

int Board::rKillDown(Vector3 pos)
{

    int row;
    int column;

    row = (int)(-pos.y+1.5);
    column = (int)(pos.x+4+0.5);

    if(row == height)
    {
        mSuperBrick->kill();
        return BRICK_SUPERBRICK;
    }


    if(row >= 0 && row < height && column >=0 && column < width && mBricksPtr[column+row*width]!= NULL)
    {
        int t = mBricksPtr[column+row*width]->getType();

        if(t==BRICK_ROCK)
        {
            mBricksPtr[column+row*width]->kick();
            return t;
        }
        else if(t==BRICK_AIR)
        {
            mBricksPtr[column+row*width]->kill();
            return t;

        }
        else if(t==BRICK_HEART)
        {
            mBricksPtr[column+row*width]->kill();
            return t;

        }
        else
        {
            rKill(column,row);
            return t;
        }
    }

    return -2;
}
int Board::rKillLeft(Vector3 pos)
{

    int row;
    int column;

    row = (int)(-pos.y+0.5);
    column = (int)(pos.x+4+0.5)-1;


    if(row >= 0 && row < height && column >=0 && column < width && mBricksPtr[column+row*width]!= NULL)
    {
        int t = mBricksPtr[column+row*width]->getType();

        if(t==BRICK_ROCK)
        {
            mBricksPtr[column+row*width]->kick();
            return t;
        }
        else if(t==BRICK_AIR)
        {
            mBricksPtr[column+row*width]->kill();
            return t;

        }
        else if(t==BRICK_HEART)
        {
            mBricksPtr[column+row*width]->kill();
            return t;

        }
        else
        {
            rKill(column,row);
            return t;
        }
    }

    return -2;
}
int Board::rKillRight(Vector3 pos)
{

    int row;
    int column;

    row = (int)(-pos.y+0.5);
    column = (int)(pos.x+4+0.5)+1;


    if(row >= 0 && row < height && column >=0 && column < width && mBricksPtr[column+row*width]!= NULL)
    {
        int t = mBricksPtr[column+row*width]->getType();

        if(t==BRICK_ROCK)
        {
            mBricksPtr[column+row*width]->kick();
            return t;
        }
        else if(t==BRICK_AIR)
        {
            mBricksPtr[column+row*width]->kill();
            return t;

        }
        else if(t==BRICK_HEART)
        {
            mBricksPtr[column+row*width]->kill();
            return t;

        }
        else
        {
            rKill(column,row);
            return t;
        }
    }

    return -2;
}
int Board::rKillUp(Vector3 pos)
{

    int row;
    int column;

    row = (int)(-pos.y-0.5);
    column = (int)(pos.x+4+0.5);


    if(row >= 0 && row < height && column >=0 && column < width && mBricksPtr[column+row*width]!= NULL)
    {
        int t = mBricksPtr[column+row*width]->getType();

        if(t==BRICK_ROCK)
        {
            mBricksPtr[column+row*width]->kick();
            return t;
        }
        else if(t==BRICK_AIR)
        {
            mBricksPtr[column+row*width]->kill();
            return t;

        }
        else if(t==BRICK_HEART)
        {
            mBricksPtr[column+row*width]->kill();
            return t;

        }
        else
        {
            rKill(column,row);
            return t;
        }
    }

    return -2;
}

void Board::killUpwards(Vector3 pos)
{

    int row;
    int column;

    row = (int)(-pos.y+0.5);
    column = (int)(pos.x+4+0.5);


    if(row >= 0 && row < height && column >=0 && column < width)
    {
        for(int i = row; i >= 0; i--)
        {
            if(mBricksPtr[column+i*width] != NULL)
                mBricksPtr[column+i*width]->kill();
        }
    }

}
