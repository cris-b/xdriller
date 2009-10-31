#ifndef GETTEXT_H
#define GETTEXT_H

#include <libintl.h>
#include "Tools.h"

#define _(String) ConvertToUTF( gettext (String) )

#endif
