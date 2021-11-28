#include <fi3d/rendering/visuals/3D/models/AnimatedModel.h>

#include <fi3d/logger/Logger.h>

using namespace fi3d;

AnimatedModel::AnimatedModel(const QString& id, 
	AnimatedModelDataPtr animatedModel)
	: Model(id),
	mAnimatedModelData(animatedModel),
	mFrameIndex(0),
	mAnimationSpeed(1000),
	mAnimationTimer(),
	mExternalTimer(Q_NULLPTR)
{
	if (mAnimatedModelData.isNull()) {
		mAnimatedModelData.reset(new AnimatedModelData());
	}

	this->setModelData(mAnimatedModelData->getAnimationFrame(mFrameIndex));
	
	QObject::connect(
		&mAnimationTimer, SIGNAL(timeout()), 
		this, SLOT(setNextFrameCycle())
	);
}

AnimatedModel::~AnimatedModel() {}

void AnimatedModel::setAnimatedModelData(AnimatedModelDataPtr animatedModel) {
	mAnimatedModelData = animatedModel;
	mFrameIndex = 0;
	if (mAnimatedModelData.isNull()) {
		mAnimatedModelData.reset(new AnimatedModelData());
	}

	this->setModelData(mAnimatedModelData->getAnimationFrame(mFrameIndex));

	emit changedAnimatedModelData(mAnimatedModelData);
}

void AnimatedModel::setAnimationSpeed(const int& animationSpeed) {
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

void AnimatedModel::setAnimate(const bool& animate) {
	if (mAnimationTimer.isActive() == animate) {
		return;
	}

	if (mExternalTimer != Q_NULLPTR) {
		qWarning() << "Animation failed to start because the AnimatedModel" <<
			this->getVisualID() << "has an external timer.";
		return;
	}

	if (animate) {
		mAnimationTimer.start(mAnimationSpeed);
	} else {
		mAnimationTimer.stop();
	}

	emit changedAnimationActive(mAnimationTimer.isActive());
}

void AnimatedModel::toggleAnimation() {
	this->setAnimate(!mAnimationTimer.isActive());
}

void AnimatedModel::setFrame(const int& frameIndex) {
	if (mAnimatedModelData->getAnimationFrameCount() == 0 || 
		frameIndex < 0 || 
		frameIndex >= mAnimatedModelData->getAnimationFrameCount()) 
	{
		return;
	}

	mFrameIndex = frameIndex;
	this->setModelData(mAnimatedModelData->getAnimationFrame(mFrameIndex));	
}

void AnimatedModel::setNextFrame() {
	if (mAnimatedModelData->getAnimationFrameCount() == 0) {
		return;
	}

	if (mFrameIndex == mAnimatedModelData->getAnimationFrameCount() - 1) {
		return;
	}

	mFrameIndex++;
	this->setModelData(mAnimatedModelData->getAnimationFrame(mFrameIndex));
}

void AnimatedModel::setPreviousFrame() {
	if (mAnimatedModelData->getAnimationFrameCount() == 0) {
		return;
	}

	if (mFrameIndex == 0) {
		return;
	}

	mFrameIndex--;
	this->setModelData(mAnimatedModelData->getAnimationFrame(mFrameIndex));
}

void AnimatedModel::setNextFrameCycle() {
	if (mAnimatedModelData->getAnimationFrameCount() == 0) {
		return;
	}

	mFrameIndex = (mFrameIndex + 1) % mAnimatedModelData->getAnimationFrameCount();
	this->setModelData(mAnimatedModelData->getAnimationFrame(mFrameIndex));
}

AnimatedModelDataPtr AnimatedModel::getAnimatedModelData() {
	return mAnimatedModelData;
}

int AnimatedModel::getAnimationSpeed() {
	return mAnimationSpeed;
}

bool AnimatedModel::isAnimationActive() {
	return mAnimationTimer.isActive();
}

int AnimatedModel::getCurrentFrameIndex() {
	return mFrameIndex;
}

void AnimatedModel::setExternalTimer(QTimer* timer) {
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
		this, SLOT(setNextFrameCycle()), 
		Qt::UniqueConnection
	);
}

void AnimatedModel::disableExternalTimer() {
	if (mExternalTimer == Q_NULLPTR) {
		return;
	}

	QObject::disconnect(
		mExternalTimer, SIGNAL(timeout()), 
		this, SLOT(setNextFrameCycle())
	);
	mExternalTimer = Q_NULLPTR;
}

EVisual AnimatedModel::getVisualType() const {
	return EVisual::ANIMATED_MODEL;
}
