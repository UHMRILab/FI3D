#pragma once
/*!
*	@author		VelazcoJD
*   @file		EIcon.h
*	@enum		fi3d::EIcon
*	@brief		Enumeration for the type of system-wide icons available.
*/

template <typename K, typename V>
class QHash;
template <typename T>
class QList;
template <typename T>
class QSharedPointer;
class QString;

namespace fi3d {
class EIcon {
private:
	/// @brief The different enumerations.
	static QHash<int, QSharedPointer<EIcon>> ENUM_TYPES;

	/// @brief The integral value of each enumeration.
	static QHash<EIcon*, int> ENUM_VALUES;

	/// @brief The name of each enumeration.
	static QHash<EIcon*, QString> ENUM_NAMES;

	/// @brief The enumeration that the instance has.
	EIcon* mValue;

	/// @brief Instantiates the static instances to represent the enumeration.
	EIcon(EIcon* type);

public:
	/// @brief Gets enumeration's integer values.
	static QList<int> keys();

	/// @brief The integer values of each type.
	enum {
		/// @brief Unknown icon.
		UNKNOWN = 0,
		/// @brief Represents the system brand's icon.
		BRAND = 1,

		/// @brief Represents the settings action.
		SETTINGS = 101,
		/// @brief Represents a refresh action.
		REFRESH = 102,
		/// @brief Represents a synchronization icon.
		SYNC = 103,
		/// @brief Represents a shutdown icon.
		SHUTDOWN = 104,

		/// @brief Represents going backward in time.
		BACKWARD = 201,
		/// @brief Represents going forward in time.
		FORWARD = 202,
		/// @brief Represents going to the beginning of time.
		BEGINNING = 203,
		/// @brief Represents going to the end of time.
		ENDING = 204,
		/// @brief Represents going to the previous item.
		PREVIOUS = 205,
		/// @brief Represents going to the next item.
		NEXT = 206,
		/// @brief Represents going to the first item.
		FIRST = 207,
		/// @brief Represents going to the last item.
		LAST = 208,

		/// @brief Represents the transverse icon.
		TRANSVERSE = 301,
		/// @brief Represents the coronal orientation.
		CORONAL = 302,
		/// @brief Represents the sagittal orientation.
		SAGITTAL = 303,

		/// @brief Represents the cut icon.
		CUT = 401,
		
		/// @brief Represent the reconstruct surface icon.
		RECONSTRUCT_SURFACE = 402,

		///  @brief Represents the pop up action icon.
		POP_UP = 403
	};

	/// @brief Constructs to an UNKNOWN state.
	EIcon();

	/// @brief Constructs with the given state.
	EIcon(const EIcon& mtype);

	/// @brief Constructs with the given state. Defined by its value.
	EIcon(const int& value);

	/// @brief Destructor.
	~EIcon();

	/*!
	*	@name Operators
	*	@brief Assignment and comparison operators for the enumeration.
	*/
	/// @{
	EIcon& operator=(const int& valueType);
	EIcon& operator=(const EIcon& other);
	bool operator==(const int& valueType) const;
	bool operator==(const EIcon& other) const;
	bool operator!=(const int& valueType) const;
	bool operator!=(const EIcon& other) const;
	/// @}

	/// @brief Gets the name of the enumerated value.
	QString getName() const;

	/// @brief Gets the integer value of the enumerated value.
	int toInt() const;
};
}