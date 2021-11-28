#ifndef FI3D_NO_CUDA

//#include <fi3d/Logger/Logger.h>

#include <fi3d/CUDA/cuda_helpers.h>
#include <fi3d/CUDA/cuda_computations.cuh>

#include <cuda_runtime.h>
#include <device_launch_parameters.h>

using namespace fi3d;
using namespace fi3d::CUDA;

/// Checks whether the was an error using any CUDA functionality. 
/// Call this method after every CUDA call.
#define gpuErrorCheck(ans) { gpuAssert((ans), __FILE__, __LINE__);}
inline void gpuAssert(cudaError_t code, const char *file, int line, bool abort = false) {
	if (code != cudaSuccess) {
		if (abort) {
			//LOG(fatal) << cudaGetErrorString(code);
			exit(code);
		}
		//LOG(error) << cudaGetErrorString(code);
	}
}

void computeMK3Workspace_sphere(ModelData& workspaceModel, float separation, 
	float depth, float range, int translations, int rotations) 
{
	//if (translations > 1024) {
	//	translations = 1024;
	//}
	//if (rotations > 1024) {
	//	rotations = 1024;
	//}

	//float szTranslation = range / (float)translations;		
	//float szRotation = 6.28318f / (float)rotations;
	//int totalPoints = translations * rotations;

	//float* rawPoints = 0;
	//gpuErrorCheck(cudaMallocManaged(&rawPoints, totalPoints * workspaceModel.getPointSize()));
	//gpuErrorCheck(cudaMemset(rawPoints, 0, totalPoints * workspaceModel.getPointSize()));
	//computeMK3Workspace_sphere_cuda<<<translations, rotations>>>(rawPoints, szTranslation, szRotation, separation, depth);
	//gpuErrorCheck(cudaPeekAtLastError());
	//gpuErrorCheck(cudaDeviceSynchronize());
	////TODO: Change so that all data is copied at once instead of point per point.
	//for (int i = 0; i < totalPoints; i++) {
	//	int pos = i * 3;
	//	workspaceModel.addPoint(rawPoints[pos], rawPoints[pos + 1], rawPoints[pos + 2]);
	//}
	//gpuErrorCheck(cudaFree(rawPoints));
}

void computeMK3Workspace_cone(ModelData& workspaceModel, float separation, 
	float dmin, float dmax, float range, int insertions, int rotations) 
{
	//if (insertions > 1024) {
	//	insertions = 1024;
	//}
	//if (rotations > 1024) {
	//	rotations = 1024;
	//}

	//float szInsertions = (dmax - dmin) / (float)insertions;
	//float szRotations = 6.28318f / (float)rotations;
	//int totalPoints = insertions * rotations;

	//float *rawPoints = 0;
	//gpuErrorCheck(cudaMallocManaged(&rawPoints, totalPoints * workspaceModel.getPointSize()));
	//gpuErrorCheck(cudaMemset(rawPoints, 0, totalPoints * workspaceModel.getPointSize()));
	//computeMK3Workspace_cone_cuda<<<insertions, rotations>>>(rawPoints, szInsertions, szRotations, separation, range, dmin, dmax);
	//gpuErrorCheck(cudaPeekAtLastError());
	//gpuErrorCheck(cudaDeviceSynchronize());
	////TODO: Change so that all data is copied at once instead of point per point.
	//for (int i = 0; i < totalPoints; i++) {
	//	int pos = i * 3;
	//	workspaceModel.addPoint(rawPoints[pos], rawPoints[pos + 1], rawPoints[pos + 2]);
	//}
	//gpuErrorCheck(cudaFree(rawPoints));
}

#endif