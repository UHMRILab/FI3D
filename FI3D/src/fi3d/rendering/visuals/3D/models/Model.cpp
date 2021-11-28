#include <fi3d/rendering/visuals/3D/models/Model.h>

#include <fi3d/logger/Logger.h>

#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkTransform.h>

using namespace fi3d;

Model::Model(const QString& name, ModelDataVPtr data)
	: Visual3D(name),
	mData(data),
	mMapper(vtkSmartPointer<vtkPolyDataMapper>::New()),
	mActor(vtkSmartPointer<vtkActor>::New())
{
	mMapper->SetInputData(mData);
	mActor->SetMapper(mMapper);
	mActor->SetUserTransform(this->getTransform());

	// Re-emit signal when visualized data changes internally
	QObject::connect(
		mData.Get(), &ModelData::changedData,
		this, &Model::onDataUpdated,
		Qt::UniqueConnection);
}

Model::~Model() {}

void Model::setModelData(ModelDataVPtr data) {
	if (mData == data) {
		return;
	}

	// Ignore the changedData signals from previous assigned data
	QObject::disconnect(
		mData.Get(), &ModelData::changedData,
		this, &Model::onDataUpdated);

	if (data.Get() == Q_NULLPTR) {
		mData = ModelDataVPtr::New();
	} else {
		mData = data;
	}

	mMapper->SetInputData(mData);
	emit changedModelData(mData);

	// Re-emit signal when visualized data changes internally
	QObject::connect(
		mData.Get(), &ModelData::changedData,
		this, &Model::onDataUpdated,
		Qt::UniqueConnection);
}

ModelDataVPtr Model::getModelData() {
	return mData;
}

void Model::setColor(const double& r, const double& g, const double& b) {
	double cr, cg, cb;
	this->getColor(cr, cg, cb);
	if (cr == r && cg == g && cb == b) {
		return;
	}

	mActor->GetProperty()->SetColor(r, g, b);
	emit changedColor(r, g, b);
}

void Model::getColor(double& r, double& g, double& b) const {
	mActor->GetProperty()->GetColor(r, g, b);
}

void Model::setPointSize(double sz) {
	// TODO: There's no getter or signal. Will this be needed? 
	// Is this function even needed?
	mActor->GetProperty()->SetPointSize(sz);
}

void Model::setLineWidth(double width) {
	mActor->GetProperty()->SetLineWidth(width);
}

void Model::getXDirection(double& x, double& y, double& z) const {
	double* dataBounds = mData->GetBounds();
	double backPoint[3] = {
		dataBounds[0],
		(dataBounds[2] + dataBounds[3]) / 2.0,
		(dataBounds[4] + dataBounds[5]) / 2.0
	};
	double frontPoint[3] = {
		dataBounds[1],
		backPoint[1],
		backPoint[2]
	};

	this->transformPoint(backPoint);
	this->transformPoint(frontPoint);

	double vec[3] = {
		frontPoint[0] - backPoint[0],
		frontPoint[1] - backPoint[1],
		frontPoint[2] - backPoint[2]
	};
	vtkMath::Normalize(vec);
	x = vec[0];
	y = vec[1];
	z = vec[2];
}

void Model::getXDirection(double direction[3]) const {
	this->getXDirection(direction[0], direction[1], direction[2]);
}

void Model::getYDirection(double& x, double& y, double& z) const {
	double* dataBounds = mData->GetBounds();
	double backPoint[3] = {
		(dataBounds[0] + dataBounds[1]) / 2.0,
		dataBounds[2],
		(dataBounds[4] + dataBounds[5]) / 2.0
	};
	double frontPoint[3] = {
		backPoint[0],
		dataBounds[3],
		backPoint[2]
	};

	this->transformPoint(backPoint);
	this->transformPoint(frontPoint);

	double vec[3] = {
		frontPoint[0] - backPoint[0],
		frontPoint[1] - backPoint[1],
		frontPoint[2] - backPoint[2]
	};
	vtkMath::Normalize(vec);
	x = vec[0];
	y = vec[1];
	z = vec[2];	
}

void Model::getYDirection(double direction[3]) const {
	this->getYDirection(direction[0], direction[1], direction[2]);
}

void Model::getZDirection(double& x, double& y, double& z) const {
	double dataBounds[6] = {0};
	mData->GetBounds(dataBounds);
	double backPoint[3] = {
		(dataBounds[0] + dataBounds[1]) / 2.0,
		(dataBounds[2] + dataBounds[3]) / 2.0,
		dataBounds[4]
	};
	double frontPoint[3] = {
		backPoint[0],
		backPoint[1],
		dataBounds[5]
	};

	this->transformPoint(backPoint);
	this->transformPoint(frontPoint);

	double vec[3] = {
		frontPoint[0] - backPoint[0],
		frontPoint[1] - backPoint[1],
		frontPoint[2] - backPoint[2]
	};
	vtkMath::Normalize(vec);
	x = vec[0];
	y = vec[1];
	z = vec[2];
}

void Model::getZDirection(double direction[3]) const {
	this->getZDirection(direction[0], direction[1], direction[2]);
}

void fi3d::Model::onDataUpdated() {
	emit changedModelData(mData);
}

void Model::setOpacity(const double& a) {
	if (a == this->getOpacity()) {
		return;
	}
	mActor->GetProperty()->SetOpacity(a);
	emit changedOpacity(a);
}

double Model::getOpacity() const {
	return mActor->GetProperty()->GetOpacity();
}

EVisual Model::getVisualType() const {
	return EVisual::MODEL;
}

vtkActor* Model::getActor() {
	return mActor;
}

void Model::setVisible(const bool& isVisible) {
	if (isVisible == this->isVisible()) {
		return;
	}
	if (isVisible)
		mActor->VisibilityOn();
	else
		mActor->VisibilityOff();
	emit changedVisibleState(isVisible);
}

bool Model::isVisible() const {
	return mActor->GetVisibility();
}

void Model::setPickable(const bool& isPickable) {
	if (isPickable == this->isPickable()) {
		return;
	}
	if (isPickable)
		mActor->PickableOn();
	else
		mActor->PickableOff();
	emit changedPickableState(isPickable);
}

bool Model::isPickable() const {
	return mActor->GetPickable();
}