#pragma once
/*!
* @author	VelazcoJD
* @file		DataManagerDialog.h
* @class	fi3d::DataManager
* @brief	GUI to display all data objects available in the DataManager.
*/

#include <fi3d/data/Study.h>
#include <fi3d/data/ModelData.h>

#include <fi3d/data/data_manager/registered_data/RegisteredImage.h>
#include <fi3d/data/data_manager/registered_data/RegisteredStudy.h>
#include <fi3d/data/data_manager/registered_data/RegisteredModel.h>

#include <QDialog>
#include <QHeaderView>

namespace Ui {
class DataManagerDialog;
}

namespace fi3d {
class DataManagerDialog : public QDialog {

	Q_OBJECT

signals:
	/// @brief Emitted when the user wants to query the models.
	void queryModels();

	/// @brief Emitted when the user wants to query the images.
	void queryImages();

	/// @brief Emitted when the user wants to load DICOM studies.
	void requestedLoadDICOMStudy();

	/// @brief Emitted when the user wants to load an FI3D study.
	void requestedLoadFI3DStudy();

	/// @brief Emitted when the user wants to load an image.
	void requestedLoadImage();

	/// @brief Emitted when the user wants to load a model.
	void requestedLoadModel();

private:
	/// @brief The GUI elements.
	QSharedPointer<Ui::DataManagerDialog> mGUI;

	/// @brief The header labels of the study table.
	QHeaderView* mStudyTableHeader;

	/// @brief The header labels of the model table.
	QHeaderView* mModelTableHeader;

public:
	///  @brief Constructor.
	DataManagerDialog();

	/// @brief Destructor.
	~DataManagerDialog();

	/// @brief Set the images to be listed.
	void setImages(const QList<RegisteredImagePtr> images);

	/// @brief Set the studies to be listed.
	void setStudies(const QList<RegisteredStudyPtr> studies);

	/// @brief Set the models to be listed.
	void setModels(const QList<RegisteredModelPtr> models);
};
}