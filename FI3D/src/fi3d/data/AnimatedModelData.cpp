#include <fi3d/data/AnimatedModelData.h>

using namespace fi3d;

AnimatedModelData::AnimatedModelData()
	: DataObject()
{}

AnimatedModelData::~AnimatedModelData() {}

EData AnimatedModelData::getDataType() const {
	return EData::ANIMATED_MODEL;
}

void AnimatedModelData::addAnimationFrame(const ModelDataVPtr animationFrame) {
	animationFrame->setDataName(tr("%1_Frame%2").arg(this->getDataName()).arg(mAnimationFrames.count()));
	mAnimationFrames.append(animationFrame);
}

void AnimatedModelData::setAnimationFrameData(const ModelDataVPtr frameData, const int& idx) {
	if (idx < 0 || idx >= mAnimationFrames.count()) {
		return;
	}

	mAnimationFrames[idx] = frameData;
}

void AnimatedModelData::setAnimationFrames(const QList<ModelDataVPtr>& animationFrames) {
	mAnimationFrames.clear();

	for (ModelDataVPtr modelData : animationFrames) {
		mAnimationFrames.append(modelData);
	}
}

QList<ModelDataVPtr> AnimatedModelData::getAnimationFrames() {
	return mAnimationFrames;
}

ModelDataVPtr AnimatedModelData::getAnimationFrame(const int& index) {
	if (mAnimationFrames.isEmpty() || index < 0 || index >= mAnimationFrames.count()) {
		return Q_NULLPTR;
	}
	return mAnimationFrames.at(index);
}

int AnimatedModelData::getAnimationFrameCount() {
	return mAnimationFrames.count();
}

void AnimatedModelData::removeAllAnimationFrames() {
	mAnimationFrames.clear();
}
