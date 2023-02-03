#pragma once
/*!
* @author	VelazcoJD
* @file		InteractorStyle.h
* @class	fi3d::InteractorStyle 
* @brief	The interactor style for the InteractiveScene.
*
* This InteractorStyle is a combination of VTK's 
* vtkInteractorStyleTrackballCamera and vtkInteractorStyleTrackballActor. 
* Camera movements are done normally as done by 
* vtkInteractorStyleTrackballCamera. The interactions done with each the actors
* are enabled when the user presses the 'a' button. The interactions done with 
* actors are for Visual3D objects.
*/

#include <fi3d/modules/IFeedbackEmitter.h>

#include <fi3d/rendering/visuals/3D/Visual3D.h>

#include <QSharedPointer>

#include <vtkPropPicker.h>
#include <vtkCellPicker.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkSmartPointer.h>

namespace fi3d {
class InteractiveScene;

class InteractorStyle : public QObject, public vtkInteractorStyleTrackballCamera, virtual public IFeedbackEmitter {

	Q_OBJECT
	Q_INTERFACES(fi3d::IFeedbackEmitter)

signals:
	/// @brief Emitted when there was a right click (not right click and hold).
	void rightClicked(const int&x, const int& y, QSharedPointer<Visual3D> pickedObject);

	/// @brief Emitted whe when the user wants to toggle Visual3D labels.
	void toggleSceneObject3DLabels();

protected:
	/// @brief Whether the object-mode is enabled or not.
	bool mIsObjectKeyDown;

	/// @brief Whether scrolling should affect slice index or series index.
	bool mIsSliceScrolling;

	/// @brief Speed at which the camera/objects rotate.
	double mRotationSpeed;

	/// @brief Speed at which the camera/objects translate.
	double mTranslationSpeed;

	/*!
	*	@name Mouse Button Flags
	*	@brief Flags with the state of left, middle, and right button presses.
	*/
	/// @{
	bool mIsLeftButtonPressed;
	bool mIsMiddleButtonPressed;
	bool mIsRightButtonPressed;
	/// @}

	/// @brief Whether the mouse has moved since the last button click
	bool mHasMouseMoved;

	/*!
	*	@name Mouse Locations
	*	@brief Locations of the mouse at different events.
	*/
	/// @{
	int mMousePosition[2];
	int mLastMousePosition[2];
	int mLeftClickPosition[2];
	int mMiddleClickPosition[2];
	int mRightClickPosition[2];
	/// @}

	/// @brief The scene this InteractorStyle is in charge of. 
	InteractiveScene* mScene;

	/// @brief Finds a point in an actor that collides with the mouse cursor.
	/// NOTE: Doesn't seem to work with slices.
	vtkSmartPointer<vtkPropPicker> mPointPicker;

	/// @brief Finds a cell that collides with the mouse cursor.
	vtkSmartPointer<vtkCellPicker> mCellPicker;

	/// @brief The Visual3D that is being interacted with.
	Visual3DPtr mPickedObject;

public:
	/// @brief Requirement for vtk objects.
	static InteractorStyle* New();
    vtkTypeMacro(InteractorStyle, vtkInteractorStyleTrackballCamera)

	/// @brief Set the scene this InteractorStyle pertains to.
	virtual void setScene(InteractiveScene* scene);

	/*!
	*	@name Interactor Event Modifiers
	*	@brief Override event beharious from base interactor style.
	*/
	/// @{
	virtual void OnChar() override;
	virtual void OnKeyDown() override;
	virtual void OnKeyUp() override;
	virtual void OnMouseMove() override;
	virtual void OnLeftButtonDown() override;
	virtual void OnLeftButtonUp() override;
	virtual void OnMiddleButtonDown() override;
	virtual void OnMiddleButtonUp() override;
	virtual void OnRightButtonDown() override;
	virtual void OnRightButtonUp() override;
	virtual void OnMouseWheelBackward() override;
	virtual void OnMouseWheelForward() override;
	/// @}

	/// @brief Sets the scrolling of the rendered image to slices.
	void setToSliceScrolling();
	
	/// @brief Sets the scrolling of the rendered image to series.
	void setToSeriesScrolling();

	/*!
	*	@name Visual3D Interaction Routines
	*	@brief Override rotation and translation routines to implement 
	*			Visual3D movement.
	*/
	/// @{
	/// @brief Implements logic of vtkInteractorStyleTrackballActor.
	virtual void Rotate() override;

	/// @brief Implements logic of vtkInteractorStyleTrackballActor.
	virtual void Pan() override;
	/// @}

protected:
	/// @brief Determine picked point with a visual at the mouse location.
	void findPickedPoint();

	/// @brief Determine picked cell in a visual at the mouse location.
	void findPickedCell();

signals:
	/*!
	* @name Implementation of the IFeedbackEmitter interface.
	*/
	/// @{
    void feedback(const QString& text) override;
    void feedbackWarning(const QString& text) override;
    void feedbackError(const QString& text) override;
    void feedbackGlobal(const QString& text) override;
    void feedbackColor(const QString& text, const Qt::GlobalColor& color) override;
	/// @}

protected:
	/// @brief Constructor.
	InteractorStyle();

	/// @brief Destructor.
	~InteractorStyle() = default;

private:
	InteractorStyle(const InteractorStyle&) = delete;
	void operator=(const InteractorStyle&) = delete;

};

/// @brief Alias for a smart pointer of this class.
using InteractorStyleVPtr = vtkSmartPointer<InteractorStyle>;

}
