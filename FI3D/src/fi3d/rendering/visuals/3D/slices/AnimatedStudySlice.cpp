#include <fi3d/rendering/visuals/3D/slices/AnimatedStudySlice.h>

#include <fi3d/logger/Logger.h>

using namespace fi3d;

AnimatedStudySlice::AnimatedStudySlice(const QString& name, 
	ESliceOrientation orientation, StudyPtr study)
	: StudySlice(name, orientation, study),
	mAnimationSpeed(1000),
	mAnimationTimer(),
	mExternalTimer(Q_NULLPTR)
{
	QObject::connect(
		&mAnimationTimer, SIGNAL(timeout()),
		this, SLOT(setSeriesIndexToNextCycle())
	);
}

AnimatedStudySlice::~AnimatedStudySlice() {}

void AnimatedStudySlice::setAnimationSpeed(const int& animationSpeed) {
	if (mAnimationSpeed == animationSpeed) {
		return;
	}

	mAnimationSpeed = animationSpeed;
	if (mAnimationTimer.isActive()) {
		mAnimationTimer.stop();
		mAnimationTimer.start(mAnimationSpeed);
	}

	emit changedAnimationSpeed(mAnimationSpeed);
}

void AnimatedStudySlice::setAnimate(const bool& animate) {
	if (mAnimationTimer.isActive() == animate) {
		return;
	}

	if (mExternalTimer != Q_NULLPTR) {
		qWarning() << "Animation failed to start because the" <<
			"AnimatedStudySlice" << this->getVisualID() <<
			"has an external timer.";
		return;
	}

	if (animate) {
		mAnimationTimer.start(mAnimationSpeed);
	} else {
		mAnimationTimer.stop();
	}

	emit changedAnimationActive(mAnimationTimer.isActive());
}

void AnimatedStudySlice::toggleAnimation() {
	this->setAnimate(!mAnimationTimer.isActive());
}

void AnimatedStudySlice::setSeriesIndexToNextCycle() {
	if (this->getSeriesMaxIndex() == 0) {
		return;
	}

	int seriesIndex = this->getSeriesIndex();
	seriesIndex = (seriesIndex + 1) % (this->getSeriesMaxIndex() + 1);

	this->setSeriesIndex(seriesIndex);
}

int AnimatedStudySlice::getAnimationSpeed() {
	return mAnimationSpeed;
}

bool AnimatedStudySlice::isAnimationActive() {
	return mAnimationTimer.isActive();
}

void AnimatedStudySlice::setExternalTimer(QTimer* timer) {
	if (timer == Q_NULLPTR) {
		this->disableExternalTimer();
		return;
	}

	if (this->isAnimationActive()) {
		this->setAnimate(false);
	}

	mExternalTimer = timer;
	QObject::connect(
		mExternalTimer, SIGNAL(timeout()),
		this,SLOT(setSeriesIndexToNextCycle()),
		Qt::UniqueConnection
	);
}

void AnimatedStudySlice::disableExternalTimer() {
	if (mExternalTimer == Q_NULLPTR) {
		return;
	}

	QObject::disconnect(
		mExternalTimer, SIGNAL(timeout()),
		this, SLOT(setNextFrameCycle())
	);
	mExternalTimer = Q_NULLPTR;
}

EVisual AnimatedStudySlice::getVisualType() const {
	return EVisual::ANIMATED_STUDY_SLICE;
}
