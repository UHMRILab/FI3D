#pragma once
/*!
* @author	VelazcoJD
* @file		StudySelectorDialog.h
* @class	fi3d::StudySelectorDialog
* @brief	Interface used to present list of studies for selection.
*/

#include <QDialog>
#include <QList>

#include <fi3d/data/DataID.h>
#include <fi3d/data/data_manager/registered_data/RegisteredStudy.h>

namespace Ui {
class StudySelectorDialog;
}

namespace fi3d {
class StudySelectorDialog : public QDialog {

	Q_OBJECT

private:
	/// @brief The GUI elements.
	QSharedPointer<Ui::StudySelectorDialog> mGUI;

	/// @brief The list of studies being shown.
	QList<DataID> mStudyList;

	/// @brief The selected study, empty string if none selected.
	DataID mSelectedStudyID;

public:
	/// @brief Constructor.
	StudySelectorDialog(QWidget *parent = nullptr);

	/// @brief Destructor.
	~StudySelectorDialog();

	/// @brief Sets the studies available.
	void setStudies(const QList<RegisteredStudyPtr>& studies);

	/// @brief Gets the selected study, empty string if no study selected.
	DataID getSelectedStudy();

private slots:
	/// @brief Handles double clicking a cell in the table.
	void onCellDoubleClick(const int& row, const int& column);

	/// @brief Handles clicking the OK button
	void onOKClick();

	/// @brief Handles clicking the cancel button
	void onCancelClick();
};
}