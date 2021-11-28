#pragma once
/*!
* @author	VelazcoJD
* @file		ModuleHandler.h
* @class	fi3d::ModuleHandler
* @brief	Abstract class defining basic features for a module handler.
*
* Every module has something in common. This is the class that defines such
* behavior. ModuleHandler enables every derived module with interactions to the
* main scene, a ModuleMessageEncoder for communication, and other general 
* tools. Derived modules have specific interactions which are added by deriving
* ModuleHandler, IModuleInteractor, ModuleMessageEncoder, and if needed, Scene.
*
* This class is the core of every module.
*/

#include <fi3d/modules/ModuleElement.h>

#include <fi3d/modules/IFeedbackEmitter.h>
#include <fi3d/modules/CommandParser.h>
#include <fi3d/modules/ModuleFactory.h>
#include <fi3d/modules/ModuleMessageEncoder.h>
#include <fi3d/modules/ModuleWindow.h>

#include <fi3d/rendering/scenes/Scene.h>

namespace fi3d {
class ModuleHandler : public QObject, public ModuleElement, virtual public IFeedbackEmitter {

	Q_OBJECT
	Q_INTERFACES(fi3d::IFeedbackEmitter)

signals:
	/// @brief Informs the FI3DController this module wants to be closed.
	void requestCloseModule();

protected:
	/// @brief Constructor. This class is only inheritable.
	ModuleHandler(const QString& moduleName, const QString& moduleAcronym, const QString& moduleID = "");

public:
	/// @brief Destructor.
	~ModuleHandler();

public slots:
	/// @brief Reset entire module to a starting state.
	virtual void reset();

	/*!
	 * @brief Pauses the module.
	 *
	 * This function does nothing. Inherting classes can override this function 
	 * to apply module-specific logic that must be executed when the module
	 * needs to be paused.
	 */
	virtual void pause();

	/*!
	 * @brief Resumes the module.
	 *
	 * This function does nothing. Inherting classes can override this function
	 * to apply module-specific logic that must be executed when the module
	 * needs to be resumed.
	 */
	virtual void resume();

	/// @brief Informs the FI3DController to close this module.
	virtual void closeModule();

	/// @brief Shows the module's GUI.
	virtual void showModuleWindow();

	/// @brief Attaches the given object as a ModuleElement of the module.
	virtual void attachModuleElement(ModuleElement* moduleElement);

protected slots:
	/// @brief Saves the given configuration to disk.
	virtual void saveConfig(const QJsonObject& config);

protected:
	/// @brief Loads the module's saved config, empty if no config was found.
	virtual QJsonObject loadConfig();

	/// @brief Sets the main scene (or unsets if given parameter is null).
	void setScene(ScenePtr scene);

	/*!
	 * @brief Sets the ModuleWindow to be used.
	 *
	 * This class has no ModuleWindow (no GUI) by default. However, this function 
	 * can be used to assign one which may include module-specific logic. See 
	 * ModuleWindow for more information.
	 */
	void setModuleWindow(ModuleWindowPtr tabPage);

	/// @brief Adds a ModuleInteraction.
	void addModuleInteraction(ModuleInteractionPtr moduleInteraction);

	/// @brief Adds a ModuleInteraction.
	void removeModuleInteraction(const QString& interactionID);

signals:
	/*!
	*	@name Implementation of the IFeedbackEmitter interface.
	*/
	/// @{
	void feedback(const QString& text);
	void feedbackWarning(const QString& text);
	void feedbackError(const QString& text);
	void feedbackGlobal(const QString& text);
	void feedbackColor(const QString& text, const Qt::GlobalColor& color);
	/// @}
};

/// @brief Alias for a smart pointer of this class.
using ModuleHandlerPtr = QSharedPointer<ModuleHandler>;

}