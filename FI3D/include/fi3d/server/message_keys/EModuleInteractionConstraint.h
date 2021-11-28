#pragma once
/*!
*	@author		VelazcoJD
*   @file		EModuleInteractionConstraint.h
*	@enum		fi3d::EModuleInteractionConstraint
*	@brief		Enumeration for the type of requests a client can make.
*
*	TODO: Move to the modules directory.
*/

template <typename K, typename V>
class QHash;
template <typename T>
class QList;
template <typename T>
class QSharedPointer;
class QString;

namespace fi3d {
class EModuleInteractionConstraint {
private:
	/// @brief The different enumerations.
	static QHash<int, QSharedPointer<EModuleInteractionConstraint>> ENUM_TYPES;

	/// @brief The integral value of each enumeration.
	static QHash<EModuleInteractionConstraint*, int> ENUM_VALUES;

	/// @brief The name of each enumeration.
	static QHash<EModuleInteractionConstraint*, QString> ENUM_NAMES;

	/// @brief Instantiates the static instances to represent the enumeration.
	EModuleInteractionConstraint* mValue;

	/// @brief Instantiates the static instances to represent the enumeration.
	EModuleInteractionConstraint(EModuleInteractionConstraint* type);

public:
	/// @brief Gets enumeration's integer values.
	static QList<int> keys();

	/// @brief The integer values of each type.
	enum {
		/// @brief Unkonwn constraint (or no constraint).
		UNKNOWN = 0,
		/// @brief Has a minimum requirement.
		MIN = 1,
		/// @brief Has a maximum requirement.
		MAX = 2,
		/// @brief Has a minimum and maximum requirement.
		RANGE = 3
	};

	/// @brief Constructs to an UNKNOWN state.
	EModuleInteractionConstraint();

	/// @brief Constructs with the given state.
	EModuleInteractionConstraint(const EModuleInteractionConstraint& mtype);

	/// @brief Constructs with the given state. Defined by its value.
	EModuleInteractionConstraint(const int& value);

	/// @brief Destructor.
	~EModuleInteractionConstraint();

	/*!
	*	@name Operators
	*	@brief Assignment and comparison operators for the enumeration.
	*/
	/// @{
	EModuleInteractionConstraint& operator=(const int& valueType);
	EModuleInteractionConstraint& operator=(const EModuleInteractionConstraint& other);
	bool operator==(const int& valueType) const;
	bool operator==(const EModuleInteractionConstraint& other) const;
	bool operator!=(const int& valueType) const;
	bool operator!=(const EModuleInteractionConstraint& other) const;
	/// @}

	/// @brief Gets the name of the enumerated value.
	QString getName() const;

	/// @brief Gets the integer value of the enumerated value.
	int toInt() const;
};
}