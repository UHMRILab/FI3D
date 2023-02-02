#pragma once
/*!
* @author	AUTHOR
* @file		ComponentHandlerTEMPLATE.h
* @class	template::ComponentHandlerTEMPLATE
* @brief	Handles the COMPONENT-NAME (TEMPLATE) component.
*
* TODO: Add information on what this component does.
*/

// @todo: Needing more fixes for this template file.

#include <fi3d/components/ComponentHandler.h>

#include <fi3d/components/ComponentRegistration.h>

#include <TEMPLATE/ComponentWindowTEMPLATE.h>

namespace template {
class ComponentHandlerTEMPLATE : public fi3d::ComponentHandler {

	Q_OBJECT
	FI3D_COMPONENT

private:
	/// @brief The component's GUI.
	ComponentWindowTEMPLATEPtr mGUI;

public:
	/// @brief Constructor.
	ComponentHandlerTEMPLATE(const QString& componentID);

	/// @brief Destructor.
	~ComponentHandlerTEMPLATE();

private:
	/// @brief Set up all the interactions.
	void setUpInteractions();

	/// @brief Set up all connections between the component elements.
	void setUpConnections();

protected slots:
	/// @brief Loads config file and applies saved defaults.
	void applyConfig();
	
	/// @brief Makes config file and saves to disk.
	void makeConfig();
};

/// @brief Alias for a smart pointer of this class.
using ComponentHandlerTEMPLATEPtr = QSharedPointer<ComponentHandlerTEMPLATE>;

}