#pragma once
/*!
* @author	VelazcoJD
* @file		cuda_computations.cuh
* @brief	All CUDA functions. These functions are executed per CUDA core.
*
* All these are CUDA functions to do various computations within the
* application. These functions are called from cuda_helpers.h by their
* corresponding use. This file should not be used directly and should be
* interacted with indirectly from cuda_helpers.h
*/

#include <cuda_runtime.h>
#include <device_launch_parameters.h>

#include <iostream>

namespace fi3d {
namespace CUDA {

/*!
 * @brief Compute sphere part of workspace.
 *
 * The workspace is composed of two spheres (top small sphere, and bottom
 * larger sphere) and a cone shape. This functions generates the sphere part 
 * (both bottom and top). The bottom or right sphere is indicated by the depth.
 * Depth min grants the top sphere whereas depth max generates bottom sphere. 
 * Therefore, you should call this method two times, one for each of the two
 * spheres.
 *
 * @param points pointer to the array of floats where points are stored
 * @param circleStepSize the number of iterations to do (how many circles)
 * @param pointStepSize the number of iterations to do in a rotation
 * @param separation the separation between the top and bottom stages
 */
__global__ void computeMK3Workspace_sphere_cuda(float* points, 
	float circleStepSize, float pointStepSize, float separation, float depth);

/*!
* @brief Compute cone part of workspace.
*
* The workspace is composed of two spheres (top small sphere, and bottom
* larger sphere) and a cone shape. This functions generates the cone part.
*
* @param points pointer to the array of floats where points are stored
* @param insertionStepSize the number of iterations down the cone
* @param pointStepSize the number of iterations to do in a translation
* @param separation the separation between the top and bottom stages
* @param range the maximum translation distance that can be achieved by 
*			the top stage
* @param depthMin the minimum depth of the needle
* @param depthMax the maximum depth of the needle
*/
__global__ void computeMK3Workspace_cone_cuda(float* points, float insertionStepSize, 
	float pointStepSize, float separation, float range, float depthMin, float depthMax);
}
}