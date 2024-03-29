#pragma once
/*!
* @author	VelazcoJD
* @file		CachedModel.h
* @class	fi::CachedModel
* @brief	A ModelData that is being cached.
*/

#include <fi3d/data/ModelData.h>
#include <FI/data/CachedData.h>

namespace fi {
class CachedModel : public fi3d::ModelData, public CachedData {

	Q_OBJECT

public:
	/// @brief VTK object dependencies.
	static CachedModel* New();
	vtkTypeMacro(CachedModel, fi3d::ModelData)

	/// @brief Deletes the underlying polygon mesh.
	virtual void release() override;

protected:
	/// @brief Constructor
	CachedModel();

	/// @brief Destructor.
	~CachedModel() = default;

private:
	CachedModel(const CachedModel&) = delete;
	void operator=(const CachedModel&) = delete;
};

/// @brief Alias for a smart pointer of this class.
using CachedModelVPtr = vtkSmartPointer<CachedModel>;

}