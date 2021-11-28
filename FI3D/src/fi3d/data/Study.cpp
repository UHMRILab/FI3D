#include <fi3d/data/Study.h>
#include <fi3d/logger/Logger.h>

#include <vtkLookupTable.h>
#include <vtkImageMapToColors.h>
#include <vtkMatrix4x4.h>

#include <QMap>
#include <QtMath>

using namespace fi3d;

Study::Study(QString studyID)
	: DataObject(),
	mPatientID(""), mPatientName(""), 
	mDate(""), mTime(""),
	mSeriesSet(),
	mThumbnail()
{
	this->setDataName(studyID);
}

Study::~Study() {}

EData Study::getDataType() const {
	return EData::STUDY;
}

void Study::addSeries(SeriesDataVPtr data) {
	qDebug() << "Enter - Adding series to study:" << this->getDataID();
	if (data.Get() == Q_NULLPTR) {
		qWarning() << "Failed to add series to study=" << this->getDataID() << "because data object is not existent.";
		return;
	}

	data->setDataName(this->getDataName());
	data->setSeriesIndex(mSeriesSet.count());
	mSeriesSet.push_back(data);

	emit changedAddedSeries(data);
	qDebug() << "Exit";
}

SeriesDataVPtr Study::createAndAddSeries() {
	SeriesDataVPtr series = SeriesDataVPtr::New();
	this->addSeries(series);
	return series;
}

int Study::getSeriesCount() const {
	return mSeriesSet.size();
}

SeriesDataVPtr Study::getSeries(const int& index) {
	return mSeriesSet.value(index, Q_NULLPTR);
}

bool Study::isSeriesIndexInRange(const int& seriesIndex) const {
	if (seriesIndex < 0 || seriesIndex >= mSeriesSet.count()) {
		return false;
	}

	return true;
}

void Study::setStudyID(const QString& studyID) {
	this->setDataName(studyID);
}

QString Study::getStudyID() const {
	return this->getDataName();
}

void Study::setPatientID(const QString& patientID) {
	mPatientID = patientID;
}

QString Study::getPatientID() const {
	return mPatientID;
}

void Study::setPatientName(const QString& patientName) {
	mPatientName = patientName;
}

QString Study::getPatientName() const {
	return mPatientName;
}

void Study::setStudyDate(const QString& date) {
	mDate = date;
}

QString Study::getStudyDate() const {
	return mDate;
}

void Study::setStudyTime(const QString& time) {
	mTime = time;
}

QString Study::getStudyTime() const {
	return mTime;
}

void Study::setThumbnail(ImageDataVPtr image) {
	mThumbnail = image;
}

ImageDataVPtr Study::getThumbnail() {
	return mThumbnail;
}
