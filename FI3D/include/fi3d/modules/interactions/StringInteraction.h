#pragma once
/*!
* @author	VelazcoJD
* @file		StringInteraction.h
* @class	fi3d::StringInteraction
* @brief	Represents a string module interaction.
*/

#include <fi3d/modules/interactions/ModuleInteraction.h>

#include <fi3d/server/message_keys/EModuleInteractionConstraint.h>

#include <QSharedPointer>
#include <QVector>

namespace fi3d {
class StringInteraction : public ModuleInteraction {

	Q_OBJECT

signals:
	/// @brief Emmitted when the interaction's value has changed.
	void changedValue(const QString& value);

protected:
	/// @brief The interaction's constraint
	fi3d::EModuleInteractionConstraint mConstraint;

public:
	/// @brief Constructor for an interaction with a NONE/UNKNOWN constraint.
	StringInteraction(const QString& id, const QString& value = "", const QString& information = "");

	/// @brief Destructor.
	~StringInteraction();

	/// @brief Gets the interaction type.
	virtual fi3d::EModuleInteraction getInteractionType() const override;

	/// @brief Gets the interaction constraint.
	virtual	fi3d::EModuleInteractionConstraint getConstraintType() const override;

	/// @brief Converts given value to a string and assisngs it.
	virtual void setValueAsJson(const QJsonValue& value) override;

	/// @brief Gets the intercaction value.
	virtual QString getValue() const;

public slots:
	/// @brief Sets the interaction value.
	virtual void setValue(const QString& value);
};

/// @brief Alias for a smart pointer of this class.
using StringInteractionPtr = QSharedPointer<StringInteraction>;

}