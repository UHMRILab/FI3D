#pragma once
/*!
* @author	VelazcoJD
* @file		ModuleWindowFI.h
* @class	fi::ModuleWindowFI
* @brief	GUI for the Framework Interface (FI) module.
*/

#include <fi3d/modules/ModuleWindow.h>

#include <QHash>
#include <QLabel>

/// @brief Forward declare the GUI class.
namespace Ui {
class ModuleWindowFI;
}

#pragma warning(push)
#pragma warning(disable: 4250)
// Class inherits from same sub-class twice, but this is needed for Qt 
// Interfaces. There is no negative side effect from this, thus turning off 
// the warning here.
namespace fi {
class ModuleWindowFI : public fi3d::ModuleWindow {

	Q_OBJECT

private:
	/// @brief The UI elements.
	Ui::ModuleWindowFI* mGUI;

	/// @brief The module interaction widgets.
	QHash<QString, QWidget*> mInteractionWidgets;

	///  @brief Some module interactions have labels, these are the labels.
	QHash<QString, QLabel*> mInteractionLabels;

public:
	/// @brief Constructor.
	ModuleWindowFI();

	/// @brief Destructor.
	~ModuleWindowFI();

	/// @brief Gets the OpenGL widget where the main scene is rendered.
	QVTKOpenGLWidget* getSceneWidget();

	/// @brief Handles the settings button being clicked.
	void onSettings();

	/// @brief Handles the sync button being toggled.
	void onSync();

	/// @brief Handles the refresh scene button being clicked.
	void onRefreshScene();

private slots:
	/// @brief Handles a module interaction being added
	void onModuleInteractionAdded(const QString& interactionID);

	/// @brief Handles a module interaction value changed
	void onModuleInteractionChange(const QJsonValue& value);

	/// @brief Handles a module interaction removed
	void onModuleInteractionRemoved(const QString& interactionID);

protected:
	/// @brief Listen to changes about the ModuleInformation.
	virtual void moduleInformationAssigned(fi3d::ModuleInformationPtr moduleInfo) override;
};

/// @brief Alias for a smart pointer of this class.
using ModuleWindowFIPtr = QSharedPointer<ModuleWindowFI>;

}
#pragma warning(pop)	// End PUSH
