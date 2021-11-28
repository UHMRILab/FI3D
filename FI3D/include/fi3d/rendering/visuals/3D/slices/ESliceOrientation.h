#pragma once
/*!
* @author	VelazcoJD
* @date		04/22/2018
* @revised  01/30/2020
* @file		ESliceOrientation.h
* @enum		fi3d::ESliceOrientation
* @brief	Enumeration for different orientations a slice can be on.
*/

template <typename K, typename V>
class QHash;
template <typename T>
class QList;
template <typename T>
class QSharedPointer;
class QString;

namespace fi3d {
class ESliceOrientation {
private:
	/// @brief The different enumerations.
	static QHash<int, QSharedPointer<ESliceOrientation>> ENUM_TYPES;

	/// @brief The integral value of each enumeration.
	static QHash<ESliceOrientation*, int> ENUM_VALUES;

	/// @brief The name of each enumeration.
	static QHash<ESliceOrientation*, QString> ENUM_NAMES;

	/// @brief The enumeration that the instance has.
	ESliceOrientation* mValue;

	 /// @brief Instantiates the static instances to represent the enumeration.
	ESliceOrientation(ESliceOrientation* type);

public:
	 /// @brief Gets a list of integer values. One per enumeration value.
	 ///	Helpful to use for an iteration.
	static QList<int> keys();

	/// @brief The integer values of each type. */
	enum {
		/// @brief Unknown orienation
		UNKNOWN = 0,
		/// @brief XY - Transverse
		XY = 1,
		/// @brief YZ - Sagittal
		YZ = 2,
		/// @brief XZ - Coronal
		XZ = 3
	};

	/// @brief Constructs to an UNKNOWN state.
	ESliceOrientation();

	/// @brief Constructs with the given state.
	ESliceOrientation(const ESliceOrientation& mtype);

	/// @brief Constructs with the given state. Defined by its value.
	ESliceOrientation(const int& value);

	/// @brief Destructor.
	~ESliceOrientation();

	/*!
	*	@name Operators
	*	@brief Assignment and comparison operators for the enumeration.
	*/
	/// @{
	ESliceOrientation& operator=(const int& valueType);
	ESliceOrientation& operator=(const ESliceOrientation& other);
	bool operator==(const int& valueType) const;
	bool operator==(const ESliceOrientation& other) const;
	bool operator!=(const int& valueType) const;
	bool operator!=(const ESliceOrientation& other) const;
	/// @}

	/// @brief Gets the name of the enumerated value.
	QString getName() const;

	/// @brief Gets the integer value of the enumerated value.
	int toInt() const;
};
}