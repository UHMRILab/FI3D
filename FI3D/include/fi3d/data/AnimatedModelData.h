#pragma once
/*!
* @author	VelazcoJD
* @file		AnimatedModelData.h
* @class	fi3d::AnimatedModelData
* @brief	Set of ModelData objects which represent an animation.
*/

#include <fi3d/data/DataObject.h>

#include <fi3d/data/ModelData.h>

namespace fi3d {
class AnimatedModelData : public DataObject {

	Q_OBJECT

signals:
	/// @brief Emitted when the animation frames have changed.
	void changedAnimationFrames(const QList<ModelDataVPtr>& animationFrames);

private:
	/// @brief The models that make up the animation.
	QList<ModelDataVPtr> mAnimationFrames;

public:
	/// @brief Constructor.
	AnimatedModelData();

	/// @brief Destructor.
	~AnimatedModelData();

	/// @brief Return the right data type.
	EData getDataType() const override;

	/// @brief Adds a frame to the animation.
	virtual void addAnimationFrame(const ModelDataVPtr animationFrame);

	/// @brief Sets the data to the specified frame.
	virtual void setAnimationFrameData(const ModelDataVPtr frameData, const int& idx);

	/// @brief Removes all the frames and sets the given frames. 
	virtual void setAnimationFrames(const QList<ModelDataVPtr>& animationFrames);

	/// @brief Gets the list of animation frames that make up this animation.
	QList<ModelDataVPtr> getAnimationFrames();

	/// @brief Gets the frame by the given index. Null if out of range.
	virtual ModelDataVPtr getAnimationFrame(const int& index);

	/// @brief Gets a count of the frames that make up this animation.
	virtual int getAnimationFrameCount();

	/// @brief Removes all animation frames
	virtual void removeAllAnimationFrames();
};

/// @brief Alias for a smart pointer of this class.
using AnimatedModelDataPtr = QSharedPointer<AnimatedModelData>;

}