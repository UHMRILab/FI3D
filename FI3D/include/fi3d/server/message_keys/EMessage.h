#pragma once
/*!
*	@author		VelazcoJD
*   @file		EMessage.h
*	@enum		fi3d::EMessage
*	@brief		Enumeration for the type of requests a client can make.
*/

template <typename K, typename V>
class QHash;
template <typename T>
class QList;
template <typename T>
class QSharedPointer;
class QString;

namespace fi3d {
class EMessage {
private:
	/// @brief The different enumerations.
	static QHash<int, QSharedPointer<EMessage>> ENUM_TYPES;

	/// @brief The integral value of each enumeration.
	static QHash<EMessage*, int> ENUM_VALUES;

	/// @brief The name of each enumeration.
	static QHash<EMessage*, QString> ENUM_NAMES;

	/// @brief The enumeration that the instance has.
	EMessage* mValue;

	/// @brief Instantiates the static instances to represent the enumeration.
	EMessage(EMessage* type);

public:
	/// @brief Gets enumeration's integer values.
	static QList<int> keys();

	/// @brief The integer values of each type.
	enum {
		/// @brief Unknown request.
		UNKNOWN = 0,
		/// @brief Authentication request, handled by the Server.
		AUTHENTICATION = 1,
		/// @brief Application request, handled by the FI3DController
		APPLICATION = 2,
		/// @brief Module request, handled by the corresponding module.
		MODULE = 3,
		/// @brief Data request, handled by the DataManager.
		DATA = 4
	};

	/// @brief Constructs to an UNKNOWN state.
	EMessage();

	/// @brief Constructs with the given state.
	EMessage(const EMessage& mtype);

	/// @brief Constructs with the given state. Defined by its value.
	EMessage(const int& value);

	/// @brief Destructor.
	~EMessage();

	/*!
	*	@name Operators
	*	@brief Assignment and comparison operators for the enumeration.
	*/
	/// @{
	EMessage& operator=(const int& valueType);
	EMessage& operator=(const EMessage& other);
	bool operator==(const int& valueType) const;
	bool operator==(const EMessage& other) const;
	bool operator!=(const int& valueType) const;
	bool operator!=(const EMessage& other) const;
	/// @}

	/// @brief Gets the name of the enumerated value.
	QString getName() const;

	/// @brief Gets the integer value of the enumerated value.
	int toInt() const;
};
}