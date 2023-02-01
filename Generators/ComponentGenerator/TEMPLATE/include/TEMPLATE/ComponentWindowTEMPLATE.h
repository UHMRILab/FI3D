#pragma once
/*!
* @author	AUTHOR
* @file		ModuleWindowTEMPLATE.h
* @class	template::ModuleWindowTEMPLATE
* @brief	GUI for the MODULE-NAME (TEMPLATE) module.
*/

#include <fi3d/modules/ModuleWindow.h>

/// @brief Forward declare the GUI class.
namespace Ui {
class ModuleWindowTEMPLATE;
}

#pragma warning(push)
#pragma warning(disable: 4250)
// Class inherits from same sub-class twice, but this is needed for Qt 
// Interfaces. There is no negative side effect from this, thus turning off 
// the warning here.
namespace template {
class ModuleWindowTEMPLATE : public fi3d::ModuleWindow
{

	Q_OBJECT

private:
	/// @brief The UI elements.
	Ui::ModuleWindowTEMPLATE* mGUI;

public:
	/// @brief Constructor.
	ModuleWindowTEMPLATE();

	/// @brief Destructor.
	~ModuleWindowTEMPLATE();

	/// @brief Gets the OpenGL widget where the main scene is rendered.
	QVTKOpenGLWidget* getSceneWidget();

};

/// @brief Alias for a smart pointer of this class.
using ModuleWindowTEMPLATEPtr = QSharedPointer<ModuleWindowTEMPLATE>;

}
#pragma warning(pop)	// End PUSH
