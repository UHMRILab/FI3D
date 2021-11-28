#pragma once
/*!
* @author	VelazcoJD
* @file		IntegerInteraction.h
* @class	fi3d::IntegerInteraction
* @brief	Represents a integer module interaction.
*/

#include <fi3d/modules/interactions/ModuleInteraction.h>

#include <fi3d/server/message_keys/EModuleInteractionConstraint.h>

#include <QSharedPointer>

namespace fi3d {
class IntegerInteraction : public ModuleInteraction {

	Q_OBJECT

signals:
	/// @brief Emmitted when the interaction's value has changed.
	void changedValue(const int& value);

	/// @brief Emmitted when the interaction's minimum constraint changed.
	void changedMinimum(const int& min);

	/// @brief Emitted when the interaction's maximum constraint changed.
	void changedMaximum(const int& max);

	/// @brief Emitted when the interaction's range constraint changed.
	void changedRange(const int& min, const int& max);

protected:
	/// @brief The interaction's constraint
	fi3d::EModuleInteractionConstraint mConstraint;

	/// @brief The minimum value allowed, when MIN or RANGE constraints.
	int mMin;

	/// @brief The maximum value allowed, when MAX or RANGE constraints.
	int mMax;

public:
	/// @brief Constructor for an interaction with a NONE/UNKNOWN constraint.
	IntegerInteraction(const QString& id, const int& value, const QString& information = "");

	/// @brief Destructor.
	~IntegerInteraction();

	/// @brief Gets the interaction type.
	virtual fi3d::EModuleInteraction getInteractionType() const override;

	/// @brief Gets the interaction constraint.
	virtual	fi3d::EModuleInteractionConstraint getConstraintType() const override;

	/// @brief Converts given value to an integer and assigns it.
	virtual void setValueAsJson(const QJsonValue& value) override;

	/// @brief Gets the intercaction value.
	virtual int getValue() const;

public slots:
	/// @brief Sets the interaction value.
	virtual void setValue(const int& value);

public:
	/// @brief Sets the min, if interaction is MIN constrained.
	virtual void setMin(const int& min);

	/// @brief Sets the max, if interaction is MAX constrained.
	virtual void setMax(const int& max);
	
	/// @brief Sets the min and max, if interaction is RANGE constrained.
	virtual void setRange(const int& min, const int& max);

	/// @brief Gets the min, if interaction is MIN or RANGE constrained.
	virtual int getMin() const;

	/// @brief Gets the max, if interaction is MAX or RANGE constrained.
	virtual int getMax() const;

	static QSharedPointer<IntegerInteraction> createMinInteraction(
		const QString& id, const int& value, const int& min, 
		const QString& information = "");

	static QSharedPointer<IntegerInteraction> createMaxInteraction(
		const QString& id, const int& value, const int& max,
		const QString& information = "");

	static QSharedPointer<IntegerInteraction> createRangeInteraction(
		const QString& id, const int& value, const int& min, const int& max,
		const QString& information = "");
};

/// @brief Alias for a smart pointer of this class.
using IntegerInteractionPtr = QSharedPointer<IntegerInteraction>;

}