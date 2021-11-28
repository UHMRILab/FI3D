#pragma once
/*!
*	@author		VelazcoJD
*   @file		EResponseStatus.h
*	@enum		fi3d::EResponseStatus
*	@brief		Enumeration for the type of status a response sent to a client is.
*/

template <typename K, typename V>
class QHash;
template <typename T>
class QList;
template <typename T>
class QSharedPointer;
class QString;

namespace fi3d {
class EResponseStatus {
private:
	/// @brief The different enumerations.
	static QHash<int, QSharedPointer<EResponseStatus>> ENUM_TYPES;

	/// @brief The integral value of each enumeration.
	static QHash<EResponseStatus*, int> ENUM_VALUES;

	/// @brief The name of each enumeration.
	static QHash<EResponseStatus*, QString> ENUM_NAMES;

	/// @brief The enumeration that the instance has.
	EResponseStatus* mValue;

	/// @brief Instantiates the static instances to represent the enumeration.
	EResponseStatus(EResponseStatus* type);

public:
	/// @brief Gets enumeration's integer values.
	static QList<int> keys();

	/// @brief The integer values of each type.
	enum {
		/// @brief Unkonwn response status.
		UNKNOWN = 0,
		/// @brief Successful response.
		SUCCESS = 1,
		/// @brief Error response.
		ERROR_RESPONSE = 2,
		/// @brief The response requires more information.
		INFO_REQUIRED = 3
	};

	/// @brief Constructs to an UNKNOWN state.
	EResponseStatus();

	/// @brief Constructs with the given state.
	EResponseStatus(const EResponseStatus& mtype);

	/// @brief Constructs with the given state. Defined by its value.
	EResponseStatus(const int& value);

	/// @brief Destructor.
	~EResponseStatus();

	/*!
	*	@name Operators
	*	@brief Assignment and comparison operators for the enumeration.
	*/
	/// @{
	EResponseStatus& operator=(const int& valueType);
	EResponseStatus& operator=(const EResponseStatus& other);
	bool operator==(const int& valueType) const;
	bool operator==(const EResponseStatus& other) const;
	bool operator!=(const int& valueType) const;
	bool operator!=(const EResponseStatus& other) const;
	/// @}

	/// @brief Gets the name of the enumerated value.
	QString getName() const;

	/// @brief Gets the integer value of the enumerated value.
	int toInt() const;
};
}