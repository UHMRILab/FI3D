#pragma once
/*!
*	@author		VelazcoJD
*   @file		EModuleInteraction.h
*	@enum		fi3d::EModuleInteraction
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
class EModuleInteraction {
private:
	/// @brief The different enumerations.
	static QHash<int, QSharedPointer<EModuleInteraction>> ENUM_TYPES;

	/// @brief The integral value of each enumeration.
	static QHash<EModuleInteraction*, int> ENUM_VALUES;

	/// @brief The name of each enumeration.
	static QHash<EModuleInteraction*, QString> ENUM_NAMES;

	/// @brief The enumeration that the instance has.
	EModuleInteraction* mValue;

	/// @brief Instantiates the static instances to represent the enumeration.
	EModuleInteraction(EModuleInteraction* type);

public:
	/// @brief Gets enumeration's integer values.
	static QList<int> keys();

	/// @brief The integer values of each type.
	enum {
		/// @brief Unknown interaction.
		UNKNOWN = 0,
		/// @brief No value, just needs to be triggered (e.g., button).
		VALUELESS = 1,
		/// @brief Requires a true/false.
		BOOL = 2,	
		/// @brief A numeric value with no decimal.
		INTEGER = 3,
		/// @brief A numeric value with decimal.
		FLOAT = 4,
		/// @brief An array of characters.
		STRING = 5,
		/// @brief A list of options, of which only one can be selected.
		SELECT = 6,
		/// @brief Two floating points.
		POINT_2D = 7,
		/// @brief Three floating points.
		POINT_3D = 8
	};

	/// @brief Constructs to an UNKNOWN state.
	EModuleInteraction();

	/// @brief Constructs with the given state.
	EModuleInteraction(const EModuleInteraction& mtype);

	/// @brief Constructs with the given state. Defined by its value.
	EModuleInteraction(const int& value);

	/// @brief Destructor.
	~EModuleInteraction();

	/*!
	*	@name Operators
	*	@brief Assignment and comparison operators for the enumeration.
	*/
	/// @{
	EModuleInteraction& operator=(const int& valueType);
	EModuleInteraction& operator=(const EModuleInteraction& other);
	bool operator==(const int& valueType) const;
	bool operator==(const EModuleInteraction& other) const;
	bool operator!=(const int& valueType) const;
	bool operator!=(const EModuleInteraction& other) const;
	/// @}

	/// @brief Gets the name of the enumerated value.
	QString getName() const;

	/// @brief Gets the integer value of the enumerated value.
	int toInt() const;
};
}