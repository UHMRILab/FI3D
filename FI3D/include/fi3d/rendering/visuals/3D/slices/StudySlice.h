#pragma once
/*!
* @author	VelazcoJD
* @file		StudySlice.h
* @class	fi3d::StudySlice
* @brief	Like an ImageSlice with an extra dimension in series selection from
*			a study.
*
* A study consists of several 3D imaging data sets, called SeriesData. Each
* series has many slices in the three dimensions (determined by the 3D ImageData
* dimensions). This Visual lets you select a series from the Study to visualize
* in the underlaying ImageSlice.
*/

#include <fi3d/rendering/visuals/3D/slices/ImageSlice.h>

#include <fi3d/data/Study.h>

namespace fi3d {
class StudySlice : public ImageSlice {

	Q_OBJECT

signals:
	/// @brief Emitted when the assigned Study has changed.
	void changedStudy(fi3d::StudyPtr study);

	/// @brief Emitted when the series indices have changed.
	void changedSeriesIndices(const int& seriesMin, const int& seriesMax);

	/// @brief Emitted when the selected series has changed.
	void changedSeriesIndex(const int& seriesIndex);

private:
	/// @brief The Study being visualized.
	StudyPtr mStudy;

	/// @brief The index of the Series that is currently visualized.
	int mSeriesIndex;

	/// @brief Whether to automatically set the patient matrix of the series
	/// when the series is changed.
	bool mAutoSetPatientMatrix;

public:
	/*!
	 * @brief Constructor.
	 *
	 * If no Study is given, a Study with one empty series is created.
	 */
	StudySlice(const QString& name, 
		ESliceOrientation orientation = ESliceOrientation::XY,
		StudyPtr study = Q_NULLPTR);

	/// @brief Destructor.
	~StudySlice();

public slots:
	/// @brief Set the Study data to display.
	virtual void setStudy(fi3d::StudyPtr study);

public:
	/// @brief Get the Study.
	virtual StudyPtr getStudy();

public slots:
	/// @brief Whether to automatically set the patient matrix of the series
	/// when the series is changed.
	void setAutoSetPatientMatrix(const bool& isAuto);

	/// @brief Set the series index to select from the study.
	void setSeriesIndex(const int& seriesIndex);

	/// @brief Sets the series to the next stopping at end.
	void setSeriesIndexToNext();

	/// @brief Sets the series to the previous stopping at beginning.
	void setSeriesIndexToPrevious();

public:
	/// @brief Gets the currently select series.
	SeriesDataVPtr getCurrentSeries();

	/// @brief Gets the index of the series being visualized.
	int getSeriesIndex() const;

	/// @brief Get the index of the minimum series. -1 if empty study.
	int getSeriesMinIndex() const;

	/// @brief Get the index of the maximum series. -1 if empty study.
	int getSeriesMaxIndex() const;

	/// @brief Get the min and max series indices. -1 if empty study.
	void getSeriesIndexRange(int& min, int& max) const;

	/// @brief Whether the index represents a series in the study.
	bool isSeriesIndexInRange(const int& seriesIndex);

	/*!
	*	@name Visual interface implementations.
	*	@brief See Visual for more information.
	*/
	/// @{
	virtual EVisual getVisualType() const override;
	/// @}

private slots:
	/// @brief Catches the changedData signal from the assigned Study and
	/// remits it as the local changedStudy signal
	void onDataUpdated();
};

/// @brief Alias for a smart pointer of this class.
using StudySlicePtr = QSharedPointer<StudySlice>;

}