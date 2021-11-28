#include <fi3d/rendering/scenes/Scene.h>

#include <fi3d/logger/Logger.h>

#include <QVTKOpenGLWidget.h>
#include <QVTKInteractor.h>
#include <QTimer>
#include <QTime>

#include <vtkAxesActor.h>
#include <vtkEventQtSlotConnect.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkInteractorStyle.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkRenderer.h>

using namespace fi3d;

Scene::Scene(QVTKOpenGLWidget* widget, const QString& id)
	: QObject(Q_NULLPTR),
	ModuleElement(),
	mSceneID(id),
	mWidget(widget),
	mRenderWindow(vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New()),
	mRenderer(vtkSmartPointer<vtkRenderer>::New()),
	mInteractor(vtkSmartPointer<QVTKInteractor>::New()),
	mInteractorStyle(vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New()),
	mSlotConnector(vtkSmartPointer<vtkEventQtSlotConnect>::New()),
	mAxesVisual(vtkSmartPointer<vtkAxesActor>::New()),
	mAreAxesVisible(true),
	mVisuals(), mVisuals3D(),
	mImageSlices(), mStudySlices(), mModels(), mAnimatedStudySlices(),
	mAnimatedModels(), mAssemblies(), mSubtitles(),
	mActorsToNames(),
	mTimeFromLastRender(0), mIsRenderScheduled(false)
{
	mRenderWindow->AddRenderer(mRenderer);
	mRenderWindow->SetInteractor(mInteractor);
	mWidget->SetRenderWindow(mRenderWindow);
	mInteractor->SetInteractorStyle(mInteractorStyle);
	mInteractor->Initialize();

	mAxesVisual->SetTotalLength(50, 50, 50);
	mAxesVisual->PickableOff();
	mAxesVisual->AxisLabelsOff();

	mRenderer->AddActor(mAxesVisual);

	// Changes to this scene that should enforce a refresh
	QObject::connect(
		this, &Scene::changedAddedSceneObject, 
		this, &Scene::render);
	QObject::connect(
		this, &Scene::changedRemovedSceneObject, 
		this, &Scene::render);
	QObject::connect(
		this, &Scene::changedBackgroundColor, 
		this, &Scene::render);
	QObject::connect(
		this, &Scene::changedAxesVisibility, 
		this, &Scene::render);
	QObject::connect(
		this, &Scene::changedCameraReset, 
		this, &Scene::render);
}

Scene::~Scene() {}

QString Scene::getSceneID() const {
	return mSceneID;
}

VisualPtr Scene::addVisual(const QString& visualID, const EVisual& visualType) {
	qDebug() << "Enter - Creating Visual=" << visualID << "of type=" << 
		visualType.getName() << "to scene=" << mSceneID;

	if (mVisuals.contains(visualID)) {
		qWarning() << "Failed to add Visual. An object with that name already exists";
		return Q_NULLPTR;
	}

	VisualPtr obj = Q_NULLPTR;
	switch (visualType.toInt()) {
		case EVisual::IMAGE_SLICE:
			obj = this->addImageSlice(visualID);
			break;
		case EVisual::STUDY_IMAGE_SLICE:
			obj = this->addStudyImageSlice(visualID);
			break; 
		case EVisual::MODEL:
			obj = this->addModel(visualID);
			break;
		case EVisual::ASSEMBLY:
			obj = this->addAssembly(visualID);
			break;
		case EVisual::SUBTITLE:
			obj = this->addSubtitle(visualID);
			break;
	}
	qDebug() << "Exit";
	return obj;
}

ImageSlicePtr Scene::addImageSlice(const QString& visualID, const ESliceOrientation& orientation, ImageDataVPtr data) {
	qDebug() << "Enter - Creating ImageSlice=" << visualID << "and adding to scene=" << mSceneID;

	if (mVisuals.contains(visualID)) {
		qWarning() << "Failed to add ImageSlice. An object with that name already exists";
		return Q_NULLPTR;
	}
	ImageSlicePtr visual(new ImageSlice(visualID, orientation, data));
	this->addToManagement(visual);

	// Connect signals that should cause the scene to redraw
	this->connectImageSliceSignals(visual.data());

	emit changedAddedSceneObject(visual);
	qDebug() << "Exit";
	return visual;
}

StudySlicePtr Scene::addStudyImageSlice(const QString& visualID, const ESliceOrientation& orientation, StudyPtr data) {
	qDebug() << "Enter - Creating StudyImageSlice=" << visualID << "and adding to scene=" << mSceneID;

	if (mVisuals.contains(visualID)) {
		qWarning() << "Failed to add StudyImageSlice. An object with that name already exists";
		return Q_NULLPTR;
	}
	StudySlicePtr visual(new StudySlice(visualID, orientation, data));
	this->addToManagement(visual);

	// Connect signals that should cause the scene to redraw
	this->connectStudySliceSignals(visual.data());

	emit changedAddedSceneObject(visual);
	qDebug() << "Exit";
	return visual;
}

ModelPtr Scene::addModel(const QString& visualID, ModelDataVPtr data) {
	qDebug() << "Enter - Creating Model=" << visualID << "and adding to scene=" << mSceneID;

	if (mVisuals.contains(visualID)) {
		qWarning() << "Failed to add Model. An object with that name already exists";
		return Q_NULLPTR;
	}
	ModelPtr visual(new Model(visualID, data));
	this->addToManagement(visual);

	// Connect signals that should cause the scene to redraw
	this->connectModelSignals(visual.data());
	
	emit changedAddedSceneObject(visual);
	qDebug() << "Exit";
	return visual;
}

AnimatedStudySlicePtr Scene::addAnimatedStudySlice(const QString& visualID, 
	const ESliceOrientation& orientation, StudyPtr data) 
{
	qDebug() << "Enter - Creating SceneAnimatedImageSlice=" << visualID << "and adding to scene=" << mSceneID;

	if (mVisuals.contains(visualID)) {
		qWarning() << "Failed to add AnimatedImageSlice. An object with that name already exists.";
		return Q_NULLPTR;
	}

	AnimatedStudySlicePtr visual(new AnimatedStudySlice(visualID, orientation, data));
	this->addToManagement(visual);

	// Connect signals that should cause the scene to redraw. 
	// AnimatedStudyImageSlice has no new signals that should cause a redraw.
	this->connectStudySliceSignals(visual.data());

	emit changedAddedSceneObject(visual);
	qDebug() << "Exit";
	return visual;
}

AnimatedModelPtr Scene::addAnimatedModel(const QString& visualID, AnimatedModelDataPtr data) {
	qDebug() << "Enter - Creating VisualAnimatedModel=" << visualID << "and adding to scene=" << mSceneID;

	if (mVisuals.contains(visualID)) {
		qWarning() << "Failed to add AnimatedModel. An object with that name already exists";
		return Q_NULLPTR;
	}

	AnimatedModelPtr visual(new AnimatedModel(visualID, data));
	this->addToManagement(visual);

	// Connect signals that should cause the scene to redraw. 
	// AnimatedModel has no new signals that should cause a redraw.
	this->connectModelSignals(visual.data());

	emit changedAddedSceneObject(visual);
	qDebug() << "Exit";
	return visual;
}

AssemblyPtr Scene::addAssembly(const QString& visualID) {
	qDebug() << "Enter - Creating Assembly=" << visualID << "and adding to scene:" << mSceneID;

	if (mVisuals.contains(visualID)) {
		qWarning() << "Failed to add Assembly. An object with that name already exists";
		return Q_NULLPTR;
	}
	AssemblyPtr visual(new Assembly(visualID));
	this->addToManagement(visual);

	// Connect signals that should cause the scene to redraw
	this->connectAssemblySignals(visual.data());
	
	emit changedAddedSceneObject(visual);
	qDebug() << "Exit";
	return visual;
}

SubtitlePtr Scene::addSubtitle(const QString& visualID, const QString& text) {
	qDebug() << "Enter - Creating Subtitle=" << visualID << "and adding to scene:" << mSceneID;

	if (mVisuals.contains(visualID)) {
		qWarning() << "Failed to add Subtitle. An object with that name already exists";
		return Q_NULLPTR;
	}
	SubtitlePtr visual(new Subtitle(visualID, text));
	this->addToManagement(visual);

	// Connect signals that should cause the scene to redraw
	this->connectSubtitleSignals(visual.data());
	
	emit changedAddedSceneObject(visual);
	qDebug() << "Exit";
	return visual;
}

bool Scene::addVisual(VisualPtr visual) {
	qDebug() << "Enter - Adding Visual=" << visual->getVisualID() <<
		"of type=" << visual->getVisualType().getName() << "to scene=" <<
		mSceneID;

	if (mVisuals.contains(visual->getVisualID())) {
		qWarning() << "Failed to add scene object because its name is already taken";
		return false;
	}

	if (visual->getVisualType() == EVisual::IMAGE_SLICE) {
		ImageSlicePtr imageSlice = qobject_cast<ImageSlicePtr>(visual);
		this->connectImageSliceSignals(imageSlice.data());
	} else if (visual->getVisualType() == EVisual::STUDY_IMAGE_SLICE) {
		StudySlicePtr studyImageSlice = qobject_cast<StudySlicePtr>(visual);
		this->connectStudySliceSignals(studyImageSlice.data());
	} else if (visual->getVisualType() == EVisual::MODEL) {
		ModelPtr model = qobject_cast<ModelPtr>(visual);
		this->connectModelSignals(model.data());
	} else if (visual->getVisualType() == EVisual::ANIMATED_STUDY_SLICE) {
		AnimatedStudySlicePtr animatedSlice = 			qobject_cast<AnimatedStudySlicePtr>(visual);
		this->connectStudySliceSignals(animatedSlice.data());
	} else if (visual->getVisualType() == EVisual::ANIMATED_MODEL) {
		AnimatedModelPtr animatedModel = qobject_cast<AnimatedModelPtr>(visual);
		this->connectModelSignals(animatedModel.data());
	} else if (visual->getVisualType() == EVisual::ASSEMBLY) {
		AssemblyPtr assembly = qobject_cast<AssemblyPtr>(visual);
		this->connectAssemblySignals(assembly.data());
	} else if (visual->getVisualType() == EVisual::SUBTITLE) {
		SubtitlePtr subtitle = qobject_cast<SubtitlePtr>(visual);
		this->connectSubtitleSignals(subtitle.data());
	} else {
		return false;
	}

	this->addToManagement(visual);
	emit changedAddedSceneObject(visual);

	qDebug() << "Exit";
	return true;
}

bool Scene::isVisualIDTaken(const QString& visualID) {
	return mVisuals.contains(visualID);
}

VisualPtr Scene::getVisual(const QString& visualID) {
	return mVisuals.value(visualID, Q_NULLPTR);
}

Visual3DPtr Scene::getVisual3D(const QString& visualID) {
	return mVisuals3D.value(visualID, Q_NULLPTR);
}

ImageSlicePtr Scene::getImageSlice(const QString& visualID) {
	return mImageSlices.value(visualID, Q_NULLPTR);
}

StudySlicePtr Scene::getStudyImageSlice(const QString& visualID) {
	return mStudySlices.value(visualID, Q_NULLPTR);
}

ModelPtr Scene::getModel(const QString& visualID) {
	return mModels.value(visualID, Q_NULLPTR);
}

AnimatedModelPtr Scene::getAnimatedModel(const QString& visualID) {
	return mAnimatedModels.value(visualID, Q_NULLPTR);
}

AnimatedStudySlicePtr Scene::getAnimatedStudySlice(const QString& visualID) {
	return mAnimatedStudySlices.value(visualID, Q_NULLPTR);
}

AssemblyPtr Scene::getAssembly(const QString& visualID) {
	return mAssemblies.value(visualID, Q_NULLPTR);
}

SubtitlePtr Scene::getSubtitle(const QString& visualID) {
	return mSubtitles.value(visualID, Q_NULLPTR);
}

QString Scene::getVisualIDByProp(vtkProp* prop) const {
	return mActorsToNames.value(prop, "");
}

EVisual Scene::getVisualType(const QString& visualID) const {
	VisualPtr obj = mVisuals.value(visualID, Q_NULLPTR);
	if (!obj.isNull()) {
		return obj->getVisualType();
	} else {
		return EVisual::UNKNOWN;
	}
}

QList<VisualPtr> Scene::getVisuals() {
	return mVisuals.values();
}

QList<Visual3DPtr> Scene::getVisuals3D() {
	return mVisuals3D.values();
}

QList<ImageSlicePtr> Scene::getImageSlices() {
	return mImageSlices.values();
}

QList<StudySlicePtr> Scene::getStudySlices() {
	return mStudySlices.values();
}

QList<ModelPtr> Scene::getModels() {
	return mModels.values();
}

QList<AnimatedModelPtr> Scene::getAnimatedModels() {
	return mAnimatedModels.values();
}

QList<AnimatedStudySlicePtr> Scene::getAnimatedStudySlices() {
	return mAnimatedStudySlices.values();
}

QList<AssemblyPtr> Scene::getAssemblies() {
	return mAssemblies.values();
}

QList<SubtitlePtr> Scene::getSubtitles() {
	return mSubtitles.values();
}

QList<QString> Scene::getVisualList() const {
	return mVisuals.keys();
}

QList<QString> Scene::getVisual3DList() const {
	return mVisuals3D.keys();
}

QList<QString> Scene::getImageSliceList() const {
	return mImageSlices.keys();
}

QList<QString> Scene::getStudySliceList() const {
	return mStudySlices.keys();
}

QList<QString> Scene::getModelList() const {
	return mModels.keys();
}

QList<QString> Scene::getAnimatedModelList() const {
	return mAnimatedModels.keys();
}

QList<QString> Scene::getAnimatedStudySliceList() const {
	return mAnimatedStudySlices.keys();
}

QList<QString> Scene::getAssemblyList() const {
	return mAssemblies.keys();
}

QList<QString> Scene::getSubtitleList() const {
	return mSubtitles.keys();
}

void Scene::removeVisual(const QString& visualID) {
	qDebug() << "Enter - removing Visual=" << visualID << "from scene=" << mSceneID;

	if (!mVisuals.contains(visualID)) {
		qWarning() << "No scene object was removed because no object with that name was found";
		return;
	}

	VisualPtr visual = mVisuals.value(visualID);

	switch (visual->getVisualType().toInt()) {
		case EVisual::IMAGE_SLICE:
			this->disconnectImageSliceSignals(mImageSlices.value(visualID).data());
			break;
		case EVisual::STUDY_IMAGE_SLICE:
			this->disconnectStudySliceSignals(mStudySlices.value(visualID).data());
			break;
		case EVisual::MODEL:
			this->disconnectModelSignals(mModels.value(visualID).data());
			break;
		case EVisual::ANIMATED_STUDY_SLICE:
			this->disconnectStudySliceSignals(mAnimatedStudySlices.value(visualID).data());
			break;
		case EVisual::ANIMATED_MODEL:
			this->disconnectModelSignals(mAnimatedModels.value(visualID).data());
			break;
		case EVisual::ASSEMBLY:
			this->disconnectAssemblySignals(mAssemblies.value(visualID).data());
			break;
		case EVisual::SUBTITLE:
			this->disconnectSubtitleSignals(mSubtitles.value(visualID).data());
			break;
	}

	this->removeFromManagement(visual);

	emit changedRemovedSceneObject(visual);
	qDebug() << "Exit - Removed Visual of type=" << visual->getVisualType().getName();
}

void Scene::clearScene() {
	qDebug() << "Enter";
	
	/*
	Some objects are in more than one list, so the order of removal is 
	important! e.g., A StudySlice is also an ImageSlice. Thus, StudySlice 
	should be removed first so it's removed from the ImageSlice list and 
	avoid leakage.
	*/

	auto animatedSliceIt = mAnimatedStudySlices.begin();
	for (; animatedSliceIt != mAnimatedStudySlices.end(); animatedSliceIt++) {
		mRenderer->RemoveActor(animatedSliceIt.value()->getActor());
		this->disconnectStudySliceSignals(animatedSliceIt.value().data());

		// Remove from ImageSlices Hash so it is not processed again 
		mImageSlices.remove(animatedSliceIt.value()->getVisualID());		

		// Remove from StudySlices Hash so it is not processed again 
		mStudySlices.remove(animatedSliceIt.value()->getVisualID());		

		emit changedRemovedSceneObject(animatedSliceIt.value());
	}
	mAnimatedStudySlices.clear();
	
	auto studySliceIt = mStudySlices.begin();
	for (; studySliceIt != mStudySlices.end(); studySliceIt++) {
		mRenderer->RemoveActor(studySliceIt.value()->getActor());
		this->disconnectImageSliceSignals(studySliceIt.value().data());
		
		// Remove from ImageSlices Hash so it is not processed again 
		mImageSlices.remove(studySliceIt.value()->getVisualID());	

		emit changedRemovedSceneObject(studySliceIt.value());
	}
	mStudySlices.clear();

	auto imageSliceIt = mImageSlices.begin();
	for (; imageSliceIt != mImageSlices.end(); imageSliceIt++) {
		mRenderer->RemoveActor(imageSliceIt.value()->getActor());
		this->disconnectImageSliceSignals(imageSliceIt.value().data());

		emit changedRemovedSceneObject(imageSliceIt.value());
	}
	mImageSlices.clear();

	auto animatedModelIt = mAnimatedModels.begin();
	for (; animatedModelIt != mAnimatedModels.end(); animatedModelIt++) {
		mRenderer->RemoveActor(animatedModelIt.value()->getActor());
		this->disconnectModelSignals(animatedModelIt.value().data());

		// Remove from Models hash so it is not processed again on the next loop
		mModels.remove(animatedModelIt.value()->getVisualID());		

		emit changedRemovedSceneObject(animatedModelIt.value());
	}
	mAnimatedModels.clear();

	auto modelsIt = mModels.begin();
	for (; modelsIt != mModels.end(); modelsIt++) {
		mRenderer->RemoveActor(modelsIt.value()->getActor());
		this->disconnectModelSignals(modelsIt.value().data());

		emit changedRemovedSceneObject(modelsIt.value());
	}
	mModels.clear();

	auto assembliesIt= mAssemblies.begin();
	for (; assembliesIt != mAssemblies.end(); assembliesIt++) {
		mRenderer->RemoveActor(assembliesIt.value()->getActor());
		this->disconnectAssemblySignals(assembliesIt.value().data());

		emit changedRemovedSceneObject(assembliesIt.value());
	}
	mAssemblies.clear();

	auto subsIt = mSubtitles.begin();
	for (; subsIt != mSubtitles.end(); subsIt++) {
		mRenderer->RemoveActor2D(subsIt.value()->getActor());
		this->disconnectSubtitleSignals(subsIt.value().data());

		emit changedRemovedSceneObject(subsIt.value());
	}
	mSubtitles.clear();

	mVisuals.clear();
	mVisuals3D.clear();
	mActorsToNames.clear();
	qDebug() << "Exit";
}

void Scene::setVisualVisible(const bool& isVisible, const QString& visualID) {
	VisualPtr visual = this->getVisual(visualID);
	if (!visual.isNull()) {
		visual->setVisible(isVisible);
	} else {
		qDebug() << "Failed to set object's visibility because Visual=" << visualID << "was not found";
	}
}

void Scene::setVisualPickable(const bool& isPickable, const QString& visualID) {
	VisualPtr visual = this->getVisual(visualID);
	if (!visual.isNull()) {
		visual->setPickable(isPickable);
	} else {
		qDebug() << "Failed to set object's pick-ability because Visual=" << visualID << "was not found";
	}

}

void Scene::setVisualOpacity(const double& a, const QString& visualID) {
	VisualPtr visual = this->getVisual(visualID);
	if (!visual.isNull()) {
		visual->setOpacity(a);
	} else {
		qDebug() << "Failed to set object's opacity because Visual=" << visualID << "was not found";
	}
}

void Scene::resetVisualTransform(const QString& visualID) {
	Visual3DPtr visual = this->getVisual3D(visualID);
	if (!visual.isNull()) {
		visual->resetTransform();
	} else {
		qDebug() << "Failed to reset object's transform because Visual=" << visualID << "was not found";
	}
}

void Scene::translateVisual(const double& x, const double& y, const double& z, const QString& visualID) {
	Visual3DPtr visual = this->getVisual3D(visualID);
	if (!visual.isNull()) {
		visual->translate(x, y , z);
	} else {
		qDebug() << "Failed to translate object because Visual=" << visualID << "was not found";
	}
}

void Scene::rotateVisual(const double &x, const double& y, const double& z, const QString& visualID) {
	Visual3DPtr visual = this->getVisual3D(visualID);
	if (!visual.isNull()) {
		visual->rotate(x, y , z);
	} else {
		qDebug() << "Failed to rotate object because Visual=" << visualID << "was not found";
	}
}

void Scene::rotateXVisual(const double& x, const QString& visualID) {
	qDebug() << "Enter - RotateX object" << visualID << "by" << x << "degrees";
	Visual3DPtr visual = this->getVisual3D(visualID);
	if (!visual.isNull()) {
		visual->rotateX(x);
	} else {
		qWarning() << "Failed to rotateX object because Visual=" << visualID << "was not found";
	}
	qDebug() << "Exit";
}

void Scene::rotateYVisual(const double& y, const QString& visualID) {
	Visual3DPtr visual = this->getVisual3D(visualID);
	if (!visual.isNull()) {
		visual->rotateY(y);
	} else {
		qDebug() << "Failed to rotateY object because Visual=" << visualID << "was not found";
	}
}

void Scene::rotateZVisual(const double& z, const QString& visualID) {
	Visual3DPtr visual = this->getVisual3D(visualID);
	if (!visual.isNull()) {
		visual->rotateZ(z);
	} else {
		qDebug() << "Failed to rotateZ object because Visual=" << visualID << "was not found";
	}
}

void Scene::setImageSliceData(vtkSmartPointer<ImageData> data, const QString& visualID) {
	ImageSlicePtr im = this->getImageSlice(visualID);
	if (!im.isNull()) {
		im->setImageData(data);
	} else {
		qDebug() << "Failed to set data of image slice because ImageSlice=" << visualID << "was not found";
	}
}

void Scene::setImageSlice(const int& sliceIndex, const ESliceOrientation& orientation, const QString& visualID) {
	ImageSlicePtr im = this->getImageSlice(visualID);
	if (!im.isNull()) {
		im->setSlice(sliceIndex, orientation);
	} else {
		qDebug() << "Failed to set slice of image slice because ImageSlice=" << visualID << "was not found";
	}
}

void Scene::setImageSliceIndex(const int& index, const QString& visualID) {
	ImageSlicePtr im = this->getImageSlice(visualID);
	if (!im.isNull()) {
		im->setSliceIndex(index);
	} else {
		qDebug() << "Failed to set slice index of image slice because" << "ImageSlice=" << visualID << "was not found";
	}
}

void Scene::setImageSliceOrientation(const ESliceOrientation& orientation, const QString& visualID) {
	ImageSlicePtr im = this->getImageSlice(visualID);
	if (!im.isNull()) {
		im->setOrientation(orientation);
	} else {
		qDebug() << "Failed to set orientation of image slice because" << "ImageSlice=" << visualID << "was not found";
	}
}

void Scene::setStudySliceData(StudyPtr data, const QString& visualID) {
	StudySlicePtr im = this->getStudyImageSlice(visualID);
	if (!im.isNull()) {
		im->setStudy(data);
	} else {
		qDebug() << "Failed to set data of study image slice because StudySlice=" << visualID << "was not found";
	}
}

void Scene::setStudySliceSeriesIndex(const int& index, const QString& visualID) {
	qDebug() << "Enter";
	StudySlicePtr im = this->getStudyImageSlice(visualID);
	if (!im.isNull()) {
		im->setSeriesIndex(index);
	} else {
		qDebug() << "Failed to set series index of study image slice" <<
			"because StudySlice=" << visualID << "was not found";
	}
	qDebug() << "Exit";
}

void Scene::setModelData(ModelDataVPtr data, const QString& visualID) {
	ModelPtr md = this->getModel(visualID);
	if (!md.isNull()) {
		md->setModelData(data);
	} else {
		qDebug() << "Failed to set data of model because Model=" << visualID << "was not found";
	}
}

void Scene::setModelColor(const double& r, const double& g, const double& b, const QString& visualID) {
	ModelPtr md = this->getModel(visualID);
	if (!md.isNull()) {
		md->setColor(r, g, b);
	} else {
		qDebug() << "Failed to set color of model because Model=" << visualID << "was not found";
	}
}

void Scene::addPartToAssembly(ModelPtr part, const QString& visualID) {
	AssemblyPtr as = this->getAssembly(visualID);
	if (!as.isNull()) {
		as->addPart(part);
	} else {
		qDebug() << "Failed to add part to assembly because Visual=" << visualID << "was not found";
	}
}

void Scene::removePartFromAssembly(const QString& partName, const QString& visualID) {
	AssemblyPtr as = this->getAssembly(visualID);
	if (!as.isNull()) {
		as->removePart(partName);
	} else {
		qDebug() << "Failed to remove part of assembly because Visual=" << visualID << "was not found";
	}
}

void Scene::setInteractorStyle(vtkSmartPointer<vtkInteractorStyle> interactorStyle) {
	mInteractorStyle->SetInteractor(nullptr);
	mInteractorStyle = interactorStyle;
	mInteractorStyle->SetInteractor(mInteractor);
}

void Scene::setBackgroundColor(const double& r, const double& g, const double& b) {
	// TODO: what if the background color is gradient?
	double current[3] = {0, 0, 0};
	mRenderer->GetBackground(current);
	if (current[0] == r && current[1] == g && current[2] == b) {
		return;
	}

	mRenderer->GradientBackgroundOff();
	mRenderer->SetBackground(r, g, b);
	emit changedBackgroundColor(r, g, b);
}

void Scene::setBackgroundGradientColor(
	const double & r1, const double & g1, const double & b1, 
	const double & r2, const double & g2, const double & b2) 
{
	mRenderer->GradientBackgroundOn();
	mRenderer->SetBackground(r1, g1, b1);
	mRenderer->SetBackground2(r2, g2, b2);

	// TODO: do we need a signal here?
}

void Scene::getBackgroundColor(double& r, double& g, double& b) const {
	mRenderer->GetBackground(r, g, b);
}

void Scene::setAxesVisible(const bool& isVisible) {
	if (isVisible == mAreAxesVisible) {
		return;
	}
	mAreAxesVisible = isVisible;

	if (mAreAxesVisible)
		mAxesVisual->VisibilityOn();
	else
		mAxesVisual->VisibilityOff();
	emit changedAxesVisibility(mAreAxesVisible);
}

void Scene::setAxesLabelsVisible(const bool & isVisible) {
	if (isVisible) {
		mAxesVisual->AxisLabelsOn();
	} else {
		mAxesVisual->AxisLabelsOff();
	}
}

void Scene::setAxesLength(const double& x, const double& y, const double& z) {
	mAxesVisual->SetTotalLength(x, y, z);
}

bool Scene::getAxesVisible() const {
	return mAreAxesVisible;
}

void Scene::render() {
	int currentTime = QTime::currentTime().msecsSinceStartOfDay();
	int dif = currentTime - mTimeFromLastRender;

	if (dif > 16) {
		mIsRenderScheduled = false;
		mRenderWindow->Render();
		mTimeFromLastRender = currentTime;
	} else {
		if (!mIsRenderScheduled) {
			QTimer::singleShot(16, this, &Scene::render);
			mIsRenderScheduled = true;
		}
	}
}

void Scene::resetCamera() {
	mRenderer->ResetCamera();
	emit changedCameraReset();
}

void Scene::worldToDisplay(const double& x, const double& y, const double& z, double& i, double& j) {
	mRenderer->SetWorldPoint(x, y, z, 1.0);
	mRenderer->WorldToDisplay();
	double* displayPoint = mRenderer->GetDisplayPoint();
	i = displayPoint[0];
	j = displayPoint[1];
}

void Scene::addToManagement(VisualPtr visual) {
	qDebug() << "Enter";
	mVisuals.insert(visual->getVisualID(), visual);
	mActorsToNames.insert(visual->getActor(), visual->getVisualID());
	if (visual->is3D()) {
		Visual3DPtr object3D;

		switch (visual->getVisualType().toInt()) {
		case EVisual::IMAGE_SLICE: {
				ImageSlicePtr imageSlice = qobject_cast<ImageSlicePtr>(visual);
				mImageSlices.insert(imageSlice->getVisualID(), imageSlice);
				object3D = imageSlice;

				// Add the data frame of the ImageData.
				mRenderer->AddViewProp(imageSlice->getDataFrameVisual()->getActor());
				imageSlice->getDataFrameVisual()->setOpacity(0.005);
			}
			break;
		case EVisual::STUDY_IMAGE_SLICE: {
				StudySlicePtr studySlice = qobject_cast<StudySlicePtr>(visual);
				
				// StudyImageSlice is an ImageSlice
				mImageSlices.insert(studySlice->getVisualID(), studySlice);
				mStudySlices.insert(studySlice->getVisualID(), studySlice);
				object3D = studySlice;

				// Add the data frame of the ImageData.
				mRenderer->AddViewProp(studySlice->getDataFrameVisual()->getActor());
				studySlice->getDataFrameVisual()->setOpacity(0.005);
			}
			break;
		case EVisual::MODEL: {
				ModelPtr model = qobject_cast<ModelPtr>(visual);
				mModels.insert(model->getVisualID(), model);
				object3D = model;
			}
			break;
		case EVisual::ANIMATED_STUDY_SLICE: {
				AnimatedStudySlicePtr animatedSlice = qobject_cast<AnimatedStudySlicePtr>(visual);

				mImageSlices.insert(animatedSlice->getVisualID(), animatedSlice);
				mStudySlices.insert(animatedSlice->getVisualID(), animatedSlice);
				mAnimatedStudySlices.insert(animatedSlice->getVisualID(), animatedSlice);
				object3D = animatedSlice;

				// Add the data frame of the ImageData.
				mRenderer->AddViewProp(animatedSlice->getDataFrameVisual()->getActor());
				animatedSlice->getDataFrameVisual()->setOpacity(0.005);
			}
			break;	
		case EVisual::ANIMATED_MODEL: {
				AnimatedModelPtr animatedModel = qobject_cast<AnimatedModelPtr>(visual);

				mModels.insert(animatedModel->getVisualID(), animatedModel);
				mAnimatedModels.insert(animatedModel->getVisualID(), animatedModel);
				object3D = animatedModel;
			}
			break;
		case EVisual::ASSEMBLY: {
				AssemblyPtr assembly = qobject_cast<AssemblyPtr>(visual);
				mAssemblies.insert(assembly->getVisualID(), assembly);
				object3D = assembly;
			}
			break;
		default:
			qWarning() << "Failed to add Visual3D to scene management because object is of unknown type.";
			break;
		}
		qDebug() << "Added 3D visual" << object3D->getVisualID() << "to management of" << mSceneID;
		mVisuals3D.insert(object3D->getVisualID(), object3D);

	} else {	// Not a 3D object
		switch (visual->getVisualType().toInt()) {
		case EVisual::SUBTITLE: {
				SubtitlePtr subtitle = qobject_cast<SubtitlePtr>(visual);
				mSubtitles.insert(subtitle->getVisualID(), subtitle);
			}
			break;
		default:
			qWarning() << "Failed to add SceneObject2D to scene management because object is of unknown type.";
			break;
		}
	}

	mRenderer->AddViewProp(visual->getActor());
	this->render();
	qDebug() << "Exit";
}

void Scene::removeFromManagement(VisualPtr visual) {
	qDebug() << "Enter";
	mVisuals.remove(visual->getVisualID());
	mActorsToNames.remove(visual->getActor());
	if (visual->is3D()) {
		switch (visual->getVisualType().toInt()) {
		case EVisual::IMAGE_SLICE:
			mImageSlices.remove(visual->getVisualID());
			break;
		case EVisual::STUDY_IMAGE_SLICE:
			// StudyImageSlice is an ImageSlice
			mImageSlices.remove(visual->getVisualID());
			mStudySlices.remove(visual->getVisualID());
			break;
		case EVisual::MODEL:
			mModels.remove(visual->getVisualID());
			break;
		case EVisual::ANIMATED_STUDY_SLICE:
			mImageSlices.remove(visual->getVisualID());
			mStudySlices.remove(visual->getVisualID());
			mAnimatedStudySlices.remove(visual->getVisualID());
			break;
		case EVisual::ANIMATED_MODEL:
			mModels.remove(visual->getVisualID());
			mAnimatedModels.remove(visual->getVisualID());
			break;
		case EVisual::ASSEMBLY:
			mAssemblies.remove(visual->getVisualID());
			break;
		default:
			qWarning() << "Failed to remove Visual3D from scene management because object is of unknown type.";
			break;
		}
		mVisuals3D.remove(visual->getVisualID());

	} else {	// Not a 3D object
		switch (visual->getVisualType().toInt()) {
		case EVisual::SUBTITLE:
			mSubtitles.remove(visual->getVisualID());
			break;
		default:
			qWarning() << "Failed to remove SceneObject2D from scene management because object is of unknown type.";
			break;
		}
	}

	// If the visual is an ImageSlice, remove the data frame actor.
	if (visual->getVisualType().isSlice()) {
		ImageSlice* slice = qobject_cast<ImageSlice*>(visual.data());
		if (slice) {
			mRenderer->RemoveViewProp(slice->getDataFrameVisual()->getActor());
		} else {
			qCritical() << "Failed to remove data frame because Visual is not a slice (despite it being a slice).";
		}
	}

	mRenderer->RemoveViewProp(visual->getActor());
	this->render();
	qDebug() << "Exit";
}

void Scene::connectVisualSignals(Visual* visual) {
	if (visual == Q_NULLPTR) {
		return;
	}

	QObject::connect(
		visual, &Visual::changedOpacity,
		this, &Scene::render);
	QObject::connect(
		visual, &Visual::changedVisibleState,
		this, &Scene::render);
	QObject::connect(
		visual, &Visual::changedPickableState,
		this, &Scene::render);
}

void Scene::connectVisual3DSignals(Visual3D* visual) {
	if (visual == Q_NULLPTR) {
		return;
	}

	this->connectVisualSignals(visual);
	QObject::connect(
		visual, &Visual3D::changedSceneParent,
		this, &Scene::render);
	QObject::connect(
		visual, &Visual3D::changedTransformation,
		this, &Scene::render);
}

void Scene::connectImageSliceSignals(ImageSlice* imageSlice) {
	if (imageSlice == Q_NULLPTR) {
		return;
	}
	
	this->connectVisual3DSignals(imageSlice);
	QObject::connect(
		imageSlice, &ImageSlice::changedOrientation,
		this, &Scene::render);
	QObject::connect(
		imageSlice, &ImageSlice::changedSlice, 
		this, &Scene::render);
}

void Scene::connectStudySliceSignals(StudySlice* studySlice) {
	if (studySlice == Q_NULLPTR) {
		return;
	}

	this->connectImageSliceSignals(studySlice);
}

void Scene::connectModelSignals(Model* model) {
	if (model == Q_NULLPTR) {
		return;
	}

	this->connectVisual3DSignals(model);
	QObject::connect(
		model, &Model::changedModelData, 
		this, &Scene::render);
	QObject::connect(
		model, &Model::changedColor, 
		this, &Scene::render);
}

void Scene::connectAssemblySignals(Assembly* assembly) {
	if (assembly == Q_NULLPTR) {
		return;
	}

	this->connectVisual3DSignals(assembly);
	QObject::connect(
		assembly, &Assembly::changedAddedPart,
		this, &Scene::render);
	QObject::connect(
		assembly, &Assembly::changedRemovedPart,
		this, &Scene::render);
	QObject::connect(
		assembly, &Assembly::changedPartOpacity,
		this, &Scene::render);
	QObject::connect(
		assembly, &Assembly::changedPartVisible,
		this, &Scene::render);
	QObject::connect(
		assembly, &Assembly::changedPartTransformation, 
		this, &Scene::render);
	QObject::connect(
		assembly, &Assembly::changedPartData,
		this, &Scene::render);
	QObject::connect(
		assembly, &Assembly::changedPartColor,
		this, &Scene::render);
}

void Scene::connectSubtitleSignals(Subtitle* subtitle) {
	if (subtitle == Q_NULLPTR) {
		return;
	}

	this->connectVisualSignals(subtitle);
	QObject::connect(
		subtitle, &Subtitle::changedText,
		this, &Scene::render);
	QObject::connect(
		subtitle, &Subtitle::changedColor,
		this, &Scene::render);
	QObject::connect(
		subtitle, &Subtitle::changedLocation,
		this, &Scene::render);
}

void Scene::disconnectVisualSignals(Visual* visual) {
	if (visual == Q_NULLPTR) {
		return;
	}
	QObject::disconnect(
		visual, &Visual::changedOpacity,
		this, &Scene::render);
	QObject::disconnect(
		visual, &Visual::changedVisibleState,
		this, &Scene::render);
	QObject::disconnect(
		visual, &Visual::changedPickableState,
		this, &Scene::render);
}

void Scene::disconnectVisual3DSignals(Visual3D* visual) {
	if (visual == Q_NULLPTR) {
		return;
	}
	this->disconnectVisualSignals(visual);
	QObject::disconnect(
		visual, &Visual3D::changedSceneParent,
		this, &Scene::render);
	QObject::disconnect(
		visual, &Visual3D::changedTransformation,
		this, &Scene::render);
}

void Scene::disconnectImageSliceSignals(ImageSlice* imageSlice) {
	if (imageSlice == Q_NULLPTR) {
		return;
	}

	this->disconnectVisual3DSignals(imageSlice);
	QObject::disconnect(
		imageSlice, &ImageSlice::changedOrientation, 
		this, &Scene::render);
	QObject::disconnect(
		imageSlice, &ImageSlice::changedSlice, 
		this, &Scene::render);
}

void Scene::disconnectStudySliceSignals(StudySlice* studySlice) {
	if (studySlice == Q_NULLPTR) {
		return;
	}

	this->disconnectImageSliceSignals(studySlice);
}

void Scene::disconnectModelSignals(Model* model) {
	if (model == Q_NULLPTR) {
		return;
	}

	this->disconnectVisual3DSignals(model);
	QObject::disconnect(
		model, &Model::changedModelData,
		this, &Scene::render);
	QObject::disconnect(
		model, &Model::changedColor,
		this, &Scene::render);
}

void Scene::disconnectAssemblySignals(Assembly* assembly) {
	if (assembly == Q_NULLPTR) {
		return;
	}

	this->disconnectVisual3DSignals(assembly);
	QObject::disconnect(
		assembly, &Assembly::changedAddedPart,
		this, &Scene::render);
	QObject::disconnect(
		assembly, &Assembly::changedRemovedPart, 
		this, &Scene::render);
	QObject::disconnect(
		assembly, &Assembly::changedPartOpacity,
		this, &Scene::render);
	QObject::disconnect(
		assembly, &Assembly::changedPartVisible,
		this, &Scene::render);
	QObject::disconnect(
		assembly, &Assembly::changedPartTransformation, 
		this, &Scene::render);
	QObject::disconnect(
		assembly, &Assembly::changedPartData,
		this, &Scene::render);
	QObject::disconnect(
		assembly, &Assembly::changedPartColor, 
		this, &Scene::render);
}

void Scene::disconnectSubtitleSignals(Subtitle* subtitle) {
    if (subtitle == Q_NULLPTR) {
		return;
	}

	this->disconnectVisualSignals(subtitle);
	QObject::disconnect(
		subtitle, &Subtitle::changedText,
		this, &Scene::render);
	QObject::disconnect(
		subtitle, &Subtitle::changedColor,
		this, &Scene::render);
	QObject::disconnect(
		subtitle, &Subtitle::changedLocation, 
		this, &Scene::render);
}
