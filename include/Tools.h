
#ifndef Tools_H
#define Tools_H

#include <Ogre.h>




//Devuelve el arbol de nodos de la scena
std::string DumpNodes(Ogre::Node *n);

//Redondea bien los incluso los numeros negativos
float goodRound(float num);

//Devuelve "true" si el archivo existe
bool fileExists(const std::string& filename);

//crea directorio, siempre devuelve "true"
int makeDirectory(const std::string& filename);

//Copia archivo de "source" a "dest"
int copyFile(const std::string& source,const std::string& dest);

//no me acuerdo pa que era esto...
void roundColourValue(Ogre::ColourValue *col);

// Converts 3D world position to screen coordinate
Ogre::Vector2 worldToScreen(const Ogre::Vector3& worldPoint, Ogre::Camera* cam);

//No funciona en windows???
std::string findAndReplace(std::string str, const std::string& findStr, const std::string& replaceStr);

//Convierte cadena de texto a UTF
Ogre::UTFString ConvertToUTF(Ogre::String String);

//Devuelve lo que ocupa una cadena de texto en la patalla (para overlays)
Ogre::Real getStringWidth(Ogre::String s, Ogre::String fontName, Ogre::Real charHeight);

//Por ejemplo, convierte 61 a "1:01"
Ogre::String secsToClockString(int s);



#endif
