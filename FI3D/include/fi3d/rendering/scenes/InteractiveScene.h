#pragma once
/*!
* @author	VelazcoJD
* @file		InteractiveScene.h
* @class	fi3d::InteractiveScene
* @brief	Similar to a Scene with the ability to interact with visuals.
*
* This is the same as a regular Scene, except that it also has the ability to
* interact with visuals to move or camera.
*/

#include <fi3d/rendering/scenes/Scene.h>

#include <QMenu>

class vtkLabelPlacementMapper;

namespace fi3d {

/// @brief Forward declaration of InteractorStyle. 
class InteractorStyle;

/// @brief Forward declaration of SliceViewer2DWidget.
class SliceViewer2DWidget;

class InteractiveScene : public Scene {

	Q_OBJECT
	friend class InteractorStyle;

protected:
	/// @brief The interactor style of the rendering window.
	vtkSmartPointer<InteractorStyle> mInteractorStyle;
	
	/// @brief Mapper used to map the Visual3D labels on the scene.
	vtkSmartPointer<vtkLabelPlacementMapper> mLabelsMapper;

	/// @brief Actors for the Visual3D labels.
	vtkSmartPointer<vtkActor2D> mLabelsActor;

	/// @brief Whether the labels are currently visible or not.
	bool mAreLabelsVisible;

	/// @brief The right click menu.
	QSharedPointer<QMenu> mRightClickMenu;

	/// @brief Right click submenu for right clicking an ImageSlice.
	QSharedPointer<QMenu> mImageSliceRightClickSubmenu;

	/// @brief List of ImageSlice clones that are open.
	QHash<QString, QSharedPointer<SliceViewer2DWidget>> mImageSliceClones;

public:
	/// @brief Constructor.
	InteractiveScene(QVTKOpenGLStereoWidget* widget, const QString& id = "");

	/// @breif Destructor.
	~InteractiveScene();

public slots:
	/// @brief Whether to display the Visual3D labels or not.
	virtual void setLabelsVisible(const bool& areVisible);

	/// @brief Toggles the labels' visibility.
	virtual void setToggleLabelsVisible();

public:
	/// @brief Gets the interactor style.
	virtual vtkSmartPointer<InteractorStyle> getInteractorStyle();

	/// @brief Adds a menu to the right click menu.
	virtual void addSubmenuToRightClickMenu(QMenu* submenu);

	/// brief Adds an action at the beginning of to the right click menu. 
	virtual QAction* prependActionToRightClickMenu(const QString& actionText);

	/// brief Adds an action at the end of to the right click menu.
	virtual QAction* appendActionToRightClickMenu(const QString& actionText);

	/// brief Adds separator at the beginning of to the right click menu.
	virtual QAction* prependSeparatorToRightClickMenu();

	/// brief Adds separator at the end of to the right click menu.
	virtual QAction* appendSeparatorToRightClickMenu();

private slots:
	/*! 
	 * @brief Shows the right click menu at the location (x, y). 
	 *
	 * If the right click was made on a visual, it adds a corresponding
	 * submenu, e.g., ability to clone a slice.
	 */
	void showRightClickMenu(const int& x, const int& y, Visual3DPtr pickedVisual);

	/// @brief Shows the SliceViewer clone of the ImageSlice.
	void showSliceViewerClone();

	/// @brief Closes the SliceViewer clone of the ImageSlice.
	void closeSliceViewerClone();
};

/// @brief Alias for a smart pointer of this class.
using InteractiveScenePtr = QSharedPointer<InteractiveScene>;

}

