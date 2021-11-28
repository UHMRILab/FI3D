#include <fi3d/data/data_manager/gui/DataManagerDialog.h>

#include "ui_DataManagerDialog.h"

using namespace fi3d;

DataManagerDialog::DataManagerDialog()
	: QDialog(),
	mGUI(new Ui::DataManagerDialog),
	mStudyTableHeader(new QHeaderView(Qt::Orientation::Horizontal, this)),
	mModelTableHeader(new QHeaderView(Qt::Orientation::Horizontal, this))
{
	mGUI->setupUi(this);
	QItemSelectionModel model;

	QStringList studyListHeaderLabels = {
		"Data ID", "Study ID", "Patient ID", "Patient Name", "Date", "Time"
	};
	mGUI->studyList_table->setColumnCount(6);
	mGUI->studyList_table->setColumnHidden(0, true);
	mGUI->studyList_table->setHorizontalHeaderLabels(studyListHeaderLabels);

	QStringList modelListHeaderLabels = {"Data ID", "Model Name"};
	mGUI->modelList_table->setColumnCount(2);
	mGUI->modelList_table->setColumnHidden(0, true);
	mGUI->modelList_table->setHorizontalHeaderLabels(modelListHeaderLabels);

	QStringList imageListHeaderLabels = {"Data ID", "Image Name"};
	mGUI->imageList_table->setColumnCount(2);
	mGUI->imageList_table->setColumnHidden(0, true);
	mGUI->imageList_table->setHorizontalHeaderLabels(imageListHeaderLabels);

	QObject::connect(
		mGUI->loadDICOMStudy_button, &QPushButton::clicked,
		this, &DataManagerDialog::requestedLoadDICOMStudy);
	QObject::connect(
		mGUI->loadFI3DStudy_button, &QPushButton::clicked,
		this, &DataManagerDialog::requestedLoadFI3DStudy);
	QObject::connect(
		mGUI->loadImage_btn, &QPushButton::clicked,
		this, &DataManagerDialog::requestedLoadImage);
	QObject::connect(
		mGUI->loadModel_btn, &QPushButton::clicked,
		this, &DataManagerDialog::requestedLoadModel);
}

DataManagerDialog::~DataManagerDialog() {}

void DataManagerDialog::setImages(const QList<RegisteredImagePtr> images) {
	mGUI->imageList_table->setRowCount(images.count());

	for (int i = 0; i < images.count(); i++) {
		QString dataID = images[i]->getImageData()->getDataID().toString();
		QString dataName = images[i]->getImageData()->getDataName();

		QTableWidgetItem* dataIDItem = new QTableWidgetItem(dataID);
		QTableWidgetItem* dataNameItem = new QTableWidgetItem(dataName);

		mGUI->imageList_table->setItem(i, 0, dataIDItem);
		mGUI->imageList_table->setItem(i, 1, dataNameItem);
	}

	mGUI->imageList_table->sortByColumn(1, Qt::SortOrder::AscendingOrder);
	mGUI->imageList_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
}

void DataManagerDialog::setStudies(const QList<RegisteredStudyPtr> studies) {
	mGUI->studyList_table->setRowCount(studies.count());

	for (int i = 0; i < studies.count(); i++) {
		StudyPtr study = studies[i]->getStudy();

		QString dataID = study->getDataID().toString();
		QString studyID = study->getStudyID();
		QString patientID = study->getPatientID();
		QString patientName = study->getPatientName();
		QString studyDate = study->getStudyDate();
		QString studyTime = study->getStudyTime();

		QTableWidgetItem* dataIDItem = new QTableWidgetItem(dataID);
		QTableWidgetItem* studyIDItem = new QTableWidgetItem(studyID);
		QTableWidgetItem* patientIDItem = new QTableWidgetItem(patientID);
		QTableWidgetItem* patientNameItem = new QTableWidgetItem(patientName);
		QTableWidgetItem* studyDateItem = new QTableWidgetItem(studyDate);
		QTableWidgetItem* studyTimeItem = new QTableWidgetItem(studyTime);
		mGUI->studyList_table->setItem(i, 0, dataIDItem);
		mGUI->studyList_table->setItem(i, 1, studyIDItem);
		mGUI->studyList_table->setItem(i, 2, patientIDItem);
		mGUI->studyList_table->setItem(i, 3, patientNameItem);
		mGUI->studyList_table->setItem(i, 4, studyDateItem);
		mGUI->studyList_table->setItem(i, 5, studyTimeItem);
	}

	mGUI->studyList_table->sortByColumn(1, Qt::SortOrder::AscendingOrder);
	mGUI->studyList_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
	mGUI->studyList_table->resizeColumnsToContents();
}

void DataManagerDialog::setModels(const QList<RegisteredModelPtr> models) {
	mGUI->modelList_table->setRowCount(models.count());

	for (int i = 0; i < models.count(); i++) {
		QString dataID = models[i]->getModelData()->getDataID().toString();
		QString dataName = models[i]->getModelData()->getDataName();

		QTableWidgetItem* dataIDItem = new QTableWidgetItem(dataID);
		QTableWidgetItem* dataNameItem = new QTableWidgetItem(dataName);

		mGUI->modelList_table->setItem(i, 0, dataIDItem);
		mGUI->modelList_table->setItem(i, 1, dataNameItem);
	}

	mGUI->modelList_table->sortByColumn(1, Qt::SortOrder::AscendingOrder);
	mGUI->modelList_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
}