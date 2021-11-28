#pragma once
/*!
* @author	VelazcoJD
* @file		ImageSlice.h
* @class	fi3d::ImageSlice
* @brief	An ImageSlice visualizes a slice of data from ImageData.
*
* The slice of an image can be in one of three orientations: Transverse (XY),
* Sagittal (YZ), or Coronal (XZ). This Visual visualizes a slice of data
* obtained from an ImageData data object. A slice of data is uniquely identified
* by its orientation and index.
*/

#include <fi3d/rendering/visuals/3D/Visual3D.h>
#include <fi3d/rendering/visuals/3D/slices/ESliceOrientation.h>
#include <fi3d/rendering/visuals/3D/models/Model.h>

#include <fi3d/data/ImageData.h>

#include <vtkImageActor.h>

class vtkImageSliceMapper;
class vtkPlane;

namespace fi3d {
class ImageSlice : public Visual3D {

	Q_OBJECT

signals:
	/// @brief Emitted When the ImageSlice has changed its data.
	void changedImageData(fi3d::ImageDataVPtr data);

	/// @brief Emitted When the ImageSlice orientation has changed.
	void changedOrientation(const fi3d::ESliceOrientation& orientation);

	/// @brief Emitted When the ImageSlice slice indices have changed.
	void changedSliceIndices(const int& sliceMin, const int& sliceMax);

	/// @brief Emitted When the ImageSlice slice has changed.
	void changedSlice(const int& sliceIndex, const fi3d::ESliceOrientation& orientation);

private:
	///  @brief A visual which has a frame around the 3D imaging data set.
	ModelPtr mDataFrame;

protected:
	/// @brief The ImageData to visualize.
	fi3d::ImageDataVPtr mImageData;

	/// @brief The current orientation of the rendered slice. 
	ESliceOrientation mOrientation;

	/// @brief Stores the slice index of each orientation of a series.
	typedef struct SeriesSliceIndices {
		int transverseIndex;
		int sagittalIndex;
		int coronalIndex;
	} SeriesSliceIndices;

	/*!
	 * @brief The slice indices set on each series.
	 *
	 * These are saved to remember the slice that was previously set.
	 */
	SeriesSliceIndices mOrientationSliceIndices;

	/// @brief The VTK mapper used to map the data to screen.
	vtkSmartPointer<vtkImageSliceMapper> mMapper;

	/// @brief the VTK actor used to render the data on screen.
	vtkSmartPointer<vtkImageActor> mActor;

public:
	/*!
	 * @brief Constructor
	 *
	 * @param id The visual ID for this Visual.
	 * @param orientation The initial orientation. Defaults to XY.
	 * @param data The ImageData to visualize. Defaults to empty object.
	 */
	ImageSlice(const QString& id, 
		ESliceOrientation orientation = ESliceOrientation::XY,
		fi3d::ImageDataVPtr data = fi3d::ImageDataVPtr::New());

	/// @brief Destructor. */
	~ImageSlice();

public slots:
	/*!
	 * @brief Sets the imaging data to visualize.
	 *
	 * Setting a null object creates a new empty data object. */
	virtual void setImageData(fi3d::ImageDataVPtr imageData);

	/*!
	 * @brief Gets the current visualized imaging data.
	 *
	 * The returned data object is never null. When nothing is visualized, 
	 * it returns an empty data object.
	 */
	virtual fi3d::ImageDataVPtr getImageData();

	/// @brief Set the orientation of this slice to Transverse (XY).
	virtual void setOrientationXY();

	/// @brief Set the orientation of this slice to Sagittal (YZ).
	virtual void setOrientationYZ();

	/// @brief Set the orientation of this slice to Coronal (XZ).
	virtual void setOrientationXZ();

	/// @brief Set the orientation of this slice.
	virtual void setOrientation(const fi3d::ESliceOrientation& orientation);

	/// @brief Cycles the orientation in the order: XY, YZ, XZ, XY.
	void setNextOrientation();

	/// @brief Cycles the orientation in the order: XY, YZ, XZ, XY.
	void setPreviousOrientation();

protected:
	/// @brief Sets the actor's orientation. Used internally.
	virtual void applyOrientationChange();

public:
	/// @brief Get the orientation.
	virtual ESliceOrientation getSliceOrientation() const;

public slots:
	/*!
	 * @brief Sets the slice.
	 *
	 * If the given slice index is out of range, nothing happens.
	 */
	virtual void setSlice(const int& sliceIndex, const fi3d::ESliceOrientation& orientation);

	/*!
	 * @brief Sets the slice under the same orientation.
	 *
	 * If the given slice index is out of range, nothing happens.
	 */
	virtual void setSliceIndex(const int& sliceIndex);

	/// @brief Sets to the 0th slice under the same orientation.
	virtual void setSliceIndexToBeginning();

	/// @brief Sets to the middle slice under the same orientation.
	virtual void setSliceIndexToMiddle();

	/// @brief Sets to the last slice under the same orientation.
	virtual void setSliceIndexToEnd();

	/// @brief Sets to the next slice, stopping at end.
	virtual void setSliceIndexToNext();

	/// @brief Sets to the next slice, stopping at beginning.
	virtual void setSliceIndexToPrevious();

public:
	/// @brief Gets the index of the current visualized slice.
	virtual int getSliceIndex() const;

	/// @brief Gets the index of the minimum slice index.
	virtual int getSliceMinIndex() const;

	/// @brief Gets the index of the maximum slice index.
	virtual int getSliceMaxIndex() const;

	/// @brief Gets the minimum and maximum slice indices.
	virtual void getSliceRange(int& min, int& max) const;

	/// @brief Check if given slice index is in range of the 
	/// current orientation.
	virtual bool isSliceIndexInRange(const int& sliceIndex);

	/// @brief Based on current slice and orientation, 
	/// create a plane out of it.
	virtual vtkSmartPointer<vtkPlane> getSliceAsPlane() const;

	/// @brief Gets the actor representing the frame around the 3D image data.
	ModelPtr getDataFrameVisual();

	/*!
	* @brief Given a voxel's position in an image (i, j, k),
	*	determine its global coordinate (x, y, z).
	*
	* The resulting coordinates are calculated using the slice separation values
	* and the matrix that defines the Visual's position and orientation in 3D 
	* space. If the given indices are out of range, the values of x, y, and z 
	* are unchanged and false is returned. True is returned when the indices 
	* represent a voxel in the image.
	*
	* @param i ith slice index.
	* @param j jth slice index.
	* @param k kth slice index.
	* @param x Variable to store the x value.
	* @param y Variable to store the y value.
	* @param z Variable to store the z value.
	* @return Whether the coordinates were calculated or not.
	*/
	virtual bool getVoxelCoordinates(const int& i, const int& j, const int& k, double& x, double& y, double& z) const;

	/*!
	* @brief Given a global coordinate (x, y, z) determine in which
	*	voxel (i, j, k) such coordinate is in.
	*
	* This function does the inverse from getVoxelCoordinates. If the 
	* coordinates do not lie in the image, false is returned.
	*
	* @param x The x global coordinate.
	* @param y The y global coordinate.
	* @param z The z global coordinate.
	* @param i Variable to store ith slice index.
	* @param j Variable to store jth slice index.
	* @param k Variable to store kth slice index.
	* @return Whether the coordinate lies in the image or not.
	*/
	virtual bool getVoxelFromCoordinates(const double& x, const double& y,
		const double& z, int& i, int& j, int& k) const;

protected:
	/// @brief Calculates the slice indices when new data is assigned.
	virtual void calculateSliceIndices();

private slots:
	/// @brief When the transform of the ImageSlice changes, it has to be applied
	///	to the data frame as well.
	void onTransformChange();

	/// @brief Catches the changedData signal from the assigned ImageData and
	/// remits it as the local changedImageData signal
	void onDataUpdated();

public:
	/*!
	*	@name Visual interface implementations.
	*	@brief See Visual for more information.
	*/
	/// @{
	virtual EVisual getVisualType() const override;
	virtual vtkImageActor* getActor() override;
	virtual void setOpacity(const double& a) override;
	virtual double getOpacity() const override;
	virtual void setVisible(const bool& isVisible) override;
	virtual bool isVisible() const override;
	virtual void setPickable(const bool& isPickable) override;
	virtual bool isPickable() const override;
	/// @}
};

/// @brief Alias for a smart pointer of this class.
using ImageSlicePtr = QSharedPointer<ImageSlice>;

}