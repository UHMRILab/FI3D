#pragma once
/*!
*	@author		VelazcoJD
*   @file		EModuleRequest.h
*	@enum		fi3d::EModuleRequest 
*	@brief		Defines the type of Module requests there can be.
*/

template <typename K, typename V>
class QHash;
template <typename T>
class QList;
template <typename T>
class QSharedPointer;
class QString;

namespace fi3d {
class EModuleRequest {
private:
	/// @brief The different enumerations.
	static QHash<int, QSharedPointer<EModuleRequest>> ENUM_TYPES;

	/// @brief The integral value of each enumeration.
	static QHash<EModuleRequest*, int> ENUM_VALUES;

	/// @brief The name of each enumeration.
	static QHash<EModuleRequest*, QString> ENUM_NAMES;

	/// @brief The enumeration that the instance has.
	EModuleRequest* mValue;

	/// @brief Instantiates the static instances to represent the enumeration.
	EModuleRequest(EModuleRequest* type);

public:
	/// @brief Gets enumeration's integer values.
	static QList<int> keys();

	/// @brief The integer values of each type.
	enum {
		/// @brief Unknown module request.
		UNKNOWN = 0,
		/// @brief Module interaction request.
		MODULE_INTERACTION = 1,
		/// @brief Module subscription request.
		SUBSCRIBE_TO_MODULE = 2,
		/// @brief Unsubsucribe from module request.
		UNSUBSCRIBE_TO_MODULE = 3,
		/// @brief Get Scene information request.
		GET_SCENE = 4,
		/// @brief Get Visual3D information request.
		GET_VISUAL = 5,
		/// @brief Hide Visual3D request.
		HIDE_VISUAL = 6,
		/// @brief Translate Visual3D request.
		TRANSLATE_VISUAL = 7,
		/// @brief Rotate Visual3D request.
		ROTATE_VISUAL = 8,
		/// @brief Select the slice in an ImageSlice request.
		SELECT_SLICE = 9,
		/// @brief Select the orientation in an ImageSlice request.
		SELECT_ORIENTATION = 10,
		/// @brief Select the series in a StudySlice request.
		SELECT_SERIES = 11
	};

	/// @brief Constructs to an UNKNOWN state.
	EModuleRequest();

	/// @brief Constructs with the given state.
	EModuleRequest(const EModuleRequest& mtype);

	/// @brief Constructs with the given state. Defined by its value.
	EModuleRequest(const int& value);

	/// @brief Destructor.
	~EModuleRequest();

	/*!
	*	@name Operators
	*	@brief Assignment and comparison operators for the enumeration.
	*/
	/// @{
	EModuleRequest& operator=(const int& valueType);
	EModuleRequest& operator=(const EModuleRequest& other);
	bool operator==(const int& valueType) const;
	bool operator==(const EModuleRequest& other) const;
	bool operator!=(const int& valueType) const;
	bool operator!=(const EModuleRequest& other) const;
	/// @}

	/// @brief Gets the name of the enumerated value.
	QString getName() const;

	/// @brief Gets the integer value of the enumerated value.
	int toInt() const;
};
}