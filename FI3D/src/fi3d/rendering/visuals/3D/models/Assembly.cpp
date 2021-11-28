#include <fi3d/rendering/visuals/3D/models/Assembly.h>

#include <fi3d/logger/Logger.h>

#include <QHash>

#include <vtkActor.h>
#include <vtkTransform.h>

using namespace fi3d;

Assembly::Assembly(const QString& id)
	: Visual3D(id),
	mActor(vtkSmartPointer<vtkAssembly>::New()),
	mParts()
{
	mActor->SetUserTransform(this->getTransform());
}

Assembly::~Assembly() {
	this->removeAllParts();
}

ModelPtr Assembly::addPart(const QString& partID, ModelDataVPtr data) {
	qDebug() << "Enter - Adding part=" << partID << "to assembly=" << this->getVisualID();
	if (mParts.contains(partID)) {
		qWarning() << "Failed to add part=" << partID << "to assembly=" << 
			this->getVisualID() << 
			"because a part with that ID already exists";
		return Q_NULLPTR;
	}

	ModelPtr newPart(new Model(partID));

	mActor->AddPart(newPart->getActor());
	this->connectPartSignals(newPart.data());

	mParts.insert(partID, newPart);
	emit changedAddedPart(newPart);
	qDebug() << "Exit";
	return newPart;
}

void Assembly::addPart(ModelPtr part) {
	qDebug() << "Enter - Adding part=" << part->getVisualID() << "to assembly=" << this->getVisualID();
	if (mParts.contains(part->getVisualID())) {
		qWarning() << "Failed to add part=" << part->getVisualID() << 
			" to assembly=" << this->getVisualID() << 
			"because a part with that ID already exists";
		return;
	}

	mActor->AddPart(part->getActor());
	this->connectPartSignals(part.data());
	
	mParts.insert(part->getVisualID(), part);
	emit changedAddedPart(part);
	qDebug() << "Exit";
}

ModelPtr Assembly::getPart(const QString& partID) {
	if (!mParts.contains(partID)) {
		qInfo() << "Failed to get part because a part with that ID does not exist";
		return Q_NULLPTR;
	}

	return mParts.value(partID, Q_NULLPTR);
}

void Assembly::removePart(const QString& partID) {
	qDebug() << "Enter - removing part=" << partID << "from assembly=" << this->getVisualID();
	
	if (!mParts.contains(partID)) {
		qInfo() << "Failed to remove part because a part with that ID was not found.";
		return;
	}

	ModelPtr part = mParts.take(partID);
	mActor->RemovePart(part->getActor());

	emit changedRemovedPart(part);
	qDebug() << "Exit";
}

void Assembly::removeAllParts() {
	qDebug() << "Enter";
	QHash<QString, ModelPtr>::iterator it = mParts.begin();
	for (; it != mParts.end(); it++) {
		mActor->RemovePart(it.value()->getActor());
		emit changedRemovedPart(it.value());
	}
	mParts.clear();
	qDebug() << "Exit";
}

QList<ModelPtr> Assembly::getParts() const {
	return mParts.values();
}

int Assembly::getPartCount() const {
	return mParts.size();
}

EVisual Assembly::getVisualType() const {
	return EVisual::ASSEMBLY;
}

vtkAssembly* Assembly::getActor() {
	return mActor;
}

void Assembly::setOpacity(const double & a) {
	QHash<QString, ModelPtr>::iterator it = mParts.begin();
	for (; it != mParts.end(); it++) {
		it.value()->setOpacity(a);
	}
}

double Assembly::getOpacity() const { 
	return 1.0; 
}

void Assembly::setVisible(const bool& isVisible) {
	if (isVisible == this->isVisible()) {
		return;
	}
	if (isVisible)
		mActor->VisibilityOn();
	else
		mActor->VisibilityOff();
	emit changedVisibleState(isVisible);
}

bool Assembly::isVisible() const {
	return mActor->GetVisibility();
}

void Assembly::setPickable(const bool& isPickable) {
	if (isPickable == this->isPickable()) {
		return;
	}
	if (isPickable)
		mActor->PickableOn();
	else
		mActor->PickableOff();
	emit changedPickableState(isPickable);
}

bool Assembly::isPickable() const {
	return mActor->GetPickable();
}

void Assembly::mapChangedPartOpacity(const double& a) {
	Visual* part = qobject_cast<Visual*>(sender());

	if (part != Q_NULLPTR) {
		emit changedPartOpacity(a, mParts.value(part->getVisualID(), Q_NULLPTR));
	}
}

void Assembly::mapChangedPartVisible(const bool& isVisible) {
	Visual* part = qobject_cast<Visual*>(sender());

	if (part != Q_NULLPTR) {
		emit changedPartVisible(isVisible, mParts.value(part->getVisualID(), Q_NULLPTR));
	}
}

void Assembly::mapChangedPartTransformation() {
	Visual* part = qobject_cast<Visual*>(sender());

	if (part != Q_NULLPTR) {
		emit changedPartTransformation(mParts.value(part->getVisualID(), Q_NULLPTR));
	}
}

void Assembly::mapChangedPartData(ModelDataVPtr data) {
	Visual* part = qobject_cast<Visual*>(sender());

	if (part != Q_NULLPTR) {
		emit changedPartData(data, mParts.value(part->getVisualID(), Q_NULLPTR));
	}
}

void Assembly::mapChangedPartColor(const double& r, const double& g, 
	const double& b) 
{
	Visual* part = qobject_cast<Visual*>(sender());

	if (part != Q_NULLPTR) {
		emit changedPartColor(r, g, b, mParts.value(part->getVisualID(), Q_NULLPTR));
	}
}

void Assembly::connectPartSignals(Model* part) {
	if (part == Q_NULLPTR) {
		return;
	}

	QObject::connect(
		part, &Model::changedOpacity, 
		this, &Assembly::mapChangedPartOpacity
	);
	QObject::connect(
		part, &Model::changedVisibleState,
		this, &Assembly::mapChangedPartVisible
	);
	QObject::connect(
		part, &Model::changedTransformation,
		this, &Assembly::mapChangedPartTransformation
	);
	QObject::connect(
		part, &Model::changedModelData,
		this, &Assembly::mapChangedPartData
	);
	QObject::connect(
		part, &Model::changedColor,
		this, &Assembly::mapChangedPartColor
	);
}
