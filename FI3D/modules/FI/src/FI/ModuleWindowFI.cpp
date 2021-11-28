#include <FI/ModuleWindowFI.h>

#include <FI/GlobalsFI.h>

#include "ui_ModuleWindowFI.h"

#include <fi3d/utilities/Icons.h>

#include <QCheckBox>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>

using namespace fi;
using namespace fi3d;

/// <summary>
/// Helper function used to tell whether the interaction is belongs to
/// FI module or the subscribed module.
/// </summary>
bool isFIInteraction(const QString& interactionID) {
	bool isFI =
		interactionID == fi::MI_IP ||
		interactionID == fi::MI_PORT ||
		interactionID == fi::MI_PASSWORD ||
		interactionID == fi::MI_CONNECT ||
		interactionID == fi::MI_DISCONNECT ||
		interactionID == fi::MI_ACTIVATE_MODULE ||
		interactionID == fi::MI_STOP_MODULE ||
		interactionID == fi::MI_SUBSCRIBE_TO_MODULE ||
		interactionID == fi::MI_SETTINGS ||
		interactionID == fi::MI_SYNCHRONIZE ||
		interactionID == fi::MI_REFRESH_SCENE;

	return isFI;
}

ModuleWindowFI::ModuleWindowFI()
	: ModuleWindow(),
	mGUI(new Ui::ModuleWindowFI),
	mInteractionWidgets()
{
	mGUI->setupUi(this);

	QIcon settingsIcon = Icons::getIcon(fi3d::EIcon::SETTINGS);
	mGUI->settings_button->setIcon(settingsIcon);

	QIcon syncIcon = Icons::getIcon(fi3d::EIcon::SYNC);
	mGUI->sync_button->setIcon(syncIcon);

	QIcon refreshIcon = Icons::getIcon(fi3d::EIcon::REFRESH);
	mGUI->refresh_button->setIcon(refreshIcon);

	QObject::connect(
		mGUI->settings_button, &QToolButton::clicked,
		this, &ModuleWindowFI::onSettings);
	QObject::connect(
		mGUI->sync_button, &QToolButton::clicked,
		this, &ModuleWindowFI::onSync);
	QObject::connect(
		mGUI->refresh_button, &QToolButton::clicked,
		this, &ModuleWindowFI::onRefreshScene);
}

ModuleWindowFI::~ModuleWindowFI() {}

QVTKOpenGLWidget* ModuleWindowFI::getSceneWidget() {
	return mGUI->Frame3D;
}

void ModuleWindowFI::onSettings() {
	this->getValuelessModuleInteraction(MI_SETTINGS)->triggerInteraction();
}

void ModuleWindowFI::onSync() {
	bool isEnable = mGUI->sync_button->isChecked();
	this->getBooleanModuleInteraction(MI_SYNCHRONIZE)->setValue(isEnable);
}

void ModuleWindowFI::onRefreshScene() {
	this->getValuelessModuleInteraction(MI_REFRESH_SCENE)->triggerInteraction();
}

void ModuleWindowFI::onModuleInteractionAdded(const QString& interactionID) {
	// Skip interactions that are of the FI module.
	if (isFIInteraction(interactionID)) {
		return;
	}
	
	ModuleInteractionPtr interaction = this->getModuleInteraction(interactionID);

	// Get layout where interactions are added
	QVBoxLayout* intes = qobject_cast<QVBoxLayout*>(mGUI->moduleInteractions_group->layout());

	// Add module interaction based on its type and constraint
	QWidget* interactionWidget = Q_NULLPTR;
	QFont fontSize;
	fontSize.setPointSize(12);
	switch(interaction->getInteractionType().toInt()) {
	case EModuleInteraction::VALUELESS: 
		{
			ValuelessInteraction* vless = qobject_cast<ValuelessInteraction*>(interaction.data());
			if(vless == Q_NULLPTR) {
				QString feedback = tr("Failed to add %1 because its type does not match.").arg(interactionID);
				emit feedbackWarning(feedback);
				return;
			}

			QPushButton* button = new QPushButton(interactionID, mGUI->moduleInteractions_group);
			button->setObjectName(interactionID);
			intes->insertWidget(intes->count() - 1, button);
			button->setFont(fontSize);

			interactionWidget = button;

			QObject::connect(
				button, &QPushButton::clicked,
				vless, &ValuelessInteraction::triggerInteraction,
				Qt::UniqueConnection);
		}
		break;
	case EModuleInteraction::BOOL:
		{
			BooleanInteraction* boolInteraction = qobject_cast<BooleanInteraction*>(interaction.data());
			if(boolInteraction == Q_NULLPTR) {
				QString feedback = tr("Failed to add %1 because its type does not match.").arg(interactionID);
				emit feedbackWarning(feedback);
				return;
			}

			QCheckBox* checkBox = new QCheckBox(interactionID, mGUI->moduleInteractions_group);
			checkBox->setObjectName(interactionID);
			checkBox->setChecked(boolInteraction->getValue());
			intes->insertWidget(intes->count() - 1, checkBox);
			checkBox->setFont(fontSize);

			interactionWidget = checkBox;

			QObject::connect(
				checkBox, &QCheckBox::stateChanged,
				[boolInteraction, checkBox](int state) {
					boolInteraction->setValue(checkBox->isChecked());
			});
		}
		break;
	case EModuleInteraction::INTEGER:
		{
			IntegerInteraction* intInteraction = qobject_cast<IntegerInteraction*>(interaction.data());
			if(intInteraction == Q_NULLPTR) {
				QString feedback = tr("Failed to add %1 because its type does not match.").arg(interactionID);
				emit feedbackWarning(feedback);
				return;
			}

			QLabel* interactionLabel = new QLabel(interactionID, mGUI->moduleInteractions_group);
			intes->insertWidget(intes->count() - 1, interactionLabel);
			mInteractionLabels.insert(interactionID, interactionLabel);
			interactionLabel->setFont(fontSize);

			QSpinBox* spinBox = new QSpinBox(mGUI->moduleInteractions_group);
			spinBox->setObjectName(interactionID);
			spinBox->setFont(fontSize);
			
			if(intInteraction->getConstraintType() == EModuleInteractionConstraint::MIN) {
				spinBox->setRange(intInteraction->getMin(), INT_MAX);
			} else if(intInteraction->getConstraintType() == EModuleInteractionConstraint::MAX) {
				spinBox->setRange(INT_MIN, intInteraction->getMax());
			} else if(interaction->getConstraintType() == EModuleInteractionConstraint::RANGE) {
				spinBox->setRange(intInteraction->getMin(), intInteraction->getMax());
			} else {
				spinBox->setRange(INT_MIN, INT_MAX);
			}

			spinBox->setValue(intInteraction->getValue());
			intes->insertWidget(intes->count() - 1, spinBox);

			interactionWidget = spinBox;

			QObject::connect(
				spinBox, QOverload<int>::of(&QSpinBox::valueChanged),
				intInteraction, &IntegerInteraction::setValue,
				Qt::UniqueConnection);
		}
		break;
	case EModuleInteraction::FLOAT:
		{
			FloatInteraction* floatInteraction = qobject_cast<FloatInteraction*>(interaction.data());
			if(floatInteraction == Q_NULLPTR) {
				QString feedback = tr("Failed to add %1 because its type does not match.").arg(interactionID);
				emit feedbackWarning(feedback);
				return;
			}

			QLabel* interactionLabel = new QLabel(interactionID, mGUI->moduleInteractions_group);
			intes->insertWidget(intes->count() - 1, interactionLabel);
			mInteractionLabels.insert(interactionID, interactionLabel);
			interactionLabel->setFont(fontSize);

			QDoubleSpinBox* spinBox = new QDoubleSpinBox(mGUI->moduleInteractions_group);
			spinBox->setObjectName(interactionID);
			spinBox->setFont(fontSize);

			if(floatInteraction->getConstraintType() == EModuleInteractionConstraint::MIN) {
				spinBox->setRange(floatInteraction->getMin(), DBL_MAX);
			} else if(floatInteraction->getConstraintType() == EModuleInteractionConstraint::MAX) {
				spinBox->setRange(DBL_MIN, floatInteraction->getMax());
			} else if(floatInteraction->getConstraintType() == EModuleInteractionConstraint::RANGE) {
				spinBox->setRange(floatInteraction->getMin(), floatInteraction->getMax());
			} else {
				spinBox->setRange(DBL_MIN, DBL_MAX);
			}

			spinBox->setValue(floatInteraction->getValue());
			intes->insertWidget(intes->count() - 1, spinBox);

			interactionWidget = spinBox;

			QObject::connect(
				spinBox, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
				floatInteraction, &FloatInteraction::setValue,
				Qt::UniqueConnection);
		}
		break;
	case EModuleInteraction::STRING:
		{
			StringInteraction* strInteraction = qobject_cast<StringInteraction*>(interaction.data());
			if(strInteraction == Q_NULLPTR) {
				QString feedback = tr("Failed to add %1 because its type does not match.").arg(interactionID);
				emit feedbackWarning(feedback);
				return;
			}

			QLabel* interactionLabel = new QLabel(interactionID, mGUI->moduleInteractions_group);
			intes->insertWidget(intes->count() - 1, interactionLabel);
			mInteractionLabels.insert(interactionID, interactionLabel);
			interactionLabel->setFont(fontSize);
			
			QLineEdit* lineEdit = new QLineEdit(mGUI->moduleInteractions_group);
			lineEdit->setObjectName(interactionID);
			lineEdit->setText(strInteraction->getValue());
			intes->insertWidget(intes->count() - 1, lineEdit);
			lineEdit->setFont(fontSize);

			interactionWidget = lineEdit;

			QObject::connect(
				lineEdit, &QLineEdit::returnPressed,
				[lineEdit, strInteraction]() {
				strInteraction->setValue(lineEdit->text());
			});
		}
		break;
	case EModuleInteraction::SELECT:
		{
			SelectInteraction* selectInteraction = qobject_cast<SelectInteraction*>(interaction.data());
			if (selectInteraction == Q_NULLPTR) {
				QString feedback = tr("Failed to add %1 because its type does not match.").arg(interactionID);
				emit feedbackWarning(feedback);
				return;
			}

			QLabel* interactionLabel = new QLabel(interactionID, mGUI->moduleInteractions_group);
			intes->insertWidget(intes->count() - 1, interactionLabel);
			mInteractionLabels.insert(interactionID, interactionLabel);
			interactionLabel->setFont(fontSize);

			QComboBox* comboBox = new QComboBox(mGUI->moduleInteractions_group);
			comboBox->setObjectName(interactionID);
			comboBox->setFont(fontSize);

			QVector<QString> options = selectInteraction->getOptions();
			for (QString option:options) {
				comboBox->addItem(option);
			}

			comboBox->setCurrentIndex(selectInteraction->getValue());
			intes->insertWidget(intes->count() - 1, comboBox);

			interactionWidget = comboBox;

			/*QObject::connect(
				comboBox, &QComboBox::activated,
				[comboBox, selectInteraction]() {
				selectInteraction->setValue(comboBox->currentIndex());
			});*/
		}
		break;
	default:
		break;
	}

	// Add to managed list
	if (interactionWidget != Q_NULLPTR) {
		mInteractionWidgets.insert(interactionID, interactionWidget);
	}

	// Hide label that says there are no module interactions.
	if (mInteractionWidgets.count() == 1) {
		mGUI->noModuleInteractions_label->setVisible(false);
	}

	// Listen for changes to the interaction to update corresponding UI
	QObject::connect(
		interaction.data(), &ModuleInteraction::changedInteractionValue,
		this, &ModuleWindowFI::onModuleInteractionChange,
		Qt::UniqueConnection);
}

void ModuleWindowFI::onModuleInteractionChange(const QJsonValue& value) {
	ModuleInteraction* interaction = qobject_cast<ModuleInteraction*>(sender());

	if (interaction == Q_NULLPTR) {
		emit feedbackWarning("Failed to update interaction because interaction was not found.");
		return;
	}

	QString interactionID = interaction->getInteractionID();

	switch (interaction->getInteractionType().toInt()) {
	case EModuleInteraction::BOOL:
		{
			QCheckBox* checkBox = qobject_cast<QCheckBox*>(mInteractionWidgets.value(interactionID));
			if (checkBox == Q_NULLPTR) {
				emit feedbackWarning(tr("Failed to update %1 interaction. Its UI was not found.").arg(interactionID));
			} else {
				checkBox->setChecked(value.toBool());
			}
		}
		break;
	case EModuleInteraction::INTEGER:
		{
			QSpinBox* spinBox = qobject_cast<QSpinBox*>(mInteractionWidgets.value(interactionID));
			if(spinBox == Q_NULLPTR) {
				emit feedbackWarning(tr("Failed to update %1 interaction. Its UI was not found.").arg(interactionID));
			} else {
				spinBox->setValue(value.toInt());
			}
		}
		break;
	case EModuleInteraction::FLOAT:
		{
			QDoubleSpinBox* spinBox = qobject_cast<QDoubleSpinBox*>(mInteractionWidgets.value(interactionID));
			if(spinBox == Q_NULLPTR) {
				emit feedbackWarning(tr("Failed to update %1 interaction. Its UI was not found.").arg(interactionID));
			} else {
				spinBox->setValue(value.toDouble());
			}
		}
		break;
	case EModuleInteraction::STRING:
		{
			
			QLineEdit* lineEdit = qobject_cast<QLineEdit*>(mInteractionWidgets.value(interactionID));
			
			if (lineEdit == Q_NULLPTR) {
				QString feedback = tr("Failed to update %1 interaction. Its UI was not found.").arg(interactionID);
				emit feedbackWarning(feedback);
			} else {
				lineEdit->setText(value.toString());
			}
		}
		break;
	case EModuleInteraction::SELECT:
		{
			QComboBox* comboBox = qobject_cast<QComboBox*>(mInteractionWidgets.value(interactionID));

			if (comboBox == Q_NULLPTR) {
				QString feedback = tr("Failed to update %1 interaction. Its UI was not found.").arg(interactionID);
				emit feedbackWarning(feedback);
			} else {
				comboBox->setCurrentIndex(value.toInt());
			}
		}
		break;
	default:
		emit feedbackWarning("Failed to update interaction. Its type is unknown.");
		break;
	}
}

void ModuleWindowFI::onModuleInteractionRemoved(const QString& interactionID) {
	// TODO: This needs to be tested
	QWidget* interactionWidget = mInteractionWidgets.take(interactionID);
	if(interactionWidget != Q_NULLPTR) {
		delete interactionWidget;
	}

	QLabel* interactionLabel = mInteractionLabels.take(interactionID);
	if(interactionLabel != Q_NULLPTR) {
		delete interactionLabel;
	}

	if (mInteractionWidgets.count() == 0) {
		mGUI->noModuleInteractions_label->setVisible(false);
	}
}

void ModuleWindowFI::moduleInformationAssigned(ModuleInformationPtr moduleInfo) const {
	QObject::connect(
		this->getModuleInformationObject(), &ModuleInformation::changedAddedInteraction,
		this, &ModuleWindowFI::onModuleInteractionAdded,
		Qt::UniqueConnection);
	QObject::connect(
		this->getModuleInformationObject(), &ModuleInformation::changedRemovedInteraction,
		this, &ModuleWindowFI::onModuleInteractionRemoved,
		Qt::UniqueConnection);
}

