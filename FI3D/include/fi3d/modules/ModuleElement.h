#pragma once
/*!
* @author	VelazcoJD
* @file		ModuleElement.h
* @class	fi3d::ModuleElement
* @brief	Contains references to the pieces every module element should have
*			access to.
*/

#include <fi3d/modules/ModuleInformation.h>

#include <fi3d/modules/interactions/ValuelessInteraction.h>
#include <fi3d/modules/interactions/BooleanInteraction.h>
#include <fi3d/modules/interactions/IntegerInteraction.h>
#include <fi3d/modules/interactions/FloatInteraction.h>
#include <fi3d/modules/interactions/StringInteraction.h>
#include <fi3d/modules/interactions/SelectInteraction.h>

namespace fi3d {
class ModuleElement {
	
	friend ModuleHandler;

private:
	/// @brief The module's information.
	ModuleInformationPtr mModuleInformation;

public:
	/// @brief Constructor.
	ModuleElement();

	/// @brief Destructor.
	~ModuleElement();

	/// @brief Gets the module name.
	QString getModuleName() const;

	/// @brief Gets the module acronym.
	QString getModuleAcronym() const;

	/// @brief Gets the module ID.
	QString getModuleID() const;

	/// @brief Gets the path to the directory where assets are stored for 
	/// this module.
	QString getModuleAssetsPath() const;

	/// @brief Gets the path to the directory where data are stored for 
	/// this module.
	QString getModuleDataPath() const;

	/// @brief Gets the ModuleHandler of the module.
	ModuleHandler* getModuleHandler();

	/// @brief Gets the main Scene of the module.
	QSharedPointer<Scene> getMainScene();

	/// @brief Gets the ModuleMessageEncoder of the module.
	QSharedPointer<ModuleMessageEncoder> getMessageEncoder();

	/// @brief Gets the CommandParser of the module.
	QSharedPointer<CommandParser> getCommandParser();

	/// @brief Gets the ModuleWindow (GUI) of the module.
	QSharedPointer<ModuleWindow> getModuleWindow();

	/// @brief Gets the ModuleInteraction instances.
	QList<ModuleInteractionPtr> getModuleInteractions();
	
	/*!
	* @name Module Interaction Getters.
	* @brief Gets the module interaction with the given ID, null if not found.
	*/
	/// @{
	ModuleInteractionPtr getModuleInteraction(const QString& id);
	ValuelessInteractionPtr getValuelessModuleInteraction(const QString& id);
	BooleanInteractionPtr getBooleanModuleInteraction(const QString& id);
	IntegerInteractionPtr getIntegerModuleInteraction(const QString& id);
	FloatInteractionPtr getFloatModuleInteraction(const QString& id);
	StringInteractionPtr getStringModuleInteraction(const QString& id);
	SelectInteractionPtr getSelectModuleInteraction(const QString& id);
	/// @}

	/*!
	 * @brief Sets the given value to the interaction with the given ID.
	 *
	 * If the interaction is not found, nothing happens and false is returned
	 *
	 * @return Whether the value was assigned or not.
	 */
	bool setModuleInteractionValue(const QString& interactionID, const QJsonValue& value);

	/*!
	 * @brief Gets you the value, as a geneirc Json value, of the interaction
	 * with the given ID.
	 *
	 * If the interaction is not found, an empty Json value is given.
	 *
	 * @return The value of the interaction.
	 */
	QJsonValue getModuleInteractionValue(const QString& interactionID);

	/// @brief Gets the ModuleInformation object, can be used to listen for
	///		changes to the module's ID.
	ModuleInformation* getModuleInformationObject() const;

protected:
	/*!
	 * @brief Called to notify ModuleInformation has been assigned.
	 *
	 * Called at the end of setModuleInformation. This function can be used by
	 * derived classes to override and use new ModuleInformation, which can only
	 * be assigned by the ModuleHandler. Internally, this function does nothing.
	 */
	virtual void moduleInformationAssigned(ModuleInformationPtr moduleInfo) const;

private:
	/// @brief Sets the ModuleInformation, set by the ModuleHandler.
	virtual void setModuleInformation(ModuleInformationPtr moduleInfo);

	/// @brief Creates a new ModuleInformation with empty objects, only used by
	/// the ModuleHandler.
	void clearModuleInformation();
};
}