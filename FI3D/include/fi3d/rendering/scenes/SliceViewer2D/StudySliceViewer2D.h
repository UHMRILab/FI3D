#pragma once
/*!
* @author	VelazcoJD
* @file		StudySliceViewer2D.h
* @class	fi3d::StudySliceViewer2D 
* @brief	Same as ImageSliceViewer2D but visualises a study instead.
*
* A regular ImageSliceViewer2D renders a slice of image for the user to see, 
* always in 2D. Aside from adding the dimension, of selecting a series in a 
* study, this class also "caps" the models by the image slice of the series 
* after it is transformed by the patient matrix. This is helpful since capping 
* models by the slice requires the position and orientation of the slice. Even
* though the slice may be in a fixed orientation, the orientation itself can be
* rotated. This viewer takes that into account.
*/

#include <fi3d/rendering/scenes/SliceViewer2D/ImageSliceViewer2D.h>

namespace fi3d {
class StudySliceViewer2D : public ImageSliceViewer2D {

	Q_OBJECT

signals:
	/// @brief Emitted When the assigned study has changed. */
	void changedStudy(fi3d::StudyPtr study);

	/// @brief Emitted When the selected series has changed.
	void changedSeriesIndices(const int& min, const int& max);

	/// @brief Emitted When the selected series has changed. */
	void changedSeriesIndex(const int& index);

private:
	/// @brief The study being visualized. 
	fi3d::StudyPtr mStudy;

	/// @brief The index of the series that is currently visualized. 
	int mSeriesIndex;

public:
	/*!
	* @brief Constructor for the window controller.
	*
	* @param widget The QtVTKOpenGLWidget used to render the scene.
	* @param id The ID of this scene.
	* @param orientation The initial orientation. Defaults to XY.
	*/
	StudySliceViewer2D(QVTKOpenGLStereoWidget* widget, const QString& id,
		ESliceOrientation orientation = ESliceOrientation::XY);

	/// @brief Destructor. 
	~StudySliceViewer2D();

public slots:
	/// @brief Set the study data to display.
	virtual void setStudy(fi3d::StudyPtr study);

public:
	/// @brief Get the study data being visualized.
	virtual fi3d::StudyPtr getStudy();

public:
	/*!
	*	@name Model Modifiers
	*	@brief Capped models are rendered considering the patient matrix.
	*/
	/// @{
	virtual ModelPtr addModel(const QString& visualID, fi3d::ModelDataVPtr data = fi3d::ModelDataVPtr::New()) override;
	
	virtual bool addVisual(VisualPtr visual) override;
	/// @}

public slots:
	/// @brief Set the series index to select from the study.
	void setSeriesIndex(const int& seriesIndex);

private slots:
	/// @brief Handles a series being removed.
	void onRemovedSeries(SeriesDataVPtr removedSeries);

	/// @brief Catches the changedData signal from the assigned Study and
	/// remits it as the local changedStudy signal
	void onDataUpdated();

public:
	/// @brief Gets the index of the series that is currently visualized.
	int getSeriesIndex() const;

	/// @brief Get the index of the minimum series. -1 if empty Study.
	int getSeriesMinIndex() const;

	/// @brief Get the index of the maximum series. -1 if empty Study. */
	int getSeriesMaxIndex() const;

	/// @brief Get the minimum and maximum series index. -1 if empty Study. */
	void getSeriesIndexRange(int& min, int& max) const;

	/// @brief Whether the given series index represents a series in the study.
	bool isSeriesIndexInRange(const int& seriesIndex);

	/// @brief Same as base class, but takes patient matrix into account.
	virtual vtkSmartPointer<vtkPlane> getCutPlane() const override;

	/// @brief Same as base class, but takes patient matrix into account.
	virtual bool getVoxelCoordinates(const int& i, const int& j, const int& k, 
		double& x, double& y, double& z) const override;

	/// @brief Same as base class, but takes patient matrix into account.
	virtual bool getVoxelFromCoordinates(const double& x, const double& y,
		const double& z, int& i, int& j, int& k) const override;
};

/// @brief Alias for a smart pointer of this class.
using StudySliceViewer2DPtr = QSharedPointer<StudySliceViewer2D>;

}