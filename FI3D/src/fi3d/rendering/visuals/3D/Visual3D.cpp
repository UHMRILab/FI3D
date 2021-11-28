#include <fi3d/rendering/visuals/3D/Visual3D.h>

#include <fi3d/logger/Logger.h>

using namespace fi3d;

Visual3D::Visual3D(const QString& id)
	: Visual(id, true),
	mTransform(vtkSmartPointer<vtkTransform>::New()),
	mParentVisual(Q_NULLPTR)
{}

Visual3D::~Visual3D() {}

void Visual3D::resetTransform() {
	mTransform->Identity();
	emit changedTransformation();
}

void Visual3D::setTransformData(vtkTransform* source) {
	this->setTransformData(source->GetMatrix());
}

void Visual3D::setTransformData(vtkMatrix4x4* source) {
	qDebug() << "Enter - Setting transform data to Visual3D:" << this->getVisualID();

	if (source == Q_NULLPTR) {
		return;
	}

	// NOTE: A copy is made so that the given matrix cannot be used to 
	// manipulate the transformation from the outside
	// NOTE: Getting current matrix of the transform and deep coying the source 
	// into it produces unwanted behavior. For some reason, the renderer 
	// sometimes does not update the visual. Making a copy, deep copying, 
	// and assigning eliminates such behavior. 
	vtkSmartPointer<vtkMatrix4x4> sourceCopy = vtkSmartPointer<vtkMatrix4x4>::New();
	sourceCopy->DeepCopy(source);
	mTransform->SetMatrix(sourceCopy);

	emit changedTransformation();
	qDebug() << "Exit";
}

void Visual3D::setParentVisual(QSharedPointer<Visual3D> parent) {
	if (parent == mParentVisual) {
		return;
	}
	mParentVisual = parent;

	if (mParentVisual.isNull()) {
		mTransform->SetInput(vtkSmartPointer<vtkTransform>::New());
	} else {
		mTransform->SetInput(mParentVisual->getTransform());
	}

	emit changedSceneParent(mParentVisual);
}

void Visual3D::setTransformToPostMultiply() {
	mTransform->PostMultiply();
}

void Visual3D::setTransformToPreMultiply() {
	mTransform->PreMultiply();
}

void Visual3D::setPosition(const double& x, const double& y, const double& z) {
	double position[3];
	this->getPosition(position);
	this->translate(x - position[0], y - position[1], z - position[2]);
}

void Visual3D::setPositionX(const double& x) {
	double position[3];
	this->getPosition(position);
	this->translate(x-position[0], position[1], position[2]);
}

void Visual3D::setPositionY(const double& y) {
	double position[3];
	this->getPosition(position);
	this->translate(position[0], y - position[1], position[2]);
}

void Visual3D::setPositionZ(const double& z) {
	double position[3];
	this->getPosition(position);
	this->translate(position[0], position[1], z - position[2]);
}

void Visual3D::translate(const double& x, const double& y, const double& z) {
	mTransform->Translate(x, y, z);
	emit changedTransformation();
}

void Visual3D::setPositionGlobal(const double& x, const double& y, 
	const double& z) 
{
	double orientation[4];
	this->getOrientationWXYZ(orientation);

	vtkSmartPointer<vtkTransform> newTran = vtkSmartPointer<vtkTransform>::New();
	newTran->Translate(x, y, z);
	newTran->RotateWXYZ(orientation[0], orientation[1], orientation[2], orientation[3]);

	this->setTransformData(newTran);

	//double newPos[3] = {x, y, z};
	//vtkSmartPointer<vtkTransform> inverseTransform = this->getTransform();
	//inverseTransform->TransformPoint(newPos, newPos);
	//this->translate(newPos[0] - position[0], newPos[1] - position[1], 
	//	newPos[2] - position[2]);
}

void Visual3D::rotate(const double& x, const double& y, const double& z) {
	mTransform->RotateX(x);
	mTransform->RotateY(y);
	mTransform->RotateZ(z);
	emit changedTransformation();
}

void Visual3D::rotateX(const double& x) {
	mTransform->RotateX(x);
	emit changedTransformation();
}

void Visual3D::rotateY(const double& y) {
	mTransform->RotateY(y);
	emit changedTransformation();
}

void Visual3D::rotateZ(const double& z) {
	mTransform->RotateZ(z);
	emit changedTransformation();
}

void Visual3D::rotateWXYZ(const double& w, const double& x, const double& y, const double& z) {
	mTransform->RotateWXYZ(w, x, y, z);
	emit changedTransformation();
}

void Visual3D::setScale(const double& x, const double& y, const double& z) {
	mTransform->Scale(x, y, z);
}

void Visual3D::transformVector(double in[3], double out[3]) const {
	mTransform->TransformVector(in, out);
}

void Visual3D::transformVector(double v[3]) const {
	this->transformVector(v, v);
}

void Visual3D::transformPoint(double in[3], double out[3]) const {
	mTransform->TransformPoint(in, out);
}

void Visual3D::transformPoint(double p[3]) const {
	this->transformPoint(p, p);
}

vtkSmartPointer<vtkTransform> Visual3D::getRelativeTransform() const {
	vtkSmartPointer<vtkTransform> t = vtkSmartPointer<vtkTransform>::New();
	t->SetMatrix(this->getRelativeMatrix());
	return t;
}

vtkSmartPointer<vtkMatrix4x4> Visual3D::getRelativeMatrix() const {
	// If there's a parent matrix, remove it to compute relative matrix
	if (!mParentVisual.isNull()) {
		vtkSmartPointer<vtkTransform> identity = vtkSmartPointer<vtkTransform>::New();
		mTransform->SetInput(identity);
	}

	// make the copy
	vtkSmartPointer<vtkMatrix4x4> m = vtkSmartPointer<vtkMatrix4x4>::New();
	for (int i = 0; i < 16; i++) {
		m->GetData()[i] = mTransform->GetMatrix()->GetData()[i];
	}

	// Assign the parent matrix back if there is one
	if (!mParentVisual.isNull()) {
		mTransform->SetInput(mParentVisual->getTransform());
	}
	return m;
}

vtkSmartPointer<vtkTransform> Visual3D::getTransformCopy() const {
	vtkSmartPointer<vtkTransform> t = vtkSmartPointer<vtkTransform>::New();
	t->SetMatrix(this->getMatrixCopy());
	return t;
}

vtkSmartPointer<vtkMatrix4x4> Visual3D::getMatrixCopy() const {
	vtkSmartPointer<vtkMatrix4x4> m = vtkSmartPointer<vtkMatrix4x4>::New();
	m->DeepCopy(mTransform->GetMatrix());
	return m;
}

void Visual3D::getTransformData(double values[16]) const {
	for (int i = 0; i < 16; i++) {
		values[i] = mTransform->GetMatrix()->GetData()[i];
	}
}

vtkSmartPointer<vtkTransform> Visual3D::getInverseTransform() const {
	vtkSmartPointer<vtkTransform> t = vtkSmartPointer<vtkTransform>::New();
	t->SetMatrix(this->getInverseMatrix());
	return t;
}

vtkSmartPointer<vtkMatrix4x4> Visual3D::getInverseMatrix() const {
	vtkSmartPointer<vtkMatrix4x4> m = vtkSmartPointer<vtkMatrix4x4>::New();
	mTransform->GetInverse(m);
	return m;
}

void Visual3D::getPosition(double& x, double& y, double& z) const {
	double p[3];
	this->getPosition(p);
	x = p[0];
	y = p[1];
	z = p[2];
}

void Visual3D::getPosition(double p[3]) const {
	mTransform->GetPosition(p);
}

void Visual3D::getOrientation(double& x, double& y, double& z) const {
	double a[3];
	this->getOrientation(a);
	x = a[0];
	y = a[1];
	z = a[2];

}

void Visual3D::getOrientation(double a[3]) const {
	mTransform->GetOrientation(a);
}

void Visual3D::getOrientationWXYZ(double a[4]) {
	mTransform->GetOrientationWXYZ(a);
}

void Visual3D::getScales(double& x, double& y, double& z) const {
	double s[3];
	this->getScales(s);
	x = s[0];
	y = s[1];
	z = s[2];

}

void Visual3D::getScales(double s[3]) const {
	mTransform->GetScale(s);
}

bool Visual3D::hasParentVisual() const {
	if (mParentVisual.isNull()) {
		return false;
	}
	return true;
}

QSharedPointer<Visual3D> Visual3D::getParentVisual() const {
	return mParentVisual;
}

QString Visual3D::getParentVisualName() const {
	if (this->hasParentVisual()) {
		return mParentVisual->getVisualID();
	} else {
		return "";
	}
}
	
vtkTransform* Visual3D::getTransform() {
	return mTransform;
}
