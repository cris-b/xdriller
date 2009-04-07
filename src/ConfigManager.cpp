#include "ConfigManager.h"


#include "tinyxml.h"

template<> ConfigManager* Ogre::Singleton<ConfigManager>::ms_Singleton = 0;
ConfigManager* ConfigManager::getSingletonPtr(void)
{
    return ms_Singleton;
}
ConfigManager& ConfigManager::getSingleton(void)
{
    assert( ms_Singleton );  return ( *ms_Singleton );
}

ConfigManager::ConfigManager(string filename)
{
    this->filename = filename;

}

void ConfigManager::setValue(string key, string value)
{
	config[ key ] = value;
}

bool ConfigManager::hasKey(string key)
{
	if(config.find( key ) == config.end())
	{
		return false;
	}
	else
	{
		return true;
	}
}

string ConfigManager::getString(string key)
{
	if(config.find( key ) != config.end())
	{

		string s = config[ key ];
		return s;
	}
	else return NULL;
}

int ConfigManager::getInt(string key)
{
	if(config.find( key ) != config.end())
	{

		string s = config[ key ];
		return StringConverter::parseInt(s);
	}
	else return 0;
}

int ConfigManager::save()
{
	TiXmlDocument doc;
	TiXmlElement* key;
	//TiXmlComment * value;
	string s;

 	TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "", "" );
	doc.LinkEndChild( decl );

	TiXmlElement * root = new TiXmlElement("xdriller");
	doc.LinkEndChild( root );

	//comentario el name no es el TiXmlElement* name;
	//name = new TiXmlComment();
	//s=" Settings for IsKaI ";
	//name->SetValue(s.c_str());
	//root->LinkEndChild( name );

	{
		ConfMap::iterator iter;

		TiXmlElement * msgs = new TiXmlElement( "Config" );
		root->LinkEndChild( msgs );

			for (iter=config.begin(); iter != config.end(); iter++)
			{
				const string & mkey=(*iter).first;
				const string & mvalue=(*iter).second;
				key = new TiXmlElement(mkey.c_str());
				key->LinkEndChild( new TiXmlText(mvalue.c_str()));
				msgs->LinkEndChild( key );
			}
	}

	doc.SaveFile(filename.c_str());

	return 0;
}

int ConfigManager::load()
{
	TiXmlDocument doc(filename.c_str());
	if (!doc.LoadFile()) return false;

	TiXmlHandle hDoc(&doc);
	TiXmlElement* pElem;
	TiXmlHandle hRoot(0);

	pElem=hDoc.FirstChildElement().Element();
	hRoot=TiXmlHandle(pElem);

	{
		config.clear();

		pElem=hRoot.FirstChild( "Config" ).FirstChild().Element();
		for( ; pElem; pElem=pElem->NextSiblingElement())
		{
			const char *pKey=pElem->Value();
			const char *pText=pElem->GetText();
			if (pKey && pText)
			{
				config[pKey]=pText;
			}
		}
	}

	return true;
}
