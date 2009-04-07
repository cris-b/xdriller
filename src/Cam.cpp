#include "Cam.h"


using namespace Ogre;

//funciones pa la Cam'ara
Cam::Cam(Camera *camera, SceneNode *_mCamNode,SceneNode *_parentNode)
{
	rx=0;
	ry=0;
	camMode=CMODE_FOLLOW;

	parentNode=_parentNode;
	mCamNode=_mCamNode;
	mCamera = camera;
	//mCamera->setFixedYawAxis(false);

}

void Cam::limitPitch(float min, float max)
{
	if(rx>max) rx=max;
	else if(rx<min) rx=min;
}

void Cam::move(Ogre::Vector3 v)
{
	mCamNode->translate(v, Node::TS_LOCAL);
	//mCamNode->setPosition(mCamNode->getPosition() + v);
}

void Cam::update( unsigned long lTimeElapsed )
{

    if(camMode == CMODE_FOLLOW)
    {

        limitPitch(-80,80);

        mCamNode->lookAt(Vector3(0,parentNode->getPosition().y,parentNode->getPosition().z),Node::TS_WORLD);

        mCamNode->setPosition(0,mCamNode->getPosition().y+
            ((parentNode->getPosition().y-mCamNode->getPosition().y)*0.95)*(lTimeElapsed/1000.0),
            9);
    }
    else if(camMode == CMODE_FOLLOW_LOOK_UP)
    {
        limitPitch(-80,80);

        mCamNode->lookAt(Vector3(0,parentNode->getPosition().y,parentNode->getPosition().z),Node::TS_WORLD);

        mCamNode->setPosition(0,mCamNode->getPosition().y+
            ((parentNode->getPosition().y-3-mCamNode->getPosition().y)*0.95)*(lTimeElapsed/1000.0),
            9);

    }


    //mCamNode->setPosition(parentNode->getPosition());
    //mCamNode->setOrientation(Quaternion(Degree(0),Vector3::UNIT_Y));

    //mCamNode->yaw(Degree(ry), Node::TS_WORLD);
    //mCamNode->pitch(Degree(rx), Node::TS_LOCAL);

    //mCamNode->translate(0.75*Ogre::Vector3::UNIT_Y, Node::TS_WORLD);


}

void Cam::setMode(int num)
{
	camMode = num;

    mCamera->setFOVy(Degree(45));

	mCamera->setOrientation(Ogre::Quaternion::IDENTITY);

}

