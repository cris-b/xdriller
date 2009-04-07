#include <Ogre.h>

#include "Tools.h"

using namespace Ogre;

void DumpNodes(std::stringstream &ss, Ogre::Node *n, int level)
{
	for(int i = 0; i < level; i++)
	{
		ss << " ";
	}
	ss << "SceneNode: " << n->getName() << std::endl;

	Ogre::SceneNode::ObjectIterator object_it = ((Ogre::SceneNode *)n)->getAttachedObjectIterator();
	Ogre::Node::ChildNodeIterator node_it = n->getChildIterator();

	Ogre::MovableObject *m;
	while(object_it.hasMoreElements())
	{
		for(int i = 0; i < level + 2; i++)
		{
			ss << " ";
		}
		m = object_it.getNext();
		ss << m->getMovableType() << ": " << m->getName() << std::endl;
	}
	while(node_it.hasMoreElements())
	{
		DumpNodes(ss, node_it.getNext(), level + 2);
	}
}


std::string DumpNodes(Ogre::Node *n)
{
	std::stringstream ss;
	ss << std::endl << "Node Hierarchy:" << std::endl;
	DumpNodes(ss, n, 0);
	return ss.str();
}

float goodRound(float num) {
    return (num > 0.0) ? floor(num + 0.5) : ceil(num - 0.5);
}





