#include <fi3d/data/DataObject.h>

using namespace fi3d;

DataObject::DataObject() 
	: QObject(),
	mID(),
	mIsCacheable(true)
{}

DataObject::~DataObject() {}

DataID DataObject::getDataID() const {
	return mID;
}

void DataObject::setDataName(const QString& name) {
	mID.setDataName(name);
}

QString DataObject::getDataName() const {
	return mID.getDataName();
}

bool DataObject::getCacheable() const {
	return mIsCacheable;
}

void DataObject::setCacheable(const bool& isCacheable) {
	if (mIsCacheable == isCacheable) {
		return;
	}
	mIsCacheable = isCacheable;
	emit changedCacheable(mIsCacheable);
}

void DataObject::dataUpdated() {
	emit changedData();
}

void DataObject::setDataQUuID(const QUuid id) {
	DataID newID(id, mID.getDataName());
	mID = newID;
}

QString DataObject::getQUuIDAsString() {
	return mID.toString();
}
