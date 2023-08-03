#pragma once
/*!
* @author	VelazcoJD
* @file		MessageKeys.h
* @brief	Set of strings that emcompass the various Json message keys in the
*			communication protocols 
*/

#include <QString>

namespace fi3d {
/*!
* @name Server request/response message keys.
*/
/// @{
extern const QString RESPONSE_STATUS;
extern const QString MESSAGE_TYPE;
extern const QString CLIENT_ID;
extern const QString MESSAGE;
extern const QString PASSWORD;
extern const QString APPLICATION_PARAMS;
extern const QString MODULE_PARAMS;
extern const QString DATA_PARAMS;
extern const QString ERROR_CODE;
/// @}

/*!
 * @name Application request/response message keys.
 */
/// @{
extern const QString ACTION_TYPE;
extern const QString MODULE_NAME;
extern const QString NAME;
extern const QString ACRONYM;
extern const QString AVAILABLE_MODULES;
extern const QString ACTIVE_MODULES;
/// @}

/*!
 * @name Module request/response message keys.
 */
/// @{
extern const QString MODULE_ID;
extern const QString REQUEST_ID;
extern const QString TARGET_VISUAL;
extern const QString IS_VISIBLE;
extern const QString TRANSLATE;
extern const QString ROTATE;
extern const QString SLICE_INDEX;
extern const QString SLICE_ORIENTATION;
extern const QString SERIES_INDEX;
extern const QString INTERACTION_ID;
extern const QString INTERACTION_VALUE;
extern const QString RESPONSE_ID;
extern const QString MODULE_INFO;
extern const QString SCENE_INFO;
extern const QString SCENE_ID;
extern const QString VISUALS_INFO;
extern const QString MODULE_INTERACTIONS;
extern const QString ID;
extern const QString TYPE;
extern const QString VALUE;
extern const QString CONSTRAINT;
extern const QString MIN_VALUE;
extern const QString MAX_VALUE;
extern const QString VALUE_OPTIONS;
extern const QString INFO;
extern const QString VISUAL_INFO;
extern const QString OPACITY;
extern const QString TRANSFORMATION;
extern const QString PARENT_ID;
extern const QString DATA_ID;
extern const QString DATA_NAME;
extern const QString DATA_TYPE;
extern const QString COLOR;
extern const QString ASSEMBLY_PART_COUNT;
extern const QString ASSEMBLY_ACTION;
extern const QString ASSEMBLY_PARTS_INFO;
extern const QString UPDATE_CONSTRAINT;
/// @}

/*!
 * @name Data request/response message keys.
 */
/// @{
extern const QString DATA;
extern const QString CACHEABLE;
extern const QString SERIES_COUNT;
extern const QString DIMENSIONS;
extern const QString ORIGIN;
extern const QString SPACING;
extern const QString DATA_FORMAT;
extern const QString VALUES;
extern const QString POINTS;
extern const QString LINE_INDICES;
extern const QString TRIANGLE_INDICES;
extern const QString PAYLOAD_POINTS_LENGTH;
extern const QString PAYLOAD_TRIANGLES_LENGTH;
extern const QString PAYLOAD_TEXTURE_COORDINATES_LENGTH;
extern const QString PAYLOAD_TEXTURE_LENGTH;
extern const QString TEXTURES;
extern const QString TEXTURE_NAME;
extern const QString TEXTURE_DIMENSION_U;
extern const QString TEXTURE_DIMENSION_V;
/// @}
}