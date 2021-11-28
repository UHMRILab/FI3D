#pragma once
/*!
*	@author		VelazcoJD
*   @file		EDM_State.h
*	@enum		fi3d::EDM_State
*	@brief		Enumeration for the state of a request made to the DataManager.
*/

template <typename K, typename V>
class QHash;
template <typename T>
class QList;
template <typename T>
class QSharedPointer;
class QString;

namespace fi3d {
class EDM_State {
private:
	/// @brief The different enumerations.
	static QHash<int, QSharedPointer<EDM_State>> ENUM_TYPES;

	/// @brief The integral value of each enumeration.
	static QHash<EDM_State*, int> ENUM_VALUES;

	/// @brief The name of each enumeration.
	static QHash<EDM_State*, QString> ENUM_NAMES;

	/// @brief The enumeration that the instance has.
	EDM_State* mValue;

	/// @brief Instantiates the static instances to represent the enumeration.
	EDM_State(EDM_State* type);

public:
	/// @brief Gets enumeration's integer values.
	static QList<int> keys();

	/// @brief The integer values of each type.
	enum {
		/// @brief Unknown response.
		UNKNOWN = 0,
		/// @brief Request made successfuly.
		SUCCESS = 1,
		/// @brief The data in the request was not found.
		DATA_NOT_FOUND = 2,
		/// @brief Failed to register data because it already is registered.
		ALREADY_REGISTERED = 3
	};

	/// @brief Constructs to an UNKNOWN state.
	EDM_State();

	/// @brief Constructs with the given state.
	EDM_State(const EDM_State& mtype);

	/// @brief Constructs with the given state. Defined by its value.
	EDM_State(const int& value);

	/// @brief Destructor.
	~EDM_State();

	/*!
	*	@name Operators
	*	@brief Assignment and comparison operators for the enumeration.
	*/
	/// @{
	EDM_State& operator=(const int& valueType);
	EDM_State& operator=(const EDM_State& other);
	bool operator==(const int& valueType) const;
	bool operator==(const EDM_State& other) const;
	bool operator!=(const int& valueType) const;
	bool operator!=(const EDM_State& other) const;
	/// @}

	/// @brief Gets the name of the enumerated value.
	QString getName() const;

	/// @brief Gets the integer value of the enumerated value.
	int toInt() const;

	/// @brief Whether the EDM_State is a success.
	bool isSuccess();
};
}
