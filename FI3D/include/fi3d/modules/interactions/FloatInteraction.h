#pragma once
/*!
* @author	VelazcoJD
* @file		FloatInteraction.h
* @class	fi3d::FloatInteraction
* @brief	Represents a float module interaction.
*/

#include <fi3d/modules/interactions/ModuleInteraction.h>

#include <fi3d/server/message_keys/EModuleInteractionConstraint.h>

#include <QSharedPointer>

namespace fi3d {
class FloatInteraction : public ModuleInteraction {

	Q_OBJECT

signals:
	/// @brief Emmitted when the interaction's value has changed.
	void changedValue(const double& value);

	/// @brief Emmitted when the interaction's minimum constraint changed.
	void changedMinimum(const double& min);

	/// @brief Emitted when the interaction's maximum constraint changed.
	void changedMaximum(const double& max);

	/// @brief Emitted when the interaction's range constraint changed.
	void changedRange(const double& min, const double& max);

protected:
	/// @brief The interaction's constraint
	fi3d::EModuleInteractionConstraint mConstraint;

	/// @brief The minimum value allowed, when MIN or RANGE constraints.
	double mMin;

	/// @brief The maximum value allowed, when MAX or RANGE constraints.
	double mMax;

public:
	/// @brief Constructor for an interaction with a NONE/UNKNOWN constraint.
	FloatInteraction(const QString& id, const double& value, const QString& information = "");

	/// @brief Destructor.
	~FloatInteraction();

	/// @brief Gets the interaction type.
	virtual fi3d::EModuleInteraction getInteractionType() const override;

	/// @brief Gets the interaction constraint.
	virtual	fi3d::EModuleInteractionConstraint getConstraintType() const override;

	/// @brief Converts given value to a double and assigns it.
	virtual void setValueAsJson(const QJsonValue& value) override;

	/// @brief Gets the intercaction value.
	virtual double getValue() const;

public slots:
	/// @brief Sets the interaction value.
	virtual void setValue(const double& value);

public:
	/// @brief Sets the min, if interaction is MIN constrained.
	virtual void setMin(const double& min);

	/// @brief Sets the max, if interaction is MAX constrained.
	virtual void setMax(const double& max);
	
	/// @brief Sets the min and max, if interaction is RANGE constrained.
	virtual void setRange(const double& min, const double& max);

	/// @brief Gets the min, if interaction is MIN or RANGE constrained.
	virtual double getMin() const;

	/// @brief Gets the max, if interaction is MAX or RANGE constrained.
	virtual double getMax() const;

	static QSharedPointer<FloatInteraction> createMinInteraction(
		const QString& id, const double& value, const double& min, 
		const QString& information = "");

	static QSharedPointer<FloatInteraction> createMaxInteraction(
		const QString& id, const double& value, const double& max,
		const QString& information = "");

	static QSharedPointer<FloatInteraction> createRangeInteraction(
		const QString& id, const double& value, const double& min, 
		const double& max, const QString& information = "");
};

/// @brief Alias for a smart pointer of this class.
using FloatInteractionPtr = QSharedPointer<FloatInteraction>;

}