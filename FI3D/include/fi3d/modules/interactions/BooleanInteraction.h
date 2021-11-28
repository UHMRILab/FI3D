#pragma once
/*!
* @author	VelazcoJD
* @file		ModuleInteraction.h
* @class	fi3d::ModuleInteraction
* @brief	Represents a boolean module interaction.
*/

#include <fi3d/modules/interactions/ModuleInteraction.h>

namespace fi3d {
class BooleanInteraction : public ModuleInteraction {

	Q_OBJECT

signals:
	/// @brief Emmitted when the interaction's value has changed.
	void changedValue(const bool& value);

public:
	/// @brief Constructor.
	BooleanInteraction(const QString& id, const bool& value = false, const QString& information = "");

	/// @brief Destructor.
	~BooleanInteraction();

	/// @brief Gets the interaction type.
	virtual fi3d::EModuleInteraction getInteractionType() const override;

	/// @brief Converts given value to boolean and assigns it.
	virtual void setValueAsJson(const QJsonValue& value) override;

	/// @brief Gets the intercaction value.
	virtual bool getValue() const;

public slots:
	/// @brief Sets the interaction value.
	virtual void setValue(const bool& value);
};

/// @brief Alias for a smart pointer of this class.
using BooleanInteractionPtr = QSharedPointer<BooleanInteraction>;

}