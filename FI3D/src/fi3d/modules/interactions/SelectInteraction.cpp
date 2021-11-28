#include <fi3d/modules/interactions/SelectInteraction.h>

#include <fi3d/logger/Logger.h>

using namespace fi3d;

SelectInteraction::SelectInteraction(const QString& id, 
	const QVector<QString>& options,
	const QString& information)
	: ModuleInteraction(id, -1, information),
	mConstraint(EModuleInteractionConstraint::UNKNOWN),
	mOptions(options)
{
	if (mOptions.count() > 0) {
		this->setValue(0);
	}
}

SelectInteraction::~SelectInteraction() {}

EModuleInteraction SelectInteraction::getInteractionType() const {
	return EModuleInteraction::SELECT;
}

EModuleInteractionConstraint SelectInteraction::getConstraintType() const {
	return mConstraint;
}

void SelectInteraction::setValueAsJson(const QJsonValue& value) {
	this->setValue(value.toInt(0));
}

int SelectInteraction::getValue() const {
	return this->getValueAsJson().toInt();
}

QString SelectInteraction::getValueOption() const {
	int index = this->getValue();
	if (index < 0 || index >= mOptions.count()) {
		return "";
	} else {
		return mOptions.at(index);
	}
}

void SelectInteraction::setValue(const int& value) {
	if (this->getValue() == value) {
		return;
	}

	int newValue = 0;
	if (mOptions.count() == 0) {
		newValue = -1;
	} else if (value < 0 || value >= mOptions.count()) {
		newValue = 0;
	} else {
		newValue = value;
	}

	this->setJsonValue(newValue);
	emit changedValue(newValue);
	emit changedValueOption(mOptions.at(newValue));
}

void SelectInteraction::setValueByOption(const QString& option) {
	if (mOptions.count() == 0) {
		return;
	}

	int index = mOptions.indexOf(option);

	this->setValue(index);
}

void SelectInteraction::setOptions(const QVector<QString>& options, const int& selection)
{
	mOptions.clear();

	if (options.count() == 0) {
		emit changedOptions(mOptions);
		this->setValue(-1);
	} else {
		for (QString option : options) {
			mOptions.append(option);
		}

		emit changedOptions(mOptions);
		this->setValue(selection);
	}
}

void SelectInteraction::appendOption(const QString& option, const bool& select) {
	qDebug() << "Enter";

	qDebug() << "Appending option" << option << "to SELECT interaction" << this->getInteractionID();

	mOptions.append(option);
	emit changedOptions(mOptions);

	if (select) {
		this->setValue(mOptions.count() - 1);
	}

	qDebug() << "Exit";
}

QVector<QString> SelectInteraction::getOptions() const {
	return mOptions;
}
