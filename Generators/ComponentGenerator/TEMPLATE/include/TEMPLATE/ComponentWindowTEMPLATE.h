#pragma once
/*!
* @author	AUTHOR
* @file		ComponentWindowTEMPLATE.h
* @class	template::ComponentWindowTEMPLATE
* @brief	GUI for the COMPONENT-NAME (TEMPLATE) component.
*/

// @todo: Needing more fixes for this template file.

#include <fi3d/components/ComponentWindow.h>

/// @brief Forward declare the GUI class.
namespace ui {
class ComponentWindowTEMPLATE;
}

#pragma warning(push)
#pragma warning(disable: 4250)
// Class inherits from same sub-class twice, but this is needed for Qt 
// Interfaces. There is no negative side effect from this, thus turning off 
// the warning here.
namespace template {
class ComponentWindowTEMPLATE : public fi3d::ComponentWindow
{

	Q_OBJECT

private:
	/// @brief The UI elements.
	Ui::ComponentWindowTEMPLATE* mGUI;

public:
	/// @brief Constructor.
	ComponentWindowTEMPLATE();

	/// @brief Destructor.
	~ComponentWindowTEMPLATE();

	/// @brief Gets the OpenGL widget where the main scene is rendered.
	QVTKOpenGLWidget* getSceneWidget();

};

/// @brief Alias for a smart pointer of this class.
using ComponentWindowTEMPLATEPtr = QSharedPointer<ComponentWindowTEMPLATE>;

}
#pragma warning(pop)	// End PUSH
