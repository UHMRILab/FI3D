#include <fi3d/rendering/scenes/InteractiveScene.h>

#include <fi3d/logger/Logger.h>

#include <fi3d/rendering/scenes/InteractorStyle.h>
#include <fi3d/rendering/scenes/SliceViewer2D/SliceViewer2DWidget.h>

#include <vtkInteractorStyleSwitch.h>
#include <vtkLabelPlacementMapper.h>
#include <vtkPointData.h>
#include <vtkPointSet.h>
#include <vtkPointSetToLabelHierarchy.h>
#include <vtkRenderer.h>
#include <vtkStringArray.h>

using namespace fi3d;

InteractiveScene::InteractiveScene(QVTKOpenGLStereoWidget* widget, const QString& id)
	: Scene(widget, id),
	mInteractorStyle(vtkSmartPointer<InteractorStyle>::New()),
	mLabelsMapper(vtkSmartPointer<vtkLabelPlacementMapper>::New()),
	mLabelsActor(vtkSmartPointer<vtkActor2D>::New()),
	mAreLabelsVisible(false),
	mRightClickMenu(new QMenu(Q_NULLPTR)),
	mImageSliceRightClickSubmenu(new QMenu("Slice Options"))
{
	mInteractorStyle->setScene(this);
	this->setInteractorStyle(mInteractorStyle);

	mLabelsActor->SetMapper(mLabelsMapper);
	mRenderer->AddActor(mLabelsActor);
	mLabelsActor->SetVisibility(mAreLabelsVisible);

	// Reset camera right click action
	QAction* resetCameraAction = mRightClickMenu->addAction("Reset View");
	QObject::connect(
		resetCameraAction, &QAction::triggered, 
		this, &InteractiveScene::resetCamera);
	
	// Re-render scene right click action
	QAction* renderCameraAction = mRightClickMenu->addAction("Refresh View");
	QObject::connect(
		renderCameraAction, &QAction::triggered, 
		this, &InteractiveScene::render);

	// Clone slice action, shown only with slice visuals
	QAction* cloneImageSlice = mImageSliceRightClickSubmenu->addAction("Clone 2D View");
	QObject::connect(
		cloneImageSlice, &QAction::triggered, 
		this, &InteractiveScene::showSliceViewerClone);
}

InteractiveScene::~InteractiveScene() {
	if (!mRightClickMenu.isNull())
		mRightClickMenu.clear();
}

void InteractiveScene::setLabelsVisible(const bool& areVisible) {
	qDebug() << "Enter - Setting labels to visible:" << (areVisible ? "True" : "False");
	if (areVisible) {
		vtkNew<vtkStringArray> labels;
		labels->SetNumberOfValues(mVisuals3D.count());
		labels->SetName("Visual3D Labels");

		vtkSmartPointer<vtkPoints> labelsLocations = vtkSmartPointer<vtkPoints>::New();

		ModelDataVPtr pointSet = ModelDataVPtr::New();
		pointSet->SetPoints(labelsLocations);
		pointSet->GetPointData()->AddArray(labels);

		QHash<QString, Visual3DPtr>::iterator it = mVisuals3D.begin();
		for (int i = 0; it != mVisuals3D.end(); it++, i++) {
			QSharedPointer<Visual3D> obj = it.value();
			double* center = obj->getActor()->GetCenter();

			labels->SetValue(i, obj->getVisualID().toStdString());
			labelsLocations->InsertNextPoint(center);
		}

		vtkNew<vtkPointSetToLabelHierarchy> labeler;
		labeler->SetInputData(pointSet);
		labeler->SetLabelArrayName("Visual3D Labels");
		labeler->Update();

		mLabelsMapper->SetInputConnection(labeler->GetOutputPort());

		mLabelsActor->SetVisibility(true);
		this->render();
	} else {
		mLabelsActor->SetVisibility(false);
	}
	this->render();

	mAreLabelsVisible = areVisible;
	qDebug() << "Exit";
}

void InteractiveScene::setToggleLabelsVisible() {
	this->setLabelsVisible(!mAreLabelsVisible);
}

vtkSmartPointer<InteractorStyle> InteractiveScene::getInteractorStyle() {
	return mInteractorStyle;
}

void InteractiveScene::addSubmenuToRightClickMenu(QMenu* submenu) {
	if (submenu == Q_NULLPTR)
		return;

	mRightClickMenu->addSeparator();
	mRightClickMenu->addMenu(submenu);
}

QAction* InteractiveScene::prependActionToRightClickMenu(const QString& actionText) {
	QAction* newAction = new QAction(actionText, mRightClickMenu.data());
	mRightClickMenu->insertAction(mRightClickMenu->actions().at(0), newAction);
	return newAction;
}

QAction* InteractiveScene::appendActionToRightClickMenu(const QString& actionText) {
	return mRightClickMenu->addAction(actionText);
}

QAction* InteractiveScene::prependSeparatorToRightClickMenu() {
	return mRightClickMenu->insertSeparator(mRightClickMenu->actions().at(0));
}

QAction* InteractiveScene::appendSeparatorToRightClickMenu() {
	return mRightClickMenu->addSeparator();
}

void InteractiveScene::showRightClickMenu(const int& x, const int& y, Visual3DPtr pickedObject) {
	QPoint menuLocation;
	menuLocation.setX(x);
	menuLocation.setY(mWidget->height() - y);

	menuLocation = mWidget->mapToGlobal(menuLocation);
	
	bool isImageSlice = false;
	QAction* imageSliceSeparator = Q_NULLPTR;
	QAction* imageSliceMenuAction = Q_NULLPTR;

	if (!pickedObject.isNull() && pickedObject->getVisualType().isSlice()) {
		imageSliceSeparator = mRightClickMenu->addSeparator();
		imageSliceMenuAction = mRightClickMenu->addMenu(mImageSliceRightClickSubmenu.data());
		mImageSliceRightClickSubmenu->setObjectName(pickedObject->getVisualID());
		isImageSlice = true;
	} 
	// TODO: Add menus based on what pickedObject is
	mRightClickMenu->exec(menuLocation);

	if (isImageSlice) {
		mRightClickMenu->removeAction(imageSliceSeparator);
		mRightClickMenu->removeAction(imageSliceMenuAction);
	}
}

void InteractiveScene::showSliceViewerClone() {
	QAction* triggeredAction = qobject_cast<QAction*>(sender());

	ImageSlicePtr imageSlice = this->getImageSlice(triggeredAction->associatedWidgets()[0]->objectName());
	SliceViewer2DWidgetPtr slicer(new SliceViewer2DWidget(imageSlice));
	QObject::connect(
		slicer.data(), &SliceViewer2DWidget::changedWindowClosing, 
		this, &InteractiveScene::closeSliceViewerClone);

	mImageSliceClones.insert(slicer->objectName(), slicer);

	slicer->show();
}

void InteractiveScene::closeSliceViewerClone() {
	SliceViewer2DWidget* slicer = qobject_cast<SliceViewer2DWidget*>(sender());
	if (slicer == Q_NULLPTR) {
		qWarning() << "Failed to Remove SliceViewer. Unknown sender.";
		return;
	}

	mImageSliceClones.remove(slicer->objectName());
}
