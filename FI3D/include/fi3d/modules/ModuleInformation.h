#pragma once
/*!
* @author	VelazcoJD
* @file		ModuleInformation.h
* @class	fi3d::ModuleInformation
* @brief	Used to uniquely identify a module.
*/

#include <fi3d/modules/interactions/ModuleInteraction.h>

#include <QHash>
#include <QObject>
#include <QSharedPointer>

namespace fi3d {
class CommandParser;
class ModuleHandler;
class ModuleMessageEncoder;
class ModuleWindow;
class Scene;
class ModuleInformation : public QObject {

	Q_OBJECT
	friend ModuleHandler;
	
signals:
	/// @brief Emitted when the module ID has changed.
	void changedModuleID(const QString& oldID, const QString& newID) const;

	/// @brief Emitted when the main Scene has changed.
	void changedMainScene() const;

	/// @brief Emitted when a module interaction is added.
	void changedAddedInteraction(const QString& interactionID);

	/// @brief Emitted when a module interaction is removed.
	void changedRemovedInteraction(const QString& interactionID);

private:
	/// @brief The module name.
	QString mModuleName;

	/// @breief The module's acronym.
	QString mModuleAcronym;

	/// @brief The module's ID.
	QString mModuleID;

	/// @brief The ModuleHandler of the module.
	ModuleHandler* mModuleHandler;

	/// @brief The main Scene of the module.
	QSharedPointer<Scene> mMainScene;

	/// @brief The ModuleMessageEncoder of the module.
	QSharedPointer<ModuleMessageEncoder> mMessageEncoder;

	/// @brief The CommandParser of the module.
	QSharedPointer<CommandParser> mCommandParser;

	/// @brief The ModuleWindow (GUI) of the module.
	QSharedPointer<ModuleWindow> mModuleWindow;

	/*! @brief List of module interactions. */
	QList<QSharedPointer<ModuleInteraction>> mInteractions;

	/// @brief Manages all the module interactions.
	QHash<QString, int> mInteractionsMap;

public:
	/// @brief Constructor.
	ModuleInformation(const QString& moduleName, const QString& moduleAcronym, const QString& moduleID);

	/// @brief Destructor.
	~ModuleInformation();

	/// @brief Gets the module name.
	QString getModuleName() const;

	/// @brief Gets the module acronym.
	QString getModuleAcronym() const;

	/// @brief Gets the module ID.
	QString getModuleID() const;

	/*!
	*	@name ModuleInformation operators.
	*	@brief Assignment and comparison operators for ModuleInformation.
	*/
	/// @{
	bool operator==(const ModuleInformation& other) const;
	bool operator==(const QString& other) const;
	bool operator!=(const ModuleInformation& other) const;
	bool operator!=(const QString& other) const;
	/// @}

	/// @brief Gets the ModuleHandler of the module.
	ModuleHandler* getModuleHandler() const;

	/// @brief Gets the main Scene of the module.
	QSharedPointer<Scene> getMainScene() const;

	/// @brief Gets the ModuleMessageEncoder of the module.
	QSharedPointer<ModuleMessageEncoder> getMessageEncoder() const;

	/// @brief Gets the CommandParser of the module.
	QSharedPointer<CommandParser> getCommandParser() const;

	/// @brief Gets the ModuleWindow (GUI) of the module.
	QSharedPointer<ModuleWindow> getModuleWindow() const;

	/// @brief Gets the ModuleInteraction instances of the module.
	QList<ModuleInteractionPtr> getModuleInteractions() const;

	/// @brief Gets the ModuleInteraction w/ the given ID, null if not found.
	ModuleInteractionPtr getModuleInteraction(const QString& interactionID) const;

private:
	/// @brief Changes the module's ID, can only be set by ModuleHandler.
	void setModuleID(const QString& newID);

	/// @brief Sets the ModuleHandler of the module.
	void setModuleHandler(ModuleHandler* moduleHandler);

	/// @brief Sets the main Scene of the module.
	void setMainScene(QSharedPointer<Scene> scene);

	/// @brief Sets the ModuleMessageEncoder of the module.
	void setMessageEncoder(QSharedPointer<ModuleMessageEncoder> messageEncoder);

	/// @brief Sets the CommandParser of the module.
	void setCommandParser(QSharedPointer<CommandParser> commandParser);

	/// @brief Sets the ModuleWindow (GUI) of the module.
	void setModuleWindow(QSharedPointer<ModuleWindow> moduleWindow);

	/// @brief Adds a module interaction.
	void addModuleInteraction(ModuleInteractionPtr moduleInteraction);

	/// @brief Removes a module interaction.
	void removeModuleInteraction(const QString& interactionID);
};

/// @brief Alias for a smart pointer of this class.
using ModuleInformationPtr = QSharedPointer<ModuleInformation>;

}