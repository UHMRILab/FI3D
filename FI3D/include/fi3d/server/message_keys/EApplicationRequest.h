#pragma once
/*!
*	@author		VelazcoJD
*   @file		EApplicationAction.h
*	@enum		fi3d::EApplicationAction
*	@brief		Enumeration for the type of actions for the overall application.
*/

template <typename K, typename V>
class QHash;
template <typename T>
class QList;
template <typename T>
class QSharedPointer;
class QString;

namespace fi3d {
class EApplicationRequest {
private:
	/// @brief The different enumerations.
	static QHash<int, QSharedPointer<EApplicationRequest>> ENUM_TYPES;

	/// @brief The integral value of each enumeration.
	static QHash<EApplicationRequest*, int> ENUM_VALUES;

	/// @brief The name of each enumeration.
	static QHash<EApplicationRequest*, QString> ENUM_NAMES;

	/// @brief The enumeration that the instance has.
	EApplicationRequest* mValue;

	/// @brief Instantiates the static instances to represent the enumeration.
	EApplicationRequest(EApplicationRequest* type);

public:
	/// @brief Gets enumeration's integer values.
	static QList<int> keys();

	/// @brief The integer values of each type.
	enum {
		/// @brief Unknown application request.
		UNKNOWN = 0,
		/// @brief Activates an instance of the module.
		START_MODULE = 1,
		/// @brief Stops an instance of the module.
		STOP_MODULE = 2,
		/// @brief Instructs the application to log a message.
		LOG = 3
	};

	/// @brief Constructs to an UNKNOWN state.
	EApplicationRequest();

	/// @brief Constructs with the given state.
	EApplicationRequest(const EApplicationRequest& mtype);

	/// @brief Constructs with the given state. Defined by its value.
	EApplicationRequest(const int& value);

	/// @brief Destructor.
	~EApplicationRequest();

	/*!
	*	@name Operators
	*	@brief Assignment and comparison operators for the enumeration.
	*/
	/// @{
	EApplicationRequest& operator=(const int& valueType);
	EApplicationRequest& operator=(const EApplicationRequest& other);
	bool operator==(const int& valueType) const;
	bool operator==(const EApplicationRequest& other) const;
	bool operator!=(const int& valueType) const;
	bool operator!=(const EApplicationRequest& other) const;
	/// @}

	/// @brief Gets the name of the enumerated value.
	QString getName() const;

	/// @brief Gets the integer value of the enumerated value.
	int toInt() const;
};
}