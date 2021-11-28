#include <fi3d/data/DataManager.h>

#include <fi3d/logger/Logger.h>

#include <fi3d/data/EData.h>
#include <fi3d/data/Filer.h>
#include <fi3d/data/data_manager/gui/StudySelectorDialog.h>

#include <fi3d/modules/ModuleFactory.h>

#include <QAction>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFileDialog>
#include <QMenu>
#include <QMessageBox>
#include <QUrl>

#include <vtkDICOMItem.h>
#include <vtkDICOMDirectory.h>
#include <vtkDICOMReader.h>

#include <vtkImageMapToColors.h>
#include <vtkLookupTable.h>
#include <vtkNew.h>
#include <vtkImageResize.h>

using namespace fi3d;

/*************************** Helper Functions ***************************/
inline void createThumbnail(RegisteredStudyPtr regStudy) {
	if (regStudy.isNull() || regStudy->getStudy().isNull()) {
		return;
	}

	StudyPtr study = regStudy->getStudy();

	if (study->getSeriesCount() == 0 ||
		study->getThumbnail().Get() != Q_NULLPTR) {
		return;
	}

	int middleSeriesIndex = study->getSeriesCount() / 2;
	SeriesDataVPtr series = study->getSeries(middleSeriesIndex);

	int* dims = series->GetDimensions();
	double* spacing = series->GetSpacing();

	int xy = dims[0] * dims[1];
	int yz = dims[1] * dims[2];
	int xz = dims[0] * dims[2];

	ImageDataVPtr thumbnail = ImageDataVPtr::New();
	//if (xy >= yz && xy >= xz) {				// Transverse Slice
	//	if (xy == 0) {
	//		return;
	//	}

	//	thumbnail->SetDimensions(dims[0], dims[1], 1);
	//	thumbnail->SetSpacing(spacing[0], spacing[1], spacing[2]);
	//} else if (yz >= xy && yz >= xz) {		// Sagittal Slice
	//	if (yz == 0) {
	//		return;
	//	}

	//	thumbnail->SetDimensions(dims[1], dims[2], 1);
	//	thumbnail->SetSpacing(spacing[1], spacing[2], spacing[0]);
	//} else {								// Coronal Slice
	//	if (xz == 0) {
	//		return;
	//	}

	//	thumbnail->SetDimensions(dims[0], dims[2], 1);
	//	thumbnail->SetSpacing(spacing[0], spacing[2], spacing[1]);
	//}

	vtkNew<vtkImageResize> resizer;
	resizer->SetInputData(series);
	resizer->SetOutput(thumbnail);
	resizer->SetOutputDimensions(64, 64, 1);
	resizer->Update();

	study->setThumbnail(thumbnail);

	if (regStudy->isPersistent()) {
		QString id = regStudy->getDataID().toString();

		QString directory = QObject::tr("%1/FI3D/thumbnails").arg(FI3D_DATA_PATH);

		Filer::checkAndCreateDirectory(directory);
		QString filepath = QObject::tr("%1/%2.jpg").arg(directory).arg(id);
		Filer::saveImageDataAsJPG(thumbnail, filepath);
	}
}

/************************ FI3D Componenet Members ************************/
REGISTER_FI3D_COMPONENT_WITH_GUI(DataManager, "Data Manager", "DM")

void DataManager::showDialog() {
	INSTANCE->mGUI->show();

	INSTANCE->updateDialogImageList();
	INSTANCE->updateDialogModelList();
	INSTANCE->updateDialogStudyList();
}

/************************ Static Members ************************/
void DataManager::onLoadDICOMStudy() {
	QUrl dirPath = QFileDialog::getExistingDirectory(INSTANCE->mGUI.data(),
		tr("Select DICOM Directory"), fi3d::FI3D::DATA_DIRECTORY,
		QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

	QString path = dirPath.toString();

	if (path.isEmpty()) {
		return;
	}

	EDM_State state = DataManager::registerStudies(path, true);

	switch (state.toInt()) {
	case EDM_State::DATA_NOT_FOUND:
		{
			QMessageBox message(INSTANCE->mGUI.data());
			message.setText("No DICOM studies were found");
			message.exec();
		}
		break;
	case EDM_State::ALREADY_REGISTERED:
		{
			QMessageBox message(INSTANCE->mGUI.data());
			message.setText("A Study with that ID is already in the system");
			message.exec();
		}
		break;
	case EDM_State::SUCCESS:
		break;
	default:
		{
			QMessageBox message(INSTANCE->mGUI.data());
			message.setText("An error ocurred");
			message.exec();
		}
		break;
	}
}

void DataManager::onLoadFI3DStudy() {
	QUrl filePath = QFileDialog::getOpenFileName(INSTANCE->mGUI.data(),
		tr("Select FI3D File"), fi3d::FI3D::DATA_DIRECTORY,
		tr("Files (*.fi3d)"));
	
	QString path = filePath.toString();

	if (path.isEmpty()) {
		return;
	}

	StudyPtr study = Filer::readStudyFromFI3DFile(path);

	QUuid dataId = INSTANCE->generateUniqueQUuID();
	INSTANCE->mUsedQUuIDs.insert(dataId, study->getStudyID());
	study->setDataQUuID(dataId);

	RegisteredStudyPtr regStudy(new RegisteredStudy(study));
	regStudy->setAsFI3DStudy(path);
	regStudy->setRegistered(true);
	regStudy->setDataLoaded(true);
	regStudy->setPersistent(true);

	createThumbnail(regStudy);

	INSTANCE->mRegisteredStudies.insert(dataId, regStudy);

	INSTANCE->updateDialogStudyList();
}

void DataManager::onLoadImage() {
	QMessageBox message(INSTANCE->mGUI.data());
	message.setText("This feature is currently in development.");
	message.exec();
}

void DataManager::onLoadModel() {
	QString path = QFileDialog::getOpenFileName(INSTANCE->mGUI.data(),
		tr("Select Model File"), fi3d::FI3D::DATA_DIRECTORY,
		tr("Files (*.stl *.obj *.ply *.fi3d)"));

	if (path.isEmpty()) {
		return;
	}

	QString dataName;
	if (path.contains('/')) {
		dataName = path.section('/', -1);
	} else {
		dataName = path;
	}
	dataName = dataName.section('.', 0, 0);

	ModelDataVPtr model = ModelDataVPtr::New();
	model->setDataName(dataName);

	QUuid dataId = INSTANCE->generateUniqueQUuID();
	INSTANCE->mUsedQUuIDs.insert(dataId, dataName);
	model->setDataQUuID(dataId);

	RegisteredModelPtr regModel(new RegisteredModel(model));
	regModel->setRegistered(true);
	regModel->setPersistent(true);
	regModel->setDataPath(path);

	INSTANCE->mRegisteredModels.insert(model->getDataID(), regModel);

	INSTANCE->updateDialogModelList();
}

DataID DataManager::showStudySelector() {
	StudySelectorDialog dialog;
	dialog.setStudies(INSTANCE->mRegisteredStudies.values());
	dialog.exec();

	return dialog.getSelectedStudy();
}

EDM_State DataManager::registerImageData(ImageDataVPtr image) {
	qDebug() << "Enter";
	if (image.Get() == Q_NULLPTR) {
		return EDM_State::DATA_NOT_FOUND;
	}

	qDebug() << "Registering ImageData:" << image->getDataName();

	if (INSTANCE->mRegisteredImages.contains(image->getDataID()))
	{
		return EDM_State::ALREADY_REGISTERED;
	}

	QUuid dataId = INSTANCE->generateUniqueQUuID();
	INSTANCE->mUsedQUuIDs.insert(dataId, image->getDataName());
	image->setDataQUuID(dataId);

	RegisteredImagePtr regImage(new RegisteredImage(image));
	regImage->setRegistered(true);
	regImage->setDataLoaded(true);
	INSTANCE->mRegisteredImages.insert(regImage->getDataID(), regImage);

	qDebug() << "Exit";
	return EDM_State::UNKNOWN;
}

EDM_State DataManager::registerStudy(StudyPtr study) {
	if (study.isNull()) {
		return EDM_State::DATA_NOT_FOUND;
	}

	if (INSTANCE->mRegisteredStudies.contains(study->getDataID())) 	{
		return EDM_State::ALREADY_REGISTERED;
	}

	QUuid dataId = INSTANCE->generateUniqueQUuID();
	INSTANCE->mUsedQUuIDs.insert(dataId, study->getStudyID());
	study->setDataQUuID(dataId);

	RegisteredStudyPtr regStudy(new RegisteredStudy(study));
	regStudy->setRegistered(true);
	regStudy->setDataLoaded(true);

	createThumbnail(regStudy);

	INSTANCE->mRegisteredStudies.insert(dataId, regStudy);

	INSTANCE->updateDialogStudyList();
	return EDM_State::SUCCESS;
}

EDM_State DataManager::registerModelData(ModelDataVPtr model) {
	qDebug() << "Enter";
	if (model.Get() == Q_NULLPTR) {
		return EDM_State::DATA_NOT_FOUND;
	}

	qDebug() << "Registering ModelData:" << model->getDataName();

	if (INSTANCE->mRegisteredModels.contains(model->getDataID())) {
		return EDM_State::ALREADY_REGISTERED;
	}

	QUuid dataId = INSTANCE->generateUniqueQUuID();
	INSTANCE->mUsedQUuIDs.insert(dataId, model->getDataName());
	model->setDataQUuID(dataId);

	RegisteredModelPtr regModel(new RegisteredModel(model));
	regModel->setRegistered(true);
	regModel->setDataLoaded(true);
	INSTANCE->mRegisteredModels.insert(regModel->getDataID(), regModel);

	INSTANCE->updateDialogModelList();

	qDebug() << "Registered ModelData" << model->getDataName() << "with ID" << model->getDataID();

	qDebug() << "Exit";
	return EDM_State::SUCCESS;
}

EDM_State DataManager::saveAndRegisterImageData(ImageDataVPtr data, 
	const EFileExtension& imageFormat, const QString& directory, 
	const bool& isPersistant) 
{
	// TODO:
	return EDM_State();
}

EDM_State DataManager::saveAndRegisterStudy(StudyPtr study, const QString& directory, const bool& isPersistant) {
	// TODO:
	return EDM_State();
}

EDM_State DataManager::saveAndRegisterModelData(ModelDataVPtr data, 
	const EFileExtension& modelFormat, const QString& directory, 
	const bool& isPersistant) 
{
	// TODO:
	return EDM_State();
}

EDM_State DataManager::saveImageData(ImageDataVPtr data, 
	const EFileExtension& imageFormat, const QString& directory) 
{
	// TODO:
	return EDM_State();
}

EDM_State DataManager::saveStudy(StudyPtr study, const QString& directory) {
	// TODO:
	return EDM_State();
}

EDM_State DataManager::saveModelData(ModelDataVPtr data, 
	const EFileExtension& modelFormat, const QString& directory) 
{
	// TODO:
	return EDM_State();
}

EDM_State DataManager::registerStudies(const QString& directory, const bool& isPersistent) {
	QVector<QVector<vtkSmartPointer<vtkStringArray>>> studiesPaths;
	QList<StudyPtr> studies = Filer::findStudies(directory, studiesPaths);
	if (studies.isEmpty()) {
		return EDM_State::DATA_NOT_FOUND;
	}

	for (int i = 0; i < studies.count(); i++) {
		RegisteredStudyPtr regStudy(new RegisteredStudy(studies[i]));
		regStudy->setPersistent(isPersistent);
		regStudy->setDataLoaded(false);
		regStudy->setAsDICOMStudy(studiesPaths[i]);
		
		QUuid dataId = INSTANCE->generateUniqueQUuID();
		INSTANCE->mUsedQUuIDs.insert(dataId, studies[i]->getStudyID());
		studies[i]->setDataQUuID(dataId);

		DataManager::loadStudyData(regStudy);
		createThumbnail(regStudy);

		INSTANCE->mRegisteredStudies.insert(dataId, regStudy);
	}

	INSTANCE->updateDialogStudyList();
	return EDM_State::SUCCESS;
}

void DataManager::loadStudyData(RegisteredStudyPtr regStudy) {
	if (regStudy->getStudy().isNull() || regStudy->isDataLoaded()) {
		return;
	}

	if (regStudy->isDICOMStudy()) {
		Filer::readStudyDataFromDICOMs(regStudy->getDICOMPaths(), regStudy->getStudy());
		regStudy->setDataLoaded(true);
	} else if (regStudy->isFI3DStudy()) {
		Filer::readStudyDataFromFI3DFile(regStudy->getFI3DStudyPath(), regStudy->getStudy());
		regStudy->setDataLoaded(true);
	} else {
		return;
	}
}

EDM_State DataManager::updateImageDataPersistantState(const DataID& dataID, 
	const bool& isPersistent) 
{
	RegisteredImagePtr regImage = INSTANCE->mRegisteredImages.value(dataID);
	if (regImage.isNull()) {
		return EDM_State::DATA_NOT_FOUND;
	}

	if (regImage->isPersistent() == isPersistent) {
		return EDM_State::SUCCESS;
	}

	//TODO: This changes the persistent list. Need a way to update that.
	regImage->setPersistent(isPersistent);

	return EDM_State::SUCCESS;
}

EDM_State DataManager::updateStudyPersistantState(const DataID& dataID, const bool& isPersistent) {
	RegisteredStudyPtr regStudy = INSTANCE->mRegisteredStudies.value(dataID);
	if (regStudy.isNull()) {
		return EDM_State::DATA_NOT_FOUND;
	}

	if (regStudy->isPersistent() == isPersistent) {
		return EDM_State::SUCCESS;
	}

	//TODO: This changes the persistent list. Need a way to update that.
	regStudy->setPersistent(isPersistent);

	return EDM_State::SUCCESS;
}

EDM_State DataManager::updateModelDataPersistantState(const DataID& dataName, const bool& isPersistent) {
	RegisteredModelPtr regModel = INSTANCE->mRegisteredModels.value(dataName);
	if (regModel.isNull()) {
		return EDM_State::DATA_NOT_FOUND;
	}

	if (regModel->isPersistent() == isPersistent) {
		return EDM_State::SUCCESS;
	}

	//TODO: This changes the persistent list. Need a way to update that.
	regModel->setPersistent(isPersistent);

	return EDM_State::SUCCESS;
}

ImageDataVPtr DataManager::getImageData(const DataID& dataID) {
	if (!INSTANCE->mRegisteredImages.contains(dataID)) {
		return Q_NULLPTR;
	}

	RegisteredImagePtr regImage = INSTANCE->mRegisteredImages.value(dataID);
	if (!regImage->isDataLoaded()) {
		// TODO: Load image data
	}
	return regImage->getImageData();
}

ImageDataVPtr DataManager::getImageDataByName(const QString& dataName) {
	for (RegisteredImagePtr image : INSTANCE->mRegisteredImages.values()) {
		if (image->getImageData()->getDataName() == dataName) {
			if (!image->isDataLoaded()) {
				// TODO: Load image data
			}
			return image->getImageData();
		}
	}

	return Q_NULLPTR;
}

ImageDataVPtr DataManager::getDataLessImageData(const DataID& dataID) {
	if (!INSTANCE->mRegisteredImages.contains(dataID)) {
		return Q_NULLPTR;
	}

	RegisteredImagePtr regImage = INSTANCE->mRegisteredImages.value(dataID);
	return regImage->getImageData();
}

ImageDataVPtr DataManager::getDataLessImageDataByName(const QString& dataName) {
	for (RegisteredImagePtr image : INSTANCE->mRegisteredImages.values()) {
		if (image->getImageData()->getDataName() == dataName) {
			return image->getImageData();
		}
	}

	return Q_NULLPTR;
}

StudyPtr DataManager::getStudy(const DataID& dataID) {
	if (!INSTANCE->mRegisteredStudies.contains(dataID)) {
		return Q_NULLPTR;
	}

	RegisteredStudyPtr regStudy = INSTANCE->mRegisteredStudies.value(dataID);
	if (!regStudy->isDataLoaded()) {
		DataManager::loadStudyData(regStudy);
	}
	return regStudy->getStudy();
}

StudyPtr DataManager::getStudyByStudyID(const QString& studyID) {
	for (RegisteredStudyPtr study : INSTANCE->mRegisteredStudies.values()) {
		if (study->getStudy()->getStudyID() == studyID) {
			if (!study->isDataLoaded()) {
				DataManager::loadStudyData(study);
			}
			return study->getStudy();
		}
	}

	return Q_NULLPTR;
}

StudyPtr DataManager::getDataLessStudy(const DataID& dataID) {
	if (!INSTANCE->mRegisteredStudies.contains(dataID)) {
		return Q_NULLPTR;
	}

	RegisteredStudyPtr regStudy = INSTANCE->mRegisteredStudies.value(dataID);
	return regStudy->getStudy();
}

StudyPtr DataManager::getDataLessStydtByStudyID(const QString& studyID) {
	for (RegisteredStudyPtr study : INSTANCE->mRegisteredStudies.values()) {
		if (study->getStudy()->getStudyID() == studyID) {
			return study->getStudy();
		}
	}

	return Q_NULLPTR;
}

ModelDataVPtr DataManager::getModelData(const DataID& dataID) {
	if (!INSTANCE->mRegisteredModels.contains(dataID)) {
		return Q_NULLPTR;
	}

	RegisteredModelPtr regModel = INSTANCE->mRegisteredModels.value(dataID);
	if (!regModel->isDataLoaded()) {
		Filer::readModelData(regModel->getDataPath(), regModel->getModelData());
		regModel->setDataLoaded(true);
	}
	return regModel->getModelData();
}

ModelDataVPtr DataManager::getModelDataByName(const QString& dataName) {
	for (RegisteredModelPtr model : INSTANCE->mRegisteredModels.values()) {
		if (model->getModelData()->getDataName() == dataName) {
			if (!model->isDataLoaded()) {
				Filer::readModelData(model->getDataPath(), model->getModelData());
				model->setDataLoaded(true);
			}
			return model->getModelData();
		}
	}

	return Q_NULLPTR;
}

ModelDataVPtr DataManager::getDataLessModelData(const DataID& dataID) {
	if (!INSTANCE->mRegisteredModels.contains(dataID)) {
		return Q_NULLPTR;
	}

	RegisteredModelPtr regModel = INSTANCE->mRegisteredModels.value(dataID);
	return regModel->getModelData();
}

ModelDataVPtr DataManager::getDataLessModelDataByName(const QString& dataName) {
	for (RegisteredModelPtr model : INSTANCE->mRegisteredModels.values()) {
		if (model->getModelData()->getDataName() == dataName) {
			return model->getModelData();
		}
	}

	return Q_NULLPTR;
}

bool DataManager::isImageNameTaken(const QString& dataName) {
	for (RegisteredImagePtr image : INSTANCE->mRegisteredImages.values()) {
		if (image->getImageData()->getDataName() == dataName) {
			return true;
		}
	}
	return false;
}

bool DataManager::isStudyIDTaken(const QString& studyID) {
	for (RegisteredStudyPtr study : INSTANCE->mRegisteredStudies.values()) {
		if (study->getStudy()->getStudyID() == studyID) {
			return true;
		}
	}

	return false;
}

bool DataManager::isModelNameTaken(const QString& dataName) {
	for (RegisteredModelPtr model : INSTANCE->mRegisteredModels.values()) {
		if (model->getModelData()->getDataName() == dataName) {
			return true;
		}
	}

	return false;
}

QList<DataID> DataManager::getImageDatas(const bool& persistentOnly) {
	// TODO: Filter through the data to only return persistent ones when
	// the persistentOnly flag is on.

	return INSTANCE->mRegisteredImages.keys();
}

QList<DataID> DataManager::getStudies(const bool& persistentOnly) {
	// TODO: Filter through the data to only return persistent ones when
	// the persistentOnly flag is on.

	return INSTANCE->mRegisteredStudies.keys();
}

QList<DataID> DataManager::getModelDatas(const bool& persistentOnly) {
	// TODO: Filter through the data to only return persistent ones when
	// the persistentOnly flag is on.

	return INSTANCE->mRegisteredModels.keys();
}

const DataManager* DataManager::getInstance() {
	return DataManager::INSTANCE.data();
}

/************************ Singleton Members ************************/
DataManager::DataManager()
	: QObject(),
    M_PERSISTENT_PATH(tr("%1/FI3D/%2").arg(FI3D_DATA_PATH).arg("DM_PersistentData.json")),
	mUsedQUuIDs(),
	mRegisteredImages(), mRegisteredStudies(), mRegisteredModels(),
	mMessageEncoder(),
	mGUI()
{
	mMessageEncoder.reset(new DataMessageEncoder(this));

	this->loadPersistantData();

	mGUI.reset(new DataManagerDialog());
	mGUI->setModal(true);
	QObject::connect(
		mGUI.data(), &DataManagerDialog::queryModels,
		this, &DataManager::updateDialogModelList);
	QObject::connect(
		mGUI.data(), &DataManagerDialog::queryImages,
		this, &DataManager::updateDialogImageList);
	QObject::connect(
		mGUI.data(), &DataManagerDialog::requestedLoadDICOMStudy,
		&DataManager::onLoadDICOMStudy);
	QObject::connect(
		mGUI.data(), &DataManagerDialog::requestedLoadFI3DStudy,
		&DataManager::onLoadFI3DStudy);
	QObject::connect(
		mGUI.data(), &DataManagerDialog::requestedLoadImage,
		&DataManager::onLoadImage);
	QObject::connect(
		mGUI.data(), &DataManagerDialog::requestedLoadModel, 
		&DataManager::onLoadModel);
}

DataManager::~DataManager() {
	this->savePersistentData();
}

void DataManager::loadPersistantData() {
	qDebug() << "Enter - Loading Persistent Data";
	QString contents = Filer::readTextFile(M_PERSISTENT_PATH).trimmed();

	QJsonParseError errorMessage;
	QJsonObject config = QJsonDocument::fromJson(contents.toUtf8(), &errorMessage).object();
	if (errorMessage.error != QJsonParseError::NoError) {
		qWarning() << "Failed to load persistent file because:" << errorMessage.errorString();
		return;
	}

	qDebug() << "Loading Images";
	QJsonArray images = config.value("Images").toArray();
	for (int i = 0; i < images.count(); i++) {
		QJsonObject imageJson = images[i].toObject();

		ImageDataVPtr image = ImageDataVPtr::New();
		image->setDataName(imageJson.value("DataName").toString());
		
		if (mRegisteredImages.contains(image->getDataID())) {
			qWarning() << "Failed to load persistent ImageData" << 
				image->getDataID() << "because the image ID is taken";
			continue;
		}

		QUuid dataId(imageJson.value("DataID").toString());
		mUsedQUuIDs.insert(dataId, image->getDataName());
		image->setDataQUuID(dataId);

		RegisteredImagePtr regImage(new RegisteredImage(image));
		regImage->setRegistered(true);
		regImage->setPersistent(true);
		regImage->setDataLoaded(false);
		regImage->setDataPath(imageJson.value("Path").toString());

		mRegisteredImages.insert(regImage->getDataID(), regImage);
	}

	qDebug() << "Loading studies";
	QJsonArray studies = config.value("Studies").toArray();
	for (int i = 0; i < studies.count(); i++) {
		QJsonObject studyJson = studies[i].toObject();

		StudyPtr study(new Study(studyJson.value("DataName").toString()));
		if (mRegisteredStudies.contains(study->getDataID())) {
			qWarning() << "Failed to load persistent Study" << study->getDataID() << "because study ID is taken";
			continue;
		}

		study->setPatientID(studyJson.value("PatientID").toString());
		study->setPatientName(studyJson.value("PatientName").toString());
		study->setStudyDate(studyJson.value("StudyDate").toString());
		study->setStudyTime(studyJson.value("StudyTime").toString());

		QUuid dataId(studyJson.value("DataID").toString());
		mUsedQUuIDs.insert(dataId, study->getStudyID());
		study->setDataQUuID(dataId);

		RegisteredStudyPtr regStudy(new RegisteredStudy(study));
		regStudy->setRegistered(true);
		regStudy->setPersistent(true);
		regStudy->setDataLoaded(false);
		
		bool isDicom = studyJson.value("isDICOM").toBool();
		bool isFI3D = studyJson.value("isFI3DFile").toBool();
		int seriesCount = studyJson.value("SeriesCount").toInt();

		if (isDicom) {
			QVector<vtkSmartPointer<vtkStringArray>> dicomFiles;
			QJsonArray studyPaths = studyJson.value("DICOMPaths").toArray();
			for (int j = 0; j < studyPaths.count(); j++) {
				QJsonArray seriesPaths = studyPaths[j].toArray();
				vtkSmartPointer<vtkStringArray> series = vtkSmartPointer<vtkStringArray>::New();
				series->SetNumberOfValues(seriesPaths.count());
				for (int k = 0; k < seriesPaths.count(); k++) {
					series->SetValue(k, seriesPaths[k].toString().toStdString());
				}
				dicomFiles.push_back(series);
			}

			regStudy->setAsDICOMStudy(dicomFiles);
		} else if (isFI3D) {
			QString filePath = studyJson.value("Path").toString();
			regStudy->setAsFI3DStudy(filePath);
		} else {
			qWarning() << "Failed to load Study" << regStudy->getDataID() << "because no path data was present.";
			return;
		}

		// Load the Thumbanil.
		QString directory = tr("%1/FI3D/thumbnails").arg(FI3D_DATA_PATH);
		Filer::checkAndCreateDirectory(directory);
		QString filePath = tr("%1/%2.jpg").arg(directory).arg(study->getDataID().toString());

		ImageDataVPtr thumbnail = Filer::readImageDataFromJPEG(filePath);
		study->setThumbnail(thumbnail);

		qDebug() << "Loaded Persistant Study" << study->getStudyID() << 
			"with DataID:" << study->getDataID().toString();

		mRegisteredStudies.insert(dataId, regStudy);
	}

	qDebug() << "Loading Models";
	QJsonArray models = config.value("Models").toArray();
	for (int i = 0; i < models.count(); i++) {
		QJsonObject modelJson = models[i].toObject();

		ModelDataVPtr model = ModelDataVPtr::New();
		model->setDataName(modelJson.value("DataName").toString());
		if (mRegisteredModels.contains(model->getDataID())) {
			qWarning() << "Failed to load persistent ModelData" << 
				model->getDataID() << "because the data ID is taken";
			continue;
		}
		QUuid dataId(modelJson.value("DataID").toString());
		mUsedQUuIDs.insert(dataId, model->getDataName());
		model->setDataQUuID(dataId);

		RegisteredModelPtr regModel(new RegisteredModel(model));
		regModel->setRegistered(true);
		regModel->setPersistent(true);
		regModel->setDataLoaded(false);
		regModel->setDataPath(modelJson.value("Path").toString());

		mRegisteredModels.insert(regModel->getDataID(), regModel);
	}

	qDebug() << "Exit";
}

void DataManager::savePersistentData() {
	QJsonObject config;

	QVariantList images;
	images.reserve(mRegisteredImages.count());
	auto imIt = mRegisteredImages.begin();
	for (; imIt != mRegisteredImages.end(); imIt++) {
		if (!imIt.value()->isPersistent()) {
			continue;
		}
		
		QJsonObject imageJson;
		imageJson.insert("DataID", imIt.value()->getDataID().toString());
		imageJson.insert("DataName", imIt.value()->getImageData()->getDataName());
		imageJson.insert("Path", imIt.value()->getDataPath());
	}
	config.insert("Images", QJsonArray::fromVariantList(images));

	QVariantList studies;
	studies.reserve(mRegisteredStudies.count());
	auto stIt = mRegisteredStudies.begin();
	for (; stIt != mRegisteredStudies.end(); stIt++) {
		if (!stIt.value()->isPersistent()) {
			continue;
		}

		StudyPtr study = stIt.value()->getStudy();

		QJsonObject studyJson;
		studyJson.insert("DataID", study->getDataID().toString());
		studyJson.insert("DataName", study->getDataName());
		studyJson.insert("PatientID", study->getPatientID());
		studyJson.insert("PatientName", study->getPatientName());
		studyJson.insert("StudyDate", study->getStudyDate());
		studyJson.insert("StudyTime", study->getStudyTime());
		studyJson.insert("SeriesCount", study->getSeriesCount());
		studyJson.insert("isDICOM", stIt.value()->isDICOMStudy());
		studyJson.insert("isFI3DFile", stIt.value()->isFI3DStudy());

		
		if (stIt.value()->isDICOMStudy()) {
			QJsonArray studyPaths;
			for (vtkStringArray* paths : stIt.value()->getDICOMPaths()) {
				QJsonArray seriesPaths;
				for (int i = 0; i < paths->GetNumberOfValues(); i++) {
					QString path(paths->GetValue(i));
					seriesPaths.push_back(path);
				}
				studyPaths.push_back(seriesPaths);
			}
			studyJson.insert("DICOMPaths", studyPaths);
		} else if (stIt.value()->isFI3DStudy()) {
			QString filePath = stIt.value()->getFI3DStudyPath();
			studyJson.insert("Path", filePath);
		} else {
			qWarning() << "Failed to save Study:" << study->getDataID() << 
				"as persistent because no data path is present";
			continue;
		}
		
		studies.push_back(studyJson);
	}
	config.insert("Studies", QJsonArray::fromVariantList(studies));

	QVariantList models;
	models.reserve(mRegisteredModels.count());
	auto modIt = mRegisteredModels.begin();
	for (; modIt != mRegisteredModels.end(); modIt++) {
		if (!modIt.value()->isPersistent()) {
			continue;
		}

		QJsonObject model;
		model.insert("DataID", modIt.value()->getDataID().toString());
		model.insert("DataName", modIt.value()->getModelData()->getDataName());
		model.insert("Path", modIt.value()->getDataPath());
		models.push_back(model);
	}
	config.insert("Models", QJsonArray::fromVariantList(models));

	QJsonDocument doc(config);
	QString text = doc.toJson();
	Filer::saveTextFile(text, M_PERSISTENT_PATH);
}

void DataManager::updateDialogImageList() {
	if (!mGUI->isVisible()) {
		return;
	}

	mGUI->setImages(mRegisteredImages.values());
}

void DataManager::updateDialogStudyList() {
	if (!mGUI->isVisible()) {
		return;
	}

	mGUI->setStudies(mRegisteredStudies.values());
}

void DataManager::updateDialogModelList() {
	if (!mGUI->isVisible()) {
		return;
	}

	mGUI->setModels(mRegisteredModels.values());
}

QUuid DataManager::generateUniqueQUuID() {
	QUuid uID = QUuid::createUuid();
	while (mUsedQUuIDs.contains(uID)) {
		uID = QUuid::createUuid();
	}
	return uID;
}