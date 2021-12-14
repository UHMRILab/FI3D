#include <fi3d/rendering/visuals/3D/slices/ImageSlice.h>

#include <fi3d/logger/Logger.h>

#include <vtkImageSliceMapper.h>
#include <vtkPlane.h>
#include <vtkOutlineFilter.h>

using namespace fi3d;

ImageSlice::ImageSlice(const QString& name, ESliceOrientation orientation,
	ImageDataVPtr data)
	: Visual3D(name),
	mDataFrame(new Model(tr("%1_Frame").arg(name))),
	mImageData(data),
	mOrientation(orientation),
	mOrientationSliceIndices(),
	mMapper(vtkSmartPointer<vtkImageSliceMapper>::New()),
	mActor(vtkSmartPointer<vtkImageActor>::New())
{
	if (data.Get() == Q_NULLPTR) {
		mImageData = ImageDataVPtr::New();
	}

	mMapper->SetInputData(mImageData);
	mActor->SetMapper(mMapper);
	mActor->SetUserTransform(this->getTransform());
	
	// Ensures that opaque models are not blocked by the iamge
	mActor->ForceOpaqueOn();
	
	// TODO: Opaque images completely block images position behind it. 
	// Need to fix this.

	this->calculateSliceIndices();
	this->applyOrientationChange();

	// Create the 3D image data frame
	mDataFrame->setTransformData(this->getTransform());
	vtkNew<vtkOutlineFilter> frameFilter;
	frameFilter->SetInputData(mImageData);
	frameFilter->SetOutput(mDataFrame->getModelData());
	frameFilter->Update();

	// Re-emit signal when visualized data changes internally
	QObject::connect(
		mImageData.Get(), &ImageData::changedData,
		this, &ImageSlice::onDataUpdated,
		Qt::UniqueConnection);

	// Update the data frame position when the slice position changes
	QObject::connect(
		this, &ImageSlice::changedTransformation,
		this, &ImageSlice::onTransformChange);
}


ImageSlice::~ImageSlice() {}

void ImageSlice::setImageData(ImageDataVPtr data) {
	if (mImageData == data) {
		return;
	}

	// Ignore the changedData signals from previous assigned data
	QObject::disconnect(
		mImageData.Get(), &ImageData::changedData,
		this, &ImageSlice::onDataUpdated);
	
	if (data.Get() == Q_NULLPTR) {
		mImageData = ImageDataVPtr::New();
	} else {
		mImageData = data;
	}

	mMapper->SetInputData(mImageData);
	this->calculateSliceIndices();

	// Re-emit signal when visualized data changes internally
	QObject::connect(
		mImageData.Get(), &ImageData::changedData,
		this, &ImageSlice::onDataUpdated,
		Qt::UniqueConnection);

	switch (mOrientation.toInt()) {
		case ESliceOrientation::XY:
			mMapper->SetSliceNumber(mOrientationSliceIndices.transverseIndex);
			break;
		case ESliceOrientation::YZ:
			mMapper->SetSliceNumber(mOrientationSliceIndices.sagittalIndex);
			break;
		case ESliceOrientation::XZ:
			mMapper->SetSliceNumber(mOrientationSliceIndices.coronalIndex);
			break;
	}

	// Update the data frame.
	vtkNew<vtkOutlineFilter> frameFilter;
	frameFilter->SetInputData(mImageData);
	frameFilter->SetOutput(mDataFrame->getModelData());
	frameFilter->Update();

	emit changedImageData(mImageData);
	emit changedSliceIndices(this->getSliceMinIndex(), this->getSliceMaxIndex());
	emit changedSlice(this->getSliceIndex(), mOrientation);
}

ImageDataVPtr ImageSlice::getImageData() {
	return mImageData;
}

void ImageSlice::setOrientationXY() {
	if (mOrientation == ESliceOrientation::XY) {
		return;
	}
	mOrientation = ESliceOrientation::XY;
	this->applyOrientationChange();
}

void ImageSlice::setOrientationYZ() {
	if (mOrientation == ESliceOrientation::YZ) {
		return;
	}
	mOrientation = ESliceOrientation::YZ;
	this->applyOrientationChange();
}

void ImageSlice::setOrientationXZ() {
	if (mOrientation == ESliceOrientation::XZ) {
		return;
	}
	mOrientation = ESliceOrientation::XZ;
	this->applyOrientationChange();
}

void ImageSlice::setOrientation(const ESliceOrientation& orientation) {
	if (mOrientation == orientation && orientation != ESliceOrientation::UNKNOWN) {
		return;
	}
	mOrientation = orientation;
	this->applyOrientationChange();
}

void ImageSlice::setNextOrientation() {
	switch (this->getSliceOrientation().toInt()) {
		case ESliceOrientation::XY:
			this->setOrientationYZ();
			break;
		case ESliceOrientation::YZ:
			this->setOrientationXZ();
			break;
		case ESliceOrientation::XZ:
			this->setOrientationXY();
			break;
	}
}

void ImageSlice::setPreviousOrientation() {
	switch (this->getSliceOrientation().toInt()) {
		case ESliceOrientation::XY:
			this->setOrientationXZ();
			break;
		case ESliceOrientation::YZ:
			this->setOrientationXY();
			break;
		case ESliceOrientation::XZ:
			this->setOrientationYZ();
			break;
	}
}

void ImageSlice::applyOrientationChange() {
	switch (mOrientation.toInt()) {
		case ESliceOrientation::XY:
			mMapper->SetOrientationToZ();
			mMapper->SetSliceNumber(mOrientationSliceIndices.transverseIndex);
			break;
		case ESliceOrientation::YZ:
			mMapper->SetOrientationToX();
			mMapper->SetSliceNumber(mOrientationSliceIndices.sagittalIndex);
			break;
		case ESliceOrientation::XZ:
			mMapper->SetOrientationToY();
			mMapper->SetSliceNumber(mOrientationSliceIndices.coronalIndex);
			break;
	}
	emit changedOrientation(mOrientation);
	emit changedSliceIndices(this->getSliceMinIndex(), this->getSliceMaxIndex());
	emit changedSlice(this->getSliceIndex(), mOrientation);
}

ESliceOrientation ImageSlice::getSliceOrientation() const {
	return mOrientation;
}

void ImageSlice::setSlice(const int& sliceIndex, const ESliceOrientation& orientation) {
	qDebug() << "Enter - Setting slice" << sliceIndex << "with orientation" << 
		orientation.getName() << "of ImageSlice" << this->getVisualID();

	// if bad orientation, bad slice index, or given values represent current 
	// values, do nothing
	if (orientation == ESliceOrientation::UNKNOWN ||
		!this->isSliceIndexInRange(sliceIndex) ||
		(orientation == mOrientation && sliceIndex == this->getSliceIndex())) 
	{
		qDebug() << "Failed to set slice";
		return;
	}

	bool emitOrientationChange = false;
	if (orientation != mOrientation) {
		emitOrientationChange = true;
		mOrientation = orientation;
	}
	
	switch (mOrientation.toInt()) {
		case ESliceOrientation::XY:
			mMapper->SetOrientationToZ();
			mOrientationSliceIndices.transverseIndex = sliceIndex;
			mMapper->SetSliceNumber(mOrientationSliceIndices.transverseIndex);
			break;
		case ESliceOrientation::YZ:
			mMapper->SetOrientationToX();
			mOrientationSliceIndices.sagittalIndex = sliceIndex;
			mMapper->SetSliceNumber(mOrientationSliceIndices.sagittalIndex);
			break;
		case ESliceOrientation::XZ:
			mMapper->SetOrientationToY();
			mOrientationSliceIndices.coronalIndex = sliceIndex;
			mMapper->SetSliceNumber(mOrientationSliceIndices.coronalIndex);
			break;
	}

	if (emitOrientationChange) {
		emit changedOrientation(mOrientation);
		emit changedSliceIndices(this->getSliceMinIndex(), this->getSliceMaxIndex());
	}

	emit changedSlice(this->getSliceIndex(), mOrientation);
	qDebug() << "Exit";
}

void ImageSlice::setSliceIndex(const int& sliceIndex) {
	if (sliceIndex == this->getSliceIndex()) {
		return;
	}

	mMapper->SetSliceNumber(sliceIndex);

	// Save the new index
	switch (this->getSliceOrientation().toInt()) {
		case ESliceOrientation::XY:
			mOrientationSliceIndices.transverseIndex = sliceIndex;
			break;
		case ESliceOrientation::YZ:
			mOrientationSliceIndices.sagittalIndex = sliceIndex;
			break;
		case ESliceOrientation::XZ:
			mOrientationSliceIndices.coronalIndex = sliceIndex;
			break;
	}

	emit changedSlice(this->getSliceIndex(), mOrientation);
}

void ImageSlice::setSliceIndexToBeginning() {
	this->setSliceIndex(0);
}

void ImageSlice::setSliceIndexToMiddle() {
	this->setSliceIndex((this->getSliceMaxIndex() - this->getSliceMinIndex()) / 2);
}

void ImageSlice::setSliceIndexToEnd() {
	this->setSliceIndex(this->getSliceMaxIndex());
}

void ImageSlice::setSliceIndexToNext() {
	if (this->getSliceIndex() == this->getSliceMaxIndex()) {
		return;
	}
	this->setSliceIndex(this->getSliceIndex() + 1);
}

void ImageSlice::setSliceIndexToPrevious() {
	if (this->getSliceIndex() == this->getSliceMinIndex()) {
		return;
	}
	this->setSliceIndex(this->getSliceIndex() - 1);
}

int ImageSlice::getSliceIndex() const {
	return mMapper->GetSliceNumber();
}

int ImageSlice::getSliceMinIndex() const {
	return mMapper->GetSliceNumberMinValue();
}

int ImageSlice::getSliceMaxIndex() const {
	return mMapper->GetSliceNumberMaxValue();
}

void ImageSlice::getSliceRange(int& min, int& max) const {
	min = this->getSliceMinIndex();
	max = this->getSliceMaxIndex();
}

bool ImageSlice::isSliceIndexInRange(const int& sliceIndex) {
	qDebug() << "Enter";
	int min, max;
	this->getSliceRange(min, max);

	if (sliceIndex < min || sliceIndex > max) {
		qDebug() << "Exit - Slice index" << sliceIndex << "is out of range";
		return false;
	}

	qDebug() << "Exit - Slice index" << sliceIndex << "is in range";
	return true;
}

vtkSmartPointer<vtkPlane> ImageSlice::getSliceAsPlane() const {
	vtkSmartPointer<vtkPlane> plane = vtkSmartPointer<vtkPlane>::New();
	double normal[3] = {0.0, 0.0, 0.0};
	double origin[3] = {0.0, 0.0, 0.0};
	mMapper->GetInput()->GetOrigin(origin);
	double* spacing = mMapper->GetInput()->GetSpacing();
	switch (this->getSliceOrientation().toInt()) {
		case ESliceOrientation::XY:
			origin[2] += this->getSliceIndex() * spacing[2];
			normal[2] = 1.0;
			break;
		case ESliceOrientation::YZ:
			origin[0] += this->getSliceIndex() * spacing[0];
			normal[0] = 1.0;
			break;
		case ESliceOrientation::XZ:
			origin[1] += this->getSliceIndex() * spacing[1];
			normal[1] = 1.0;
			break;
	}

	this->transformVector(normal);
	this->transformPoint(origin);

	plane->SetOrigin(origin);
	plane->SetNormal(normal);

	return plane;
}

ModelPtr ImageSlice::getDataFrameVisual() {
	return mDataFrame;
}

bool ImageSlice::getVoxelCoordinates(const int& i, const int& j, const int& k, double& x, double& y, double& z) const {
	// if indices are negative, they are bad
	if (i < 0 || j < 0 || k < 0) {
		return false;
	}

	// if an index is out of range with their corresponding dimension, 
	// they are bad
	int* dims = mMapper->GetInput()->GetDimensions();
	if (i >= dims[0] || j >= dims[1] || k >= dims[2]) {
		return false;
	}
	
	double* spacing = mMapper->GetInput()->GetSpacing();
	double* origin = mMapper->GetInput()->GetOrigin();

	// Compute its (x, y, z) based on the data origin and spacing
	x = origin[0] + i * spacing[0];
	y = origin[1] + j * spacing[1];
	z = origin[2] + k * spacing[2];

	// Transform the point to its position in 3D space based on the Visual's 
	// transformation
	double coord[3] = {x, y, z};
	this->transformPoint(coord);
	x = coord[0];
	y = coord[1];
	z = coord[2];

	return true;
}

bool ImageSlice::getVoxelFromCoordinates(const double& x, const double& y, 
	const double& z, int& i, int& j, int& k) const 
{
	double* spacing = mMapper->GetInput()->GetSpacing();
	double* origin = mMapper->GetInput()->GetOrigin();

	// Transform global coordinate by the inverse of this Visual's transform. 
	double coord[3] = {x, y, z};
	this->getInverseTransform()->TransformPoint(coord, coord);
	
	// dummy variables to store computed i, j, ks
	int ii, ij, ik;
	ii = (coord[0] - origin[0]) / spacing[0];
	ij = (coord[1] - origin[1]) / spacing[1];
	ik = (coord[2] - origin[2]) / spacing[2];

	// if calculated indices are negative, they are bad
	if (ii < 0 || ij < 0 || ik < 0) {
		return false;
	}

	// if a calculated index is out of range with their corresponding dimension,
	// they are bad
	int* dims = mMapper->GetInput()->GetDimensions();
	if (ii >= dims[0] || ij >= dims[1] || ik >= dims[2]) {
		return false;
	}

	// Indices are good, assign the index values to the out variables
	i = ii;
	j = ij;
	k = ik;
	return true;
}

void ImageSlice::calculateSliceIndices() {
	int* dimensions = mMapper->GetInput()->GetDimensions();

	if (mOrientationSliceIndices.transverseIndex >= dimensions[2]) {
		mOrientationSliceIndices.transverseIndex = dimensions[2] / 2;
	}

	if (mOrientationSliceIndices.sagittalIndex >= dimensions[0]) {
		mOrientationSliceIndices.sagittalIndex = dimensions[0] / 2;
	}

	if (mOrientationSliceIndices.coronalIndex >= dimensions[1]) {
		mOrientationSliceIndices.coronalIndex = dimensions[1] / 2;
	}
}

void ImageSlice::onTransformChange() {
	mDataFrame->setTransformData(this->getTransform());
}

void ImageSlice::setOpacity(const double& a) {
	if (a == this->getOpacity()) {
		return;
	}
	mActor->SetOpacity(a);
	emit changedOpacity(a);
}

double ImageSlice::getOpacity() const {
	return mActor->GetOpacity();
}

void ImageSlice::onDataUpdated() {
	emit changedImageData(mImageData);
}

EVisual ImageSlice::getVisualType() const {
	return EVisual::IMAGE_SLICE;
}

vtkImageActor* ImageSlice::getActor() {
	return mActor;
}

void ImageSlice::setVisible(const bool & isVisible) {
	if (isVisible == this->isVisible()) {
		return;
	}
	if (isVisible) 
		mActor->VisibilityOn();
	else 
		mActor->VisibilityOff();
	emit changedVisibleState(isVisible);
}

bool ImageSlice::isVisible() const {
	return mActor->GetVisibility();
}

void ImageSlice::setPickable(const bool & isPickable) {
	if (isPickable == this->isPickable()) {
		return;
	}
	if (isPickable) 
		mActor->PickableOn();
	else 
		mActor->PickableOff();
	emit changedPickableState(isPickable);
}

bool ImageSlice::isPickable() const {
	return mActor->GetPickable();
}
