#include <fi3d/data/data_manager/gui/StudySelectorDialog.h>

#include "ui_StudySelectorDialog.h"

using namespace fi3d;

StudySelectorDialog::StudySelectorDialog(QWidget *parent) :
    QDialog(parent),
    mGUI(new Ui::StudySelectorDialog),
	mSelectedStudyID()
{
	mGUI->setupUi(this);
	
	QStringList studyListHeaderLabels = {
		"Data ID", "Study ID", "Patient ID", "Patient Name", "Date", "Time"
	};
	mGUI->studyList_table->setColumnCount(6);
	mGUI->studyList_table->setColumnHidden(0, true);
	mGUI->studyList_table->setHorizontalHeaderLabels(studyListHeaderLabels);
	mGUI->studyList_table->verticalHeader()->setVisible(false);

	QObject::connect(
		mGUI->studyList_table, &QTableWidget::cellDoubleClicked,
		this, &StudySelectorDialog::onCellDoubleClick);

	QObject::connect(
		mGUI->ok_button, &QPushButton::clicked,
		this, &StudySelectorDialog::onOKClick);

	QObject::connect(
		mGUI->cancel_button, &QPushButton::clicked,
		this, &StudySelectorDialog::onCancelClick);
}

StudySelectorDialog::~StudySelectorDialog() {}

void StudySelectorDialog::setStudies(const QList<RegisteredStudyPtr>& studies) 
{
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
}

DataID StudySelectorDialog::getSelectedStudy() {
	return mSelectedStudyID;
}

void StudySelectorDialog::onCellDoubleClick(const int& row, 
	const int& column) 
{
	QTableWidgetItem* dataID = mGUI->studyList_table->item(row, 0);
	QTableWidgetItem* studyID = mGUI->studyList_table->item(row, 1);
	mSelectedStudyID = DataID(dataID->text(), studyID->text());
	this->close();
}

void StudySelectorDialog::onOKClick() {
	int currentRow = mGUI->studyList_table->currentRow();
	if (currentRow != -1) {
		QTableWidgetItem* dataID = mGUI->studyList_table->item(currentRow, 0);
		QTableWidgetItem* studyID = mGUI->studyList_table->item(currentRow, 1);
		mSelectedStudyID = DataID(dataID->text(), studyID->text());
	}
	this->close();
}

void StudySelectorDialog::onCancelClick() {
	mSelectedStudyID = DataID();
	this->close();
}
