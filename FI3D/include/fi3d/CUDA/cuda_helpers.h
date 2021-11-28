#pragma once
/*!
* @author	VelazcoJD
* @file		cuda_helpers.h
* @brief	Functions used within the entire application that invoke CUDA to
*			perform parallel computations.
*
* This header file contains function definitions that are used to help using
* CUDA computations. Each function consists in managing the shared memory with
* the GPU as well as formatting the data in a way that is useful to the GPU.
* The actual CUDA calculations are done in cuda_computaitons.
*
* Every function has a "twin" function in cuda_computations with the exact
* name plus "_cuda" suffix. In other words, every CUDA function has a helper
* function. The helper functions should be used throughout the application,
* and CUDA functions are then invoked from here.
*/

#include <fi3d/data/ModelData.h>

#include <vtkSmartPointer.h>
#include <vtkPoints.h>

namespace fi3d {
namespace CUDA {
/*!
 *	@brief Compute sphere part of the workspace.
 *
 * The workspace is composed of two spheres (top small sphere, and bottom 
 * larger sphere) and a cone shape. This functions generates the sphere part 
 * (both bottom and top). The bottom or right sphere is indicated by the depth.
 * Depth min grants the top sphere whereas depth max generates bottom sphere. 
 * Therefore, you should call this method two times, one for each of 
 * the two spheres.
 *
 *	@param workspace the object where the points information will be stored
 *	@param separation the separation between top and bottom stage
 *	@param depth the minimum/maximum depth for top/bottom sphere 
 *	@param range the range for which the top stage can be translated
 *	@param translations the number of translations for which points will be 
 *			calculated, acceptable values [1-1024]
 *	@param rotations the number of rotations for which points will be 
 *			calculated, acceptable values [1-1024]
 */
 //TODO: Fix this function to work without the Model class which was removed
void computeMK3Workspace_sphere(fi3d::ModelData& workspace, float separation, 
	float depth, float range, int translations, int rotations);

/*!
 * @brief Compute cone part of workspace.
 *
 * The workspace is composed of two spheres (top small sphere, and bottom 
 * larger sphere) and a cone shape. This functions generates the cone part.
 *
 *	@param workspace the object where the points/vertices will be stored
 *	@param separation the separation between top and bottom stage
 *	@param dmin the minimum needle depth length
 *	@param dmax the maximum needle depth length
 *	@param range the range for which the top stage can be translated
 *	@param insertions the number of translations for which points will be 
 *			calculated, acceptable values [1-1024]
 *	@param rotations the number of rotations for which points will be 
 *			calculated, acceptable values [1-1024]
 */
 //TODO: Fix this function to work without the Model class which was removed
void computeMK3Workspace_cone(fi3d::ModelData& workspace, float separation, 
	float dmin, float dmax, float range, int insertions, int rotations);
}
}