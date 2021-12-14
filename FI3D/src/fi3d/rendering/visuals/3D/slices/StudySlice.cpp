#include <fi3d/rendering/visuals/3D/slices/StudySlice.h>

#include <fi3d/logger/Logger.h>

#include <vtkImageSliceMapper.h>

using namespace fi3d;

StudySlice::StudySlice(const QString& name, ESliceOrientation orientation, StudyPtr study)
	: ImageSlice(name, orientation),
	mAutoSetPatientMatrix(true)
{
	this->setStudy(study);
}

StudySlice::~StudySlice() {}

void StudySlice::setStudy(StudyPtr study) {
	qDebug() << "Enter - Setting study to StudySlice:" << this->getVisualID();
	if (!study.isNull() && mStudy == study) {
		qDebug() << "Exit - The same stuy was trying to be set.";
		return;
	}

	if (!mStudy.isNull()) {
		QObject::disconnect(
			mStudy.get(), &Study::changedRemovedSeries,
			this, &StudySlice::onRemovedSeries);
		// Remove old study's signals
		QObject::disconnect(
			mStudy.get(), &Study::changedData,
			this, &StudySlice::onDataUpdated);
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
		if (mAutoSetPatientMatrix) {
			this->setTransformData(mStudy->getSeries(mSeriesIndex)->getPatientMatrix());
		}
	}
	emit changedStudy(mStudy);

	if (mStudy->getSeriesCount() == 0) {
		emit changedSeriesIndices(-1, -1);
	} else {
		emit changedSeriesIndices(0, mStudy->getSeriesCount() - 1);
	}
	
	emit changedSeriesIndex(mSeriesIndex);

	// Correct the rendered slice if the removed series is affected
	QObject::connect(
		mStudy.get(), &Study::changedRemovedSeries,
		this, &StudySlice::onRemovedSeries,
		Qt::UniqueConnection);
	// Re-emit signal when visualized data changes internally
	QObject::connect(
		mStudy.get(), &Study::changedData,
		this, &StudySlice::onDataUpdated,
		Qt::UniqueConnection);

	qDebug() << "Exit";
}

StudyPtr StudySlice::getStudy() {
	return mStudy;
}

void StudySlice::setAutoSetPatientMatrix(const bool& isAuto) {
	if (mAutoSetPatientMatrix == isAuto) {
		return;
	}

	mAutoSetPatientMatrix = isAuto;
}

void StudySlice::setSeriesIndex(const int& seriesIndex) {
	if (mSeriesIndex == seriesIndex || 
		!mStudy->isSeriesIndexInRange(seriesIndex)) 
	{
		return;
	}

	// Set the new series
	mSeriesIndex = seriesIndex;

	SeriesDataVPtr series = mStudy->getSeries(mSeriesIndex);
	this->setImageData(series);

	if (mAutoSetPatientMatrix) {
		this->setTransformData(series->getPatientMatrix());
	}

	emit changedSeriesIndex(this->getSeriesIndex());
}

void StudySlice::setSeriesIndexToNext() {
	if (mSeriesIndex == this->getSeriesMaxIndex()) {
		return;
	}

	this->setSeriesIndex(mSeriesIndex + 1);
}

void StudySlice::setSeriesIndexToPrevious() {
	if (mSeriesIndex == this->getSeriesMinIndex()) {
		return;
	}

	this->setSeriesIndex(mSeriesIndex - 1);
}

SeriesDataVPtr StudySlice::getCurrentSeries() {
	if (mStudy->getSeriesCount() == 0) {
		return SeriesDataVPtr::New();
	}

	return mStudy->getSeries(mSeriesIndex);
}

int StudySlice::getSeriesIndex() const {
	return mSeriesIndex;
}

int StudySlice::getSeriesMinIndex() const {
	if (mStudy->getSeriesCount() == 0) {
		return -1;
	} else {
		return 0;
	}
}

int StudySlice::getSeriesMaxIndex() const {
	if (mStudy->getSeriesCount() == 0) {
		return -1;
	} else {
		return mStudy->getSeriesCount() - 1;
	}
}

void StudySlice::getSeriesIndexRange(int& min, int& max) const {
	min = this->getSeriesMinIndex();
	max = this->getSeriesMaxIndex();
}

bool StudySlice::isSeriesIndexInRange(const int& seriesIndex) {
	return mStudy->isSeriesIndexInRange(seriesIndex);
}

EVisual StudySlice::getVisualType() const {
	return EVisual::STUDY_IMAGE_SLICE;
}

void StudySlice::onRemovedSeries(SeriesDataVPtr removedSeries) {
	int seriesIndex = removedSeries->getSeriesIndex();

	// If it was the last series, then set back to an empty study
	if (mStudy->getSeriesCount() == 0) {
		mSeriesIndex = -1;
		this->setImageData(Q_NULLPTR);
		return;
	}

	if (seriesIndex > mSeriesIndex) {
		return;
	} else if (seriesIndex == mSeriesIndex) {
		if (mStudy->isSeriesIndexInRange(seriesIndex)) {
			// Temporarely set to -1 and reassign so the new slice in that
			// position gets rendered
			mSeriesIndex = -1;
			this->setSeriesIndex(seriesIndex);
		} else {
			// If it was the last one to be removed, go back down by 1
			this->setSeriesIndex(seriesIndex - 1);
		}
	} else {
		// Don't do anything, simply change the current series to be 1
		// minus than what it used to be
		mSeriesIndex -= 1;
	}
}

void StudySlice::onDataUpdated() {
	emit changedStudy(mStudy);
}