#include <fi3d/data/SeriesData.h>

#include <vtkObjectFactory.h>

using namespace fi3d;

vtkStandardNewMacro(SeriesData)

SeriesData::SeriesData()
	: ImageData(),
	mSeriesIndex(-1),
	mPatientMatrix(vtkSmartPointer<vtkMatrix4x4>::New()),
	mMetaData(vtkSmartPointer<vtkDICOMMetaData>::New())
{
}

SeriesData::~SeriesData() {
}

EData SeriesData::getDataType() const {
	return EData::SERIES;
}

void SeriesData::setSeriesIndex(const int& index) {
	mSeriesIndex = index;
}

int SeriesData::getSeriesIndex() {
	return mSeriesIndex;
}

void SeriesData::setPatientMatrix(vtkSmartPointer<vtkMatrix4x4> patientMatrix) {
	mPatientMatrix = patientMatrix;
}

vtkSmartPointer<vtkMatrix4x4> SeriesData::getPatientMatrix() {
	return mPatientMatrix;
}

void SeriesData::setMetaData(vtkSmartPointer<vtkDICOMMetaData> metaData) {
	mMetaData = metaData;
}

vtkSmartPointer<vtkDICOMMetaData> SeriesData::getMetaData() {
	return mMetaData;
}
