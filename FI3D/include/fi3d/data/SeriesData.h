#pragma once
/*!
* @author	VelazcoJD
* @file		SeriesData.h
* @class	fi3d::SeriesData
* @brief	An ImageData that is part of a Study.
*
* This class is the same as an ImageData but also contains information about
* the ImageData as a series of the Study.
*/

#include <fi3d/data/ImageData.h>

#include <vtkDICOMMetaData.h>

namespace fi3d {
class Study;
class SeriesData : public ImageData {

	Q_OBJECT
	friend class Study;

private:
	/// @brief The index of this series within the Study.
	int mSeriesIndex;

	/// @brief The patient matrix defining position and orientation of image.
	vtkSmartPointer<vtkMatrix4x4> mPatientMatrix;

	/// @brief The meta-data of the series obtain from the DICOMs.
	vtkSmartPointer<vtkDICOMMetaData> mMetaData;

public:
	/// @brief VTK object dependencies.
	static SeriesData* New();
	vtkTypeMacro(SeriesData, ImageData)

	/// @brief Return the right data type.
	EData getDataType() const override;

private:
	/// @brief Sets the series index this image data has in the study.
	void setSeriesIndex(const int& index);

public:
	/// @brief Gets the series index this image data has in the study.
	int getSeriesIndex();

	/// @brief Sets the patient matrix.
	void setPatientMatrix(vtkSmartPointer<vtkMatrix4x4> patientMatrix);

	/// @brief Gets the patient matrix.
	vtkSmartPointer<vtkMatrix4x4> getPatientMatrix();

	/// @brief Sets the meta-data object.
	void setMetaData(vtkSmartPointer<vtkDICOMMetaData> metaData);

	/// @brief Gets the meta-data object.
	vtkSmartPointer<vtkDICOMMetaData> getMetaData();

protected:
	/// @brief Constructor.
	SeriesData();

	/// @brief Destructor.
	~SeriesData() override = default;

private:
	SeriesData(const SeriesData&) = delete;
	void operator=(const SeriesData&) = delete;
};

/// @brief Alias for a smart pointer of this class.
using SeriesDataVPtr = vtkSmartPointer<SeriesData>;

}