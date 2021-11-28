#pragma once
/*!
* @author	VelazcoJD
* @file		ModuleHandlerFI.h
* @class	fi::ModuleHandlerFI
* @brief	Handles the FI amodule.
*
* Add information on what this module does.
*/

#include <fi3d/modules/ModuleHandler.h>

#include <fi3d/modules/ModuleRegistration.h>
#include <fi3d/server/network/Message.h>

#include <FI/scene/SceneFI.h>
#include <FI/ModuleWindowFI.h>
#include <FI/ModuleMenuDialog.h>
#include <FI/ServerConnection.h>

namespace fi {
class ModuleHandlerFI : public fi3d::ModuleHandler {

	Q_OBJECT
	FI3D_MODULE

private:
	/// @brief The module's GUI.
	ModuleWindowFIPtr mGUI;
	
	/// @brief The module menu.
	ModuleMenuDialogPtr mModuleMenu;

	/// @brief The main scene.
	SceneFIPtr mScene;

	/// @brief The connection with the server.
	ServerConnectionPtr mConnection;

	/// @brief The ID of the module being subscribed to.
	QString mSubscribedModuleID;

	/// @brief Whether the module is being synchronized.
	bool mIsSynchronized;

public:
	/// @brief Constructor.
	ModuleHandlerFI(const QString& moduleID);

	/// @brief Destructor.
	~ModuleHandlerFI();

private:
	/// @brief Setups all the interactions.
	void setUpInteractions();

public slots:
	/// @brief Checks that the ModuleMenuDialog is not running, if so, 
	/// cancel closing request.
	virtual void closeModule() override;

	/// @brief Override so that module menu is shown instead.
	virtual void showModuleWindow() override;

	/// @brief Start the connection to the FI3D.
	void startFI3DConnection();

	/// @brief Stops the connection with the FI3D.
	void stopFI3DConnection();

	/// @brief Sends a request to activate a module with the given name.
	void activateModule(const QString& moduleName);
	
	/// @brief Sends a request to activate a module with the given name.
	void stopModule(const QString& moduleID);

	/// @brief Sends a request to subscribe to the module with the given ID.
	void subscribeToModule(const QString& moduleID);

	/// @brief Unsubscribe from module and pops up module menu.
	void onSettings();

	/// @brief Enables/Disables syncrhonization of scene.
	void onSynchronize(const bool& isEnabled);

	/// @brief Calls the server to refresh the Scene.
	void onRefreshScene();

private slots:
	/// @brief Handles an application response.
	void onApplicationMessage(fi3d::MessagePtr message);

	/// @brief Handles a module response.
	void onModuleMessage(fi3d::MessagePtr message);

	/*!
	* @name Module interaction response handlers.
	*/
	/// @{
	void handleAddModuleInteraction(const QJsonObject& interactionJson);
	void handleUpdateModuleInteraction(const QJsonObject& interactionJson);
	void handleRemoveModuleInteraction(const QJsonObject& interactionJson);
	/// @}

	/*!
	* @name Module Interaction Requestors.
	* @brief Sends request when user interacts with module interaction.
	*/
	/// @{
	void onValuelessInteraction();
	void onBooleanInteraction(const bool& value);
	void onIntegerInteraction(const int& value);
	void onFloatInteraction(const float& value);
	void onStringInteraction(const QString& value);
	void onSelectInteraction(const int& value);
	/// @}

	/// @brief Handles the ModuleMenuDialog closing.
	void onModuleMenuDialogClose();

protected slots:
	/// <summary>
	/// Loads config file and applies saved defaults.
	/// </summary>
	void applyConfig();

	/// <summary>
	/// Makes config file and saves to disk.
	/// </summary>
	void makeConfig();
};

/// @brief Alias for a smart pointer of this class.
using ModuleHandlerFIPtr = QSharedPointer<ModuleHandlerFI>;

}
