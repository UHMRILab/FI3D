#pragma once
/*!
* @author	VelazcoJD
* @file		ModuleInteraction.h
* @class	fi3d::ValuelessInteraction
* @brief	Represents a valueless module interaction.
*/

#include <fi3d/modules/interactions/ModuleInteraction.h>

namespace fi3d {
class ValuelessInteraction : public ModuleInteraction {

	Q_OBJECT

signals:
	/// @brief Emmitted when the interaction is triggered.
	void triggered();

public:
	/// @brief Constructor.
	ValuelessInteraction(const QString& id,	const QString& information = "");

	/// @brief Destructor.
	~ValuelessInteraction();

	/// @brief Gets the interaction type.
	virtual fi3d::EModuleInteraction getInteractionType() const override;

	/// @brief Triggers the interaction, independently of the given value.
	virtual void setValueAsJson(const QJsonValue& value) override;

public slots:
	/// @brief Triggers the valueless interaction.
	virtual void triggerInteraction();
};

/// @brief Alias for a smart pointer of this class.
using ValuelessInteractionPtr = QSharedPointer<ValuelessInteraction>;

}