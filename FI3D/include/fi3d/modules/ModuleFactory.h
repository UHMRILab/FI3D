#pragma once
/*!
* @author	VelazcoJD
* @file		ModuleFactory.h
* @class	fi3d::ModuleFactory
* @brief	Modules self-register and are created here.
*/

#include <QHash>
#include <QObject>
#include <QSharedPointer>

namespace fi3d {
/// @brief Forward declaration of ModuleHandler.
class ModuleHandler;

class ModuleFactory : public QObject {

	Q_OBJECT

	/// @brief Delegate for a module creator function.
	typedef QSharedPointer<ModuleHandler>(*ModuleCreator)(const QString&);

	/// @brief List of available modules.
	static QHash<QString, ModuleCreator>* mModuleCreators;

	/// @brief Map of available modules to their acronym.
	static QHash<QString, QString>* mModuleAcronyms;

	/// @brief Class is static.
	ModuleFactory();

public:
	/// @brief Destructor.
	~ModuleFactory();

	/// @brief Registers a new module.
	static bool registerModule(const QString& moduleName, const QString& moduleAcronym, ModuleCreator creatorFunction);

	/*! 
	 * @brief Creates a module based on the given name with the given ID. 
	 *
	 * If no module has that name, null is returned. 
	 */
	static QSharedPointer<ModuleHandler> createModule(const QString& moduleName, const QString& moduleID);

	/// @brief Gets a list of the names of the available modules.
	static QList<QString> getListOfAvailableModules();

	/*! 
	 * @brief Gets the acronym of the given module. 
	 * 
	 * If the module is non-existent, returns an empty string. 
	 */
	static QString getModuleAcronym(const QString& moduleName);

	/// @brief Check whether a module with the given name exists.
	static bool isModuleExistent(const QString& moduleName);
};
}
