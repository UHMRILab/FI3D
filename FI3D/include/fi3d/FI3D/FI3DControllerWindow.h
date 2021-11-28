#pragma once
/*!
* @author	VelazcoJD
* @file		FI3DControllerWindow.h
* @class	fi3d::FI3DControllerWindow
* @brief	The main window.
*/

#include <QMainWindow>

#include <QPushButton>
#include <QList>

/// @brief Forward declaration of the GUI.
namespace Ui {
class FI3DControllerWindow;
}

namespace fi3d {
class FI3DControllerWindow : public QMainWindow {

	Q_OBJECT

signals:
	/// @brief When a component button is clicked
	void componentButtonClicked(const QString& componentName);

	/// @brief When a module button is clicked
	void moduleButtonClicked(const QString& moduleName, const QString& moduleID);

private:
	/// @brief The GUI.
	Ui::FI3DControllerWindow *mGUI;

	/// @brief The sample component and module buttons
	QPushButton *mComponentButton, *mModuleButton;

public:
	/// @brief Constructor.
	FI3DControllerWindow(QWidget *parent = nullptr);

	/// @brief Destructor.
	~FI3DControllerWindow();

	/// @brief Sets the components, creating the component buttons.
	void setComponents(QList<QString> componentNames);

	/// @brief Sets the modules, creating the module buttons.
	void setModules(QList<QString> moduleNames);

private slots:
	/// @brief Handles when a component button is clicked
	void onComponentClicked();

	/// @brief Handles when a module button is clicked
	void onModuleClicked();
};
}