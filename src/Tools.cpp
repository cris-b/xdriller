#include <Ogre.h>
#include <sys/stat.h>

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#include "windows.h"
#endif

#include "Tools.h"

#include <iostream>



using namespace Ogre;

int copyFile(const std::string& source,const std::string& dest)
{
   std::ifstream ifs(source.c_str(),std::ios::in|std::ios::binary);
   std::ofstream ofs(dest.c_str(),std::ios::out|std::ios::binary);

   if(!ifs)
   {
      std::cerr <<"Can't open " <<  source << std::endl;
      return false;
   }
   if(!ofs)
   {
      std::cerr <<"Can't open " <<  dest << std::endl;
      return false;
   }

   //while(f1 && f1.get(ch) )
   //   f2.put(ch);
   //return false;

   ofs << ifs.rdbuf();

   return true;
}

int makeDirectory(const std::string& filename)
{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    //?
#else
    mkdir(filename.c_str(),0700);
#endif
    return true;
}

bool fileExists(const std::string& filename)
{
  std::fstream fin;

  //this will fail if more capabilities to read the
  //contents of the file is required (e.g. \private\...)
  fin.open(filename.c_str() );

  if(fin.is_open())
    {
    fin.close();
    return true;
    }
  fin.close();

  return false;
}


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
    return (num >= 0.0) ? floor(num + 0.5) : ceil(num - 0.5);
}


void roundColourValue(ColourValue *col)
{
    col->r = goodRound(col->r * 10.0)/10.0;
    col->g = goodRound(col->g * 10.0)/10.0;
    col->b = goodRound(col->b * 10.0)/10.0;
}




