#include <fi3d/rendering/scenes/SliceViewer2D/ImageSliceViewer2D.h>

#include <fi3d/logger/Logger.h>

#include <fi3d/utilities/ModelAlgorithms.h>

#include <QVTKOpenGLWidget.h>

#include <vtkCellPicker.h>
#include <vtkEventQtSlotConnect.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkImageViewer2.h>
#include <vtkInteractorStyleImage.h>
#include <vtkPlane.h>
#include <vtkPropPicker.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>

using namespace fi3d;

ImageSliceViewer2D::ImageSliceViewer2D(QVTKOpenGLWidget* widget, const QString& id, ESliceOrientation orientation)
	: Scene(widget, id),
	mImageData(ImageDataVPtr::New()),
	mOrientation(orientation),
	mOrientationSliceIndices{0, 0, 0},
	mViewer(vtkSmartPointer<vtkImageViewer2>::New()),
	mCoordinateText2D(),
	mMessageText2D(),
	mOrientationText2D(),
	mSliceIndexText2D(),
	mOriginalModels()
{
	mViewer->SetRenderWindow(mRenderWindow);
	mViewer->SetRenderer(mRenderer);

	vtkSmartPointer<vtkInteractorStyleImage> imStyle = vtkSmartPointer<vtkInteractorStyleImage>::New();
	mRenderWindow->GetInteractor()->SetInteractorStyle(imStyle);

	mViewer->SetInputData(mImageData);
	mRenderer->SetBackground(0.0, 0.0, 0.0);

	mCoordinateText2D = this->addSubtitle("Coordinate Information");
	mMessageText2D = this->addSubtitle("Message");
	mOrientationText2D = this->addSubtitle("Orientation");
	mSliceIndexText2D = this->addSubtitle("Current Slice Index");

	mCoordinateText2D->setLocationTopLeft();
	mCoordinateText2D->justifyTextLeft();
	mCoordinateText2D->justifyTextTop();
	mCoordinateText2D->setText("");

	mMessageText2D->setLocationTopRight();
	mMessageText2D->justifyTextRight();
	mMessageText2D->justifyTextTop();

	mOrientationText2D->setLocationBottomLeft();
	mOrientationText2D->justifyTextLeft();
	mOrientationText2D->justifyTextBottom();
	mOrientationText2D->setColor(1.0, 0.89, 0.15);

	mSliceIndexText2D->setLocationBottomRight();
	mSliceIndexText2D->justifyTextRight();
	mSliceIndexText2D->justifyTextBottom();
	mSliceIndexText2D->setColor(1.0, 0.89, 0.15);
	mSliceIndexText2D->setText("Slice 0/0");
	
	this->setAxesVisible(false);
	this->applyOrientationChange();

	QObject::connect(
		this, &ImageSliceViewer2D::changedSlice, 
		this, &ImageSliceViewer2D::render);
	QObject::connect(
		this, &ImageSliceViewer2D::changedImageData,
		this, &ImageSliceViewer2D::render);
	QObject::connect(
		this, &ImageSliceViewer2D::changedMessageText, 
		this, &ImageSliceViewer2D::render);
	mSlotConnector->Connect(
		mRenderWindow->GetInteractor(), vtkCommand::MouseMoveEvent, 
		this, SLOT(onMouseMove()));
}

ImageSliceViewer2D::~ImageSliceViewer2D() {}

void ImageSliceViewer2D::enableInputInteractions(const bool& isEnable) {
	if (isEnable) {
		vtkSmartPointer<vtkInteractorStyleImage> imStyle = vtkSmartPointer<vtkInteractorStyleImage>::New();
		mRenderWindow->GetInteractor()->SetInteractorStyle(imStyle);
	} else {
		mRenderWindow->GetInteractor()->SetInteractorStyle(Q_NULLPTR);
	}
}

void ImageSliceViewer2D::capModels() {
	vtkSmartPointer<vtkPlane> cutPlane = this->getCutPlane();

	QHash<QString, ModelPtr>::iterator it = mOriginalModels.begin();
	for (; it != mOriginalModels.end(); it++) {
		QSharedPointer<Model> cappedModel = mModels.value(it.key());
		if (cappedModel.isNull()) {
			qWarning() << "Trying to cap a model that is not existing in the list of models.";
			continue;
		}

		ModelDataVPtr tData = ModelAlgorithms::transform(it.value()->getModelData(), it.value()->getTransformCopy());
		cappedModel->setModelData(ModelAlgorithms::capClip(tData, cutPlane));
	}
}

void ImageSliceViewer2D::capModel(const QString& visualID) {
	ModelPtr originalModel = mOriginalModels.value(visualID);
	ModelPtr cappedModel = mModels.value(visualID);

	if (originalModel.isNull() || cappedModel.isNull()) {
		qWarning() << "Failed to cap model:" << visualID << "because the model was not found.";
		return;
	}

	vtkSmartPointer<vtkPlane> cutPlane = this->getCutPlane();
	ModelDataVPtr tData = ModelAlgorithms::transform(originalModel->getModelData(), originalModel->getTransformCopy());
	cappedModel->setModelData(ModelAlgorithms::capClip(tData, cutPlane));
}

void ImageSliceViewer2D::capModel(ModelPtr model) {
	if (model.isNull()) {
		qWarning() << "Failed to cap model because given model was null.";
		return;
	}

	ModelPtr originalModel = mOriginalModels.value(model->getVisualID());

	if (originalModel.isNull()) {
		qWarning() << "Failed to cap model=" << model->getVisualID() << "because the model was not found.";
		return;
	}

	vtkSmartPointer<vtkPlane> cutPlane = this->getCutPlane();
	ModelDataVPtr tData = ModelAlgorithms::transform(originalModel->getModelData(), originalModel->getTransformCopy());
	model->setModelData(ModelAlgorithms::capClip(tData, cutPlane));

}

void ImageSliceViewer2D::updateCapModel() {
	// TODO: Need to take into account that the children of this sender
	// also need to be capped.
	Visual* visual = qobject_cast<Visual*>(sender());

	if (visual != Q_NULLPTR) {
		this->capModel(visual->getVisualID());
	}
}

ModelDataVPtr ImageSliceViewer2D::getModelCappedData(const QString& visualID) {
	ModelPtr model = mModels.value(visualID, Q_NULLPTR);
	if (!model.isNull()) {
		return model->getModelData();
	} 
	return Q_NULLPTR;
}

ModelPtr ImageSliceViewer2D::addModel(const QString& visualID, ModelDataVPtr data) {
	ModelPtr cappedModel = Scene::addModel(visualID, data);
	if (cappedModel.isNull()) {
		return Q_NULLPTR;
	}

	ModelPtr originalModel(new Model(visualID, data));
	mOriginalModels.insert(visualID, originalModel);
	this->capModel(cappedModel);

	// Connect the changes to the original model to the capped version
	QObject::connect(
		originalModel.data(), &Model::changedOpacity,
		cappedModel.data(), &Model::setOpacity);
	QObject::connect(
		originalModel.data(), &Model::changedVisibleState,
		cappedModel.data(), &Model::setVisible);
	QObject::connect(
		originalModel.data(), &Model::changedPickableState,
		cappedModel.data(), &Model::setPickable);
	QObject::connect(
		originalModel.data(), &Model::changedHolographicState,
		cappedModel.data(), &Model::setHolographic);
	QObject::connect(
		originalModel.data(), &Model::changedSceneParent,
		cappedModel.data(), &Model::setParentVisual);
	QObject::connect(
		originalModel.data(), &Model::changedColor,
		cappedModel.data(), &Model::setColor);
	
	// The data change and transformation signals forces the capped model 
	// to be recapped with new data/transformation
	QObject::connect(
		originalModel.data(), &Model::changedTransformation, 
		this, &ImageSliceViewer2D::updateCapModel);
	QObject::connect(
		originalModel.data(), &Model::changedModelData,
		this, &ImageSliceViewer2D::updateCapModel);
	return originalModel;
}

bool ImageSliceViewer2D::addVisual(VisualPtr visual) {
	// If it's a model, we handle it. 
	if (visual->getVisualType() == EVisual::MODEL) {
		if (mVisuals.contains(visual->getVisualID())) {
			qWarning() << "Failed to add scene object=" << 
				visual->getVisualID() << "because that name is already taken";
			return false;
		}

		ModelPtr model = qobject_cast<ModelPtr>(visual);
		if (model.isNull()) {
			qCritical() << "Failed to add object because it claims to be a model although it's not.";
			return false;
		}

		ModelPtr cappedModel(new Model(model->getVisualID()));
		Scene::addVisual(cappedModel);
		mOriginalModels.insert(model->getVisualID(), model);
		this->capModel(cappedModel);
		
		//TODO: If parent moves, we would need to know to update cap as well
		cappedModel->setOpacity(model->getOpacity());
		cappedModel->setVisible(model->isVisible());
		cappedModel->setPickable(model->isPickable());
		cappedModel->setHolographic(model->isHolographic());
		double r, g, b;
		model->getColor(r, g, b);
		cappedModel->setColor(r, g, b);

		// Connect the changes to the original model to the capped version
		QObject::connect(
			model.data(), &Model::changedOpacity,
			cappedModel.data(), &Model::setOpacity);
		QObject::connect(
			model.data(), &Model::changedVisibleState,
			cappedModel.data(), &Model::setVisible);
		QObject::connect(
			model.data(), &Model::changedPickableState, 
			cappedModel.data(), &Model::setPickable);
		QObject::connect(
			model.data(), &Model::changedHolographicState,
			cappedModel.data(), &Model::setHolographic);
		QObject::connect(
			model.data(), &Model::changedColor,
			cappedModel.data(), &Model::setColor);

		// The data change and transformation signals forces the capped model 
		// to be recapped with new data/transformation
		QObject::connect(
			model.data(), &Model::changedTransformation, 
			this, &ImageSliceViewer2D::updateCapModel);
		QObject::connect(
			model.data(), &Model::changedModelData, 
			this, &ImageSliceViewer2D::updateCapModel);

		return true;
	} else {
		return Scene::addVisual(visual);
	}
}

VisualPtr ImageSliceViewer2D::getVisual(const QString& visualID) {
	VisualPtr visual = Scene::getVisual(visualID);
	if (!visual.isNull() && visual->getVisualType() == EVisual::MODEL) {
		return mOriginalModels.value(visual->getVisualID(), Q_NULLPTR);
	} else {
		return visual;
	}
}

Visual3DPtr ImageSliceViewer2D::getVisual3D(const QString& visualID) {
	Visual3DPtr visual = Scene::getVisual3D(visualID);
	if (!visual.isNull() && visual->getVisualType() == EVisual::MODEL) {
		return mOriginalModels.value(visual->getVisualID(), Q_NULLPTR);
	} else {
		return visual;
	}
}

ModelPtr ImageSliceViewer2D::getModel(const QString& visualID) {
	return mOriginalModels.value(visualID, Q_NULLPTR);
}

QList<Visual3DPtr> ImageSliceViewer2D::getVisuals3D() {
	// TODO: There was a bug here, it was fixed but need to test the fix
	QList<Visual3DPtr> visuals = Scene::getVisuals3D();
	for (int i = visuals.length() - 1; i >= 0; i--) {
		if (visuals[i]->getVisualType() == EVisual::MODEL) {
			visuals.replace(i, mOriginalModels.value(visuals[i]->getVisualID()));
		}
	}
	return visuals;
}

QList<ModelPtr> ImageSliceViewer2D::getModels() {
	return mOriginalModels.values();
}

void ImageSliceViewer2D::removeVisual(const QString& visualID) {
	mOriginalModels.remove(visualID);
	Scene::removeVisual(visualID);
}

void ImageSliceViewer2D::clearScene() {
	mOriginalModels.clear();
	Scene::clearScene();
}

void ImageSliceViewer2D::setVisualVisible(const bool& isVisible, const QString& visualID) {
	if (mOriginalModels.contains(visualID)) {
		mOriginalModels.value(visualID)->setVisible(isVisible);
	} else {
		Scene::setVisualVisible(isVisible, visualID);
	}
}

void ImageSliceViewer2D::setVisualPickable(const bool& isPickable, const QString& visualID) {
	if (mOriginalModels.contains(visualID)) {
		mOriginalModels.value(visualID)->setPickable(isPickable);
	} else {
		Scene::setVisualPickable(isPickable, visualID);
	}
}

void ImageSliceViewer2D::setVisualOpacity(const double& a, const QString& visualID) {
	if (mOriginalModels.contains(visualID)) {
		mOriginalModels.value(visualID)->setOpacity(a);
	} else {
		Scene::setVisualOpacity(a, visualID);
	}
}

void ImageSliceViewer2D::resetVisualTransform(const QString& visualID) {
	if (mOriginalModels.contains(visualID)) {
		mOriginalModels.value(visualID)->resetTransform();
	} else {
		Scene::resetVisualTransform(visualID);
	}
}

void ImageSliceViewer2D::translateVisual(const double& x, const double& y, const double& z, const QString& visualID) {
	if (mOriginalModels.contains(visualID)) {
		mOriginalModels.value(visualID)->translate(x, y, z);
	} else {
		Scene::translateVisual(x, y, z, visualID);
	}
}

void ImageSliceViewer2D::rotateVisual(const double& x, const double& y, const double& z, const QString& visualID) {
	if (mOriginalModels.contains(visualID)) {
		mOriginalModels.value(visualID)->rotate(x, y, z);
	} else {
		Scene::rotateVisual(x, y, z, visualID);
	}
}

void ImageSliceViewer2D::rotateXVisual(const double& x, const QString& visualID) {
	if (mOriginalModels.contains(visualID)) {
		mOriginalModels.value(visualID)->rotateX(x);
	} else {
		Scene::rotateXVisual(x, visualID);
	}
}

void ImageSliceViewer2D::rotateYVisual(const double& y, const QString& visualID) {
	if (mOriginalModels.contains(visualID)) {
		mOriginalModels.value(visualID)->rotateY(y);
	} else {
		Scene::rotateYVisual(y, visualID);
	}
}

void ImageSliceViewer2D::rotateZVisual(const double& z, const QString& visualID) {
	if (mOriginalModels.contains(visualID)) {
		mOriginalModels.value(visualID)->rotateZ(z);
	} else {
		Scene::rotateZVisual(z, visualID);
	}
}

void ImageSliceViewer2D::setModelData(ModelDataVPtr data, const QString& modelName) {
	if (mOriginalModels.contains(modelName)) {
		mOriginalModels.value(modelName)->setModelData(data);
	} else {
		Scene::setModelData(data, modelName);
	}
}

void ImageSliceViewer2D::setModelColor(const double& r, const double& g, const double& b, const QString& modelName) {
	if (mOriginalModels.contains(modelName)) {
		mOriginalModels.value(modelName)->setColor(r, g, b);
	} else {
		Scene::setModelColor(r, g, b, modelName);
	}
}

void ImageSliceViewer2D::onMouseMove() {
	int* pos = mRenderWindow->GetInteractor()->GetEventPosition();
	int i, j, k;
	i = j = k = 0;

	if (this->getVoxelFromFrameCoordinate(pos[0], pos[1], i, j, k)) {
		double x, y, z;
		this->getVoxelCoordinates(i, j, k, x, y, z);

		double intensity = mImageData->GetScalarComponentAsDouble(i, j, k, 0);

		QString coord = 
			tr("I: %1 J: %2 K: %3\nX: %4 mm Y: %5 mm Z: %6 mm\nIntensity:%7").
			arg(i).arg(j).arg(k).arg(x).arg(y).arg(z).arg(intensity);
		mCoordinateText2D->setText(coord);
	}
	this->render();
}

void ImageSliceViewer2D::setImageData(ImageDataVPtr data) {
	if (mImageData == data) {
		return;
	}

	if (data.Get() == Q_NULLPTR) {
		mImageData = ImageDataVPtr::New();
	} else {
		mImageData = data;
	}

	mViewer->SetInputData(mImageData);
	this->calculateSliceIndices();

	switch (mOrientation.toInt()) {
		case ESliceOrientation::XY:
			mViewer->SetSlice(mOrientationSliceIndices.transverseIndex);
			break;
		case ESliceOrientation::YZ:
			mViewer->SetSlice(mOrientationSliceIndices.sagittalIndex);
			break;
		case ESliceOrientation::XZ:
			mViewer->SetSlice(mOrientationSliceIndices.coronalIndex);
			break;
	}

	mCoordinateText2D->setText("Hover over image for coordinate information");
	this->capModels();

	QString sliceText = tr("Slice %1/%2")
		.arg(mViewer->GetSlice())
		.arg(mViewer->GetSliceMax() - mViewer->GetSliceMin());
	mSliceIndexText2D->setText(sliceText);

	emit changedImageData(mImageData);
	emit changedSliceIndices(this->getSliceMinIndex(), this->getSliceMaxIndex());
	emit changedSlice(this->getSliceIndex(), mOrientation);
}

void ImageSliceViewer2D::setMessageText(const QString& messageText) {
	if (mMessageText2D->getText() == messageText) {
		return;
	}

	mMessageText2D->setText(messageText);
	emit changedMessageText(messageText);
}

void ImageSliceViewer2D::setLabelsVisible(const bool& areVisible) {
	mCoordinateText2D->setVisible(areVisible);
	mMessageText2D->setVisible(areVisible);
	mSliceIndexText2D->setVisible(areVisible);
	mOrientationText2D->setVisible(areVisible);
}

ImageDataVPtr ImageSliceViewer2D::getImageData() {
	return mImageData;
}

void ImageSliceViewer2D::setOrientationXY() {
	mOrientation = ESliceOrientation::XY;
	this->applyOrientationChange();
}

void ImageSliceViewer2D::setOrientationYZ() {
	mOrientation = ESliceOrientation::YZ;
	this->applyOrientationChange();
}

void ImageSliceViewer2D::setOrientationXZ() {
	mOrientation = ESliceOrientation::XZ;
	this->applyOrientationChange();
}

void ImageSliceViewer2D::setOrientation(ESliceOrientation orientation) {
	mOrientation = orientation;
	this->applyOrientationChange();
}

void ImageSliceViewer2D::setNextOrientation() {
	switch (this->getSliceOrientation().toInt()) {
		case ESliceOrientation::XY:
			this->setOrientationYZ();
			break;
		case ESliceOrientation::YZ:
			this->setOrientationXZ();
			break;
		case ESliceOrientation::XZ:
			this->setOrientationXY();
			break;
	}
}

void ImageSliceViewer2D::setPreviousOrientation() {
	switch (this->getSliceOrientation().toInt()) {
		case ESliceOrientation::XY:
			this->setOrientationXZ();
			break;
		case ESliceOrientation::YZ:
			this->setOrientationXY();
			break;
		case ESliceOrientation::XZ:
			this->setOrientationYZ();
			break;
	}
}

void ImageSliceViewer2D::applyOrientationChange() {
	// set the viewer to the right orientation
	switch (mOrientation.toInt()) {
		case ESliceOrientation::XY:
			mViewer->SetSliceOrientationToXY();
			mViewer->SetSlice(mOrientationSliceIndices.transverseIndex);
			break;
		case ESliceOrientation::YZ:
			mViewer->SetSliceOrientationToYZ();
			mViewer->SetSlice(mOrientationSliceIndices.sagittalIndex);
			break;
		case ESliceOrientation::XZ:
			mViewer->SetSliceOrientationToXZ();
			mViewer->SetSlice(mOrientationSliceIndices.coronalIndex);
			break;
	}
	
	mOrientationText2D->setText(mOrientation.getName());
	QString sliceText = tr("Slice %1/%2")
		.arg(mViewer->GetSlice())
		.arg(mViewer->GetSliceMax() - mViewer->GetSliceMin());
	mSliceIndexText2D->setText(sliceText);
	this->render();
	this->capModels();
	
	emit changedOrientation(mOrientation);
	emit changedSliceIndices(this->getSliceMinIndex(), this->getSliceMaxIndex());
	emit changedSlice(this->getSliceIndex(), mOrientation);
}

ESliceOrientation ImageSliceViewer2D::getSliceOrientation() const {
	return this->mOrientation;
}

void ImageSliceViewer2D::setSlice(const int& sliceIndex, const ESliceOrientation& orientation) {
	qDebug() << "Enter - Setting slice" << sliceIndex << 
		"with orientation" << orientation.getName() << 
		"of ImageSliceViewer2D" << this->getSceneID();

	// if bad orientation, bad slice index, or given values represent
	// current values, do nothing
	if (orientation == ESliceOrientation::UNKNOWN ||
		!this->isSliceIndexInRange(sliceIndex) ||
		(orientation == mOrientation && sliceIndex == this->getSliceIndex())) 
	{
		qDebug() << "Failed to set slice";
		return;
	}

	bool emitOrientationChange = false;
	if (orientation != mOrientation) {
		emitOrientationChange = true;
		mOrientation = orientation;
		mOrientationText2D->setText(mOrientation.getName());
	}

	switch (mOrientation.toInt()) {
		case ESliceOrientation::XY:
			mViewer->SetSliceOrientationToXY();
			mOrientationSliceIndices.transverseIndex = sliceIndex;
			mViewer->SetSlice(mOrientationSliceIndices.transverseIndex);
			break;
		case ESliceOrientation::YZ:
			mViewer->SetSliceOrientationToYZ();
			mOrientationSliceIndices.sagittalIndex = sliceIndex;
			mViewer->SetSlice(mOrientationSliceIndices.sagittalIndex);
			break;
		case ESliceOrientation::XZ:
			mViewer->SetSliceOrientationToXZ();
			mOrientationSliceIndices.coronalIndex = sliceIndex;
			mViewer->SetSlice(mOrientationSliceIndices.coronalIndex);
			break;
	}

	QString sliceText = tr("Slice %1/%2")
		.arg(mViewer->GetSlice())
		.arg(mViewer->GetSliceMax() - mViewer->GetSliceMin());
	mSliceIndexText2D->setText(sliceText);

	this->capModels();

	if (emitOrientationChange) {
		emit changedOrientation(mOrientation);
		emit changedSliceIndices(this->getSliceMinIndex(), this->getSliceMaxIndex());
	}

	emit changedSlice(this->getSliceIndex(), mOrientation);
	qDebug() << "Exit";
}

void ImageSliceViewer2D::setSliceIndex(const int& sliceIndex) {
	qDebug() << "Enter - Setting slice" << sliceIndex << 
		"to current orientation" << mOrientation.getName() <<
		"of ImageSliceViewer2D" << this->getSceneID();

	if (sliceIndex == this->getSliceIndex()) {
		return;
	}

	mViewer->SetSlice(sliceIndex);
	
	switch (mOrientation.toInt()) {
		case ESliceOrientation::XY:
			mOrientationSliceIndices.transverseIndex = sliceIndex;
			break;
		case ESliceOrientation::YZ:
			mOrientationSliceIndices.sagittalIndex = sliceIndex;
			break;
		case ESliceOrientation::XZ:
			mOrientationSliceIndices.coronalIndex = sliceIndex;
			break;
	}

	QString sliceText = tr("Slice %1/%2")
		.arg(mViewer->GetSlice())
		.arg(mViewer->GetSliceMax() - mViewer->GetSliceMin());
	mSliceIndexText2D->setText(sliceText);

	this->capModels();
	emit changedSlice(sliceIndex, mOrientation);
	qDebug() << "Exit";
}

void ImageSliceViewer2D::setSliceIndexToBeginning() {
	this->setSliceIndex(0);
}

void ImageSliceViewer2D::setSliceIndexToMiddle() {
	this->setSliceIndex((this->getSliceMaxIndex() - this->getSliceMinIndex()) / 2);
}

void ImageSliceViewer2D::setSliceIndexToEnd() {
	this->setSliceIndex(this->getSliceMaxIndex());
}

void ImageSliceViewer2D::setSliceIndexToNext() {
	if (this->getSliceIndex() == this->getSliceMaxIndex()) {
		return;
	}
	this->setSliceIndex(this->getSliceIndex() + 1);
}

void ImageSliceViewer2D::setSliceIndexToPrevious() {
	if (this->getSliceIndex() == this->getSliceMinIndex()) {
		return;
	}
	this->setSliceIndex(this->getSliceIndex() - 1);
}

int ImageSliceViewer2D::getSliceIndex() const {
	return mViewer->GetSlice();
}

int ImageSliceViewer2D::getSliceMinIndex() const {
	return mViewer->GetSliceMin();
}

int ImageSliceViewer2D::getSliceMaxIndex() const {
	return mViewer->GetSliceMax();
}

void ImageSliceViewer2D::getSliceRange(int& min, int& max) const {
	min = this->getSliceMinIndex();
	max = this->getSliceMaxIndex();
}

bool ImageSliceViewer2D::isSliceIndexInRange(const int& sliceIndex) {
	int min, max;
	this->getSliceRange(min, max);

	if (sliceIndex < min || sliceIndex > max) {
		return false;
	}

	return true;
}

vtkSmartPointer<vtkPlane> ImageSliceViewer2D::getCutPlane() const {
	vtkSmartPointer<vtkPlane> cutPlane = vtkSmartPointer<vtkPlane>::New();
	
	double *origin = mViewer->GetInput()->GetOrigin();
	double *spacing = mViewer->GetInput()->GetSpacing();

	// A small value, 0.01, is added to offset the cut plane slightly. 
	// This is so that the cut models get rendered in-front of the image.
	switch (mOrientation.toInt()) {
		case ESliceOrientation::XY:
			cutPlane->SetOrigin(
				origin[0], 
				origin[1], 
				origin[2] + spacing[2] * (this->getSliceIndex() + 0.01));
			cutPlane->SetNormal(0.0, 0.0, 1.0);
			break;
		case ESliceOrientation::YZ:
			cutPlane->SetOrigin(
				origin[0] + spacing[0] * (this->getSliceIndex() + 0.01),
				origin[1],
				origin[2]);
			cutPlane->SetNormal(1.0, 0.0, 0.0);
			break;
		case ESliceOrientation::XZ:
			cutPlane->SetOrigin(
				origin[0], 
				origin[1] + spacing[1] * (this->getSliceIndex() - 0.01),
				origin[2]);
			cutPlane->SetNormal(0.0, -1.0, 0.0);
			break;
	}

	return cutPlane;
}

bool ImageSliceViewer2D::getVoxelCoordinates(const int& i, const int& j, 
	const int& k, double& x, double& y, double& z) const 
{
	// if indices are negative, they are bad
	if (i < 0 || j < 0 || k < 0) {
		return false;
	}
	
	// if an index is out of range with their corresponding dimension, 
	// they are bad
	int* dims = mViewer->GetInput()->GetDimensions();
	if (i >= dims[0] || j >= dims[1] || k >= dims[2]) {
		return false;
	}

	double* spacing = mViewer->GetInput()->GetSpacing();
	double* origin = mViewer->GetInput()->GetOrigin();

	// Compute its (x, y, z) based on the data origin and spacing
	x = origin[0] + i * spacing[0];
	y = origin[1] + j * spacing[1];
	z = origin[2] + k * spacing[2];

	return true;
}

bool ImageSliceViewer2D::getVoxelFromCoordinates(const double & x, 
	const double & y, const double & z, int & i, int & j, int & k) const 
{
	double* spacing = mViewer->GetInput()->GetSpacing();
	double* origin = mViewer->GetInput()->GetOrigin();

	// dummy variables to store computed i, j, ks
	int ii, ij, ik;
	ii = (x - origin[0]) / spacing[0];
	ij = (y - origin[1]) / spacing[1];
	ik = (z - origin[2]) / spacing[2];

	// if calculated indices are negative, they are bad
	if (ii < 0 || ij < 0 || ik < 0) {
		return false;
	}

	// if a calculated index is out of range with their corresponding 
	// dimension, they are bad
	int* dims = mViewer->GetInput()->GetDimensions();
	if (ii >= dims[0] || ij >= dims[1] || ik >= dims[2]) {
		return false;
	}

	// Indices are good, assign the index values to the out variables
	i = ii;
	j = ij;
	k = ik;
	return true;
}

bool ImageSliceViewer2D::getVoxelFromFrameCoordinate(const int& x, const int& y, int& i, int& j, int& k) {
	vtkNew<vtkPropPicker> propPicker;
	propPicker->Pick(x, y, 0, mRenderer);
	vtkProp* pickedActor = propPicker->GetViewProp();
	if (pickedActor == mViewer->GetImageActor()) {
		vtkNew<vtkCellPicker> cellPicker;
		cellPicker->Pick(x, y, 0, mRenderer);
		cellPicker->GetPointIJK(i, j, k);

		// call render because the pickers sometimes make the screen dark.
		this->render();			
		return true;
	}

	// call render because the pickers sometime make the screen dark.
	this->render();
	return false;
}

bool ImageSliceViewer2D::getCoordinatesFromFrameCoordinate(const int& wx, 
	const int& wy, double& x, double& y, double& z) 
{
	int i, j, k;
	if (this->getVoxelFromFrameCoordinate(wx, wy, i, j, k)) {
		this->getVoxelCoordinates(i, j, k, x, y, z);
		return true;
	}
	return false;
}

void ImageSliceViewer2D::calculateSliceIndices() {
	int* dimensions = mViewer->GetInput()->GetDimensions();

	mOrientationSliceIndices.transverseIndex = dimensions[2] / 2;
	mOrientationSliceIndices.sagittalIndex = dimensions[0] / 2;
	mOrientationSliceIndices.coronalIndex = dimensions[1] / 2;
}
