#pragma once
/*!
* @author	VelazcoJD
* @file		FI3DController.h
* @class	fi3d::FI3DController
* @brief	Controls the whole FI3D application.
*/

#include <QObject>

#include <fi3d/modules/ModuleHandler.h>

namespace fi3d {

class ApplicationMessageEncoder;
class FI3DControllerWindow;
class ModuleDialog;

class FI3DController : public QObject {

	Q_OBJECT

/// ************ Static members ************
private:
	/// @brief There's only one FI3DController
	static QSharedPointer<FI3DController> INSTANCE;

	/// @brief Whether the controller has been initiated.
	static bool sIsInitiated;

	/*!
	 * @brief Type that holds function for an initiation, e.g., a component.
	 *
	 * Called before the main window has been created and visualized.
	 */
	typedef bool(*Initiator)();

	/*!
	 * @brief Type that holds function for a clean-up. e.g., a component.
	 *
	 * Called after the main window has been closed.
	 */
	typedef void(*Cleaner)();

	/// @brief The function to call which shows a dialog for the component.
	typedef void(*Dialoger)();

	/// @brief Struct to hold the initiator and its name.
	typedef struct ComponentInformation {
		QString name;
		QString acronym;
		Initiator initiator;
		Cleaner cleaner;
		Dialoger dialoger;
	} ComponentInformation;

	/// @brief List of initiators that have no GUI.
	static QHash<QString, ComponentInformation>* sComponents;

	/// @brief List of initiators that have a GUI.
	static QHash<QString, ComponentInformation>* sComponentsWithGUI;

public:
	/// @brief Initiates resources needed for the system.
	static void init();

	/// @brief Cleans up the application.
	static void clean();

	/*!
	 * @brief Registers the initiation and clean-up functions of a component.
	 *
	 * The name of the component is used for all visualization purposes (shown
	 * in GUIs). The acronym, or short 1-word name, is used for organization
	 * of files in the data and assets directory. 
	 * 
	 * The initiation functions are called before the main window has been
	 * loaded. The clean functions are called after the main window has been
	 * closed.
	 *
	 * @param componentID The ID of the component.
	 * @param acronym An acronym of the component (or 1-word identifier).
	 * @param initiator The initiation function of the component.
	 * @param cleaner The cleaner function of the component.
	 * @param dialoger. The function of the component called to show its GUI.
	 *		Defaults to null, which implies the component has no GUI.
	 */
	static bool addComponent(const QString& componentID, const QString& acronym, 
		Initiator initiator, Cleaner cleaner, Dialoger dialoger = Q_NULLPTR);

public slots:
	/// @brief Shows the component's gui.
	static void showComponentGUI(const QString& componentID);
	
	/*! 
	 * @brief Activates a module instance of the givem module type and assigns 
	 *		the given ID. 
	 */
	static void activateModule(const QString& moduleName, const QString& moduleID);

	/// @brief Stops the module with the given module ID.
	static void stopModule(const QString& moduleID);

	/*! 
	 * @brief Sets the module with the given module ID as the focused module. 
	 * 
	 * Pauses the previously focused module and resumes the newly focused 
	 * module.
	 */
	static void setModuleFocus(const QString& moduleID);

	/// @brief Shows the main window GUI.
	static void showMainWindow();

public:
	/// @brief Get a static pointer to the running FI3D controller.            
	static const FI3DController* getInstance();

/// ************ Singleton members ************
signals:
	/// @brief Emitted when a module is activated.
	void startedModule(const QString& moduleID, const QString& moduleName);

	/// @brief Emmitted when a module is stopped.
	void stoppedModule(const QString& moduleID, const QString& moduleName);

private:
	/// @brief The main GUI.
	QSharedPointer<FI3DControllerWindow> mGUI;

	/// @brief Map of the active modules, in parallel to mTabs.
	QHash<QString, fi3d::ModuleHandlerPtr> mActiveModules;

	/// @brief The module that is being "focused".
	fi3d::ModuleHandlerPtr mFocusedModule;

	/// @brief Message encoder that handles application level server messages. 
	QSharedPointer<ApplicationMessageEncoder> mMessageEncoder;

	/// @brief Constructor.
	explicit FI3DController();

public:
	/// @brief Destructor.
	~FI3DController();

private:
	/*! 
	 * @brief Sets up the module. 
	 *
	 * This adds tab page to GUI, verifies name is unique, 
	 * and connects feedback. 
	 */
	void setupModule(fi3d::ModuleHandlerPtr newModule);

	/*! 
	 * @brief Ensures the module name is unique. 
	 *
	 * If the module ID already exists, it appends "_new" to it. 
	 */
	bool verifyModuleID(QString& moduleID);

private slots:
	/// @brief Triggered by QAction. The Module type is obtained from its text.
	void activateModuleByAction();

	/// @brief Closes the module by a close ModuleWindow event.
	void closeModuleByModuleRequest();

	/// @brief Handles global feedback
	void handleGlobalFeedback(const QString& feedback);
};

/// @brief Alias for a smart pointer of this class.
using FI3DControllerPtr = QSharedPointer<FI3DController>;

}