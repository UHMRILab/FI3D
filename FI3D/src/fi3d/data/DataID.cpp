#include <fi3d/data/DataID.h>

using namespace fi3d;

DataID::DataID(const QUuid& id, const QString& name) 
	: QUuid(id),
	mName(name)
{}

DataID::~DataID() {}

QString DataID::getDataName() const {
	return mName;
}

void DataID::setDataName(const QString& name) {
	mName = name;
}

bool DataID::isIDValid() const {
	return !this->isNull();
}

DataID& DataID::operator=(const DataID& other) {
	QUuid::operator=(other);
	this->mName = other.mName;
	return *this;
}
