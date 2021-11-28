#include <fi3d/data/Filer.h>

#include <fi3d/logger/Logger.h>
#include <fi3d/data/EData.h>
#include <fi3d/FI3D/FI3D.h>

#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>

#include <vtkDICOMDirectory.h>
#include <vtkDICOMItem.h>
#include <vtkDICOMParser.h>
#include <vtkDICOMReader.h>

#include <vtkJPEGReader.h>
#include <vtkPNGReader.h>
#include <vtkSTLReader.h>
#include <vtkOBJReader.h>
#include <vtkPLYReader.h>

#include <vtkPNGWriter.h>
#include <vtkJPEGWriter.h>
#include <vtkSTLWriter.h>
#include <vtkPLYWriter.h>

#include <vtkImageMapToColors.h>
#include <vtkLookupTable.h>

using namespace fi3d;

bool Filer::checkAndCreateDirectory(const QString &dirPath) {
    if (!QDir(dirPath).exists()) {
        return QDir().mkdir(dirPath);
    } else {
        return true;
    }
}

QString Filer::readTextFile(const QString& filePath) {
	qDebug() << "Enter - Reading text file:" << filePath;
	QString contents;
	QFile textFile(filePath);
	if (!textFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		qWarning() << "Failed to read text file:" << filePath << "because:" << textFile.errorString();
		return contents;
	}

	contents = textFile.readAll();
	textFile.close();

	qDebug() << "Exit - Read" << contents.count() << "characters";
	return contents;
}

QJsonObject Filer::readJSONFile(const QString& filePath) {
	QString text = Filer::readTextFile(filePath);

	QJsonParseError errorMessage;
	QJsonObject jsonObject = QJsonDocument::fromJson(text.toUtf8(), &errorMessage).object();
	if (errorMessage.error != QJsonParseError::NoError) {
		qInfo() << "Failed to read josn file:" << filePath << "because:" << errorMessage.errorString();
		return QJsonObject();
	}

	return jsonObject;
}

void Filer::readCSVFile(const QString& filePath, QVector<QVector<QString>>& outEntries, const bool& keepHeader) {
	qDebug() << "Enter - Reading CSV with header:" << (keepHeader ? "True" : "False") << "and path:" << filePath;
	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		qWarning() << "Failed to open CSV file";
		return;
	}

	// remove the headers
	if (!keepHeader) {
		file.readLine();
	}

	// read the CSV lines
	int lineNumber = 0;
	while (!file.atEnd()) {
		QByteArray line = file.readLine();
		if (line.size() == 0) {
			continue;
		}
		QString element("");
		QVector<QString> entry;
		outEntries.push_back(entry);
		for (int i = 0; i < line.size(); i++) {
			if (line[i] == ',') {
				outEntries[lineNumber].push_back(element);
				element = "";
				continue;
			}
			element += line[i];
		}
		outEntries[lineNumber++].push_back(element);
	}

	file.close();
	qDebug() << "Exit - Got" << outEntries.size() << "entries";
}

void Filer::readImageData(const QString& filePath, ImageDataVPtr outData) {
	EFileExtension ext = Filer::getFileExtension(filePath);
	if (ext == EFileExtension::JPEG || ext == EFileExtension::JPG) {
		Filer::readImageDataFromJPEG(filePath, outData);
	} else if (ext == EFileExtension::PNG) {
		Filer::readImageDataFromPNG(filePath, outData);
	}
}

ImageDataVPtr Filer::readImageData(const QString& filePath) {
	ImageDataVPtr data = ImageDataVPtr::New();
	Filer::readImageData(filePath, data);
	return data;
}

void Filer::readImageDataFromJPEG(const QString& filePath, ImageDataVPtr outData) {
	qDebug() << "Enter - Reading JPEG data from:" << filePath;
	if (outData.Get() == Q_NULLPTR) {
		qWarning() << "Out data object cannot be null";
		return;
	}
	vtkNew<vtkJPEGReader> reader;
	reader->SetFileName(filePath.toStdString().c_str());
	reader->SetOutput(outData);
	reader->Update();

	int* dims = outData->GetDimensions();
	qDebug() << "Read image with dimensions:" << dims[0] << "," << dims[1] << "," << dims[2];

	qDebug() << "Exit";
}

ImageDataVPtr Filer::readImageDataFromJPEG(const QString& filePath) {
	ImageDataVPtr data = ImageDataVPtr::New();
	Filer::readImageDataFromJPEG(filePath, data);
	return data;
}

void Filer::readImageDataFromPNG(const QString& filePath, ImageDataVPtr outData) {
	qDebug() << "Enter - Reading PNG data from: " << filePath;
	if (outData.Get() == Q_NULLPTR) {
		qWarning() << "Out data object cannot be null";
		return;
	}
	vtkNew<vtkPNGReader> reader;
	reader->SetFileName(filePath.toStdString().c_str());
	reader->SetOutput(outData);
	reader->Update();
	qDebug() << "Exit";
}

ImageDataVPtr Filer::readImageDataFromPNG(const QString& filePath) {
	ImageDataVPtr data = ImageDataVPtr::New();
	Filer::readImageDataFromPNG(filePath, data);
	return data;
}

void Filer::readModelData(const QString& filePath, vtkSmartPointer<ModelData> outData) {
	EFileExtension ext = Filer::getFileExtension(filePath);
	if (ext == EFileExtension::STL) {
		Filer::readModelDataFromSTL(filePath, outData);
	} else if (ext == EFileExtension::OBJ) {
		Filer::readModelDataFromOBJ(filePath, outData);
	} else if (ext == EFileExtension::PLY) {
		//TODO:
	}
}

vtkSmartPointer<ModelData> Filer::readModelData(const QString& filePath) {
	vtkSmartPointer<ModelData> modelData = vtkSmartPointer<ModelData>::New();
	Filer::readModelData(filePath, modelData);
	return modelData;
}

void Filer::readModelDataFromSTL(const QString& filePath, vtkSmartPointer<ModelData> outData) {
	qDebug() << "Enter - Reading STL data from: " << filePath;
	if (outData.Get() == Q_NULLPTR) {
		qWarning() << "Out data object cannot be null";
		return;
	}
	vtkNew<vtkSTLReader> reader;
	reader->SetFileName(filePath.toStdString().c_str());
	reader->SetOutput(outData);
	reader->Update();
	qDebug() << "Exit - Got ModelData with " << outData->GetNumberOfPoints() << " points";
}

vtkSmartPointer<ModelData> Filer::readModelDataFromSTL(const QString& filePath) {
	vtkSmartPointer<ModelData> modelData = vtkSmartPointer<ModelData>::New();
	Filer::readModelDataFromSTL(filePath, modelData);
	return modelData;
}

void Filer::readModelDataFromOBJ(const QString& filePath, vtkSmartPointer<ModelData> outData) {
	qDebug() << "Enter - Reading STL data from: " << filePath;
	if (outData.Get() == Q_NULLPTR) {
		qWarning() << "Out data object cannot be null";
		return;
	}
	vtkNew<vtkOBJReader> reader;
	reader->SetFileName(filePath.toStdString().c_str());
	reader->SetOutput(outData);
	reader->Update();
	qDebug() << "Exit - Got ModelData with " << outData->GetNumberOfPoints() << " points";
}

vtkSmartPointer<ModelData> Filer::readModelDataFromOBJ(const QString& filePath) {
	vtkSmartPointer<ModelData> modelData = vtkSmartPointer<ModelData>::New();
	Filer::readModelDataFromOBJ(filePath, modelData);
	return modelData;
}

vtkSmartPointer<SeriesData> Filer::readSeriesData(vtkSmartPointer<vtkStringArray> dicomFiles) {
	vtkSmartPointer<SeriesData> image = vtkSmartPointer<SeriesData>::New();

	vtkNew<vtkLookupTable> bwLut;
	bwLut->SetTableRange(0, 1300);
	bwLut->SetSaturationRange(0, 0);
	bwLut->SetHueRange(0, 0);
	bwLut->SetValueRange(0, 1);
	// Setting ramp to linear was suggested by Gobbi, creator of vtkDICOM.
	bwLut->SetRampToLinear();
	bwLut->Build();

	vtkSmartPointer<SeriesData> readData = vtkSmartPointer<SeriesData>::New();
	vtkSmartPointer<vtkDICOMReader> reader = vtkSmartPointer<vtkDICOMReader>::New();
	reader->SetFileNames(dicomFiles);
	reader->SetOutput(readData);
	reader->Update();

	if (reader->GetFileIndexArray()->GetNumberOfComponents() != 1) {
		reader->SortingOff();
		reader->Update();
	}

	vtkNew<vtkImageMapToColors> readerColors;
	readerColors->SetInputData(reader->GetOutput());
	readerColors->SetLookupTable(bwLut);
	readerColors->SetOutput(image);
	readerColors->Update();

	image->setPatientMatrix(reader->GetPatientMatrix());
	image->setMetaData(reader->GetMetaData());

	return image;
}

QList<StudyPtr> Filer::findStudies(const QString& directoryPath, 
	QVector<QVector<vtkSmartPointer<vtkStringArray>>>& outStudiesPaths)
{
	qDebug() << "Enter - Searching studies from directory: " << directoryPath;
	QList<StudyPtr> studies;

	QDir currentDir(directoryPath);
	QStringList dirs = currentDir.entryList(QDir::AllDirs | QDir::NoSymLinks);
	for (QString dir : dirs) {
		if (dir == "." || dir == "..") {
			continue;
		}

		QVector<QVector<vtkSmartPointer<vtkStringArray>>> studyPaths;
		QString dirPath = QObject::tr("%1/%2").arg(directoryPath).arg(dir);
		QList<StudyPtr> foundStudies = Filer::findStudies(dirPath, studyPaths);

		outStudiesPaths.append(studyPaths);
		studies.append(foundStudies);
	}

	vtkNew<vtkDICOMDirectory> dicomFinder;
	dicomFinder->SetDirectoryName(directoryPath.toStdString().c_str());
	// Only looks for studies that have pixel data
	dicomFinder->RequirePixelDataOn();		
	dicomFinder->SetScanDepth(1);
	dicomFinder->FollowSymlinksOff();
	dicomFinder->Update();

	int studyCount = dicomFinder->GetNumberOfStudies();
	if (studyCount == 0) {
		qDebug() << "Exit - no studies found.";
		return studies;
	}

	QString studyID;
	if (directoryPath.contains('/')) {
		studyID = directoryPath.section('/', -1);
	} else {
		studyID = directoryPath;
	}

	for (int i = 0; i < studyCount; i++) {
		StudyPtr study(new Study());

		vtkDICOMItem patientMD = dicomFinder->GetPatientRecordForStudy(i);
		vtkDICOMItem studyMD = dicomFinder->GetStudyRecord(i);
		study->setPatientName(QString::fromStdString(patientMD.Get(DC::PatientName).AsString()));
		study->setPatientID(QString::fromStdString(patientMD.Get(DC::PatientID).AsString()));
		study->setStudyDate(QString::fromStdString(studyMD.Get(DC::StudyDate).AsString()));
		study->setStudyTime(QString::fromStdString(studyMD.Get(DC::StudyTime).AsString()));

		QString studyIDMD = QString::fromStdString(studyMD.Get(DC::StudyID).AsString());

		if (studyIDMD.isEmpty()) {
			if (studyCount == 1) {
				studyIDMD = studyID;
			} else {
				studyIDMD = QObject::tr("%1_%2").arg(studyID).arg(i);
			}
		}

		study->setStudyID(studyIDMD);

		// Get the start and end indices for the series of the study
		int startIndex = dicomFinder->GetFirstSeriesForStudy(i);
		int endIndex = dicomFinder->GetLastSeriesForStudy(i);

		QVector<vtkSmartPointer<vtkStringArray>> dicomFiles;
		for (int j = startIndex; j <= endIndex; j++) {
			vtkSmartPointer<vtkStringArray> foundFiles = dicomFinder->GetFileNamesForSeries(j);
			dicomFiles.push_back(foundFiles);
		}

		outStudiesPaths.append(dicomFiles);
		studies.push_back(study);
	}

	qDebug() << "Exit - found" << studyCount << "studies";
	return studies;	
}

void Filer::readStudyDataFromDICOMs(QVector<vtkSmartPointer<vtkStringArray>> paths, StudyPtr study) {
	for (vtkSmartPointer<vtkStringArray> seriesPaths : paths) {
		SeriesDataVPtr series = Filer::readSeriesData(seriesPaths);
		study->addSeries(series);
	}
}

void Filer::readStudyDataFromFI3DFile(const QString& path, StudyPtr study) {
	QString text = Filer::readTextFile(path);

	QJsonParseError errorMessage;
	QJsonObject studyText = QJsonDocument::fromJson(text.toUtf8(), &errorMessage).object();
	if (errorMessage.error != QJsonParseError::NoError) {
		qWarning() << "Failed to load study:" << errorMessage.errorString();
		return;
	}

    QJsonArray series = studyText.value("Series").toArray();
    Filer::convertSeriesDataFromJSON(series, study);
}

void Filer::readStudyMetaDataFromHicsFile(const QString& path, StudyPtr study) 
{
	QString text = Filer::readTextFile(path);

	QJsonParseError errorMessage;
	QJsonObject studyText = QJsonDocument::fromJson(text.toUtf8(), &errorMessage).object();
	if (errorMessage.error != QJsonParseError::NoError) {
		qWarning() << "Failed to load Study meta data:" << errorMessage.errorString();
		return;
	}

	study->setStudyID(studyText.value("StudyID").toString());
	study->setPatientID(studyText.value("PatientID").toString());
	study->setPatientName(studyText.value("PatientName").toString());
	study->setStudyDate(studyText.value("Date").toString());
	study->setStudyTime(studyText.value("Time").toString());
}

StudyPtr Filer::readStudyFromFI3DFile(const QString& filePath) {
	QString text = Filer::readTextFile(filePath);

	QJsonParseError errorMessage;
	QJsonObject studyText = QJsonDocument::fromJson(text.toUtf8(), &errorMessage).object();
	if (errorMessage.error != QJsonParseError::NoError) {
		qWarning() << "Failed to load study:" << errorMessage.errorString();
		return Q_NULLPTR;
	}

	StudyPtr study(new Study(studyText.value("StudyID").toString("")));
	study->setPatientID(studyText.value("PatientID").toString("UNKNOWN"));
	study->setPatientName(studyText.value("PatientName").toString("UNKNOWN"));
	study->setStudyDate(studyText.value("Date").toString("UNKNOWN"));
	study->setStudyTime(studyText.value("Time").toString("UNKNOWN"));
	// TODO: Have to put that this study was loaded from a FI3D file.

    QJsonArray series = studyText.value("Series").toArray();
    Filer::convertSeriesDataFromJSON(series, study);
	return study;
}

void Filer::saveTextFile(const QString& text, const QString& filePath) {
	qDebug() << "Enter - Saving text with" << text.count() << "characters to file:" << filePath;
	QFile textFile(filePath);
	if (!textFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
		qWarning() << "Failed to write text file:" << filePath << "because:" << textFile.errorString();
		return;
	}

	textFile.write(text.toLatin1());
	textFile.close();

	qDebug() << "Exit";
}

void Filer::saveJSONFile(const QJsonObject& jsonObject, const QString& filePath) {
	QJsonDocument doc(jsonObject);
	QString text = doc.toJson(QJsonDocument::JsonFormat::Compact);

	Filer::saveTextFile(text, filePath);
}

void Filer::saveImageDataAsJPG(ImageData* imageData, const QString& fileName) {
	qDebug() << "Enter";
	if (imageData == Q_NULLPTR || fileName.isEmpty()) {
		qDebug() << "Exit - ImageData is null or no file name was given.";
		return;
	}

	vtkNew<vtkJPEGWriter> writer;
	writer->SetInputData(imageData);
	writer->SetFileName(fileName.toStdString().c_str());
	writer->Write();

	qDebug() << "Exit";
}

void Filer::saveImageDataAsJPGs(ImageData* imageData, 
	const ESliceOrientation& sliceOrientation, const QString& fileName, 
	const QString& filePath) 
{
	qDebug() << "Enter";
	if (imageData == Q_NULLPTR || sliceOrientation.toInt() == ESliceOrientation::UNKNOWN) {
		return;
	}

	// Create the directoy if it doesn't exist
	QString directory = QObject::tr("%1/%2").arg(filePath).arg(fileName);
	if (!QDir(directory).exists()) {
		QDir().mkdir(directory);
	}

	// Save the corresponding slices to the directory.
	int* dims = imageData->GetDimensions();
	switch (sliceOrientation.toInt()) {
		case ESliceOrientation::XY:
			{
				for (int i = 0; i < dims[2]; i++) {
					ImageDataVPtr result = ImageDataVPtr::New();
					result->SetDimensions(dims[0], dims[1], 1);
					result->AllocateScalars(VTK_UNSIGNED_CHAR, 3);

					for (int x = 0; x < dims[0]; x++) {
						for (int y = 0; y < dims[1]; y++) {
							unsigned char value = static_cast<unsigned char*>(imageData->GetScalarPointer(x, y, i))[0];
							unsigned char* pixel = static_cast<unsigned char*>(result->GetScalarPointer(x, y, 0));
							pixel[0] = value;
							pixel[1] = value;
							pixel[2] = value;
						}
					}

					QString fileName = QObject::tr("%1/%2_%3.jpg").arg(directory).arg(fileName).arg(i);

					vtkNew<vtkJPEGWriter> writer;
					writer->SetInputData(result);
					writer->SetFileName(fileName.toStdString().c_str());
					writer->Write();
				}
			}
			break;
		case ESliceOrientation::YZ:
			{
				for (int i = 0; i < dims[0]; i++) {
					ImageDataVPtr result = ImageDataVPtr::New();
					result->SetDimensions(1, dims[1], dims[2]);
					result->AllocateScalars(VTK_UNSIGNED_CHAR, 3);

					for (int y = 0; y < dims[1]; y++) {
						for (int z = 0; z < dims[2]; z++) {
							unsigned char value = static_cast<unsigned char*>(imageData->GetScalarPointer(i, y, z))[0];
							unsigned char* pixel = static_cast<unsigned char*>(result->GetScalarPointer(0, y, z));
							pixel[0] = value;
							pixel[1] = value;
							pixel[2] = value;
						}
					}

					QString fileName = QObject::tr("%1/%2_%3.jpg").arg(directory).arg(fileName).arg(i);

					vtkNew<vtkJPEGWriter> writer;
					writer->SetInputData(result);
					writer->SetFileName(fileName.toStdString().c_str());
					writer->Write();
				}
			}
			break;
		case ESliceOrientation::XZ:
			{
				for (int i = 0; i < dims[1]; i++) {
					ImageDataVPtr result = ImageDataVPtr::New();
					result->SetDimensions(dims[0], 1, dims[2]);
					result->AllocateScalars(VTK_UNSIGNED_CHAR, 3);

					for (int x = 0; x < dims[0]; x++) {
						for (int z = 0; z < dims[2]; z++) {
							unsigned char value = static_cast<unsigned char*>(imageData->GetScalarPointer(x, i, z))[0];
							unsigned char* pixel = static_cast<unsigned char*>(result->GetScalarPointer(x, 0, z));
							pixel[0] = value;
							pixel[1] = value;
							pixel[2] = value;
						}
					}

					QString fileName = QObject::tr("%1/%2_%3.jpg").arg(directory).arg(fileName).arg(i);

					vtkNew<vtkJPEGWriter> writer;
					writer->SetInputData(result);
					writer->SetFileName(fileName.toStdString().c_str());
					writer->Write();
				}
			}
			break;
	}
	qDebug() << "Exit";
}

void Filer::saveModelDataToSTL(ModelDataVPtr data, const QString& filePath) {
	qDebug() << "Enter - Saving ModelData to STL to file: " << filePath;
	vtkNew<vtkSTLWriter> writer;
	QString fileName;
	if (filePath.section('.', -1).toLower() == "stl") {
		fileName = filePath;
	} else {
		fileName = filePath + ".stl";
	}
	writer->SetInputData(data);
	writer->SetFileName(fileName.toStdString().c_str());
	writer->Write();
	qDebug() << "Exit";
}

void Filer::saveStudyAsFI3DFile(const StudyPtr study, const QString& filePath) {
	if (study.isNull()) {
		return;
	}

	QVariantList seriesList;
	seriesList.reserve(study->getSeriesCount());
	for (int i = 0; i < study->getSeriesCount(); i++) {
		vtkSmartPointer<SeriesData> series = study->getSeries(i);
		int* dims = series->GetDimensions();
		double* spac = series->GetSpacing();
		vtkSmartPointer<vtkMatrix4x4> patientMatrix = series->getPatientMatrix();

		QJsonArray dimensions;
		dimensions.append(dims[0]);
		dimensions.append(dims[1]);
		dimensions.append(dims[2]);

		QJsonArray spacing;
		spacing.append(spac[0]);
		spacing.append(spac[1]);
		spacing.append(spac[2]);

		QJsonArray patientMatrixText;
		for (int i = 0; i < 16; i++) {
			patientMatrixText.append(patientMatrix->GetData()[i]);
		}

		QVariantList values;
		values.reserve(dims[0] * dims[1] * dims[2]);
		for (int z = 0; z < dims[2]; z++) {
			for (int y = 0; y < dims[1]; y++) {
				for (int x = 0; x < dims[0]; x++) {
					double val = series->GetScalarComponentAsDouble(x, y, z, 0);
					values.append(val);
				}
			}
		}

		QJsonObject seriesText;
		seriesText.insert("Dimensions", dimensions);
		seriesText.insert("Spacing", spacing);
		seriesText.insert("PatientMatrix", patientMatrixText);
		seriesText.insert("IntensityDataType", series->GetScalarType());
		seriesText.insert("Values", QJsonArray::fromVariantList(values));

		seriesList.append(seriesText);
	}

	QJsonObject studyText;
	studyText.insert("FileType", 1);		// TODO: Enumerate this
	studyText.insert("DataType", EData::STUDY);
	studyText.insert("StudyID", study->getStudyID());
	studyText.insert("PatientID", study->getPatientID());
	studyText.insert("PatientName", study->getPatientName());
	studyText.insert("Date", study->getStudyDate());
	studyText.insert("Time", study->getStudyTime());
	studyText.insert("SeriesCount", study->getSeriesCount());
	studyText.insert("Series", QJsonArray::fromVariantList(seriesList));

	Filer::saveJSONFile(studyText, QObject::tr("%1/%2.FI3D").arg(filePath).arg(study->getStudyID()));
}

EFileExtension Filer::getFileExtension(const QString& filePath) {
	QString fileName = "";
	if (filePath.contains('/')) {
		fileName = filePath.section('/', -1);
	} else {
		fileName = filePath;
	}

	QString ext = "";
	if (fileName.contains('.')) {
		ext = fileName.toLower().section('.', -1);
	}

	return EFileExtension(ext);
}

void Filer::convertSeriesDataFromJSON(QJsonArray& seriesJSONArray,
	StudyPtr study) 
{
	for (int i = 0; i < seriesJSONArray.count(); i++) {
		QJsonObject seriesJSON = seriesJSONArray[i].toObject();
		QJsonArray dimsJSON = seriesJSON.value("Dimensions").toArray();
		QJsonArray spacJSON = seriesJSON.value("Spacing").toArray();
		QJsonArray patientMatrix = seriesJSON.value("PatientMatrix").toArray();
		int intensityDataType = seriesJSON.value("IntensityDataType").toInt();

		vtkSmartPointer<SeriesData> series = study->createAndAddSeries();
		series->SetDimensions(dimsJSON[0].toInt(), dimsJSON[1].toInt(), dimsJSON[2].toInt());
		series->SetSpacing(spacJSON[0].toDouble(), spacJSON[1].toDouble(), spacJSON[2].toDouble());
		series->AllocateScalars(intensityDataType, 1);

		int* dims = series->GetDimensions();

		QJsonArray values = seriesJSON.value("Values").toArray();
		int xySize = dims[0] * dims[1];
		for (int i = 0; i < values.count(); i++) {
			int z = i / (xySize);
			int y = (i - (xySize * z)) / dims[0];
			int x = i % dims[0];

			double val = values[i].toDouble();
			series->SetScalarComponentFromDouble(x, y, z, 0, val);
		}

		vtkSmartPointer<vtkMatrix4x4> pMatrix = vtkSmartPointer<vtkMatrix4x4>::New();
		for (int i = 0; i < 16; i++) {
			pMatrix->GetData()[i] = patientMatrix[i].toDouble();
		}
		series->setPatientMatrix(pMatrix);
	}
}

QString Filer::getDirectoryPathDialog(const QString& path) {
	QString dirPath = QFileDialog::getExistingDirectory(
		Q_NULLPTR,
		QObject::tr("Select Directory"), 
		path,
		QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	
	return dirPath;
}

QString Filer::getFilePathDialog(const QString& extension, const QString& path) {
	return QFileDialog::getOpenFileName(
		Q_NULLPTR, 
		QObject::tr("Select %1 File").arg(extension.toUpper()), 
		path, 
		QObject::tr("Files (*.%1)").arg(extension));
}

QString Filer::getFilveSavePathDialog(const QString& extension, const QString& path) {
	return QFileDialog::getSaveFileName(
		Q_NULLPTR, 
		"Save as...", 
		path, 
		QObject::tr("Files (*.%1)").arg(extension));
}
