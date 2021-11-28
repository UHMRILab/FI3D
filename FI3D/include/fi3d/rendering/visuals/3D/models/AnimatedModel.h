#pragma once
/*!
* @author	VelazcoJD
* @file		AnimatedModel.h
* @class	fi3d::AnimatedModel
* @brief	An AnimatedModel renders and cycles through polygonal meshes at a
*			chosen rate.
*/

#include <fi3d/rendering/visuals/3D/models/Model.h>

#include <fi3d/data/AnimatedModelData.h>

#include <QTimer>

namespace fi3d {
class AnimatedModel : public Model {

	Q_OBJECT

signals:
	/// @brief Emitted when the assigned animated model has changed.
	void changedAnimatedModelData(const fi3d::AnimatedModelDataPtr data);

	/// @brief Emmitted when the animation is played or paused.
	void changedAnimationActive(const bool& isAnimationActive);

	/// @brief Emmitted when the animation speed is changed.
	void changedAnimationSpeed(const int& animationSpeed);

protected:
	/// @brief The data to display.
	fi3d::AnimatedModelDataPtr mAnimatedModelData;

	/// @brief The animation frame index being rendered.
	int mFrameIndex;

	/// @brief The animation speed, in milliseconds, defaults to 1000 (1 sec).
	int mAnimationSpeed;

	/// @brief The timer used to cycle through the animation.
	QTimer mAnimationTimer;

	/// @brief The external timer which controls the animation speed.
	QTimer* mExternalTimer;

public:
	/*!
	* @brief Constructor.
	*
	* @param id The visual ID for this Visual.
	* @param data The data to assign. Defaults to an empty data object.
	*/
	AnimatedModel(const QString& name, fi3d::AnimatedModelDataPtr animatedModel = Q_NULLPTR);

	/// @brief Destructor.
	~AnimatedModel();

public slots:
	/// @brief Set the data to display.
	void setAnimatedModelData(fi3d::AnimatedModelDataPtr animatedModel);

	/// @brief Sets the animation speed, in milliseconds.
	void setAnimationSpeed(const int& animationSpeed);

	/// @brief Whether to play or pause the animation.
	void setAnimate(const bool& animate);

	/// @brief Toggles the animation.
	void toggleAnimation();

	/// @brief Sets the animation frame.
	void setFrame(const int& frameIndex);

	/// @brief Sets animation frame to next, stopping at end.
	void setNextFrame();

	/// @brief Sets animation frame to previous, stopping at beginning.
	void setPreviousFrame();

	/// @brief Sames as setNextFrame but cycles from end to beginning.
	void setNextFrameCycle();

public:
	/// @brief get the data.
	fi3d::AnimatedModelDataPtr getAnimatedModelData();

	/// @brief The animation speed, in milliseconds.
	int getAnimationSpeed();

	/// @brief True if currently being animated. False if paushed. */
	bool isAnimationActive();

	/// @brief Gets the index of the frame currently being rendered. */
	int getCurrentFrameIndex();

	/*!
	 * @brief Sets the timer in charge of the animation.
	 *
	 * This will disable the internal timer and use the given one instead. 
	 * Null timer disables the external timer.
	 *
	 * When the external timer is set, the internal timer and animation speed 
	 * is not set. Instead, the animation speed and enable/disable are handled 
	 * by the given timer. When the external timer is set, the setAnimate(true)
	 * will always fail (since it's using the external timer) and the 
	 * isAnimationActive will always return false. Additionally, 
	 * setAnimationSpeed will store the given speed but will not affect the 
	 * animation speed (until the external timer is removed).
	 */
	void setExternalTimer(QTimer* timer);

	/// @brief Removes the external timer, enabling the internal timer.
	void disableExternalTimer();

	/*!
	*	@name Visual interface implementations.
	*	@brief See Visual for more information.
	*/
	/// @{
	virtual EVisual getVisualType() const override;
	/// @}
};

/// @brief Alias for a smart pointer of this class.
using AnimatedModelPtr = QSharedPointer<AnimatedModel>;

}