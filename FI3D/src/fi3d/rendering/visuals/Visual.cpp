#include <fi3d/rendering/visuals/Visual.h>

using namespace fi3d;

Visual::Visual(const QString& id, const bool& is3D)
	: QObject(),
	mID(id),
	mIs3D(is3D),
	mIsHolographic(true),
	mIsInteractable(true)
{
}

Visual::~Visual() {}

void Visual::setHolographic(const bool& is) {
	if (mIsHolographic == is) {
		return;
	}
	mIsHolographic = is;
	emit changedHolographicState(mIsHolographic);
}

void Visual::setInteractable(const bool& isInteractable) {
	if (mIsInteractable == isInteractable)
		return;
	
	// If interactable, then it must be pickable.
	if (isInteractable)
		this->setPickable(true);

	mIsInteractable = isInteractable;
	emit changedInteractableState(mIsInteractable);
}

QString Visual::getVisualID() const {
	return mID;
}

bool Visual::is3D() const {
	return mIs3D;
}

bool Visual::isHolographic() const {
	return mIsHolographic;
}

bool Visual::isInteractable() const {
	return mIsInteractable;
}
