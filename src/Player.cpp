#include "Player.h"
#include "Tools.h"
#include "SoundManager.h"


#define FALL_SPEED  -0.004
#define FALL_ACC    -0.0001
#define H_SPEED      0.004
#define H_SPEED_FALL 0.002
#define H_ACC        0.0005
#define H_FRICTION   0.0001
#define BORED_TIME   6000

#define MAX_LIVES    10
#define ROCK_BREAK_AIR 0.05

#define AIR_FULL_USE_TIME   100000.0

Player::Player(Board *mBoard)
{
    this->mBoard = mBoard;

    mSceneMgr = Root::getSingletonPtr()->getSceneManager( "ST_GENERIC" );

    alive = true;
    finished = false;
    air = 1;
    idleTime = 0;
    fallTime = 1000;
    lives = 3;
    depth = 0;
    lastDepth = 0;
    orientation = LOOK_DOWN;
    scale = 1;
    endFloor = false;
    points = 0;

    Vector3 startPos(0,2,0);
    speed = Vector3(0,0,0);
    orientationAngle = 0;

    mEnt = mSceneMgr->createEntity("player", "tux.mesh");
    mEnt->setMaterialName("tux");


    mNode = mSceneMgr->getRootSceneNode()->createChildSceneNode( "player" , startPos);
    mScaleNode = mNode->createChildSceneNode( "player_scale_node");
    mScaleNode->attachObject(mEnt);

    Vector3 pos = mNode->getPosition();



    mBox = AxisAlignedBox(Vector3(pos.x-0.5,pos.y-0.5,pos.z-0.5),
                          Vector3(pos.x+0.5,pos.y+0.5,pos.z+0.5));




    mAnimationState = mEnt->getAnimationState("Idle");
    mAnimationState->setLoop(true);
    mAnimationState->setEnabled(true);

    starsParticle = mSceneMgr->createParticleSystem("StarsParticle", "Stars");
    starsParticle->getEmitter(0)->setEnabled(false);

    plus20Particle = mSceneMgr->createParticleSystem("Plus20Particle", "Plus20");
    plus20Particle->getEmitter(0)->setEnabled(false);

    heartsParticle = mSceneMgr->createParticleSystem("HeartsParticle", "Hearts");
    heartsParticle->getEmitter(0)->setEnabled(false);

    mNode->attachObject(starsParticle);
    mNode->attachObject(plus20Particle);
    mNode->attachObject(heartsParticle);



}

Player::~Player()
{

    mScaleNode->detachAllObjects();
    mNode->detachAllObjects();
	mNode->removeAndDestroyChild(mScaleNode->getName());
	mNode->getParentSceneNode()->removeAndDestroyChild(mNode->getName());

	if (mEnt)
		mSceneMgr->destroyEntity(mEnt);
    if (starsParticle)
        mSceneMgr->destroyParticleSystem(starsParticle);
    if (plus20Particle)
        mSceneMgr->destroyParticleSystem(plus20Particle);

}

void Player::update(unsigned long lTimeElapsed)
{

    //cubo para colisionar arriba
    AxisAlignedBox tmpBox = AxisAlignedBox(getPosition().x-0.01,getPosition().y-0.0,getPosition().z-0.5,
                                           getPosition().x+0.01,getPosition().y+0.3,getPosition().z+0.5);

    Brick *colBrick = mBoard->detectCollision(tmpBox);
    if(colBrick != NULL && colBrick->getType() == BRICK_AIR)
    {
        colBrick->kill();
        setAir(getAir()+0.2);
    }
    else if(colBrick != NULL && colBrick->getType() == BRICK_HEART)
    {
        colBrick->kill();
        livesUp(1);
    }
    else if(colBrick != NULL && _falling == false)
    {
        speed.y = 0;
        Real tmpScale = fabs(colBrick->getPosition().y-mNode->getPosition().y);

        if(tmpScale < scale) scale = tmpScale;

    }

    if(scale < 1 && alive)  //aki es donde muere aplastado
    {

        alive = false;
        lives --;
        speed.x=0;

        SoundManager::getSingleton().playSound(SOUND_SQUASH);
    }


    if(alive)
    {
        //Orienta al amigo

        mNode->resetOrientation();

        mNode->rotate(Quaternion(Degree(-90),Vector3::UNIT_X));

        if(orientation == LOOK_LEFT)
        {
            if(orientationAngle > -90)
            {
                orientationAngle -= lTimeElapsed*2;
                if(orientationAngle < -90) orientationAngle = -90;
            }

        }
        else if(orientation == LOOK_RIGHT)
        {
            if(orientationAngle < 90)
            {
                orientationAngle += lTimeElapsed*2;
                if(orientationAngle > 90) orientationAngle = 90;
            }

        }
        else
        {
            if(orientationAngle > 0)
            {
                orientationAngle -= lTimeElapsed*2;
                if(orientationAngle < 0) orientationAngle = 0;
            }
            if(orientationAngle < 0)
            {
                orientationAngle += lTimeElapsed*2;
                if(orientationAngle > 0) orientationAngle = 0;
            }
        }
        mNode->rotate(Quaternion(Degree(orientationAngle),Vector3::UNIT_Z));
    }

    if(alive && !finished)
    {

        //Friccion horizontal

        if(speed.x > 0)
        {
            speed.x -= H_FRICTION*lTimeElapsed;
            if(speed.x < 0) speed.x = 0;
        }

        if(speed.x < 0)
        {
            speed.x += H_FRICTION*lTimeElapsed;
            if(speed.x > 0) speed.x = 0;
        }

        //caida

        speed.y += FALL_ACC*lTimeElapsed;
        if(speed.y < FALL_SPEED) speed.y=FALL_SPEED;


        //cubo para colisionar abajo
        tmpBox.setExtents(getPosition().x-0.4,getPosition().y-0.5+speed.y*lTimeElapsed,getPosition().z-0.5,
                          getPosition().x+0.4,getPosition().y+0.0+speed.y*lTimeElapsed,getPosition().z+0.5);

        colBrick = mBoard->detectCollision(tmpBox);

        if(colBrick != NULL && colBrick->getType() == BRICK_AIR)
        {
            colBrick->kill();
            setAir(getAir()+0.2);
            if(_falling == false)
            {
                _falling = true;

            }
        }
        else if(colBrick != NULL && colBrick->getType() == BRICK_HEART)
        {
            colBrick->kill();
            livesUp(1);
            if(_falling == false)
            {
                _falling = true;

            }
        }
        else if(colBrick != NULL)
        {

            if(_falling == true)
            {
                _falling = false;
                if(fallTime > 700) SoundManager::getSingleton().stopSound(SOUND_FALLING);
                fallTime = 0;
            }
            speed.y = 0;
            mNode->setPosition(Vector3(getPosition().x,colBrick->getPosition().y+1.0001,0));
        }
        else if(getPosition().y+speed.y*lTimeElapsed < -(mBoard->getHeight()-1) && mBoard->mSuperBrick->isAlive())
        {

            if(_falling == true)
            {
                _falling = false;
                if(fallTime > 700) SoundManager::getSingleton().stopSound(SOUND_FALLING);
                fallTime = 0;
            }
            speed.y = 0;
            mNode->setPosition(Vector3(getPosition().x,-(mBoard->getHeight()-1),0));

        }
        else
        {
            if(_falling == false)
            {
                _falling = true;
            }
        }

        if(_moveLeft)
        {
            speed.x += -H_ACC*lTimeElapsed;

            if(speed.x < -H_SPEED && !_falling) speed.x=-H_SPEED;
            if(speed.x < -H_SPEED &&  _falling) speed.x=-H_SPEED_FALL;

        }
        //cubo para colisionar izquierda
        tmpBox.setExtents(getPosition().x-0.4+speed.x*lTimeElapsed,getPosition().y-0.5,getPosition().z-0.5,
                          getPosition().x+0.0+speed.x*lTimeElapsed,getPosition().y+0.1,getPosition().z+0.5);

        colBrick = mBoard->detectCollision(tmpBox);

        if(colBrick != NULL && colBrick->getType() == BRICK_AIR)
        {
            colBrick->kill();
            setAir(getAir()+0.2);
        }
        else if(colBrick != NULL && colBrick->getType() == BRICK_HEART)
        {
            colBrick->kill();
            livesUp(1);
        }
        else if(colBrick != NULL)
        {
            speed.x = 0;
            mNode->setPosition(Vector3(colBrick->getPosition().x+0.9001,getPosition().y,0));
        }

        if(getPosition().x+speed.x<-4)
        {
            speed.x=0;
            mNode->setPosition(Vector3(-3.999,getPosition().y,0));
        }

        if(_moveRight)
        {
            speed.x += +H_ACC*lTimeElapsed;

            if(speed.x > +H_SPEED && !_falling) speed.x=+H_SPEED;
            if(speed.x > +H_SPEED &&  _falling) speed.x=+H_SPEED_FALL;


        }

        //cubo para colisionar derecha
        tmpBox.setExtents(getPosition().x-0.0+speed.x*lTimeElapsed,getPosition().y-0.5,getPosition().z-0.5,
                          getPosition().x+0.4+speed.x*lTimeElapsed,getPosition().y+0.1,getPosition().z+0.5);

        colBrick = mBoard->detectCollision(tmpBox);


        if(colBrick != NULL && colBrick->getType() == BRICK_AIR)
        {
            colBrick->kill();
            setAir(getAir()+0.2);
        }
        else if(colBrick != NULL && colBrick->getType() == BRICK_HEART)
        {
            colBrick->kill();
            livesUp(1);
        }
        else if(colBrick != NULL)
        {
            speed.x = 0;
            mNode->setPosition(Vector3(colBrick->getPosition().x-0.9001,getPosition().y,0));
        }
        if(getPosition().x+speed.x>mBoard->getWidth()-5)
        {
            speed.x=0;
            mNode->setPosition(Vector3(mBoard->getWidth()-5.001,getPosition().y,0));
        }

        if(_falling)
        {
            fallTime+=lTimeElapsed;
        }
    }
    else if (!alive && !finished)
    {
        //gravedad

        speed.y += FALL_ACC*lTimeElapsed;
        if(speed.y < FALL_SPEED) speed.y=FALL_SPEED;

        //cubo para colisionar abajo
        tmpBox.setExtents(getPosition().x-0.4,getPosition().y-0.5+speed.y*lTimeElapsed,getPosition().z-0.5,
                          getPosition().x+0.4,getPosition().y+0.0+speed.y*lTimeElapsed,getPosition().z+0.5);

        colBrick = mBoard->detectCollision(tmpBox);

        if(colBrick != NULL)
        {
            speed.y = 0;
            _falling = false;
            fallTime = 0;

            //desactiva el posicionamiento del monigote porque con un
            //bloque encima lo mandaria pa'arriba
            //mNode->setPosition(Vector3(getPosition().x,colBrick->getPosition().y+1.0001,0));
        }

        else if(-getPosition().y > mBoard->getHeight()-1 && mBoard->mSuperBrick->isAlive())
        {
            speed.y = 0;
            _falling = false;
            fallTime = 0;

        }
        else if(_falling == false)
        {
            _falling = true;

        };
    }
    //else //alive && finished
    //{
    //}

    if(endFloor)
    {
        if(mNode->getPosition().y < endFloorPosY+1)
        {
            mNode->setPosition(mNode->getPosition().x,endFloorPosY+1,mNode->getPosition().z);
            finished = true;
            speed.x = 0;
            speed.y = 0;
            _falling=false;
            fallTime=0;
            orientation = LOOK_DOWN;
        }
    }

    if(scale < 1)
    {
        mScaleNode->setScale(1,1,scale);
        mScaleNode->setPosition(0,0,-(1-scale)/2.0);
    }

    if(!alive && mAnimationState->getAnimationName() != "Dye")
    {
        mAnimationState->setEnabled(false);
    }
    else if(mAnimationState->getAnimationName() == "Walk")
    {
        if(fabs(speed.x) < 0.001)
        {

            mAnimationState->setEnabled(false);
            mAnimationState = mEnt->getAnimationState("Idle");
            mAnimationState->setLoop(true);
            mAnimationState->setEnabled(true);
            mAnimationState->setTimePosition(0);

        }

    }
    else if(mAnimationState->getAnimationName() == "Break_up" ||
            mAnimationState->getAnimationName() == "Break_down" ||
            mAnimationState->getAnimationName() == "Front_break" ||
            mAnimationState->getAnimationName() == "Bored_1" ||
            mAnimationState->getAnimationName() == "Bored_2" ||
            mAnimationState->getAnimationName() == "Bored_3")
    {
        if(mAnimationState->hasEnded())
        {
            mAnimationState->setEnabled(false);
            mAnimationState = mEnt->getAnimationState("Idle");
            mAnimationState->setLoop(true);
            mAnimationState->setEnabled(true);
            mAnimationState->setTimePosition(0);

        }
    }
    else if(mAnimationState->getAnimationName() == "Idle" && fallTime > 700)
    {
        mAnimationState->setEnabled(false);
        mAnimationState = mEnt->getAnimationState("Falling");
        mAnimationState->setLoop(true);
        mAnimationState->setEnabled(true);
        mAnimationState->setTimePosition(0);

        SoundManager::getSingleton().playSound(SOUND_FALLING);
    }
    else if(mAnimationState->getAnimationName() == "Idle")
    {
        idleTime += lTimeElapsed;

        if(idleTime > BORED_TIME)
        {
            int oneOfThree = rand() % 3;

            mAnimationState->setEnabled(false);
            if(oneOfThree == 0) mAnimationState = mEnt->getAnimationState("Bored_1");
            if(oneOfThree == 1) mAnimationState = mEnt->getAnimationState("Bored_2");
            if(oneOfThree == 2) mAnimationState = mEnt->getAnimationState("Bored_3");
            mAnimationState->setLoop(false);
            mAnimationState->setEnabled(true);
            mAnimationState->setTimePosition(0);

            idleTime = 0;
        }
        if(fabs(speed.x) > 0 && !_falling)
        {

            mAnimationState->setEnabled(false);
            mAnimationState = mEnt->getAnimationState("Walk");
            mAnimationState->setLoop(true);
            mAnimationState->setEnabled(true);
            mAnimationState->setTimePosition(0);
            idleTime = 0;

        }
    }
    else if(mAnimationState->getAnimationName() == "Falling" && !_falling)
    {
        mAnimationState->setEnabled(false);
        mAnimationState = mEnt->getAnimationState("Idle");
        mAnimationState->setLoop(true);
        mAnimationState->setEnabled(true);
        mAnimationState->setTimePosition(0);
    }

    mNode->setPosition(getPosition()+speed*lTimeElapsed);


    mBox.setExtents(getPosition().x-0.4,getPosition().y-0.5,getPosition().z-0.5,
                     getPosition().x+0.4,getPosition().y+0.4,getPosition().z+0.5);


    _moveLeft = false;
    _moveRight = false;
    _moveJump = false;

    mAnimationState->addTime(0.001*lTimeElapsed);

    if(!finished)
    {
        lastDepth = depth;
        depth = (int)-mNode->getPosition().y+1;
        if(depth < 0) depth = 0;

        setAir(air-lTimeElapsed/AIR_FULL_USE_TIME);
    }

    if(air <= 0 && alive)
    {
        alive=false;
        lives --;

        speed.x = 0;

        orientation = LOOK_DOWN;

        mAnimationState->setEnabled(false);
        mAnimationState = mEnt->getAnimationState("Dye");
        mAnimationState->setLoop(false);
        mAnimationState->setEnabled(true);
        mAnimationState->setTimePosition(0);

    }

}


void Player::moveLeft()
{
    if(alive && !finished)
    {
        _moveLeft = true;
        orientation = LOOK_LEFT;
        mEnt->setMaterialName("tux");
    }
}

void Player::moveRight()
{
    if(alive && !finished)
    {
    _moveRight = true;
    orientation = LOOK_RIGHT;
    mEnt->setMaterialName("tux");
    }
}

void Player::moveUp()
{
    if(alive && !finished)
    {
    orientation = LOOK_UP;
    mEnt->setMaterialName("tux_look_up");
    }
}

void Player::moveDown()
{
    if(alive && !finished)
    {
    orientation = LOOK_DOWN;
    mEnt->setMaterialName("tux");
    }
}

void  Player::breakBlock()
{
    if(alive && !finished)
    {
        if(orientation==LOOK_LEFT)
        {
            if(fabs(goodRound(getPosition().x)-getPosition().x) < 0.2 )
            {
                int t = mBoard->rKillLeft(getPosition());
                if( t == BRICK_AIR ) setAir(getAir()+0.2);
                else if( t == BRICK_ROCK ) setAir(getAir()-ROCK_BREAK_AIR);
                else if( t == BRICK_HEART ) livesUp(1);

            }
            mAnimationState->setEnabled(false);
            mAnimationState = mEnt->getAnimationState("Front_break");
            mAnimationState->setLoop(false);
            mAnimationState->setEnabled(true);
            mAnimationState->setTimePosition(0);
            idleTime = 0;

        }
        else if(orientation==LOOK_RIGHT)
        {
            if(fabs(goodRound(getPosition().x)-getPosition().x) < 0.2 )
            {
                int t = mBoard->rKillRight(getPosition());
                if( t == BRICK_AIR ) setAir(getAir()+0.2);
                else if( t == BRICK_ROCK ) setAir(getAir()-ROCK_BREAK_AIR);
                else if( t == BRICK_HEART ) livesUp(1);

            }
            mAnimationState->setEnabled(false);
            mAnimationState = mEnt->getAnimationState("Front_break");
            mAnimationState->setLoop(false);
            mAnimationState->setEnabled(true);
            mAnimationState->setTimePosition(0);
            idleTime = 0;
        }
        else if(orientation==LOOK_UP)
        {
            {
                int t = mBoard->rKillUp(getPosition());
                if( t == BRICK_AIR ) setAir(getAir()+0.2);
                else if( t == BRICK_ROCK ) setAir(getAir()-ROCK_BREAK_AIR);
                else if( t == BRICK_HEART ) livesUp(1);
            }
            mAnimationState->setEnabled(false);
            mAnimationState = mEnt->getAnimationState("Break_up");
            mAnimationState->setLoop(false);
            mAnimationState->setEnabled(true);
            mAnimationState->setTimePosition(0);
            idleTime = 0;
        }
        else if(orientation==LOOK_DOWN)
        {
            {
                int t = mBoard->rKillDown(getPosition());
                if( t == BRICK_AIR ) setAir(getAir()+0.2);
                else if( t == BRICK_ROCK ) setAir(getAir()-ROCK_BREAK_AIR);
                else if( t == BRICK_HEART ) livesUp(1);
            }
            mAnimationState->setEnabled(false);
            mAnimationState = mEnt->getAnimationState("Break_down");
            mAnimationState->setLoop(false);
            mAnimationState->setEnabled(true);
            mAnimationState->setTimePosition(0);
            idleTime = 0;
        }
    }
}

void Player::setAir(Real air)
{
    if(air > this->air)
    {
        plus20Particle->getEmitter(0)->setEnabled(true);
        points++;
    }

    this->air = air;

    if(this->air<0) this->air = 0;
    if(this->air>1) this->air = 1;


}

void Player::resurrect()
{
    if(lives<=0)
    {
        finished = true;
        return;
    }
    alive = true;
    mScaleNode->setScale(1,1,1);
    mScaleNode->setPosition(0,0,0);
    scale = 1;
    air = 1;
    orientation = LOOK_DOWN;

    starsParticle->getEmitter(0)->setEnabled(true);

    //redeondea la posicion a la posicion entera mas cercana
    Vector3 pos = mNode->getPosition();
    mNode->setPosition(goodRound(pos.x),goodRound(pos.y),goodRound(pos.z));

    SoundManager::getSingleton().playSound(SOUND_RESURRECT);

    mAnimationState->setEnabled(false);
    mAnimationState = mEnt->getAnimationState("Idle");
    mAnimationState->setLoop(true);
    mAnimationState->setEnabled(true);
    mAnimationState->setTimePosition(0);

}

void Player::setEndFloor()
{
    endFloor = true;
    endFloorPosY = mNode->getPosition().y - 10.0;
}

void Player::livesUp(int n)
{
    lives += n;
    if(lives >= MAX_LIVES) lives = MAX_LIVES;

    heartsParticle->getEmitter(0)->setEnabled(true);
}
