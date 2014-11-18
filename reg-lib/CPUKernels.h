#pragma once
#include "Kernels.h"
#include"Context.h"


//Kernel functions for affine deformation field 
class CPUAffineDeformationFieldKernel : public AffineDeformationFieldKernel {
public:
	CPUAffineDeformationFieldKernel(Context* con, std::string nameIn) : AffineDeformationFieldKernel( nameIn) {
		this->deformationFieldImage = con->getCurrentDeformationField();
		this->affineTransformation = con->getTransformationMatrix();
		this->mask = con->getCurrentReferenceMask();
	}


	void execute(bool compose = false);
	
	mat44 *affineTransformation;
	nifti_image *deformationFieldImage;
	int* mask;
};
//Kernel functions for block matching
class CPUBlockMatchingKernel : public BlockMatchingKernel {
public:

	CPUBlockMatchingKernel(Context* con, std::string name) : BlockMatchingKernel( name) {
		target = con->getCurrentReference();
		result = con->getCurrentWarped();
		params = con->getBlockMatchingParams();
		mask = con->getCurrentReferenceMask();
	}

	void execute();
	nifti_image* target;
	nifti_image* result;
	_reg_blockMatchingParam* params;
	int* mask;

};
//a kernel function for convolution (gaussian smoothing?)
class CPUConvolutionKernel : public ConvolutionKernel {
public:

	CPUConvolutionKernel(std::string name) : ConvolutionKernel(name) {
	}

	void execute(nifti_image *image, float *sigma, int kernelType, int *mask = NULL, bool *timePoints = NULL, bool *axis = NULL);
private:
	bool *nanImagePtr;
	float *densityPtr;

};

//kernel functions for numerical optimisation
class CPUOptimiseKernel : public OptimiseKernel {
public:

	CPUOptimiseKernel(Context* con, std::string name) : OptimiseKernel( name) {
		transformationMatrix = con->getTransformationMatrix();
		blockMatchingParams = con->getBlockMatchingParams();
	}
	_reg_blockMatchingParam *blockMatchingParams;
	mat44 *transformationMatrix;

	void execute(bool affine);
};

//kernel functions for image resampling with three interpolation variations
class CPUResampleImageKernel : public ResampleImageKernel {
public:
	CPUResampleImageKernel(Context* con, std::string name) : ResampleImageKernel( name) {
		floatingImage = con->getCurrentFloating();
		warpedImage = con->getCurrentWarped();
		deformationField = con->getCurrentDeformationField();
		mask = con->getCurrentReferenceMask();
	}
	nifti_image *floatingImage;
	nifti_image *warpedImage;
	nifti_image *deformationField;
	int *mask;

	void execute(int interp, float paddingValue, bool *dti_timepoint = NULL, mat33 * jacMat = NULL);
};

