#pragma once
/*!
* @author	VelazcoJD
* @file		Study.h
* @class	fi3d::Study
* @brief	Contains data about a single study read from DICOM files.
*
* Each study has patient name and ID, the date and time of the study, and many 
* other study information. It also has a list of data sets (series) for the
* study.
*/

#include <fi3d/data/DataObject.h>

#include <fi3d/data/SeriesData.h>

#include <QVector>
#include <QSharedPointer>

#include <vtkMatrix4x4.h>
#include <vtkSmartPointer.h>

namespace fi3d {
class Study : public DataObject {

	Q_OBJECT

signals:
	/// @brief Signal indicating a series was added to the study.                 
	void changedAddedSeries(fi3d::SeriesDataVPtr series);

	/// @brief Signal indicating a series was removed from the study.
	void changedRemovedSeries(fi3d::SeriesDataVPtr series);

private:
	/// @brief The patient ID of the study.
	QString mPatientID;

	/// @brief The patient name of the study.
	QString mPatientName;

	/// @brief The date of the study.
	QString mDate;

	/// @brief The time of the study. 
	QString mTime;

	/// @brief List of data sets (series). 
	QVector<SeriesDataVPtr> mSeriesSet;

	/// @brief A low resolution image of a slice to represent the Study
	/// visually.
	ImageDataVPtr mThumbnail;

public:
	/// @brief Constructs a study with the given ID.
	Study(QString studyID = "");

	/// @brief Destructor.
	~Study();

	/// @brief Return the right data type.
	EData getDataType() const override;

	/// @brief Adds a data set to this study.
	void addSeries(SeriesDataVPtr data);

	/// @brief Creates a new series and adds it to the study.
	SeriesDataVPtr createAndAddSeries();

	/// @brief Removes the series at the given index, returns if
	/// removed successfully.
	bool removeSeries(const int& index);

	/// @brief Get the number of data sets (series) in the study.
	int getSeriesCount() const;

	/// @brief Gets a data set from the study.
	SeriesDataVPtr getSeries(const int& seriesIndex);

	/// @brief Whether the given index represents a series in the study.
	bool isSeriesIndexInRange(const int& seriesIndex) const;

	/// @brief Sets the study ID, this is the same as setDataName.
	void setStudyID(const QString& studyID);

	/// @brief Gets the study ID, this is the same as getDataName.
	QString getStudyID() const;

	/// @brief Sets the patient ID.
	void setPatientID(const QString& patientID);

	/// @brief Gets the patient ID.
	QString getPatientID() const;

	/// @brief Gets the patient name.
	void setPatientName(const QString& patientName);

	/// @brief Gets the paitnet Name.
	QString getPatientName() const;

	/// @brief Sets the study's date.
	void setStudyDate(const QString& date);

	/// @brief Gets the study's date.
	QString getStudyDate() const;

	/// @brief Sets the study's time.
	void setStudyTime(const QString& time);

	/// @brief Gets the study's time.
	QString getStudyTime() const;

	/// @brief Sets the thumbnail of the Study.
	void setThumbnail(ImageDataVPtr image);

	/// @brief Gets the thumnail of the Study.
	ImageDataVPtr getThumbnail();

};

/// @brief Alias for a smart pointer of this class.
using StudyPtr = QSharedPointer<Study>;

}
