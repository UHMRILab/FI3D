#pragma once
/*!
* @author	VelazcoJD
* @file		ModuleWindowDEMO.h
* @class	demo::ModuleWindowDEMO
* @brief	GUI for the Demonstration (DEMO) module.
*/

#include <fi3d/modules/ModuleWindow.h>

/// @brief Forward declare the GUI class.
namespace Ui {
class ModuleWindowDEMO;
}

#pragma warning(push)
#pragma warning(disable: 4250)
// Class inherits from same sub-class twice, but this is needed for Qt 
// Interfaces. There is no negative side effect from this, thus turning off 
// the warning here.
namespace demo {
class ModuleWindowDEMO : public fi3d::ModuleWindow {

	Q_OBJECT

private:
	/// @brief The UI elements.
	Ui::ModuleWindowDEMO* mGUI;

public:
	/// @brief Constructor.
	ModuleWindowDEMO();

	/// @brief Destructor.
	~ModuleWindowDEMO();

	/// @brief Gets the OpenGL widget where the main scene is rendered.
	QVTKOpenGLWidget* getSceneWidget();

private:
	/*!
	*	@name GUI Callbacks.
	*	@brief Functions to handle GUI inputs.
	*/
	/// @{
	void onSlice(const int& index);
	void onPhase(const int& index);
	void onSurfaceEnable();
	void onAnimateEnable();
	/// @}

public slots:
	/*!
	*	@name GUI Updaters.
	*	@brief Updates GUI elements based on given values.
	*/
	/// @{
	void updateSlice(const int& index);
	void updatePhase(const int& index);
	void updateSurfaceEnable(const bool& isEnabled);
	void updateAnimateEnable(const bool& isEnabled);
	void updateIP(const QString& IP);
	void updatePort(const int& port);
	void updateNumberOfConnectedFI(const int& connectionCount);
	/// @}
};

/// @brief Alias for a smart pointer of this class.
using ModuleWindowDEMOPtr = QSharedPointer<ModuleWindowDEMO>;

}
#pragma warning(pop)	// End PUSH
