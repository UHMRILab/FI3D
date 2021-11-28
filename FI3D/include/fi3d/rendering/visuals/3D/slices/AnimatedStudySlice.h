#pragma once
/*!
* @author	VelazcoJD
* @file		AnimatedStudySlice.h
* @class	fi3d::AnimatedStudySlice
* @brief	An AnimatedStudySlice is a StudySlice with a timer that allows
*			it to cycle through the series in the Study.
*/

#include <fi3d/rendering/visuals/3D/slices/StudySlice.h>

#include <QTimer>

namespace fi3d {
class AnimatedStudySlice : public StudySlice {

	Q_OBJECT

signals:
	/// @brief Emmitted when the animation is played or paused.
	void changedAnimationActive(const bool& isAnimationActive);

	/// @brief Emmitted when the animation speed is changed.
	void changedAnimationSpeed(const int& animationSpeed);

private:
	/// @brief The animation speed, in milliseconds, defaults to 1000 (1 sec).
	int mAnimationSpeed;

	/// @brief The timer used to cycle through the animation. */
	QTimer mAnimationTimer;

	/// @brief The external timer which controls the animation speed.
	QTimer* mExternalTimer;

public:
	/*!
	 * @brief Constructor.
	 *
	 * @param id The visual ID for this Visual.
	 * @param orientation The orientation to start with. Defaults to XY.
	 * @param study The study. Defaults to study with one empty series.
	 */
	AnimatedStudySlice(const QString& name, 
		ESliceOrientation orientation = ESliceOrientation::XY,
		StudyPtr study = Q_NULLPTR);

	/// @brief Destructor.
	~AnimatedStudySlice();

public slots:
	/// @brief Sets the animation speed, in milliseconds.
	void setAnimationSpeed(const int& animationSpeed);

	/// @brief Whether to play or pause the animation.
	void setAnimate(const bool& animate);

	/// @brief Toggles the animation.
	void toggleAnimation();

	/// @brief Sames as setSeriesIndexToNext but cycles from end to
	/// beginning.
	void setSeriesIndexToNextCycle();

public:
	/// @brief The animation speed, in milliseconds.
	int getAnimationSpeed();

	/// @brief True if currently being animated. False if paushed.
	bool isAnimationActive();

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
using AnimatedStudySlicePtr = QSharedPointer<AnimatedStudySlice>;

}
