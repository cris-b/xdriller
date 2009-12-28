
#ifndef TOOLS_H
#define TOOLS_H

#include <Ogre.h>

using namespace Ogre;

std::string DumpNodes(Ogre::Node *n);

float goodRound(float num);

bool fileExists(const std::string& filename);

int makeDirectory(const std::string& filename);

int copyFile(const std::string& source,const std::string& dest);

void roundColourValue(ColourValue *col);

Vector2 worldToScreen(const Vector3& worldPoint, Camera* cam);

std::string findAndReplace(std::string str, const std::string& findStr, const std::string& replaceStr);

Ogre::UTFString ConvertToUTF(Ogre::String String);

Ogre::Real getStringWidth(Ogre::String s, Ogre::String fontName, Ogre::Real charHeight);

Ogre::String secsToClockString(int s);

#endif
