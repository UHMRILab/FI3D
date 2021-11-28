#pragma once
/*!
*	@author		VelazcoJD
*   @file		Icons.h
*	@enum		fi3d::Icons
*	@brief		Helper class to load system theme icons.
*/

#include <fi3d/utilities/EIcon.h>

#include <QIcon>

namespace fi3d {
class Icons {
private:
	/// @brief This is a static class, so no constructor available.
	Icons();

public:
	/// @brief Destructor.
	~Icons();

	/// <summary>
	///  @brief Gets the icon based on the available icon types.
	/// </summary>
	/// <param name="iconType">The icon type.</param>
	/// <returns>The icon.</returns>
	static QIcon getIcon(const EIcon& iconType);
};
}

