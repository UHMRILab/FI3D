#pragma once
/*!
* @author	VelazcoJD
* @file		FI3D.h
* @class	fi3d::FI3D
* @brief	Entry point for the FI3D application.
*
* Creating an instance of this object and calling the run function takes over
* to run the FI3D application.
*/

#include <QObject>

namespace fi3d {
class FI3D {
public:
	/// @brief The path to the assets directory.
	static const QString ASSETS_DIRECTORY;

	/// @brief The path to the data directory.
	static const QString DATA_DIRECTORY;

	/// @brief Constructor.
	FI3D();

	/// @brief Destructor.
	~FI3D();

	/// @brief Starts the FI3D application.
	int run(int argc, char *argv[]);
};
}
