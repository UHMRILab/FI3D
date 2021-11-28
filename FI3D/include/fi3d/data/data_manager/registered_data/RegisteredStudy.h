#pragma once
/*!
* @author	VelazcoJD
* @file		RegisteredStudy.h
* @class	fi3d::RegisteredStudy
* @brief	Contains information about a registered Study.
*/

#include <fi3d/data/data_manager/registered_data/RegisteredData.h>

#include <fi3d/data/data_manager/registered_data/ImageDataJson.h>
#include <fi3d/data/Study.h>

#include <vtkStringArray.h>

namespace fi3d {
class RegisteredStudy : public RegisteredData {
private:
	/// @brief The study itself.
	StudyPtr mStudy;

	///  @brief Whether the study was loaded from DICOM files. 
	bool mIsDICOMStudy;

	/// @brief Whether the study was laoded from a FI3D file.
	bool mIsFI3DStudy;

	/// @brief The paths to the DICOM files, if loaded from DICOMs.
	QVector<vtkSmartPointer<vtkStringArray>> mDICOMPaths;

	/// @brief The path to the FI3D file, if loaded from a FI3D file.
	QString mFI3DFilePath;

	/// @brief The study in its Json format.
	QVector<ImageDataJson> mSeriesJson;

public:
	/// @brief Constructor.
	RegisteredStudy(StudyPtr study);

	/// @brief Destructor.
	~RegisteredStudy();

	/// @brief Gets the Study object itself.
	StudyPtr getStudy();

	/// @brief Gets the Study's ID.
	DataID getDataID() override;

	/// @brief Set whether the study is a DICOM study.
	void setAsDICOMStudy(QVector<vtkSmartPointer<vtkStringArray>> paths);

	/*!
	 * @brief Gets the paths to the DICOM files of this study.
	 *
	 * The returned list is empty of the Study was not loaded from DICOMs.
	 */
	QVector<vtkSmartPointer<vtkStringArray>> getDICOMPaths();

	/// @brief Gets whether the study was loaded from DICOMs.
	bool isDICOMStudy();

	/// @brief Set whether the study is a FI3D study.
	void setAsFI3DStudy(const QString& filePath);

	/// @brief Gets the FI3D study file path, empty if non
	QString getFI3DStudyPath();

	/// @brief Gets wether the study was loaded from a FI3D file.
	bool isFI3DStudy();

	/// @brief Sets the Message representation of a slice in the study.
	void setMessage(const int& sliceIndex, 
		const ESliceOrientation& orientation, const int& seriesIndex, 
		MessagePtr message);

	/// @brief Gets the Message representation of a slice in the study.
	MessagePtr getMessage(const int& sliceIndex, const ESliceOrientation& orientation, const int& seriesIndex);
};

/// @brief Alias for a smart pointer of this class.
using RegisteredStudyPtr = QSharedPointer<RegisteredStudy>;

}