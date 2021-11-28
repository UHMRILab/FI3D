#ifdef FI3D_NO_CUDA

#include <fi3d/logger/Logger.h>

#include <fi3d/CUDA/cuda_helpers.h>

using namespace fi3d::CUDA;

void fi3d::CUDA::computeMK3Workspace_sphere(fi3d::ModelData& workspaceModel, float separation, 
	float depth, float range, int translations, int rotations) 
{
	qWarning() << "CUDA was not enabled for this build. GPU functions are disabled";
}

void fi3d::CUDA::computeMK3Workspace_cone(fi3d::ModelData& workspaceModel, float separation, float dmin, float dmax,
		float range, int insertions, int rotations) 
{
	qWarning() << "CUDA was not enabled for this build. GPU functions are disabled";
}

#endif
