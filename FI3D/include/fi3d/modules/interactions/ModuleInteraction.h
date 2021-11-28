#pragma once
/*!
* @author	VelazcoJD
* @file		ModuleInteraction.h
* @class	fi3d::ModuleInteraction
* @brief	Represents a single module interaction.
*
* By default, all module interactions are sent to framework interfaces. If one
* wishes to excluse an interaction being sent, this can be disabled by using
* the function disableInFrameworkInterfaces after creating the interaction.
*/

#include <fi3d/server/message_keys/EModuleInteraction.h>
#include <fi3d/server/message_keys/EModuleInteractionConstraint.h>

#include <QJsonValue>
#include <QObject>
#include <QString>

namespace fi3d {
class ModuleInteraction : public QObject {

	Q_OBJECT

signals:
	/// @brief Emmitted when the interaction's value has changed.
	void changedInteractionValue(const QJsonValue& newValue);

	/// @brief Emitted when the interaction's constraint has changed.
	void changedInteractionConstraint();

	/// @brief Emitted when the interaction's information has changed.
	void changedInformation(const QString& newInfo);

private:
	/// @brief The interaction ID. Used to identify the interaction.
	QString mID;

	/// @brief The current value of the interaction.
	QJsonValue mValue;

	/// @brief Info about what the interaction does.
	QString mInfo;

	/// @brief Whether the interaciton is enabled to be used by a framework
	/// interface.
	bool mIsEnabledForFrameworkInterface;

public:
	/// @brief Constructor.
	ModuleInteraction(const QString& id, const QJsonValue& value = QJsonValue::Null, const QString& information = "");

	/// @brief Destructor.
	~ModuleInteraction();

	/// @brief Gets the ModuleInteraction ID.
	QString getInteractionID() const;

	/// @brief Gets the interaction type.
	virtual EModuleInteraction getInteractionType() const = 0;

	/// @brief Gets the interaction constraint.
	virtual	EModuleInteractionConstraint getConstraintType() const;

protected:
	/// @brief Sets the interaction's value, as a generic Json value.
	virtual void setJsonValue(const QJsonValue& value);

public slots:
	/// @brief Sets the value as Json, to be handled by derived classes.
	virtual void setValueAsJson(const QJsonValue& value) = 0;

	/// @brief Sets the interaction's information.
	virtual void setInformation(const QString& information);

public:
	/// @brief Gets the interaction's value in its Json format.
	virtual QJsonValue getValueAsJson() const;

	/// @brief Gets the interaction's information.
	virtual QString getInformation() const;

	/// @brief Whether the interaction is enabled for framework interfaces.
	bool isEnabledForFrameworkInterfaces() const;

	/// @brief Makes the interaction not accessable by any framework interface.
	void disableForFrameworkInterfaces();
};

/// @brief Alias for a smart pointer of this class.
using ModuleInteractionPtr = QSharedPointer<ModuleInteraction>;

}