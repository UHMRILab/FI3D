#pragma once
/*!
* @author	VelazcoJD
* @file		EData.h
* @enum		fi3d::EData
* @brief	Enumeration for the type of data objects in FI3D.
*/

template <typename K, typename V>
class QHash;
template <typename T>
class QList;
template <typename T>
class QSharedPointer;
class QString;

namespace fi3d {
class EData {
private:
	/// @brief The different enumerations.
	static QHash<int, QSharedPointer<EData>> ENUM_TYPES;

	/// @brief The integral value of each enumeration.
	static QHash<EData*, int> ENUM_VALUES;

	/// @brief The name of each enumeration.
	static QHash<EData*, QString> ENUM_NAMES;

	/// @brief The enumeration that the instance has.
	EData* mValue;

	/// @brief Instantiates the static instances to represent the enumeration.
	EData(EData* type);

public:
	/// @brief Gets enumeration's integer values.
	static QList<int> keys();

	/// @brief The integer values of each type.
	enum {
		/// @brief Unknown data object.
		UNKNOWN = 0,
		/// @brief A 3D Image.
		IMAGE = 1,
		/// @brief A 3D Image that is part of a Study.
		SERIES = 2,
		/// @brief A Study.
		STUDY = 3,
		/// @brief A Model.
		MODEL = 4,
		/// @brief A set of Model objects that form an animation.
		ANIMATED_MODEL = 5,
	};

	/// @brief Constructs to an UNKNOWN state.
	EData();

	/// @brief Constructs with the given state.
	EData(const EData& mtype);

	/// @brief Constructs with the given state. Defined by its value.
	EData(const int& value);

	/// @brief Destructor.
	~EData();

	/*!
	*	@name Operators
	*	@brief Assignment and comparison operators for the enumeration.
	*/
	/// @{
	EData& operator=(const int& valueType);
	EData& operator=(const EData& other);
	bool operator==(const int& valueType) const;
	bool operator==(const EData& other) const;
	bool operator!=(const int& valueType) const;
	bool operator!=(const EData& other) const;
	/// @}

	/// @brief Gets the name of the enumerated value.
	QString getName() const;

	/// @brief Gets the integer value of the enumerated value.
	int toInt() const;
};
}