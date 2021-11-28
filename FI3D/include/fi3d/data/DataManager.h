#pragma once
/*!
* @author	VelazcoJD
* @file		DataManager.h
* @class	fi3d::DataManager
* @brief	Manages data for the entire system.
*
* The DataManager is a big component of the FI3D. Any module can register data
* to be managed by it. All the data registered here can be retrieved by 
* FIs. Modules should register their data here if they wish to send data 
* to other modules. Modules can retrieve data from the manager by providing 
* the data's name.
*
* Data that has a path is automatically loaded when FI3D is executed, known
* as persistent data. Each data object registered must have a unique ID. If
* the given ID is already in use, an error message is returned during 
* registration. 
*/

#include <fi3d/data/data_manager/EDM_State.h>
#include <fi3d/data/data_manager/EFileExtension.h>
#include <fi3d/data/data_manager/DataMessageEncoder.h>
#include <fi3d/data/data_manager/registered_data/RegisteredImage.h>
#include <fi3d/data/data_manager/registered_data/RegisteredStudy.h>
#include <fi3d/data/data_manager/registered_data/RegisteredModel.h>

#include <fi3d/FI3D/FI3DComponentRegistration.h>

#include <QJsonValue>
#include <QHash>
#include <QMenu>
#include <QObject>
#include <QString>

#include <vtkSmartPointer.h>

#include <fi3d/data/data_manager/gui/DataManagerDialog.h>

namespace fi3d {
class DataManager : public QObject {

	Q_OBJECT
	FI3D_COMPONENT_WITH_GUI(DataManager)
	friend class DataMessageEncoder;

/****************** Static members for the singleton ******************/
/// Methods related GUI
public slots:
	/// @brief Pops ups the dialog used to load a new DICOM study.
	static void onLoadDICOMStudy();

	/// @brief Pops ups the dialog used to load a new FI3D study.
	static void onLoadFI3DStudy();

	/// @brief Pop ups the dialog used to load a new image.
	static void onLoadImage();

	/// @brief Pop ups the dialog used to load a model.
	static void onLoadModel();

public:
	/// @brief Dialog returns a selected study, empty string if none selected.
	static DataID showStudySelector();

/// Methods related to the registration of data
public:
	/// @brief Registers model data in the manager.
	static EDM_State registerImageData(ImageDataVPtr data);

	/// @brief Register the given study.
	static EDM_State registerStudy(StudyPtr study);

	/// @brief Registers model data in the manager.
	static EDM_State registerModelData(ModelDataVPtr data);
	
	/// @brief Registers and saves the ImageData to disk.
	static EDM_State saveAndRegisterImageData(ImageDataVPtr data,
		const EFileExtension& imageFormat, const QString& directory,
		const bool& isPersistant);

	/// @brief Registers and saves the Study to disk.
	static EDM_State saveAndRegisterStudy(StudyPtr study, const QString& directory, const bool& isPersistant);

	/// @brief Registers and saves the ModelData to disk.
	static EDM_State saveAndRegisterModelData(ModelDataVPtr data,
		const EFileExtension& modelFormat, const QString& directory,
		const bool& isPersistant);

	/// @brief Saves the ImageData to disk.
	static EDM_State saveImageData(ImageDataVPtr data, const EFileExtension& imageFormat, const QString& directory);

	/// @brief Saves the Study to disk.
	static EDM_State saveStudy(StudyPtr study, const QString& directory);

	/// @brief Saves the ModelData to disk.
	static EDM_State saveModelData(ModelDataVPtr data, const EFileExtension& modelFormat, const QString& directory);

	/*!
	 * @brief Registers studies in the manager located in the given directory.
	 *
	 * A directory may contain multiple studies since DICOM information is 
	 * stored in many files and is not trivial to know which files belong to a 
	 * specific study. Thus, one can specify a directory which is used to 
	 * search for studies. Note only one study name is given, this is because 
	 * it cannot be pre-determined how many studies there may be. As a result, 
	 * if more than one study is found, the given name will be used and "_#" 
	 * will be appended where # is the index of the study as it was found. The 
	 * user then may decide to rename these using the GUI.
	 *
	 * From the given directory, studies will be searched down the 
	 * sub-directories for up to 50 levels. This is to avoid infinite loops 
	 * from symbolic links.
	 *
	 * @param directory Path to directory, its name is used as Study IDs.
	 * @param directory The directory to start searching for studies.
	 */
	static EDM_State registerStudies(const QString& directory, const bool& isPersistent);

private:
	/// @brief Given the ManagedStudy, load the actual study data.
	static void loadStudyData(RegisteredStudyPtr regStudy);

public:
	/// @brief Updates the persistent state of an ImageData object.
	static EDM_State updateImageDataPersistantState(const DataID& dataID, const bool& isPersistent);

	/// @brief Updates the persistent state of a study.
	static EDM_State updateStudyPersistantState(const DataID& dataID, const bool& isPersistent);

	/// @brief Updates the persistent state of a model data object.
	static EDM_State updateModelDataPersistantState(const DataID& dataID, const bool& isPersistent);

	/// @brief Get an ImageData, null if not found.
	static ImageDataVPtr getImageData(const DataID& dataID);

	/// @brief Get the first found ImageData with that name, null if not found.
	static ImageDataVPtr getImageDataByName(const QString& dataName);

	/*!
	 * @brief Gets the ImageData not guaranteeing its data has been loaded.
	 *
	 * Use this function when meta-data about the data is needed (such as,
	 * data name or ID) but the data itself is not required. 
	 */
	static ImageDataVPtr getDataLessImageData(const DataID& dataID);

	/*!
	 * @brief Gets the ImageData not guaranteeing its data has been loaded.
	 *
	 * Use this function when meta-data about the data is needed (such as,
	 * data name or ID) but the data itself is not required.
	 */
	static ImageDataVPtr getDataLessImageDataByName(const QString& dataName);

	/// @brief Get a study, null if not found.
	static StudyPtr getStudy(const DataID& dataID);

	/// @brief Get the first found Study with that StudyID, null if not found.
	static StudyPtr getStudyByStudyID(const QString& studyID);

	/*!
	 * @brief Gets the Study not guaranteeing its data has been loaded.
	 *
	 * Use this function when meta-data about the Study is needed (such as,
	 * StudyID, thumbnail, etc.) but the data itself is not required.
	 */
	static StudyPtr getDataLessStudy(const DataID& dataID);

	/*!
	 * @brief Gets the Study not guaranteeing its data has been loaded.
	 *
	 * Use this function when meta-data about the Study is needed (such as,
	 * StudyID, thumbnail, etc.) but the data itself is not required.
	 */
	static StudyPtr getDataLessStydtByStudyID(const QString& studyID);

	/// @brief Get a Modeldata, null if not found.
	static ModelDataVPtr getModelData(const DataID& dataID);

	/// @brief Get the first found ModelData with that name, null if not found.
	static ModelDataVPtr getModelDataByName(const QString& dataName);

	/*!
	 * @brief Gets the ModelData not guaranteeing its data has been loaded.
	 *
	 * Use this function when meta-data about the data is needed (such as,
	 * data name or ID) but the data itself is not required.
	 */
	static ModelDataVPtr getDataLessModelData(const DataID& dataID);

	/*!
	 * @brief Gets the ModelData not guaranteeing its data has been loaded.
	 *
	 * Use this function when meta-data about the data is needed (such as,
	 * data name or ID) but the data itself is not required.
	 */
	static ModelDataVPtr getDataLessModelDataByName(const QString& dataName);

	/// @brief Query whether there is an ImageData with that name.
	static bool isImageNameTaken(const QString& dataName);

	/// @brief Query whether there is a Study with that ID.
	static bool isStudyIDTaken(const QString& dataName);

	/// @brief Query whether there is a ModelData with that name.
	static bool isModelNameTaken(const QString& dataName);

	/// @brief Gets a list of the ImageData objects.
	static QList<DataID> getImageDatas(const bool& persistentOnly = false);

	/// @brief Gets a list of the Study objects.
	static QList<DataID> getStudies(const bool& persistentOnly = false);

	/// @brief Gets a list of the ModelData objects.
	static QList<DataID> getModelDatas(const bool& persistentOnly = false);

	/*! @brief Get a pointer to the running data manager. */
	static const DataManager* getInstance();

/****************** Singleton members ******************/
private:
	/// @brief Path to location of file with persistent data.
	const QString M_PERSISTENT_PATH;

	/// @brief Hash table with all the used up QUuIDs.
	QHash<DataID, QString> mUsedQUuIDs;

	/// @brief Hash table with all the managed image data sets.
	QHash<DataID, RegisteredImagePtr> mRegisteredImages;

	/// @brief Hash table with all the managed studies.
	QHash<DataID, RegisteredStudyPtr> mRegisteredStudies;

	/// @brief Hash table with all the managed model data sets.
	QHash<DataID, RegisteredModelPtr> mRegisteredModels;

	/// @brief Message encoder used to communicate with clients.
	DataMessageEncoderPtr mMessageEncoder;

	/// @brief The dialog used to interact with this manager.
	QSharedPointer<DataManagerDialog> mGUI;

	/// @brief There's only one data manager, thus, it cannot be instantiated.
	DataManager();

public:
	/*! @brief Destructor. */
	~DataManager();

private:
/// Methods related to the saving/loading of persistent data
	/// @brief Reads a data manager configuration file.
	void loadPersistantData();

	/// @brief Saves the persistent data meta-data to configuration file.
	void savePersistentData();

/// Methods related to the GUI and data dialogs
	/// @brief Updates the list of images that are listed.
	void updateDialogImageList();

	/// @brief Updates the list of studies that are listed.
	void updateDialogStudyList();

	/// @brief Updates the list of models that are listed.
	void updateDialogModelList();

	/// @brief Gets a unique QUuID.
	QUuid generateUniqueQUuID();
};
}