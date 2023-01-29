//FI3DOmniverseConnector.h

#pragma once
/*!
* @author	Khang Tran
* @file		FI3DOmniverseConnector.h
* @class	fi3d::FI3DOmniverseConnector
* @brief	The Omniverse Connector for FI3D.
* 
* The Omniverse Connector is the FI3D component used to
* 		1) manage connection to Nucleus Server;
* 		2) communicate with the Nucleus Server;
* 		3) manage file for files in Nucleus Server.
*/

#include <fi3d/FI3D/FI3DComponentRegistration.h>

//#include <QObject>
//or
//#include <QJsonObject>

#include "OmniClient.h"		// Should I include OmniClient.h here or in the .h?

// What else to create for this component?
//#include <fi3d/omni_connector/?????>

namespace fi3d {
class FI3DOmniverseConnector : public QObject hay public ??? {
	Q_OBJECT

	//FI3D_COMPONENT_WITH_GUI(FI3DOmniverseConnector)
	//or
	//FI3D_COMPONENT(FI3DOmniverseConnector)
}	

}