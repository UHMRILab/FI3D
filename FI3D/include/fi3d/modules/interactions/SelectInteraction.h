#pragma once
/*!
* @author	VelazcoJD
* @file		StringInteraction.h
* @class	fi3d::StringInteraction
* @brief	Represents a select module interaction.
* 
* A select interaction gives a set of options, of which only one can be
* selected. In this interaction, there are two values that are important:
* the index (corresponding to the selected option) and the option at the index.
* For this interaction, the value (main value) will always refer to the 
* option's index. To see the option itself, one can use the index to obtain it.
*/

#include <fi3d/modules/interactions/ModuleInteraction.h>

#include <fi3d/server/message_keys/EModuleInteractionConstraint.h>

#include <QSharedPointer>
#include <QVector>

namespace fi3d {
class SelectInteraction : public ModuleInteraction {

	Q_OBJECT

signals:
	/// @brief Emmitted when the interaction's value has changed, giving
	/// the selected option's index.
	void changedValue(const int& value);

	/// @brief Emitted when the interaction's value has changed, giving
	/// the selected option's text value.
	void changedValueOption(const QString& index);

	/// @brief Emitted when the interaction's options have changed.
	void changedOptions(const QVector<QString>& options);

protected:
	/// @brief The interaction's constraint
	fi3d::EModuleInteractionConstraint mConstraint;

	/// @brief The values allowed, when SELECT constraint.
	QVector<QString> mOptions;

public:
	/// <summary>
	/// Constructs a Select interaction with NONE/UNKNOWN constraint.
	/// </summary>
	/// 
	/// The interaction is created with the given options. If no options are
	/// given, the value of the interaction will be set to -1. By default,
	/// the first option (0th index) will be assigned as initial value, unless
	/// no options are given which then -1 is assigned.
	/// 
	/// <param name="id">The ID of the interaction.</param>
	/// <param name="options">The list of options.</param>
	/// <param name="information">Description of the interaction.</param>
	SelectInteraction(
		const QString& id,
		const QVector<QString>& options = QVector<QString>(),
		const QString& information = "");

	/// @brief Destructor.
	~SelectInteraction();

	/// @brief Gets the interaction type.
	virtual fi3d::EModuleInteraction getInteractionType() const override;

	/// @brief Gets the interaction constraint.
	virtual	fi3d::EModuleInteractionConstraint getConstraintType() const override;

	/// @brief Converts given value to an int (the index) and assisngs it.
	virtual void setValueAsJson(const QJsonValue& value) override;

	/// @brief Gets the intercaction value (the index).
	virtual int getValue() const;

	/// @brief Gets the interaction's option value (option at index).
	virtual QString getValueOption() const;

public slots:
	/// <summary>
	/// Sets the interaction value.
	/// </summary>
	/// 
	/// If the given index is out of bounds, 0 will be assigned instead.
	virtual void setValue(const int& value);

	/// <summary>
	/// Assigns the value based on the given option instead of index.
	/// </summary>
	/// 
	/// This function searches for an option that matches the given text in the
	/// list of options. If it's found, the index of that option will be 
	/// assigned. If multiple options match the given text, the first one will
	/// be assigned.
	/// <param name="option"></param>
	virtual void setValueByOption(const QString& option);

	/// @brief Sets the options.
	virtual void setOptions(const QVector<QString>& options, const int& selection = 0);

	/*!
	* @brief Appends an option.
	* @param option The option to add into the list of options.
	* @param select Whether the new option should be marked as selected.
	*/
	virtual void appendOption(const QString& option, const bool& select = false);

public:
	/// @brief Gets a copy of the options.
	virtual QVector<QString> getOptions() const;
};

/// @brief Alias for a smart pointer of this class.
using SelectInteractionPtr = QSharedPointer<SelectInteraction>;

}