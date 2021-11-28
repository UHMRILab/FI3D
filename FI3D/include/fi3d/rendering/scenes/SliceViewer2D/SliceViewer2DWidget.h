#pragma once
/*!
* @author	VelazcoJD
* @file		SliceViewer2DWidget.h
* @class	fi3d::SliceViewer2DWidget
* @brief	A widget with a 2D scene used to interact with imaging data.
*
* This viewer has two modes: ImageData and Study modes. The first mode only 
* visualizes ImageData, one piece of 3D imaging data. It gives the user the 
* ability to change the slice and orientation of the viewed image. The second 
* mode lets this viewer visualize a study, which has the same functions as the
* first mode but also has a slider to select the series of the study.
*
* The mode that is active in this viewer depends on the type of data that was 
* set. For instance, if one constructs this viewer with ImageData, the viewer 
* will be set to that mode. It has the same behavior for Study data. One can 
* change the mode of the viewer by simply setting data of the wanted mode. For 
* instance, if this viewer is in Study mode, simply set ImageData by the 
* setImageData function, and it will automatically change to ImageData mode.
*
* Lastly, one can set the visualized data by setting a ImageSlice or StudySlice 
* Visual to it, which enables ImageData or Study mode respectively. The data 
* visualized in this viewer is obtained from the given Visual. Additionally,
* this viewer connects itself to the Visual so that changes in the ImageSlice 
* (or StudySlice) are also applied to this viewer, and vice versa. In other 
* words, this viewer and the given Visual will remain in sync. 
*/

#include <QWidget>

#include "ui_SliceViewer2DWidget.h"

#include <fi3d/rendering/scenes/SliceViewer2D/StudySliceViewer2D.h>
#include <fi3d/rendering/visuals/3D/slices/ESliceOrientation.h>

#include <QPoint>

class QCloseEvent;
class QMenu;

namespace fi3d {
class SliceViewer2DWidget : public QWidget {

	Q_OBJECT

signals:
	/// @brief Emitted when there was a right click on the scene.
	void changedRightClickPosition(const QPoint& pos);

	/// @brief Emitted when there was a right click on the scene.
	void changedRightClickGlobalPosition(const QPoint& pos);

	/// @brief Emitted when there was a right click on the scene.
	void changedRightClickVTKPosition(const double& x, const double& y);

	/// @brief Emitted when the window is about to close.
	void changedWindowClosing();

private:
	/// @brief UI elements.
	Ui::SliceViewer2DWidget ui;

	/// @brief The right click menu to display on a right click in the scene.
	QSharedPointer<QMenu> mRightClickMenu;

	/// @brief Instance of the last right click position.
	QPoint mLastRightClickPosition;

	/// @brief Whether this viewer is in ImageData or Study mode.
	bool mIsStudyMode;

	/// @brief The scene where the slice is visualized in ImageData mode.
	ImageSliceViewer2DPtr mImageDataScene2D;

	/// @brief The scene where slice is visualized.
	StudySliceViewer2DPtr mStudyScene2D;

public:
	/*!
	 * @brief Constructs a widget without any data, defaults to Study mode.
	 *
	 * @param name The name of this window, it also assigns title.
	 */
	SliceViewer2DWidget(QString name);

	/*!
	 * @brief Constructs a widget and visualizes the given imageData.
	 *
	 * @param imageData The ImageData to visualize.
	 *	Defaults to transverse (XY), and middle slice.
	 */
	SliceViewer2DWidget(fi3d::ImageDataVPtr imageData);

	/*!
	 * @brief Constructs a SliceViewer2DWidget and visualizes the given study.
	 *
	 * @param study The Study to visualize. 
	 *	Defaults to middle series, transverse (XY), and middle slice.
	 */
	SliceViewer2DWidget(fi3d::StudyPtr study);

	/*!
	 * @brief Constructs a widget and mimics the given ImageSlice.
	 *
	 * If the given ImageSlice is a StudySlice, the mode is set to Study. 
	 * Otherwise, it is set to ImageData.
	 *
	 * @param imageSlice The ImageSlice object to mimic.
	 */
	SliceViewer2DWidget(ImageSlicePtr imageSlice);

	/// @brief Destructor. */
	~SliceViewer2DWidget();

private:
	/// @brief Called internally when a new SliceViewer2DWidget is created.
	void init();

protected:
	/// @brief Function is called before the window is closed.
	virtual void closeEvent(QCloseEvent* event) override;

private slots:
	/// @brief Sets the slice to the first slice.
	void setSliceToFirst();

	/// @brief Sets the slice to the previous slice
	void setSliceToPrevious();

	/// @brief Sets the slice to the next slice
	void setSliceToNext();

	/// @brief Sets the slice to the last slice
	void setSliceToLast();

	/// @brief Sets the orientation to XY (transverse).
	void setOrientationXY();
	
	/// @brief Sets the orientation to YZ (sagittal). 
	void setOrientationYZ();

	/// @brief Sets the orientation to XZ (coronal).
	void setOrientationXZ();

private:
	/// @brief Changes the orientation in the scene.
	void setOrientation(const fi3d::ESliceOrientation& orientation);

private slots:
	/// @brief Changes the series to the first series
	void setSeriesToFirst();

	/// @brief Changes the series to the previous series
	void setSeriesToPrevious();

	/// @brief Changes the series to the next series
	void setSeriesToNext();

	/// @brief Changes the series to the last series
	void setSeriesToLast();

	/*!
	* @name UI Updatders
	* @brief Updates the GUI elements. 
	* 
	* Used specially for syncing with ImageSlice and StudySlice
	*/
	/// @{
	void updateSeriesIndices(const int& min, const int& max);
	void updateSeriesIndex(const int& index);
	void updateOrientation(const fi3d::ESliceOrientation& orientation);
	void updateSliceIndices(const int& min, const int& max);
	void updateSliceIndex(const int& index);
	/// @}

	/*! 
	 * @brief Used to update the window title. 
	 *
	 * This occurs when the study data being visualized changes. 
	 */
	void updateViewerTitle();

public:
	/// @brief Sets the visibility of the orientation controls.
	void setOrientationControlsVisible(const bool& isVisible);

	/// @brief Get the scene for ImageData mode. Null if in Study mode.
	ImageSliceViewer2D* getImageDataScene();

	/// @brief Get the scene for Study mode. Null if in ImageData mode.
	StudySliceViewer2D* getStudyScene();

	/*! 
	 * @brief Sets the ImageData to visualize. 
	 * 
	 * Changes to ImageData mode if in Study mode. 
	 */
	void setImageData(fi3d::ImageDataVPtr imageData);

	/*!
	 * @brief Sets the Study to visualize. 
	 *
	 * Changes to Study mode if in ImageData mode. 
	 */
	void setStudy(fi3d::StudyPtr study);

	/// @brief Sets the menu to display when a right click is made.
	void setRightClickMenu(QSharedPointer<QMenu> menu);

	/// @brief Whether this viewer is in StudyMode mode or ImageData mode.
	bool isStudyMode();

	/*! 
	 * @brief Get the last right click position.
	 *
	 * These coordinates are relative to the frame widget. 
	 */
	QPoint getLastRightClickPosition();

	/*! 
	 * @brief Get the last right click position in global coordinates.
	 * 
	 * These coordinates the position in the whole application.
	 */
	QPoint getLastRightClickPositionInGlobalCoordinates();

	/*! 
	 * @brief Get the last right click position in vtk frame coordinates. 
	 * 
	 * These coordinates represent the position using vtk's coordinate system. 
	 */
	void getLastRightClickVTKFramePosition(double& x, double& y);

private slots:
	/// @brief Catches the right click event and emit the right click signals.
	void rightClick(const QPoint& pos);
};

/// @brief Alias for a smart pointer of this class.
using SliceViewer2DWidgetPtr = QSharedPointer<SliceViewer2DWidget>;

}