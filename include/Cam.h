#ifndef Cam_H
#define Cam_H

#include <Ogre.h>


#define CMODE_FOLLOW            0
#define CMODE_FOLLOW_LOOK_UP    1
#define CMODE_FOLLOW_ABOVE      2
#define CMODE_FOLLOW_FAR        3



using namespace Ogre;

class Cam
{
	public:
	Cam(Camera *camera, SceneNode *_mCamNode,SceneNode *_parentNode);

    void update( unsigned long lTimeElapsed );
	void setMode(int num);
	int  getMode() { return camMode;};



	void move(Vector3 v);

	void setParentSceneNode(SceneNode *newNode)
	{
		parentNode = newNode;
	};

	SceneNode *getParentSceneNode()
	{
		return parentNode;
	};

	SceneNode *getSceneNode()
	{
		return mCamNode;
	};

	void setPosition(Vector3 pos)
	{
		mCamNode->setPosition(pos);
	};
	void setPosition(Real x, Real y, Real z)
	{
		mCamNode->setPosition(x,y,z);
	};

    Vector3 getPosition()
    {
        return mCamNode->getPosition();
    }

	void setParentPos(Vector3 pos)
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
	Camera* mCamera;
	SceneNode *mCamNode;
	SceneNode *parentNode;

    float rx,ry;
    float z;
	int camMode;
	void limitPitch(float min, float max);


};

#endif

