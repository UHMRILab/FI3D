#pragma once
/*!
* @author	AUTHOR
* @file		ModuleHandlerTEMPLATE.h
* @class	template::ModuleHandlerTEMPLATE
* @brief	Handles the MODULE-NAME (TEMPLATE) module.
*
* TODO: Add information on what this module does.
*/

#include <fi3d/modules/ModuleHandler.h>

#include <fi3d/modules/ModuleRegistration.h>

#include <TEMPLATE/ModuleWindowTEMPLATE.h>

namespace template {
class ModuleHandlerTEMPLATE : public fi3d::ModuleHandler {

	Q_OBJECT
	FI3D_MODULE

private:
	/// @brief The module's GUI.
	ModuleWindowTEMPLATEPtr mGUI;

public:
	/// @brief Constructor.
	ModuleHandlerTEMPLATE(const QString& moduleID);

	/// @brief Destructor.
	~ModuleHandlerTEMPLATE();

private:
	/// @brief Set up all the interactions.
	void setUpInteractions();

	/// @brief Set up all connections between the module elements.
	void setUpConnections();

protected slots:
	/// @brief Loads config file and applies saved defaults.
	void applyConfig();
	
	/// @brief Makes config file and saves to disk.
	void makeConfig();
};

/// @brief Alias for a smart pointer of this class.
using ModuleHandlerTEMPLATEPtr = QSharedPointer<ModuleHandlerTEMPLATE>;

}