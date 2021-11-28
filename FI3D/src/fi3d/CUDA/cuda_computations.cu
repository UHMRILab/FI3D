#include <fi3d/CUDA/cuda_computations.cuh>

#include <math.h>
#include <iostream>

__global__ void fi3d::CUDA::computeMK3Workspace_sphere_cuda(float* points, float circleStepSize, float pointStepSize, 
	float separation, float depthMin) 
{
	int i = blockIdx.x;
	int j = threadIdx.x;

	float cRange = (i+1) * circleStepSize;
	float cTheta = (j+1) * pointStepSize;

	float x = cRange * cosf(cTheta);
	float y = cRange * sinf(cTheta);
	
	float magnitude = sqrtf(x * x + y * y + separation * separation);

	float px = x + depthMin * x / magnitude;
	float py = y + depthMin * y / magnitude;
	float pz = separation + depthMin * separation / magnitude;

	int position = (i * blockDim.x + j) * 3;
	points[position] = px;
	points[position + 1] = py;
	points[position + 2] = pz;
}

__global__ void fi3d::CUDA::computeMK3Workspace_cone_cuda(float * points, float insertionStepSIze, float pointStepSize,
	float separation, float range, float depthMin, float depthMax) 
{
	int i = blockIdx.x;
	int j = threadIdx.x;

	float cDepth = (i + 1) * insertionStepSIze + depthMin;
	float cTheta = (j + 1) * pointStepSize;

	float x = range * cosf(cTheta);
	float y = range * sinf(cTheta);

	float magnitude = sqrtf(x * x + y * y + separation * separation);

	float px = x + cDepth * x / magnitude;
	float py = y + cDepth * y / magnitude;
	float pz = separation + cDepth * separation / magnitude;

	int pos = (i * blockDim.x + j) * 3;
	points[pos] = px;
	points[pos + 1] = py;
	points[pos + 2] = pz;
}
