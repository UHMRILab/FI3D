#include <fi3d/data/data_manager/registered_data/RegisteredStudy.h>

using namespace fi3d;

RegisteredStudy::RegisteredStudy(StudyPtr study) 
	: RegisteredData(),
	mStudy(study),
	mIsDICOMStudy(false),
	mIsFI3DStudy(false),
	mDICOMPaths(), mFI3DFilePath(""),
	mSeriesJson()
{
}

RegisteredStudy::~RegisteredStudy() {}

StudyPtr RegisteredStudy::getStudy() {
	return mStudy;
}

DataID RegisteredStudy::getDataID() {
	if (mStudy.isNull()) {
		return QUuid();
	} else {
		return mStudy->getDataID();
	}
}

void RegisteredStudy::setAsDICOMStudy(QVector<vtkSmartPointer<vtkStringArray>> paths) {
	mIsDICOMStudy = true;
	mDICOMPaths = paths;
}

QVector<vtkSmartPointer<vtkStringArray>> RegisteredStudy::getDICOMPaths() {
	return mDICOMPaths;
}

bool RegisteredStudy::isDICOMStudy() {
	return mIsDICOMStudy;
}

void RegisteredStudy::setAsFI3DStudy(const QString& filePath) {
	mIsFI3DStudy = true;
	mFI3DFilePath = filePath;
}

QString RegisteredStudy::getFI3DStudyPath() {
	return mFI3DFilePath;
}

bool RegisteredStudy::isFI3DStudy() {
	return mIsFI3DStudy;
}

void RegisteredStudy::setMessage(const int& sliceIndex,
	const ESliceOrientation & orientation, const int & seriesIndex, 
	MessagePtr message) 
{
	if (seriesIndex < 0 || seriesIndex >= mStudy->getSeriesCount()) {
		return;
	}

	// Ensure the series JSON object is present
	if (seriesIndex >= mSeriesJson.count()) {
		while (mSeriesJson.count() < seriesIndex) {
			SeriesDataVPtr series = mStudy->getSeries(mSeriesJson.count());
			ImageDataJson seriesJson(series);
			mSeriesJson.append(seriesJson);
		}
	}

	mSeriesJson[seriesIndex].setMessage(sliceIndex, orientation, message);
}

MessagePtr RegisteredStudy::getMessage(const int& sliceIndex, 
	const ESliceOrientation& orientation, const int& seriesIndex) 
{
	if (seriesIndex < 0 || seriesIndex >= mStudy->getSeriesCount()) {
		return Q_NULLPTR;
	}

	// Ensure the series JSON object is present
	if (seriesIndex >= mSeriesJson.count()) {
		while (mSeriesJson.count() <= seriesIndex) {
			SeriesDataVPtr series = mStudy->getSeries(mSeriesJson.count());
			ImageDataJson seriesJson(series);
			mSeriesJson.append(seriesJson);
		}
	}

	return mSeriesJson[seriesIndex].getMessage(sliceIndex, orientation);
}

