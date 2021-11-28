#include <fi3d/data/data_manager/registered_data/RegisteredImage.h>

using namespace fi3d;

RegisteredImage::RegisteredImage(ImageDataVPtr imageData, const QString& path)
	: RegisteredData(), ImageDataJson(imageData),
	mPath(path)
{}

RegisteredImage::~RegisteredImage() {}

DataID RegisteredImage::getDataID() {
	ImageDataVPtr image = this->getImageData();
	if (image.Get() == Q_NULLPTR) {
		return QUuid();
	} else {
		return image->getDataID();
	}
}

QString RegisteredImage::getDataPath() {
	return mPath;
}

void RegisteredImage::setDataPath(const QString& path) {
	mPath = path;
}


