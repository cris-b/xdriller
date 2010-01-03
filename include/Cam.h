#ifndef Cam_H
#define Cam_H

#include <Ogre.h>


#define CMODE_FOLLOW            0
#define CMODE_FOLLOW_LOOK_UP    1
#define CMODE_FOLLOW_ABOVE      2
#define CMODE_FOLLOW_FAR        3




class Cam
{
	public:
	Cam(Ogre::Camera *camera, Ogre::SceneNode *_mCamNode,Ogre::SceneNode *_parentNode);

    void update( unsigned long lTimeElapsed );
	void setMode(int num);
	int  getMode() { return camMode;};



	void move(Ogre::Vector3 v);

	void setParentSceneNode(Ogre::SceneNode *newNode)
	{
		parentNode = newNode;
	};

	Ogre::SceneNode *getParentSceneNode()
	{
		return parentNode;
	};

	Ogre::SceneNode *getSceneNode()
	{
		return mCamNode;
	};

	void setPosition(Ogre::Vector3 pos)
	{
		mCamNode->setPosition(pos);
	};
	void setPosition(Ogre::Real x, Ogre::Real y, Ogre::Real z)
	{
		mCamNode->setPosition(x,y,z);
	};

    Ogre::Vector3 getPosition()
    {
        return mCamNode->getPosition();
    }

	void setParentPos(Ogre::Vector3 pos)
	{
		parentNode->setPosition(pos);
	};

 	void setAngles(float _rx, float _ry)
	{
		rx -= _ry*0.13;
		ry -= _rx*0.13;
	};

	void setDistance(float z)
	{
        this->z = z;
    };

	private:
	Ogre::Camera* mCamera;
	Ogre::SceneNode *mCamNode;
	Ogre::SceneNode *parentNode;

    float rx,ry;
    float z;
	int camMode;
	void limitPitch(float min, float max);


};

#endif

