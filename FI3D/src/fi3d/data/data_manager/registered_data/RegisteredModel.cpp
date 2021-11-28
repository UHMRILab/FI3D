#include <fi3d/data/data_manager/registered_data/RegisteredModel.h>

using namespace fi3d;

RegisteredModel::RegisteredModel(ModelDataVPtr modelData, const QString& path) 
	: RegisteredData(),
	mModelData(modelData),
	mPath(path),
	mMessage(new Message())
{}

RegisteredModel::~RegisteredModel() {}

ModelDataVPtr RegisteredModel::getModelData() {
	return mModelData;
}

DataID RegisteredModel::getDataID() {
	if (mModelData.Get() == Q_NULLPTR) {
		return QUuid();
	} else {
		return mModelData->getDataID();
	}
}

QString RegisteredModel::getDataPath() {
	return mPath;
}

void RegisteredModel::setDataPath(const QString& path) {
	mPath = path;
}

MessagePtr RegisteredModel::getMessage() {
	return mMessage;
}

void RegisteredModel::setMessage(MessagePtr message) {
	mMessage = message;
}
