#include <fi3d/rendering/scenes/SliceViewer2D/SliceViewer2DWidget.h>

#include <fi3d/FI3D/FI3D.h>
#include <fi3d/logger/Logger.h>
#include <fi3d/utilities/Icons.h>

#include <QCloseEvent>
#include <QMenu>

using namespace fi3d;

SliceViewer2DWidget::SliceViewer2DWidget(QString name)
	: QWidget(),
	mRightClickMenu(Q_NULLPTR),
	mLastRightClickPosition(),
	mIsStudyMode(true),
	mImageDataScene2D(Q_NULLPTR),
	mStudyScene2D(Q_NULLPTR) 
{
	this->setObjectName(name);
	this->init();
	this->updateViewerTitle();
}

SliceViewer2DWidget::SliceViewer2DWidget(ImageDataVPtr imageData) 
	: QWidget(),
	mRightClickMenu(Q_NULLPTR),
	mLastRightClickPosition(),
	mIsStudyMode(false),
	mImageDataScene2D(Q_NULLPTR),
	mStudyScene2D(Q_NULLPTR)
{
	if (imageData.Get() == Q_NULLPTR) {
		this->setObjectName("BAD DATA");
	} else {
		this->setObjectName(imageData->getDataName());
	}

	this->init();
	mImageDataScene2D->setImageData(imageData);
	this->updateViewerTitle();
}

SliceViewer2DWidget::SliceViewer2DWidget(StudyPtr study) {
	if (study.isNull()) {
		this->setObjectName("BAD DATA");
	} else {
		this->setObjectName(study->getStudyID());
	}

	this->init();
	mStudyScene2D->setStudy(study);
	this->updateViewerTitle();
}

SliceViewer2DWidget::SliceViewer2DWidget(ImageSlicePtr imageSlice) 
	: QWidget(),
	mRightClickMenu(Q_NULLPTR),
	mLastRightClickPosition(),
	mIsStudyMode(true),
	mImageDataScene2D(Q_NULLPTR),
	mStudyScene2D(Q_NULLPTR) 
{
	if (imageSlice.isNull()) {
		this->setObjectName("BAD DATA");
	} else {
		this->setObjectName(imageSlice->getVisualID());
	}

	if (!imageSlice.isNull() && imageSlice->getVisualType() == EVisual::IMAGE_SLICE) {
		mIsStudyMode = false;
	}

	this->init();
	if (imageSlice.isNull()) {
		return;
	}

	if (imageSlice->getVisualType() == EVisual::IMAGE_SLICE) {
		mImageDataScene2D->setImageData(imageSlice->getImageData());
		mImageDataScene2D->setSlice(imageSlice->getSliceIndex(), imageSlice->getSliceOrientation());

		// Connect the signals that keep the ImageSlice with the 
		// ImageSliceViewer2D in sync
		QObject::connect(
			mImageDataScene2D.data(), &ImageSliceViewer2D::changedImageData,
			imageSlice.data(), &ImageSlice::setImageData,
			Qt::UniqueConnection);
		QObject::connect(
			mImageDataScene2D.data(), &ImageSliceViewer2D::changedOrientation, 
			imageSlice.data(), &ImageSlice::setOrientation,
			Qt::UniqueConnection);
		QObject::connect(
			mImageDataScene2D.data(), &ImageSliceViewer2D::changedSlice, 
			imageSlice.data(), &ImageSlice::setSlice,
			Qt::UniqueConnection);

		QObject::connect(
			imageSlice.data(), &ImageSlice::changedImageData,
			mImageDataScene2D.data(), &ImageSliceViewer2D::setImageData,
			Qt::UniqueConnection);
		QObject::connect(
			imageSlice.data(), &ImageSlice::changedOrientation,
			mImageDataScene2D.data(), &ImageSliceViewer2D::setOrientation, 
			Qt::UniqueConnection);
		QObject::connect(
			imageSlice.data(), &ImageSlice::changedSlice,
			mImageDataScene2D.data(), &ImageSliceViewer2D::setSlice,
			Qt::UniqueConnection);

	} else if (imageSlice->getVisualType().isStudySlice()) {
		StudySlicePtr studySlice = qobject_cast<StudySlicePtr>(imageSlice);
		mStudyScene2D->setStudy(studySlice->getStudy());
		mStudyScene2D->setSeriesIndex(studySlice->getSeriesIndex());
		mStudyScene2D->setSlice(studySlice->getSliceIndex(), studySlice->getSliceOrientation());

		// Connect the signals that keep the StudySlice with the 
		// StudySliceViewer2D in sync
		QObject::connect(
			mStudyScene2D.data(), &StudySliceViewer2D::changedStudy, 
			studySlice.data(), &StudySlice::setStudy, 
			Qt::UniqueConnection);
		QObject::connect(
			mStudyScene2D.data(), &StudySliceViewer2D::changedSeriesIndex, 
			studySlice.data(), &StudySlice::setSeriesIndex, 
			Qt::UniqueConnection);
		QObject::connect(
			mStudyScene2D.data(), &StudySliceViewer2D::changedOrientation,
			studySlice.data(), &StudySlice::setOrientation, 
			Qt::UniqueConnection);
		QObject::connect(
			mStudyScene2D.data(), &StudySliceViewer2D::changedSlice,
			studySlice.data(), &StudySlice::setSlice,
			Qt::UniqueConnection);

		QObject::connect(
			studySlice.data(), &StudySlice::changedStudy, 
			mStudyScene2D.data(), &StudySliceViewer2D::setStudy, 
			Qt::UniqueConnection);
		QObject::connect(
			studySlice.data(), &StudySlice::changedSeriesIndex, 
			mStudyScene2D.data(), &StudySliceViewer2D::setSeriesIndex,
			Qt::UniqueConnection);
		QObject::connect(
			studySlice.data(), &StudySlice::changedOrientation, 
			mStudyScene2D.data(), &StudySliceViewer2D::setOrientation, 
			Qt::UniqueConnection);
		QObject::connect(
			studySlice.data(), &StudySlice::changedSlice,
			mStudyScene2D.data(), &StudySliceViewer2D::setSlice,
			Qt::UniqueConnection);
	}

	this->updateViewerTitle();
}

SliceViewer2DWidget::~SliceViewer2DWidget() {}

void SliceViewer2DWidget::init() {
	ui.setupUi(this);
	
	// Set the icons
	QIcon first = Icons::getIcon(EIcon::FIRST);
	ui.firstSlice_button->setIcon(first);

	QIcon last = Icons::getIcon(EIcon::LAST);
	ui.lastSlice_button->setIcon(last);

	QIcon previous = Icons::getIcon(EIcon::PREVIOUS);
	ui.previousSlice_button->setIcon(previous);

	QIcon next = Icons::getIcon(EIcon::NEXT);
	ui.nextSlice_button->setIcon(next);

	QIcon transverse = Icons::getIcon(EIcon::TRANSVERSE);
	ui.transverse_button->setIcon(transverse);

	QIcon sagittal = Icons::getIcon(EIcon::SAGITTAL);
	ui.sagittal_button->setIcon(sagittal);

	QIcon coronal = Icons::getIcon(EIcon::CORONAL);
	ui.coronal_button->setIcon(coronal);

	QIcon beginning = Icons::getIcon(EIcon::BEGINNING);
	ui.beginningSeries_button->setIcon(beginning);

	QIcon end = Icons::getIcon(EIcon::ENDING);
	ui.endSeries_button->setIcon(end);

	QIcon backward = Icons::getIcon(EIcon::BACKWARD);
	ui.backwardSeries_button->setIcon(backward);

	QIcon forward = Icons::getIcon(EIcon::FORWARD);
	ui.forwardSeries_button->setIcon(forward);

	// Create the corresponding scene and connect signals that handle GUI events
	if (mIsStudyMode) {
		mStudyScene2D.reset(new StudySliceViewer2D(ui.Frame2D, this->objectName(), ESliceOrientation::XY));
		
		QObject::connect(
			ui.slideSelector_slider, &QSlider::valueChanged,
			mStudyScene2D.data(), &StudySliceViewer2D::setSliceIndex,
			Qt::UniqueConnection);
		QObject::connect(
			ui.dataSelector_slider, &QSlider::valueChanged, 
			mStudyScene2D.data(), &StudySliceViewer2D::setSeriesIndex, 
			Qt::UniqueConnection);
		QObject::connect(
			ui.beginningSeries_button, &QToolButton::clicked,
			this, &SliceViewer2DWidget::setSeriesToFirst,
			Qt::UniqueConnection);
		QObject::connect(
			ui.backwardSeries_button, &QToolButton::clicked,
			this, &SliceViewer2DWidget::setSeriesToPrevious,
			Qt::UniqueConnection);
		QObject::connect(
			ui.forwardSeries_button, &QToolButton::clicked,
			this, &SliceViewer2DWidget::setSeriesToNext,
			Qt::UniqueConnection);
		QObject::connect(
			ui.endSeries_button, &QToolButton::clicked,
			this, &SliceViewer2DWidget::setSeriesToLast,
			Qt::UniqueConnection);
	} else {
		mImageDataScene2D.reset(new ImageSliceViewer2D(ui.Frame2D, this->objectName(), ESliceOrientation::XY));
		QObject::connect(
			ui.slideSelector_slider, &QSlider::valueChanged,
			mImageDataScene2D.data(), &ImageSliceViewer2D::setSliceIndex,
			Qt::UniqueConnection);
	}

	// Connect signals that handle GUI events, similar to any type of scene.
	QObject::connect(
		ui.Frame2D, &QVTKOpenGLWidget::customContextMenuRequested, 
		this, &SliceViewer2DWidget::rightClick, 
		Qt::UniqueConnection);
	QObject::connect(
		ui.firstSlice_button, &QToolButton::clicked,
		this, &SliceViewer2DWidget::setSliceToFirst,
		Qt::UniqueConnection);
	QObject::connect(
		ui.previousSlice_button, &QToolButton::clicked,
		this, &SliceViewer2DWidget::setSliceToPrevious,
		Qt::UniqueConnection);
	QObject::connect(
		ui.nextSlice_button, &QToolButton::clicked,
		this, &SliceViewer2DWidget::setSliceToNext,
		Qt::UniqueConnection);
	QObject::connect(
		ui.lastSlice_button, &QToolButton::clicked,
		this, &SliceViewer2DWidget::setSliceToLast,
		Qt::UniqueConnection);
	QObject::connect(
		ui.transverse_button, &QToolButton::clicked,
		this, &SliceViewer2DWidget::setOrientationXY,
		Qt::UniqueConnection);
	QObject::connect(
		ui.sagittal_button, &QToolButton::clicked,
		this, &SliceViewer2DWidget::setOrientationYZ, 
		Qt::UniqueConnection);
	QObject::connect(
		ui.coronal_button, &QToolButton::clicked,
		this, &SliceViewer2DWidget::setOrientationXZ, 
		Qt::UniqueConnection);
	
	// Connect signals that should update GUI if there are internal changes
	// to the scene.
	if (mIsStudyMode) {
		QObject::connect(
			mStudyScene2D.data(), &StudySliceViewer2D::changedStudy, 
			this, &SliceViewer2DWidget::updateViewerTitle,
			Qt::UniqueConnection);
		QObject::connect(
			mStudyScene2D.data(), &StudySliceViewer2D::changedSeriesIndices, 
			this, &SliceViewer2DWidget::updateSeriesIndices,
			Qt::UniqueConnection);
		QObject::connect(
			mStudyScene2D.data(), &StudySliceViewer2D::changedSeriesIndex, 
			this, &SliceViewer2DWidget::updateSeriesIndex,
			Qt::UniqueConnection);
		QObject::connect(
			mStudyScene2D.data(), &StudySliceViewer2D::changedOrientation,
			this, &SliceViewer2DWidget::updateOrientation, 
			Qt::UniqueConnection);
		QObject::connect(
			mStudyScene2D.data(), &StudySliceViewer2D::changedSliceIndices, 
			this, &SliceViewer2DWidget::updateSliceIndices, 
			Qt::UniqueConnection);
		QObject::connect(
			mStudyScene2D.data(), &StudySliceViewer2D::changedSlice, 
			this, &SliceViewer2DWidget::updateSliceIndex, 
			Qt::UniqueConnection);
	} else {
		QObject::connect(
			mImageDataScene2D.data(), &ImageSliceViewer2D::changedImageData,
			this, &SliceViewer2DWidget::updateViewerTitle, 
			Qt::UniqueConnection);
		QObject::connect(
			mImageDataScene2D.data(), &ImageSliceViewer2D::changedOrientation,
			this, &SliceViewer2DWidget::updateOrientation, 
			Qt::UniqueConnection);
		QObject::connect(
			mImageDataScene2D.data(), &ImageSliceViewer2D::changedSliceIndices,
			this, &SliceViewer2DWidget::updateSliceIndices, 
			Qt::UniqueConnection);
		QObject::connect(
			mImageDataScene2D.data(), &ImageSliceViewer2D::changedSlice, 
			this, &SliceViewer2DWidget::updateSliceIndex,
			Qt::UniqueConnection);
	}

	if (!mIsStudyMode) {
		ui.dataSelect_widget->setVisible(false);
	}
}

void SliceViewer2DWidget::closeEvent(QCloseEvent* event) {
	emit changedWindowClosing();
	event->accept();
}

void SliceViewer2DWidget::setSliceToFirst() {
	if (mIsStudyMode) {
		mStudyScene2D->setSliceIndex(mStudyScene2D->getSliceMinIndex());
	} else {
		mImageDataScene2D->setSliceIndex(mImageDataScene2D->getSliceMinIndex());
	}
}

void fi3d::SliceViewer2DWidget::setSliceToPrevious() {
	if (mIsStudyMode) {
		int sliceIndex = mStudyScene2D->getSliceIndex() - 1;
		mStudyScene2D->setSliceIndex(sliceIndex);
	} else {
		int sliceIndex = mImageDataScene2D->getSliceIndex() - 1;
		mImageDataScene2D->setSliceIndex(sliceIndex);
	}
}

void fi3d::SliceViewer2DWidget::setSliceToNext() {
	if (mIsStudyMode) {
		int sliceIndex = mStudyScene2D->getSliceIndex() + 1;
		mStudyScene2D->setSliceIndex(sliceIndex);
	} else {
		int sliceIndex = mImageDataScene2D->getSliceIndex() + 1;
		mImageDataScene2D->setSliceIndex(sliceIndex);
	}
}

void fi3d::SliceViewer2DWidget::setSliceToLast() {
	if (mIsStudyMode) {
		mStudyScene2D->setSliceIndex(mStudyScene2D->getSliceMaxIndex());
	} else {
		mImageDataScene2D->setSliceIndex(mImageDataScene2D->getSliceMaxIndex());
	}
}

void SliceViewer2DWidget::setOrientationXY() {
	this->setOrientation(ESliceOrientation::XY);
}

void SliceViewer2DWidget::setOrientationYZ() {
	this->setOrientation(ESliceOrientation::YZ);
}

void SliceViewer2DWidget::setOrientationXZ() {
	this->setOrientation(ESliceOrientation::XZ);
}

void SliceViewer2DWidget::setOrientation(const ESliceOrientation& orientation) {
	if (mIsStudyMode) {
		mStudyScene2D->setOrientation(orientation);
	} else {
		mImageDataScene2D->setOrientation(orientation);
	}
}

void SliceViewer2DWidget::setSeriesToFirst() {
	if (mIsStudyMode) {
		mStudyScene2D->setSeriesIndex(mStudyScene2D->getSeriesMinIndex());
	}
}

void SliceViewer2DWidget::setSeriesToPrevious() {
	if (mIsStudyMode) {
		int sliceIndex = mStudyScene2D->getSeriesIndex() - 1;
		mStudyScene2D->setSeriesIndex(sliceIndex);
	}
}

void SliceViewer2DWidget::setSeriesToNext() {
	if (mIsStudyMode) {
		int sliceIndex = mStudyScene2D->getSeriesIndex() + 1;
		mStudyScene2D->setSeriesIndex(sliceIndex);
	}
}

void SliceViewer2DWidget::setSeriesToLast() {
	if (mIsStudyMode) {
		mStudyScene2D->setSeriesIndex(mStudyScene2D->getSeriesMaxIndex());
	}
}

void SliceViewer2DWidget::updateSeriesIndices(const int& min, const int& max) {
	ui.dataSelector_slider->blockSignals(true);
	ui.dataSelector_slider->setMinimum(min);
	ui.dataSelector_slider->setMaximum(max);
	ui.dataSelector_slider->blockSignals(false);

	if (min == max) {
		ui.dataSelect_widget->hide();
	} else {
		ui.dataSelect_widget->show();
	}
}

void SliceViewer2DWidget::updateSeriesIndex(const int& index) {
	ui.dataSelector_slider->blockSignals(true);
	ui.dataSelector_slider->setValue(index);
	ui.dataSelector_slider->blockSignals(false);
}

void SliceViewer2DWidget::updateOrientation(const ESliceOrientation& orientation) {
	// TODO: Make the orientation buttons checkable and check the appropriate one.

	/*switch (orientation.toInt()) {
		case ESliceOrientation::XY:
			ui.transverse_rdbtn->blockSignals(true);
			ui.transverse_rdbtn->setChecked(true);
			ui.transverse_rdbtn->blockSignals(false);
			break;
		case ESliceOrientation::YZ:
			ui.sagittal_rdbtn->blockSignals(true);
			ui.sagittal_rdbtn->setChecked(true);
			ui.sagittal_rdbtn->blockSignals(false);
			break;
		case ESliceOrientation::XZ:
			ui.coronal_rdbtn->blockSignals(true);
			ui.coronal_rdbtn->setChecked(true);
			ui.coronal_rdbtn->blockSignals(false);
			break;
	}*/
}

void SliceViewer2DWidget::updateSliceIndices(const int& min, const int& max) {
	ui.slideSelector_slider->blockSignals(true);
	ui.slideSelector_slider->setMinimum(min);
	ui.slideSelector_slider->setMaximum(max);
	ui.slideSelector_slider->blockSignals(false);
}

void SliceViewer2DWidget::updateSliceIndex(const int& index) {
	ui.slideSelector_slider->blockSignals(true);
	ui.slideSelector_slider->setValue(index);
	ui.slideSelector_slider->blockSignals(false);
}

void SliceViewer2DWidget::updateViewerTitle() {
	QString windowTitle = tr("%1 - ").arg(this->objectName());

	if (mIsStudyMode) {
		if (mStudyScene2D->getStudy().isNull()) {
			windowTitle = tr("%1%2").arg(windowTitle).arg("NO STUDY DATA SET");
		} else {
			windowTitle = tr("%1%2")
				.arg(windowTitle)
				.arg(mStudyScene2D->getStudy()->objectName());
		}
	} else {
		windowTitle = tr("%1%2")
			.arg(windowTitle)
			.arg(mImageDataScene2D->getImageData()->objectName());
	}
	
	this->setWindowTitle(windowTitle);
}

void SliceViewer2DWidget::setOrientationControlsVisible(const bool& isVisible) {
	ui.transverse_button->setVisible(isVisible);
	ui.sagittal_button->setVisible(isVisible);
	ui.coronal_button->setVisible(isVisible);
}

ImageSliceViewer2D* SliceViewer2DWidget::getImageDataScene() {
	return mImageDataScene2D.data();
}

StudySliceViewer2D* SliceViewer2DWidget::getStudyScene() {
	return mStudyScene2D.data();
}

void SliceViewer2DWidget::setImageData(ImageDataVPtr imageData) {
	// TODO:
}

void SliceViewer2DWidget::setStudy(StudyPtr study) {
	// TODO:
}

void SliceViewer2DWidget::setRightClickMenu(QSharedPointer<QMenu> menu) {
	if (mRightClickMenu == menu) {
		return;
	}
	mRightClickMenu = menu;
}

bool SliceViewer2DWidget::isStudyMode() {
	return mIsStudyMode;
}

QPoint SliceViewer2DWidget::getLastRightClickPosition() {
	return mLastRightClickPosition;
}

QPoint SliceViewer2DWidget::getLastRightClickPositionInGlobalCoordinates() {
	return ui.Frame2D->mapToGlobal(mLastRightClickPosition);
}

void SliceViewer2DWidget::getLastRightClickVTKFramePosition(double& x, 
	double& y) 
{
	x = mLastRightClickPosition.x();
	y = ui.Frame2D->height() - mLastRightClickPosition.y();
}

void SliceViewer2DWidget::rightClick(const QPoint& pos) {
	
	mLastRightClickPosition = pos;
	QPoint globalPoint = ui.Frame2D->mapToGlobal(pos);
	double x, y;
	this->getLastRightClickVTKFramePosition(x, y);

	if (!mRightClickMenu.isNull()) {
		mRightClickMenu->exec(globalPoint);
	}

	emit changedRightClickPosition(mLastRightClickPosition);
	emit changedRightClickGlobalPosition(globalPoint);
	emit changedRightClickVTKPosition(x, y);
}
