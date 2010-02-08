#ifndef GETTEXT_H
#define GETTEXT_H

#if OGRE_PLATFORM == OGRE_PLATFORM_LINUX

#include <libintl.h>
#include "Tools.h"

//#define _(String) ConvertToUTF( gettext (String) )
#define _(String) gettext(String)

#else

#define _(String) String

#endif // OGRE_PLATFORM == OGRE_PLATFORM_LINUX


#endif // GETTEXT_H
