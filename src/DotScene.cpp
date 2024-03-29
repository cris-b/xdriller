/*------------------------------------------------------------------------------
*  DotScene.cpp
*
*  This file provides some methods for the parsing of a .scene file, with
*  support for userData.
*------------------------------------------------------------------------------*/

#include "DotScene.h"
//-->
#include <Ogre.h>
//<--

#include <tinyxml2.h>

using namespace std;
using namespace Ogre;

static Light* LoadLight( tinyxml2::XMLElement *XMLLight, SceneManager *mSceneMgr )
{
	tinyxml2::XMLElement *XMLDiffuse, *XMLSpecular, *XMLAttentuation, *XMLPosition;

	// Create a light (point | directional | spot | radPoint)
        String lightName = XMLLight->Attribute("name");
	Light* l = mSceneMgr->createLight( lightName );
        SceneNode *lightNode = mSceneMgr->getRootSceneNode()->createChildSceneNode( lightName + "Node" );
        lightNode->attachObject(l);

	if( !XMLLight->Attribute("type") || String(XMLLight->Attribute("type")) == "point" )
		l->setType( Light::LT_POINT );
	else if( String(XMLLight->Attribute("type")) == "directional")
		l->setType( Light::LT_DIRECTIONAL );
	else if( String(XMLLight->Attribute("type")) == "spot")
		l->setType( Light::LT_SPOTLIGHT );
	else if( String(XMLLight->Attribute("type")) == "radPoint")
		l->setType( Light::LT_POINT );

	XMLDiffuse = XMLLight->FirstChildElement("colourDiffuse");
	if( XMLDiffuse ){
		ColourValue Diffuse;
		Diffuse.r = Ogre::StringConverter::parseReal( XMLDiffuse->Attribute("r") );
		Diffuse.g = Ogre::StringConverter::parseReal( XMLDiffuse->Attribute("g") );
		Diffuse.b = Ogre::StringConverter::parseReal( XMLDiffuse->Attribute("b") );
		Diffuse.a = 1;
		l->setDiffuseColour(Diffuse);
	}
	XMLSpecular = XMLLight->FirstChildElement("colourSpecular");
	if( XMLSpecular ){
		ColourValue Specular;
		Specular.r = Ogre::StringConverter::parseReal( XMLSpecular->Attribute("r") );
		Specular.g = Ogre::StringConverter::parseReal( XMLSpecular->Attribute("g") );
		Specular.b = Ogre::StringConverter::parseReal( XMLSpecular->Attribute("b") );
		Specular.a = 1;
		l->setSpecularColour(Specular);
	}

	XMLAttentuation = XMLLight->FirstChildElement("lightAttenuation");
	if( XMLAttentuation )
	{
		//get defaults incase not all values specified
		Real range, constant, linear, quadratic;
		range = l->getAttenuationRange();
		constant = l->getAttenuationConstant();
		linear = l->getAttenuationLinear();
		quadratic = l->getAttenuationQuadric();

		if( XMLAttentuation->Attribute("range") )
			range = StringConverter::parseReal( XMLAttentuation->Attribute("range") );
		if( XMLAttentuation->Attribute("constant") )
			constant = StringConverter::parseReal( XMLAttentuation->Attribute("constant") );
		if( XMLAttentuation->Attribute("linear") )
			linear = StringConverter::parseReal( XMLAttentuation->Attribute("linear") );
		if( XMLAttentuation->Attribute("quadratic") )
			quadratic = StringConverter::parseReal( XMLAttentuation->Attribute("quadratic") );

		l->setAttenuation( range, constant, linear, quadratic );
	}

	XMLPosition = XMLLight->FirstChildElement("position");
	if( XMLPosition )
	{
        Vector3 p = Vector3(0,0,0);
        if( XMLPosition->Attribute("x") )
            p.x = StringConverter::parseReal( XMLPosition->Attribute("x") );
        if( XMLPosition->Attribute("y") )
            p.y = StringConverter::parseReal( XMLPosition->Attribute("y") );
        if( XMLPosition->Attribute("z") )
            p.z = StringConverter::parseReal( XMLPosition->Attribute("z") );


        //NOTA:: esto no funciona porque se pone primero la posicion antes que el tipo, ver desert.xml

	    if(l->getType() == Light::LT_DIRECTIONAL)
	    {
	        lightNode->setDirection( p );
	    }
	    else
	    {
            lightNode->setPosition( p );
	    }
	}

	//castShadows      (true | false) "true"
	l->setCastShadows( true );
	if( XMLLight->Attribute("castShadows") )
		if( String(XMLLight->Attribute("castShadows")) == "false" )
			l->setCastShadows( false );

	//visible         (true | false) "true"
	l->setVisible( true );
	if( XMLLight->Attribute("visible") )
		if( String(XMLLight->Attribute("visible")) == "false" )
			l->setVisible( false );

	return l;
}

void CDotScene::parseDotScene( const String &SceneName, const String& groupName, SceneManager *yourSceneMgr, SceneNode *pAttachNode, String sPrependNode)
{
	// set up shared object values
	m_sPrependNode = sPrependNode;
	mSceneMgr = yourSceneMgr;

	tinyxml2::XMLDocument   *XMLDoc = NULL;
	tinyxml2::XMLElement   *XMLRoot, *XMLNodes;

	try
	{
		DataStreamPtr pStream = ResourceGroupManager::getSingleton().
			openResource( SceneName, groupName );

		String data = pStream->getAsString();
		// Open the .scene File
		XMLDoc = new tinyxml2::XMLDocument();
		XMLDoc->Parse( data.c_str() );

		//siguientes lineas comentadas pa evitar error si dentro de zip
		//
		//pStream->close();
		//pStream.setNull();

		if( XMLDoc->Error() )
		{
			//We'll just log, and continue on gracefully
			LogManager::getSingleton().logMessage("[dotSceneLoader] The tinyxml2::XMLDocument reported an error");
			delete XMLDoc;
			return;
		}
	}
	catch(...)
	{
		//We'll just log, and continue on gracefully
		LogManager::getSingleton().logMessage("[dotSceneLoader] Error creating tinyxml2::XMLDocument");
		delete XMLDoc;
		return;
	}

	// Validate the File
	XMLRoot = XMLDoc->RootElement();
	if( String( XMLRoot->Value()) != "scene"  ) {
		LogManager::getSingleton().logMessage( "[dotSceneLoader]Error: Invalid .scene File. Missing <scene>" );
		delete XMLDoc;
		return;
	}

	// figure out where to attach any nodes we create
	if(pAttachNode == NULL)
	{
		pAttachNode = mSceneMgr->getRootSceneNode();
	}

	XMLNodes = XMLRoot->FirstChildElement( "nodes" );

	// Read in the scene nodes
	if( XMLNodes )
	{
		processNode(XMLNodes->FirstChildElement( "node" ), pAttachNode);
	}

	// Close the XML File
	delete XMLDoc;
}

void CDotScene::processNode(tinyxml2::XMLElement *XMLNode, SceneNode *pAttach)
{
	tinyxml2::XMLElement *XMLPosition, *XMLRotation, *XMLScale,  *XMLEntity,  *XMLSubEntity, *XMLBillboardSet,  *XMLLight, *XMLUserData;

	while( XMLNode )
	{
		// Process the current node
		// Grab the name of the node
		String NodeName = XMLNode->Attribute("name");
		// First create the new scene node
		SceneNode* NewNode = pAttach->createChildSceneNode( m_sPrependNode + NodeName );
		Vector3 TempVec;
		String TempValue;

		// Now position it...
		XMLPosition = XMLNode->FirstChildElement("position");
		if( XMLPosition ){
			TempValue = XMLPosition->Attribute("x");
			TempVec.x = StringConverter::parseReal(TempValue);
			TempValue = XMLPosition->Attribute("y");
			TempVec.y = StringConverter::parseReal(TempValue);
			TempValue = XMLPosition->Attribute("z");
			TempVec.z = StringConverter::parseReal(TempValue);
			NewNode->setPosition( TempVec );
		}

		// Rotate it...
		XMLRotation = XMLNode->FirstChildElement("rotation");
		if( XMLRotation ){
			Quaternion TempQuat;
			TempValue = XMLRotation->Attribute("qx");
			TempQuat.x = StringConverter::parseReal(TempValue);
			TempValue = XMLRotation->Attribute("qy");
			TempQuat.y = StringConverter::parseReal(TempValue);
			TempValue = XMLRotation->Attribute("qz");
			TempQuat.z = StringConverter::parseReal(TempValue);
			TempValue = XMLRotation->Attribute("qw");
			TempQuat.w = StringConverter::parseReal(TempValue);
			NewNode->setOrientation( TempQuat );
		}

		// Scale it.
		XMLScale = XMLNode->FirstChildElement("scale");
		if( XMLScale ){
			TempValue = XMLScale->Attribute("x");
			TempVec.x = StringConverter::parseReal(TempValue);
			TempValue = XMLScale->Attribute("y");
			TempVec.y = StringConverter::parseReal(TempValue);
			TempValue = XMLScale->Attribute("z");
			TempVec.z = StringConverter::parseReal(TempValue);
			NewNode->setScale( TempVec );
		}

		XMLLight = XMLNode->FirstChildElement( "light" );
		if( XMLLight )
			NewNode->attachObject( LoadLight( XMLLight, mSceneMgr ) );

		// Check for an Entity
		XMLEntity = XMLNode->FirstChildElement("entity");
		if( XMLEntity )
		{
			String EntityName, EntityMeshFilename;
			EntityName = XMLEntity->Attribute( "name" );
			EntityMeshFilename = XMLEntity->Attribute( "meshFile" );

			// Create entity
			Entity* NewEntity = mSceneMgr->createEntity(EntityName, EntityMeshFilename);

                        //castShadows      (true | false) "true"
                        NewEntity->setCastShadows( false );
                        if( XMLEntity->Attribute("castShadows") )
                           if( String(XMLEntity->Attribute("castShadows")) == "true" )
                              NewEntity->setCastShadows( true );

			// Go though sub entities
			XMLSubEntity = XMLEntity->FirstChildElement( "subentities" );

			if( XMLSubEntity ) {
				XMLSubEntity = XMLSubEntity->FirstChildElement( "subentity" );

				while( XMLSubEntity ) {
					Ogre::SubEntity* subEntity;
					Ogre::uint subEntityIndex;

					subEntityIndex = Ogre::StringConverter::parseInt( XMLSubEntity->Attribute( "index" ) );
					subEntity = NewEntity->getSubEntity( subEntityIndex );

					if( XMLSubEntity->Attribute( "materialName" ) )
						subEntity->setMaterialName( XMLSubEntity->Attribute( "materialName" ) );

					XMLSubEntity= XMLSubEntity->NextSiblingElement( "subentity" );
				}
			}


                        NewNode->attachObject( NewEntity );
		}

		XMLBillboardSet = XMLNode->FirstChildElement( "billboardSet" );
		if( XMLBillboardSet )
		{
			String TempValue;

			BillboardSet* bSet = mSceneMgr->createBillboardSet( NewNode->getName() );

			BillboardType Type;
			TempValue = XMLBillboardSet->Attribute( "type" );
			if( TempValue == "orientedCommon" )
				Type = BBT_ORIENTED_COMMON;
			else if( TempValue == "orientedSelf" )
				Type = BBT_ORIENTED_SELF;
			else Type = BBT_POINT;

			BillboardOrigin Origin;
			TempValue = XMLBillboardSet->Attribute( "type" );
			if( TempValue == "bottom_left" )
				Origin = BBO_BOTTOM_LEFT;
			else if( TempValue == "bottom_center" )
				Origin = BBO_BOTTOM_CENTER;
			else if( TempValue == "bottomRight"  )
				Origin = BBO_BOTTOM_RIGHT;
			else if( TempValue == "left" )
				Origin = BBO_CENTER_LEFT;
			else if( TempValue == "right" )
				Origin = BBO_CENTER_RIGHT;
			else if( TempValue == "topLeft" )
				Origin = BBO_TOP_LEFT;
			else if( TempValue == "topCenter" )
				Origin = BBO_TOP_CENTER;
			else if( TempValue == "topRight" )
				Origin = BBO_TOP_RIGHT;
			else
				Origin = BBO_CENTER;

			bSet->setBillboardType( Type );
			bSet->setBillboardOrigin( Origin );


			TempValue = XMLBillboardSet->Attribute( "name" );
			bSet->setMaterialName( TempValue );

			int width, height;
			width = (int) StringConverter::parseReal( XMLBillboardSet->Attribute( "width" ) );
			height = (int) StringConverter::parseReal( XMLBillboardSet->Attribute( "height" ) );
			bSet->setDefaultDimensions( width, height );
			bSet->setVisible( true );
			NewNode->attachObject( bSet );

			tinyxml2::XMLElement *XMLBillboard;

			XMLBillboard = XMLBillboardSet->FirstChildElement( "billboard" );

			while( XMLBillboard )
			{
				// TempValue;
				TempVec = Vector3( 0, 0, 0 );
				ColourValue TempColour(1,1,1,1);

				XMLPosition = XMLBillboard->FirstChildElement( "position" );
				if( XMLPosition ){
					TempValue = XMLPosition->Attribute("x");
					TempVec.x = StringConverter::parseReal(TempValue);
					TempValue = XMLPosition->Attribute("y");
					TempVec.y = StringConverter::parseReal(TempValue);
					TempValue = XMLPosition->Attribute("z");
					TempVec.z = StringConverter::parseReal(TempValue);
				}

				tinyxml2::XMLElement* XMLColour = XMLBillboard->FirstChildElement( "colourDiffuse" );
				if( XMLColour ){
					TempValue = XMLColour->Attribute("r");
					TempColour.r = StringConverter::parseReal(TempValue);
					TempValue = XMLColour->Attribute("g");
					TempColour.g = StringConverter::parseReal(TempValue);
					TempValue = XMLColour->Attribute("b");
					TempColour.b = StringConverter::parseReal(TempValue);
				}

				bSet->createBillboard( TempVec, TempColour);

				XMLBillboard = XMLBillboard->NextSiblingElement( "billboard" );
			}
		}

		XMLUserData = XMLNode->FirstChildElement( "userData" );
		if ( XMLUserData )
		{
			tinyxml2::XMLElement *XMLProperty;
			XMLProperty = XMLUserData->FirstChildElement("property");
			while ( XMLProperty )
			{
				String first = NewNode->getName();
				String second = XMLProperty->Attribute("name");
				String third = XMLProperty->Attribute("data");
				String type = XMLProperty->Attribute("type");
				nodeProperty newProp(first,second,third,type);
				nodeProperties.push_back(newProp);
				XMLProperty = XMLProperty->NextSiblingElement("property");
			}
		}
//-->		/*
		/*		Added by @lpha_Max ! :-)
		*		"Global" vars :
		*			NewNode --> Node where attach the object...
		*			mSceneMgr --> SceneManager
		*/
		tinyxml2::XMLElement* XMLCamera = XMLNode->FirstChildElement( "camera" );
		if ( XMLCamera )
		{
			String CameraName = XMLCamera->Attribute( "name" );
			Camera *cam = mSceneMgr->createCamera(CameraName);
			NewNode->attachObject(cam);
		}

		tinyxml2::XMLElement* XMLParticle = XMLNode->FirstChildElement( "particleSystem" );
		if ( XMLParticle )
		{
			String ParticleName = XMLParticle->Attribute( "name" );
			String ParticleFile = XMLParticle->Attribute( "file" );
			ParticleSystem* ParticleS = mSceneMgr->createParticleSystem(ParticleName, ParticleFile);
			NewNode->attachObject(ParticleS);
                }
//<--
		tinyxml2::XMLElement * ChildXMLNode;
		ChildXMLNode = XMLNode->FirstChildElement( "node" );
		if(ChildXMLNode)
			processNode(ChildXMLNode, NewNode);	// recurse to do all my children

		XMLNode = XMLNode->NextSiblingElement( "node" ); // process my next sibling
	}
}

String CDotScene::getProperty(String ndNm, String prop)
{
	for ( unsigned int i = 0 ; i < nodeProperties.size(); i++ )
	{
		if ( nodeProperties[i].nodeName == ndNm && nodeProperties[i].propertyNm == prop )
		{
			return nodeProperties[i].valueName;
		}
	}
	return " ";
}
