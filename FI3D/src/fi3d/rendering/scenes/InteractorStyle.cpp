#include <fi3d/rendering/scenes/InteractorStyle.h>

#include <fi3d/logger/Logger.h>

#include <fi3d/rendering/scenes/InteractiveScene.h>

#include <QVTKInteractor.h>

#include <vtkCamera.h>
#include <vtkObjectFactory.h>
#include <vtkRenderer.h>

using namespace fi3d;

vtkStandardNewMacro(InteractorStyle);

InteractorStyle::InteractorStyle()
	: QObject(Q_NULLPTR),
	mIsObjectKeyDown(false),
	mIsSliceScrolling(true),
	mRotationSpeed(1.0), mTranslationSpeed(1.0),
	mIsLeftButtonPressed(false), mIsMiddleButtonPressed(false), 
	mIsRightButtonPressed(false),
	mMousePosition{0, 0}, mLastMousePosition{0, 0},
	mLeftClickPosition{0, 0}, mMiddleClickPosition{0, 0}, mRightClickPosition{0, 0},
	mScene(Q_NULLPTR), 
	mPicker(vtkSmartPointer<vtkCellPicker>::New()),
	mPickedObject(Q_NULLPTR)
{
	mPicker->SetTolerance(0.001);
}

InteractorStyle::~InteractorStyle() {}

void InteractorStyle::setScene(InteractiveScene* scene) {
	if (mScene != Q_NULLPTR) {
		QObject::disconnect(
			this, &InteractorStyle::toggleSceneObject3DLabels,
			mScene, &InteractiveScene::setToggleLabelsVisible
		);
		QObject::disconnect(
			this, &InteractorStyle::rightClicked,
			mScene, &InteractiveScene::showRightClickMenu
		);
		this->removeFeedbackPropagator(mScene);
	}

	mScene = scene;
	if (mScene != Q_NULLPTR) {
		QObject::connect(
			this, &InteractorStyle::toggleSceneObject3DLabels,
			mScene, &InteractiveScene::setToggleLabelsVisible,
			Qt::UniqueConnection
		);
		QObject::connect(
			this, &InteractorStyle::rightClicked,
			mScene, &InteractiveScene::showRightClickMenu,
			Qt::UniqueConnection
		);
		this->addFeedbackPropagator(mScene);
	}
}

void InteractorStyle::OnChar() {
	switch (this->Interactor->GetKeyCode()) {
		case 'j':
		case 'J':
			emit toggleSceneObject3DLabels();
			break;
		default:
			vtkInteractorStyleTrackballCamera::OnChar();
			break;
	}
}

void InteractorStyle::OnKeyDown() {
	switch (this->Interactor->GetKeyCode()) {
		case 'o':
		case 'O':
			mIsObjectKeyDown = true;
			break;
		default:
			vtkInteractorStyleTrackballCamera::OnKeyDown();
			break;
	}
}

void InteractorStyle::OnKeyUp() {
	switch (this->Interactor->GetKeyCode()) {
		case 'o':
		case 'O':
			mIsObjectKeyDown = false;
			break;
		default:
			vtkInteractorStyleTrackballCamera::OnKeyUp();
			break;
	}
}

void InteractorStyle::OnMouseMove() {
	mHasMouseMoved = true;
	mLastMousePosition[0] = mMousePosition[0];
	mLastMousePosition[1] = mMousePosition[1];
	this->GetInteractor()->GetEventPosition(mMousePosition);
	
	if (!mIsObjectKeyDown) {
		vtkInteractorStyleTrackballCamera::OnMouseMove();
		return;
	}

	if (mIsLeftButtonPressed) {
		this->Rotate();
	} else if (mIsMiddleButtonPressed) {
		this->Pan();
	}
}

void InteractorStyle::OnLeftButtonDown() {
	qDebug() << "Enter";
	mIsLeftButtonPressed = true;
	mHasMouseMoved = false;
	this->GetInteractor()->GetEventPosition(mLeftClickPosition);
	if (!mIsObjectKeyDown) {
		vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
	} else {
		this->findPickedVisual3D();
		if (mPickedObject.isNull()) {
			return;
		} else {
			this->StartRotate();
		}
	}
	qDebug() << "Exit";
}

void InteractorStyle::OnLeftButtonUp() {
	mIsLeftButtonPressed = false;

	if (!mHasMouseMoved && mIsObjectKeyDown) {
		if (!mPickedObject.isNull() && mPickedObject->getVisualType().isSlice()) {
			ImageSlice* slice = qobject_cast<ImageSlice*>(mPickedObject.data());
			slice->setPreviousOrientation();
		}
	} 
	mPickedObject = Q_NULLPTR;
	
	if (!mIsObjectKeyDown) {
		vtkInteractorStyleTrackballCamera::OnLeftButtonUp();
	} else {
		this->EndRotate();
	}
}

void InteractorStyle::OnMiddleButtonDown() {
	qDebug() << "Enter";
	mIsMiddleButtonPressed = true;
	mHasMouseMoved = false;
	this->GetInteractor()->GetEventPosition(mMiddleClickPosition);
	if (!mIsObjectKeyDown) {
		vtkInteractorStyleTrackballCamera::OnMiddleButtonDown();
	} else {
		this->findPickedVisual3D();
		if (mPickedObject.isNull()) {
			return;
		} else {
			this->StartPan();
		}
	}
	qDebug() << "Exit";
}

void InteractorStyle::OnMiddleButtonUp() {
	mIsMiddleButtonPressed = false;
	mPickedObject = Q_NULLPTR;

	if (!mHasMouseMoved) {
		mIsSliceScrolling = !mIsSliceScrolling;
	}

	if (!mIsObjectKeyDown) {
		vtkInteractorStyleTrackballCamera::OnMiddleButtonUp();
	} else {
		this->EndPan();
	}
}

void InteractorStyle::OnRightButtonDown() {
	mIsRightButtonPressed = true;
	mHasMouseMoved = false;
	this->GetInteractor()->GetEventPosition(mRightClickPosition);
	if (!mIsObjectKeyDown) {
		vtkInteractorStyleTrackballCamera::OnRightButtonDown();
	}
	this->findPickedVisual3D();
}

void InteractorStyle::OnRightButtonUp() {
	mIsRightButtonPressed = false;

	if (!mHasMouseMoved && mIsObjectKeyDown) {
		if (!mPickedObject.isNull() && mPickedObject->getVisualType().isSlice()) {
			ImageSlice* slice = qobject_cast<ImageSlice*>(mPickedObject.data());
			slice->setNextOrientation();
		}
	} else if (!mHasMouseMoved) {
		emit rightClicked(mRightClickPosition[0], mRightClickPosition[1], mPickedObject);
	}
	mPickedObject = Q_NULLPTR;

	if (!mIsObjectKeyDown) {
		vtkInteractorStyleTrackballCamera::OnRightButtonUp();
	}
}

void InteractorStyle::OnMouseWheelBackward() {
	if (!mIsObjectKeyDown) {
		vtkInteractorStyleTrackballCamera::OnMouseWheelBackward();
		return;
	}

	if (mScene == Q_NULLPTR) {
		return;
	}

	this->findPickedVisual3D();
	if (mPickedObject.isNull()) {
		return;
	}

	if (mPickedObject->getVisualType().isStudySlice() && !mIsSliceScrolling) {
		StudySlice* slice = qobject_cast<StudySlice*>(mPickedObject.data());
		slice->setSeriesIndexToPrevious();
	} else if (mPickedObject->getVisualType().isSlice()) {
		ImageSlice* slice = qobject_cast<ImageSlice*>(mPickedObject.data());
		slice->setSliceIndexToPrevious();
	}
    mPickedObject = Q_NULLPTR;
}

void InteractorStyle::OnMouseWheelForward() {
	if (!mIsObjectKeyDown) {
		vtkInteractorStyleTrackballCamera::OnMouseWheelForward();
		return;
	}

	if (mScene == Q_NULLPTR) {
		return;
	}

	this->findPickedVisual3D();
	if (mPickedObject.isNull()) {
		return;
	}

	if (mPickedObject->getVisualType().isStudySlice() && !mIsSliceScrolling) {
		StudySlice* slice = qobject_cast<StudySlice*>(mPickedObject.data());
		slice->setSeriesIndexToNext();
	} else if (mPickedObject->getVisualType().isSlice()) {
		ImageSlice* slice = qobject_cast<ImageSlice*>(mPickedObject.data());
		slice->setSliceIndexToNext();
	}
    mPickedObject = Q_NULLPTR;
}

void InteractorStyle::setToSliceScrolling() {
	mIsSliceScrolling = true;
}

void InteractorStyle::setToSeriesScrolling() {
	mIsSliceScrolling = false;
}

void InteractorStyle::Rotate() {
	if (!mIsObjectKeyDown) {
		vtkInteractorStyleTrackballCamera::Rotate();
		return;
	}

	if (mScene == Q_NULLPTR || mPickedObject.isNull() || !mPickedObject->isInteractable()) {
		return;
	}

	vtkRenderWindowInteractor *rwi = this->Interactor;
	vtkCamera *cam = this->CurrentRenderer->GetActiveCamera();

	// First get the origin of the assembly
	double *obj_center = mPickedObject->getActor()->GetCenter();

	// GetLength gets the length of the diagonal of the bounding box
	double boundRadius = mPickedObject->getActor()->GetLength() * 0.5;

	// Get the view up and view right vectors
	double view_up[3], view_look[3], view_right[3];

	cam->OrthogonalizeViewUp();
	cam->ComputeViewPlaneNormal();
	cam->GetViewUp(view_up);
	vtkMath::Normalize(view_up);
	cam->GetViewPlaneNormal(view_look);
	vtkMath::Cross(view_up, view_look, view_right);
	vtkMath::Normalize(view_right);

	// Get the furtherest point from object position+origin
	double outsidept[3];

	outsidept[0] = obj_center[0] + view_right[0] * boundRadius;
	outsidept[1] = obj_center[1] + view_right[1] * boundRadius;
	outsidept[2] = obj_center[2] + view_right[2] * boundRadius;

	// Convert them to display coord
	double disp_obj_center[3];

	this->ComputeWorldToDisplay(obj_center[0], obj_center[1], obj_center[2], disp_obj_center);

	this->ComputeWorldToDisplay(outsidept[0], outsidept[1], outsidept[2], outsidept);

	double radius = sqrt(vtkMath::Distance2BetweenPoints(disp_obj_center, outsidept));
	double nxf = (rwi->GetEventPosition()[0] - disp_obj_center[0]) / radius;

	double nyf = (rwi->GetEventPosition()[1] - disp_obj_center[1]) / radius;

	double oxf = (rwi->GetLastEventPosition()[0] - disp_obj_center[0]) / radius;

	double oyf = (rwi->GetLastEventPosition()[1] - disp_obj_center[1]) / radius;

	if (((nxf * nxf + nyf * nyf) <= 1.0) &&
		((oxf * oxf + oyf * oyf) <= 1.0)) {
		double newXAngle = vtkMath::DegreesFromRadians(asin(nxf));
		double newYAngle = vtkMath::DegreesFromRadians(asin(nyf));
		double oldXAngle = vtkMath::DegreesFromRadians(asin(oxf));
		double oldYAngle = vtkMath::DegreesFromRadians(asin(oyf));

		double **rotate = new double*[2];

		rotate[0] = new double[4];
		rotate[1] = new double[4];

		rotate[0][0] = newXAngle - oldXAngle;
		rotate[0][1] = view_up[0];
		rotate[0][2] = view_up[1];
		rotate[0][3] = view_up[2];

		rotate[1][0] = oldYAngle - newYAngle;
		rotate[1][1] = view_right[0];
		rotate[1][2] = view_right[1];
		rotate[1][3] = view_right[2];

		// Block signals for first rotation to avoid emitting the same ObjectTransformation signal twice.
		mPickedObject->setTransformToPostMultiply();
		mPickedObject->blockSignals(true);
		mPickedObject->rotateWXYZ(rotate[0][0], rotate[0][1], rotate[0][2], rotate[0][3]);
		mPickedObject->blockSignals(false);
		mPickedObject->rotateWXYZ(rotate[1][0], rotate[1][1], rotate[1][2], rotate[1][3]);
		mPickedObject->setTransformToPreMultiply();

		delete[] rotate[0];
		delete[] rotate[1];
		delete[] rotate;

		if (this->AutoAdjustCameraClippingRange) {
			this->CurrentRenderer->ResetCameraClippingRange();
		}

		rwi->Render();
	}
}

void InteractorStyle::Pan() {
	if (!mIsObjectKeyDown) {
		vtkInteractorStyleTrackballCamera::Pan();
		return;
	}

	if (mScene == Q_NULLPTR || mPickedObject.isNull() || !mPickedObject->isInteractable()) {
		return;
	}

	vtkRenderWindowInteractor *rwi = this->Interactor;

	// Use initial center as the origin from which to pan
	double *obj_center = mPickedObject->getActor()->GetCenter();

	double disp_obj_center[3], new_pick_point[4];
	double old_pick_point[4], motion_vector[3];

	this->ComputeWorldToDisplay(obj_center[0], obj_center[1], obj_center[2], disp_obj_center);

	this->ComputeDisplayToWorld(rwi->GetEventPosition()[0], 
		rwi->GetEventPosition()[1],
		disp_obj_center[2],
		new_pick_point);

	this->ComputeDisplayToWorld(rwi->GetLastEventPosition()[0],
		rwi->GetLastEventPosition()[1],
		disp_obj_center[2],
		old_pick_point);

	motion_vector[0] = new_pick_point[0] - old_pick_point[0];
	motion_vector[1] = new_pick_point[1] - old_pick_point[1];
	motion_vector[2] = new_pick_point[2] - old_pick_point[2];
	
	mPickedObject->setTransformToPostMultiply();
	mPickedObject->translate(motion_vector[0], motion_vector[1], motion_vector[2]);
	mPickedObject->setTransformToPreMultiply();

	if (this->AutoAdjustCameraClippingRange) {
		this->CurrentRenderer->ResetCameraClippingRange();
	}

	rwi->Render();
}

void InteractorStyle::findPickedVisual3D() {
	if (mScene == Q_NULLPTR) {
		return;
	}
	mPicker->Pick(mMousePosition[0], mMousePosition[1], 0.0, mScene->mRenderer);
	vtkProp* prop = mPicker->GetViewProp();
	mPickedObject = mScene->getVisual3D(mScene->getVisualIDByProp(prop));
}
