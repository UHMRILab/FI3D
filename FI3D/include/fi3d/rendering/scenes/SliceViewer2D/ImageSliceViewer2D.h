#pragma once
/*!
* @author	VelazcoJD
* @date		12/12/2018
* @revised	02/02/2020
* @file		ImageSliceViewer2D.h
* @class	fi3d::ImageSliceViewer2D
* @brief	A scene specifically used to display a slice in 2D.
*
* This class is to not be confused with a Scene that is assigned an ImageSlice.
* This class uses vtk's native ImageViewer which offers some image processing
* functionalities. It also takes care to make sure the window cannot be rotated
* (as it is a slice shown only). In essence, this is a 2D window and makes
* viewing a single slice simpler to manage. If one wants to rotate a slice
* and/or assign more slices, use a Scene and assign ImageSlice objects to it.
*
* This scene keeps two versions of the models added. One version holds the
* original data and is not rendered. It is used to observe modifications. That
* is, you should use these models for transformations and data changes. These
* models will then modify their capped version accordingly.
*
* Adding models keep the Model in a separate list and out of the renderer. A
* new Model is created with the same information but its data capped by the
* slice plane. The original model is always returned back. You can get the
* capped data of the Model by the getCappedModelData function. All interactions
* should be made with the original Model and this object will make sure the
* changes are applied to its capped version.
*
* The Model modifiers, such as translate rotate, etc., are applied to the
* original model. The original Model is connected to its capped version through
* signals which then updates its capped version. With that in mind, it is safe
* to assign models that appear in other scenes since these will not be added to
* the renderer (their capped version will).
*/

#include <fi3d/rendering/scenes/Scene.h>

class vtkImageViewer2;

namespace fi3d {
class ImageSliceViewer2D : public Scene {

	Q_OBJECT

signals:
	/// @brief Emitted when the visualized data has changed.
	void changedImageData(fi3d::ImageDataVPtr data);

	/// @brief Emitted when the slice indices have changed.
	void changedSliceIndices(const int& min, const int& max);

	/// @brief Emitted when the selected slice has changed.
	void changedSlice(const int& sliceIndex, const ESliceOrientation& orientation);

	/// @brief Emitted when the selected orientation has changed.
	void changedOrientation(const ESliceOrientation& orientation);

	/// @brief Emitted when the message subtitle text has changed.
	void changedMessageText(const QString& text);

protected:
	/// @brief The data displayed in the viewer.
	fi3d::ImageDataVPtr mImageData;

	/// @brief The orientation of this 2D Window.
	ESliceOrientation mOrientation;

	/// @brief Store the slice index of each orientation of a series.
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

	/// @brief The viewer in charge of displaying the slice to window.
	vtkSmartPointer<vtkImageViewer2> mViewer;

	/// @brief Displays slice coordinate information on top left.
	SubtitlePtr mCoordinateText2D;

	/// @brief Displays arbitrary message on top right,
	SubtitlePtr mMessageText2D;

	/// @brief Displays orientation information on bottom left.
	SubtitlePtr mOrientationText2D;

	/// @brief Displays slice information on bottom right.
	SubtitlePtr mSliceIndexText2D;

	/*!
	 * @brief Added models are placed here.
	 *
	 * The displayed models contain these objects after their data has
	 * been capped.
	 *
	 * This hash table is to keep the original data since capping a Visual 
	 * replaces the data. Every Visual has a parallel Visual in the Scene's 
	 * hash table of models. When a Visual in this hash table is transformed, 
	 * the signal is caught to cap the Visual on its new location/orientation 
	 *  or new data.
	 */
	QHash<QString, ModelPtr> mOriginalModels;

public:
	/*!
	* @brief Constructor for the scene.
	*
	* @param widget The QtVTKOpenGLWidget used to render the visuals.
	* @param id The ID of this scene.
	* @param orientation The initial orientation. Defaults to XY.
	*/
	ImageSliceViewer2D(QVTKOpenGLStereoWidget* widget, const QString& id,
		ESliceOrientation orientation = ESliceOrientation::XY);

	/*! @brief Destructor. */
	~ImageSliceViewer2D();

	/// @brief Disables/enables the mouse interactions on the scene.
	virtual void enableInputInteractions(const bool& isEnable);

	/*! @brief Caps all models. */
	virtual void capModels();

	/*! @brief Caps the model identified by the given name. */
	virtual void capModel(const QString& visualID);

	/*! @brief Caps the given Model by the slice plane. */
	virtual void capModel(ModelPtr model);

private slots:
	/// @brief Same as cap model but used for signals.
	void updateCapModel();

public:
	/// @brief Gets the model's capped data.
	virtual fi3d::ModelDataVPtr getModelCappedData(const QString& modelName);

	/*!
	*	@name Model Modifiers
	*	@brief This scene handles models differently.
	*/
	/// @{
	virtual ModelPtr addModel(const QString& visualID, fi3d::ModelDataVPtr data = fi3d::ModelDataVPtr::New()) override;
	virtual bool addVisual(VisualPtr visual) override;
	virtual VisualPtr getVisual(const QString& visualID) override;
	virtual Visual3DPtr getVisual3D(const QString& visualID) override;
	virtual ModelPtr getModel(const QString& visualID) override;
	virtual QList<Visual3DPtr> getVisuals3D() override;
	virtual QList<ModelPtr> getModels() override;
	virtual void removeVisual(const QString& visualID) override;
	virtual void clearScene() override;
	virtual void setVisualVisible(const bool& isVisible, const QString& visualID) override;
	virtual void setVisualPickable(const bool& isPickable, const QString& visualID) override;
	virtual void setVisualOpacity(const double& a, const QString& visualID) override;
	virtual void resetVisualTransform(const QString& visualID) override;
	virtual void translateVisual(const double& x, const double& y, const double& z, const QString& visualID) override;
	virtual void rotateVisual(const double& x, const double& y, const double& z, const QString& visualID) override;
	virtual void rotateXVisual(const double& x, const QString& visualID) override;
	virtual void rotateYVisual(const double& y, const QString& visualID) override;
	virtual void rotateZVisual(const double& z, const QString& visualID) override;
	virtual void setModelData(fi3d::ModelDataVPtr data, const QString& visualID) override;
	virtual void setModelColor(const double& r, const double& g, const double& b, const QString& visualID) override;
	/// @}

private slots:
	/*!
	 * @brief Event handler for when the mouse moves.
	 *
	 * When the mouse moves over the slice on the Scene, It updates the
	 * coordinate information.
	 */
	void onMouseMove();

public slots:
	/// @brief Sets the ImageData to display.
	void setImageData(fi3d::ImageDataVPtr data);

	/// @brief Sets the message to display.
	void setMessageText(const QString& messageText);

	/// @brief Sets the visibility of all subtitles (slice, orientation, 
	/// coordinate, and message).
	void setLabelsVisible(const bool& areVisible);

public:
	/// @brief Get the ImageData being displayed in this scene. 
	fi3d::ImageDataVPtr getImageData();

public slots:
	/// @brief Set the orientation to Transverse (XY).
	void setOrientationXY();

	/// @brief Set the orientation to Sagittal (YZ). 
	void setOrientationYZ();

	/// @brief Set the orientation to Coronal (XZ).
	void setOrientationXZ();

	/// @brief Set the orientation of this slice viewer.
	void setOrientation(ESliceOrientation orientation);

	/*!
	 * @brief Changes to the next orientation.
	 *
	 * Cycles through the orientations in the order: XY, YZ, XZ, XY.
	 */
	void setNextOrientation();

	/*!
	 * @brief Changes to the previous orientation.
	 *
	 * Cycles through the orientations in the order: XY, YZ, XZ, XY.
	 */
	void setPreviousOrientation();

private:
	/// @brief Used internally to apply an orientation change.
	void applyOrientationChange();

public:
	/// @brief Gets the current orientation.
	ESliceOrientation getSliceOrientation() const;

public slots:
	/// @brief Sets the slice of the image data.
	virtual void setSlice(const int& sliceIndex, const fi3d::ESliceOrientation& orientation);

	/// @brief Changes the slice in the current orientation.
	void setSliceIndex(const int& sliceIndex);

	/// @brief Sets the 0th index slice of the current orientation.
	virtual void setSliceIndexToBeginning();

	/// @brief Sets the middle index slice of the current orientation.
	virtual void setSliceIndexToMiddle();

	/// @brief Sets the last index slice of the current orientation.
	virtual void setSliceIndexToEnd();

	/// @brief Sets to the next slice, stopping at end.
	virtual void setSliceIndexToNext();

	/// @brief Sets to the next slice, stopping at beginning.
	virtual void setSliceIndexToPrevious();

public:
	/// @brief Gets the index of the current slice.
	int getSliceIndex() const;

	/// @brief Gets the index of the minimum slice.
	int getSliceMinIndex() const;

	/// @brief Gets the index of the maximum slice.
	int getSliceMaxIndex() const;

	/// @brief Gets the index of the minimum slice.
	void getSliceRange(int& min, int& max) const;

	/// @brief Check if given index is in range of the current orientation.
	bool isSliceIndexInRange(const int& sliceIndex);

	/// @brief Based on current slice and orientation, create a plane out of it.
	virtual vtkSmartPointer<vtkPlane> getCutPlane() const;

	/*!
	* @brief Given a voxel's position in an image (i, j, k), determine
	* its global coordinate (x, y, z).
	*
	* The resulting coordinates are calculated using the slice separation 
	* values and the matrix that defines the Visual's position and orientation 
	* in 3D space. If the given indices are out of range, the values of 
	* x, y, and z are unchanged and false is returned. True is returned when 
	* the indices represent a voxel in the image.
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
	* voxel (i, j, k) such coordinate is in.
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

	/*!
	* @brief Given a frame point (x, y) determine the image data's
	* slice indices the point is pointing to.
	*
	* @param x The x frame coordinate.
	* @param y The y frame coordinate.
	* @param i Variable to store ith slice index.
	* @param j Variable to store jth slice index.
	* @param k Variable to store kth slice index.
	* @return True if the given frame point is pointing to the slice.
	*/
	virtual bool getVoxelFromFrameCoordinate(const int& x, const int& y, int& i, int& j, int& k);

	/*!
	* @brief Given a frame coordinate (x, y) determine the image data's
	* global coordinate the point is pointing to.
	*
	* @param wx The x frame coordinate.
	* @param wy The y frame coordinate.
	* @param x Variable to store the x value.
	* @param y Variable to store the y value.
	* @param z Variable to store the z value.
	* @return True if the given frame point is pointing to the slice.
	*/
	virtual bool getCoordinatesFromFrameCoordinate(const int& wx, const int& wy, double& x, double& y, double& z);

private:
	/// @brief Calculates the slice indices when a new study is set.
	void calculateSliceIndices();
};

/// @brief Alias for a smart pointer of this class.
using ImageSliceViewer2DPtr = QSharedPointer<ImageSliceViewer2D>;

}