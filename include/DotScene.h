/*------------------------------------------------------------------------------
*  DotScene.h
*
*  This file provides some methods for the parsing of a .scene file, with
*  support for userData.
*------------------------------------------------------------------------------*/

#ifndef DOT_SCENE_H
#define DOT_SCENE_H

#include <Ogre.h>
#include <vector>

namespace tinyxml2
{
   class XMLElement;
}

class nodeProperty
{
public:
	Ogre::String nodeName;
	Ogre::String propertyNm;
	Ogre::String valueName;
	Ogre::String typeName;

	nodeProperty(Ogre::String node,Ogre::String propertyName,Ogre::String value,Ogre::String type)
	{
		nodeName = node;
		propertyNm = propertyName;
		valueName = value;
		typeName = type;
	}
};

class CDotScene
{
public:
	CDotScene() : mSceneMgr(0) {}
	virtual ~CDotScene() {}

	void parseDotScene( const Ogre::String &SceneName, const Ogre::String& groupName, Ogre::SceneManager *yourSceneMgr, Ogre::SceneNode *pAttachNode = NULL, Ogre::String sPrependNode = "");
	Ogre::String getProperty(Ogre::String ndNm, Ogre::String prop);

	std::vector<nodeProperty> nodeProperties;

protected:
	void processNode(tinyxml2::XMLElement *XMLNode, Ogre::SceneNode *pAttach);
	Ogre::SceneManager *mSceneMgr;
	Ogre::String m_sPrependNode;
};

#endif
