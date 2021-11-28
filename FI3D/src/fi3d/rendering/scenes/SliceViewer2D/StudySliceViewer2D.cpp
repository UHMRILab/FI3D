#include <fi3d/rendering/scenes/SliceViewer2D/StudySliceViewer2D.h>

#include <fi3d/logger/Logger.h>	

#include <vtkImageViewer2.h>
#include <vtkPlane.h>

using namespace fi3d;

StudySliceViewer2D::StudySliceViewer2D(QVTKOpenGLWidget* widget, const QString& id, ESliceOrientation orientation)
	: ImageSliceViewer2D(widget, id, orientation)
{
	this->setStudy(Q_NULLPTR);
}

StudySliceViewer2D::~StudySliceViewer2D() {}

void StudySliceViewer2D::setStudy(StudyPtr study) {
	if (mStudy == study) {
		return;
	}

	if (study.isNull()) {
		mStudy.reset(new Study(""));
	} else {
		mStudy = study;
	}

	if (mStudy->getSeriesCount() == 0) {
		mSeriesIndex = -1;
		this->setImageData(Q_NULLPTR);
	} else {
		mSeriesIndex = mStudy->getSeriesCount() / 2;
		this->setImageData(mStudy->getSeries(mSeriesIndex));

		// Update the patient matrix in all the visualized models
		vtkSmartPointer<vtkMatrix4x4> pMatrix = mStudy->getSeries(mSeriesIndex)->getPatientMatrix();
		vtkSmartPointer<vtkMatrix4x4> invertMatrix = vtkSmartPointer<vtkMatrix4x4>::New();
		vtkMatrix4x4::Invert(pMatrix, invertMatrix);
		QHash<QString, ModelPtr>::iterator it = mModels.begin();
		for (; it != mModels.end(); it++) {
			it.value()->setTransformData(invertMatrix);
		}
	}

	this->resetCamera();

	emit changedStudy(mStudy);
	
	if (mStudy->getSeriesCount() == 0) {
		emit changedSeriesIndices(-1, -1);
	} else {
		emit changedSeriesIndices(0, mStudy->getSeriesCount() - 1);
	}

	emit changedSeriesIndex(mSeriesIndex);
}

StudyPtr StudySliceViewer2D::getStudy() {
	return mStudy;
}

ModelPtr StudySliceViewer2D::addModel(const QString& visualID, ModelDataVPtr data)  {
	ModelPtr model = ImageSliceViewer2D::addModel(visualID, data);
	if (!model.isNull() && mSeriesIndex != -1) {
		vtkSmartPointer<vtkMatrix4x4> pMatrix = mStudy->getSeries(mSeriesIndex)->getPatientMatrix();
		vtkSmartPointer<vtkMatrix4x4> invertMatrix = vtkSmartPointer<vtkMatrix4x4>::New();
		vtkMatrix4x4::Invert(pMatrix, invertMatrix);

		mModels.value(model->getVisualID())->setTransformData(invertMatrix);
	}
	return model;
}

bool StudySliceViewer2D::addVisual(VisualPtr visual) {
	bool added = ImageSliceViewer2D::addVisual(visual);
	if (added && visual->getVisualType() == EVisual::MODEL && mSeriesIndex != -1) {
		vtkSmartPointer<vtkMatrix4x4> pMatrix = mStudy->getSeries(mSeriesIndex)->getPatientMatrix();
		vtkSmartPointer<vtkMatrix4x4> invertMatrix = vtkSmartPointer<vtkMatrix4x4>::New();
		vtkMatrix4x4::Invert(pMatrix, invertMatrix);

		mModels.value(visual->getVisualID())->setTransformData(invertMatrix);
	}

	return added;
}

void StudySliceViewer2D::setSeriesIndex(const int& seriesIndex) {
	qDebug() << "Enter - Selecting series:" << seriesIndex << "from StudySliceViewer2D:" << this->getSceneID();
	if (mSeriesIndex == seriesIndex || !mStudy->isSeriesIndexInRange(seriesIndex)) {
		return;
	}

	// Set the new series
	mSeriesIndex = seriesIndex;

	vtkSmartPointer<SeriesData> series = mStudy->getSeries(mSeriesIndex);
	mViewer->SetInputData(series);

	// Update the patient matrix in all the visualized models
	vtkSmartPointer<vtkMatrix4x4> pMatrix = mStudy->getSeries(mSeriesIndex)->getPatientMatrix();
	vtkSmartPointer<vtkMatrix4x4> invertMatrix = vtkSmartPointer<vtkMatrix4x4>::New();
	vtkMatrix4x4::Invert(pMatrix, invertMatrix);
	QHash<QString, ModelPtr>::iterator it = mModels.begin();
	for (; it != mModels.end(); it++) {
		it.value()->setTransformData(invertMatrix);
	}

	emit changedImageData(series);
	emit changedSeriesIndex(this->getSeriesIndex());
	emit changedSliceIndices(this->getSliceMinIndex(), this->getSliceMaxIndex());
	emit changedSlice(this->getSliceIndex(), mOrientation);
	qDebug() << "Enter";
}

int StudySliceViewer2D::getSeriesIndex() const {
	return mSeriesIndex;
}

int StudySliceViewer2D::getSeriesMinIndex() const {
	if (mStudy->getSeriesCount() == 0) {
		return -1;
	} else {
		return 0;
	}
}

int StudySliceViewer2D::getSeriesMaxIndex() const {
	if (mStudy->getSeriesCount() == 0) {
		return -1;
	} else {
		return mStudy->getSeriesCount() - 1;
	}
}

void StudySliceViewer2D::getSeriesIndexRange(int& min, int& max) const {
	min = this->getSeriesMinIndex();
	max = this->getSeriesMaxIndex();
}

bool StudySliceViewer2D::isSeriesIndexInRange(const int& seriesIndex) {
	return mStudy->isSeriesIndexInRange(seriesIndex);
}

vtkSmartPointer<vtkPlane> StudySliceViewer2D::getCutPlane() const {
	vtkSmartPointer<vtkPlane> cutPlane = ImageSliceViewer2D::getCutPlane();

	if (mSeriesIndex != -1) {
		double* origin = cutPlane->GetOrigin();
		double* normal = cutPlane->GetNormal();

		vtkNew<vtkTransform> transform;
		transform->SetMatrix(mStudy->getSeries(mSeriesIndex)->getPatientMatrix());
		transform->TransformVector(cutPlane->GetNormal(), cutPlane->GetNormal());
		transform->TransformPoint(cutPlane->GetOrigin(), cutPlane->GetOrigin());
	}

	return cutPlane;
}

bool StudySliceViewer2D::getVoxelCoordinates(const int& i, const int& j, 
	const int& k, double& x, double& y, double& z) const 
{
	bool result = ImageSliceViewer2D::getVoxelCoordinates(i, j, k, x, y, z);
	if (result && mSeriesIndex != -1) {
		vtkSmartPointer<vtkTransform> patientTransform = vtkSmartPointer<vtkTransform>::New();

		patientTransform->SetMatrix(mStudy->getSeries(this->getSeriesIndex())->getPatientMatrix());
		double* patientCoordinate = patientTransform->TransformPoint(x, y, z);
		x = patientCoordinate[0];
		y = patientCoordinate[1];
		z = patientCoordinate[2];
	}
	return result;
}

bool StudySliceViewer2D::getVoxelFromCoordinates(const double& x, 
	const double& y, const double& z, int& i, int& j, int& k) const 
{
	if (mSeriesIndex == -1) {
		return false;
	}

	vtkSmartPointer<vtkMatrix4x4> patientMatrix = mStudy->getSeries(this->getSeriesIndex())->getPatientMatrix();
	vtkSmartPointer<vtkMatrix4x4> patientInverse = vtkSmartPointer<vtkMatrix4x4>::New();

	vtkMatrix4x4::Invert(patientMatrix, patientInverse);

	vtkSmartPointer<vtkTransform> patientInverseTransform = vtkSmartPointer<vtkTransform>::New();
	patientInverseTransform->SetMatrix(patientInverse);

	double invertedPoint[3] = {x, y, z};
	patientInverseTransform->TransformPoint(invertedPoint, invertedPoint);

	return ImageSliceViewer2D::getVoxelFromCoordinates(invertedPoint[0], invertedPoint[1], invertedPoint[2], i, j, k);
}
