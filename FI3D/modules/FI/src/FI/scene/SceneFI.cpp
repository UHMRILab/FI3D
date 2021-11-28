#include <FI/scene/SceneFI.h>

#include <fi3d/logger/Logger.h>

#include <fi3d/server/message_keys/MessageKeys.h>
#include <fi3d/server/message_keys/EModuleResponse.h>

#include <QJsonObject>
#include <QJsonArray>

#include <vtkSmartPointer.h>
#include <vtkTransform.h>

using namespace fi;
using namespace	fi3d;

/// helper function to parse a transformation matrix.
inline vtkSmartPointer<vtkMatrix4x4> parseTransform(QJsonArray& transformJson) {
	vtkSmartPointer<vtkMatrix4x4> mat = vtkSmartPointer<vtkMatrix4x4>::New();

	// If given array does not have sufficient values, return default transform.
	if (transformJson.count() != 16) {
		qWarning() << "Failed to parse transform because 16 values weren't given";
		return mat;
	}

	for (int i = 0; i < 16; i++) {
		mat->GetData()[i] = transformJson[i].toDouble();
	}

	return mat;
}

SceneFI::SceneFI(DataCachePtr cache, QVTKOpenGLWidget* widget, const QString& id)
	: InteractiveScene(widget, id),
	mSubscribedSceneID(""),
	mCache(cache)
{
	if (mCache.isNull()) {
		qInfo() << "No Data cache was given to Scene" << id << "Creating new cache.";
		mCache.reset(new DataCache());
	}

	this->setBackgroundGradientColor(0.5, 0.5, 0.5, 0.9, 0.9, 0.9);
}

SceneFI::~SceneFI() {}

void SceneFI::handleModuleResponse(const QJsonArray& visualsInfo) {
	qDebug() << "Enter";
	
	qDebug() << "Handling" << visualsInfo.count() << "visual updates";

	QList<QJsonObject> parentChanges;
	for (int i = 0; i < visualsInfo.count(); i++) {
		QJsonObject visualInfo = visualsInfo[i].toObject();
		
		EModuleResponse responseID(visualInfo.value(RESPONSE_ID).toInt());
		switch (responseID.toInt()) {
			case EModuleResponse::ADD_VISUAL:
				this->handleAddVisual(visualInfo);
				parentChanges.append(visualInfo);
				break;
			case EModuleResponse::REFRESH_VISUAL:
				this->handleRefreshVisual(visualInfo);
				parentChanges.append(visualInfo);
				break;
			case EModuleResponse::REMOVE_VISUAL:
				this->handleRemoveVisual(visualInfo);
				break;
			case EModuleResponse::DATA_CHANGE:
				this->handleDataChange(visualInfo);
				break;
			case EModuleResponse::HIDE_VISUAL:
				this->handleHideVisual(visualInfo);
				break;
			case EModuleResponse::TRANSFORM_VISUAL:
				this->handleTransformVisual(visualInfo);
				break;
			case EModuleResponse::PARENT_CHANGE:
				this->handleParentChange(visualInfo);
				parentChanges.append(visualInfo);
				break;
			case EModuleResponse::SET_VISUAL_OPACITY:
				this->handleSetVisualOpacity(visualInfo);
				break;
			case EModuleResponse::SET_SLICE:
				this->handleSetSlice(visualInfo);
				break;
			case EModuleResponse::SET_OBJECT_COLOR:
				this->handleSetColor(visualInfo);
				break;
			default:
				break;
		}
	}

	// Ensure parenting was set as order of Visual updates may not come in order.
	for (QJsonObject visualInfo : parentChanges) {
		Visual3DPtr visual = mVisuals3D.value(visualInfo.value(ID).toString());
		Visual3DPtr parent = mVisuals3D.value(visualInfo.value(PARENT_ID).toString());
		if (!parent.isNull() && !visual.isNull() && visual->getParentVisual() != parent) {
			visual->setParentVisual(parent);
			QJsonArray transJson = visualInfo.value(TRANSFORMATION).toArray();
			vtkSmartPointer<vtkMatrix4x4> transform = parseTransform(transJson);
			visual->setTransformData(transform);
		}
	}

	qDebug() << "Exit";
}

void SceneFI::handleAddVisual(const QJsonObject& visualJson) {
	QString visualID = visualJson.value(ID).toString();
	if (this->isVisualIDTaken(visualID)) {
		QString fb = tr("Failed to add Visual: %1. A visual with that ID already exists").arg(visualID);
		emit feedbackWarning(fb);
		return;
	}

	Visual3DPtr visual;
	EVisual visualType(visualJson.value(TYPE).toInt());
	if (visualType == EVisual::STUDY_IMAGE_SLICE) {
		StudySlicePtr slice = this->addStudyImageSlice(visualID);

		// Disable auto setting of patient matrix, as it is not part
		// of the requested data and instead part of the visual's transform;
		slice->setAutoSetPatientMatrix(false);

		QString dataID = visualJson.value(DATA_ID).toString();
		int index = visualJson.value(SLICE_INDEX).toInt();
		ESliceOrientation orien = visualJson.value(SLICE_ORIENTATION).toInt();
		int series = visualJson.value(SERIES_INDEX).toInt();

		StudyPromisePtr prom = mCache->getStudy(dataID, index, orien, series);

		if (prom->isResolved()) {
			slice->setStudy(prom->getResult());
			slice->setSeriesIndex(series);
			slice->setSlice(index, orien);
		} else {
			QObject::connect(
				prom.data(), &ImagePromise::resolved,
				this,
				[slice, prom, index, orien, series]() {
				slice->setStudy(prom->getResult());
				slice->setSeriesIndex(series);
				slice->setSlice(index, orien);
			});
		}

		QObject::connect(
			slice.data(), &StudySlice::changedSlice,
			this, &SceneFI::onSliceChange);

		visual = slice;
	} else if (visualType == EVisual::MODEL) {
		ModelPtr model = this->addModel(visualID);
		QJsonArray color = visualJson.value(COLOR).toArray();
		model->setColor(color[0].toDouble(), color[1].toDouble(), color[2].toDouble());

		QString dataID = visualJson.value(DATA_ID).toString();
		ModelPromisePtr prom = mCache->getModelData(dataID);

		if (prom->isResolved()) {
			model->setModelData(prom->getResult());
		} else {
			QObject::connect(
				prom.data(), &ModelPromise::resolved,
				this, [model, prom]() {
				model->setModelData(prom->getResult());
			});
		}

		visual = model;
	} else if (visualType == EVisual::ASSEMBLY) {
		visual = this->addAssembly(visualID);
		// TODO: Add parts
	} else {
		qDebug() << "Failed to add Visual. Visual type is unknown.";
		return;
	}

	visual->setVisible(visualJson.value(IS_VISIBLE).toBool());
	visual->setOpacity(visualJson.value(OPACITY).toDouble());

	QString parentID = visualJson.value(PARENT_ID).toString();
	Visual3DPtr parent = this->getVisual3D(parentID);
	if (!parent.isNull()) {
		visual->setParentVisual(parent);
	}

	QJsonArray transform = visualJson.value(TRANSFORMATION).toArray();
	vtkSmartPointer<vtkMatrix4x4> mat = parseTransform(transform);
	visual->setTransformData(mat);
}

void SceneFI::handleRefreshVisual(const QJsonObject& visualJson) {
	QString visualID = visualJson.value(ID).toString();
	Visual3DPtr visual = this->getVisual3D(visualID);

	if (visual.isNull()) {
		QString fb = tr("Failed to refresh Visual %1 because it does not exist.").arg(visualID);
		emit feedbackWarning(fb);
		return;
	}

	if (visual->getVisualType() == EVisual::STUDY_IMAGE_SLICE) {
		StudySlicePtr slice = this->getStudyImageSlice(visualID);

		QString dataID = visualJson.value(DATA_ID).toString();
		int index = visualJson.value(SLICE_INDEX).toInt();
		ESliceOrientation orien = visualJson.value(SLICE_ORIENTATION).toInt();
		int series = visualJson.value(SERIES_INDEX).toInt();

		StudyPromisePtr prom = mCache->getStudy(dataID, index, orien, series);

		if (prom->isResolved()) {
			slice->setStudy(prom->getResult());
			slice->setSeriesIndex(series);
			slice->setSlice(index, orien);
		} else {
			QObject::connect(
				prom.data(), &ImagePromise::resolved,
				this,
				[slice, prom, index, orien, series]() {
				slice->setStudy(prom->getResult());
				slice->setSeriesIndex(series);
				slice->setSlice(index, orien);
			});
		}

		QObject::connect(
			slice.data(), &StudySlice::changedSlice,
			this, &SceneFI::onSliceChange);

		visual = slice;
	} else if (visual->getVisualType() == EVisual::MODEL) {
		ModelPtr model = this->getModel(visualID);
		QJsonArray color = visualJson.value(COLOR).toArray();
		model->setColor(color[0].toDouble(), color[1].toDouble(), color[2].toDouble());

		QString dataID = visualJson.value(DATA_ID).toString();
		ModelPromisePtr prom = mCache->getModelData(dataID);

		if (prom->isResolved()) {
			model->setModelData(prom->getResult());
		} else {
			QObject::connect(
				prom.data(), &ModelPromise::resolved,
				this, [model, prom]() {
				model->setModelData(prom->getResult());
			});
		}

		visual = model;
	} else if (visual->getVisualType() == EVisual::ASSEMBLY) {
		visual = this->addAssembly(visualID);
	} else {
		qDebug() << "Failed to add Visual. Visual type is unknown.";
		return;
	}

	visual->setVisible(visualJson.value(IS_VISIBLE).toBool());
	visual->setOpacity(visualJson.value(OPACITY).toDouble());

	QString parentID = visualJson.value(PARENT_ID).toString();
	Visual3DPtr parent = this->getVisual3D(parentID);
	if (!parent.isNull()) {
		visual->setParentVisual(parent);
	}

	QJsonArray transform = visualJson.value(TRANSFORMATION).toArray();
	vtkSmartPointer<vtkMatrix4x4> mat = parseTransform(transform);
	visual->setTransformData(mat);
}

void SceneFI::handleRemoveVisual(const QJsonObject& visualInfo) {
	QString visualID = visualInfo.value(ID).toString();
	if (!this->isVisualIDTaken(visualID)) {
		QString fb = tr("Failed to remove Visual: %1. Visual does not exist.").arg(visualID);
		emit feedbackWarning(fb);
		return;
	}

	this->removeVisual(visualID);
}

void SceneFI::handleDataChange(const QJsonObject& visualInfo) {
	QString visualID = visualInfo.value(ID).toString();
	Visual3DPtr visual = this->getVisual3D(visualID);

	if (visual.isNull()) {
		QString fb = tr("Failed to change data of Visual: %1. Does not exist.").arg(visualID);
		emit feedbackWarning(fb);
		return;
	}

	if (visual->getVisualType() == EVisual::STUDY_IMAGE_SLICE) {
		StudySlice* slice = qobject_cast<StudySlice*>(visual.data());

		QString dataID = visualInfo.value(DATA_ID).toString();
		int index = visualInfo.value(SLICE_INDEX).toInt();
		int orien = visualInfo.value(SLICE_ORIENTATION).toInt();
		int series = visualInfo.value(SERIES_INDEX).toInt();

		StudyPromisePtr prom = mCache->getStudy(dataID, index, orien, series);

		if (prom->isResolved()) {
			slice->setStudy(prom->getResult());
			slice->setSeriesIndex(series);
			slice->setSlice(index, orien);
		} else {
			QObject::connect(
				prom.data(), &ImagePromise::resolved,
				this, [slice, prom, index, orien, series]() {
				slice->setStudy(prom->getResult());
				slice->setSeriesIndex(series);
				slice->setSlice(index, orien);
			});
		}
	} else if (visual->getVisualType() == EVisual::MODEL) {
		Model* model = qobject_cast<Model*>(visual.data());

		QString dataID = visualInfo.value(DATA_ID).toString();
		ModelPromisePtr prom = mCache->getModelData(dataID);

		if (prom->isResolved()) {
			model->setModelData(prom->getResult());
		} else {
			QObject::connect(
				prom.data(), &ModelPromise::resolved,
				this, [model, prom]() {
				model->setModelData(prom->getResult());
			});
		}
	} else if (visual->getVisualType() == EVisual::ASSEMBLY) {
		Assembly* assembly = qobject_cast<Assembly*>(visual.data());
		// TODO: Modify parts
	} else {
		qDebug() << "Failed to change data of Visual. Visual type is unknown.";
		return;
	}
}

void SceneFI::handleHideVisual(const QJsonObject& visualInfo) {
	QString visualID = visualInfo.value(ID).toString();
	if (!this->isVisualIDTaken(visualID)) {
		QString fb = tr("Failed to change visibility of Visual: %1. Visual does not exist").arg(visualID);
		emit feedbackWarning(fb);
		return;
	}

	this->setVisualVisible(visualInfo.value(IS_VISIBLE).toBool(), visualID);
}

void SceneFI::handleTransformVisual(const QJsonObject& visualInfo) {
	QString visualID = visualInfo.value(ID).toString();
	Visual3DPtr visual = this->getVisual3D(visualID);
	if (visual.isNull()) {
		QString fb = tr("Failed to transform Visual: %1. Visual does not exist").arg(visualID);
		emit feedbackWarning(fb);
		return;
	}

	QJsonArray transJson = visualInfo.value(TRANSFORMATION).toArray();
	vtkSmartPointer<vtkMatrix4x4> trans = parseTransform(transJson);
	visual->setTransformData(trans);
}

void SceneFI::handleParentChange(const QJsonObject& visualInfo) {
	QString visualID = visualInfo.value(ID).toString();
	Visual3DPtr visual = this->getVisual3D(visualID);

	if (visual.isNull()) {
		QString fb = tr("Failed to change parent of Visual: %1. Visual does not exist").arg(visualID);
		emit feedbackWarning(fb);
		return;
	}

	QString parentID = visualInfo.value(PARENT_ID).toString();
	Visual3DPtr parent = this->getVisual3D(parentID);
	if (parent.isNull()) {
		QString fb = tr("Failed to change parent of Visual: %1. Parent %2 does not exist").arg(visualID).arg(parentID);
		emit feedbackWarning(fb);
		return;
	}
}

void SceneFI::handleSetVisualOpacity(const QJsonObject& visualInfo) {
	QString visualID = visualInfo.value(ID).toString();
	if (!this->isVisualIDTaken(visualID)) {
		QString fb = tr("Failed to change opacity of Visual: %1. Visual does not exist").arg(visualID);
		emit feedbackWarning(fb);
		return;
	}

	this->setVisualOpacity(visualInfo.value(OPACITY).toDouble(), visualID);
}

void SceneFI::handleSetSlice(const QJsonObject& visualInfo) {
	QString visualID = visualInfo.value(ID).toString();
	if (!this->isVisualIDTaken(visualID)) {
		QString fb = tr("Failed to set slice of Visual: %1. Visual does not exist").arg(visualID);
		emit feedbackWarning(fb);
		return;
	}

	int index = visualInfo.value(SLICE_INDEX).toInt();
	ESliceOrientation orien = visualInfo.value(SLICE_ORIENTATION).toInt();
	int seires = visualInfo.value(SERIES_INDEX).toInt();
	this->setStudySliceSeriesIndex(seires, visualID);
	this->setImageSlice(index, orien, visualID);
}

void SceneFI::handleSetColor(const QJsonObject& visualInfo) {
	QString visualID = visualInfo.value(ID).toString();
	ModelPtr model = this->getModel(visualID);
	if (model.isNull()) {
		QString fb = tr("Failed to set color of Model: %1. Model does not exist.").arg(visualID);
		emit feedbackWarning(fb);
		return;
	}

	QJsonArray color = visualInfo.value(COLOR).toArray();
	double r = color[0].toDouble();
	double g = color[1].toDouble();
	double b = color[2].toDouble();

	model->setColor(r, g, b);
}

void SceneFI::onSliceChange(const int& sliceIndex,
	const ESliceOrientation orientation) 
{
	qDebug() << "Enter";

	StudySlice* slice = qobject_cast<StudySlice*>(sender());
	if (slice == Q_NULLPTR) {
		qWarning() << "Failed to request slice data. No slice information was given";
		return;
	}

	CachedStudyPtr stu = qobject_cast<CachedStudyPtr>(slice->getStudy());

	if (!stu->isSliceCached(sliceIndex, orientation, slice->getSeriesIndex())) {
		StudyPromisePtr stuPro = mCache->getStudy(stu->getFI3DDataID(),
			slice->getSliceIndex(), slice->getSliceOrientation(),
			slice->getSeriesIndex());

		// Get when the promise resolves so that the scene is re-rendered to 
		// update the slice data being rendered
		QObject::connect(
			stuPro.data(), &StudyPromise::resolved, 
			this, [this, stu, slice]() {
			// Modified function makes sure the render call is aware new data
			// is available to be render. 
			stu->getSeries(slice->getSeriesIndex())->Modified();
			this->render();
		});
	}

	qDebug() << "Exit";
}
