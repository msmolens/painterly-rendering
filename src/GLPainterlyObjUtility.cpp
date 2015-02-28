#include "GLPainterlyObj.h"

void
GLPainterlyObj::make_gaussian (float weights[],
			       int dim,
			       float sigma)
{
	float sum = 0;
	int dim_over_2 = (dim>>1);
	for (int x = 0; x < dim; x++) {
		sum += (weights[x] = 1.0 * gaussian1 (x-dim_over_2, sigma));
	}
#ifdef _DEBUG
	std::cerr << "dim: " << dim << std::endl;
#endif
	for (int i = 0; i < dim; i++) {
		weights[i] /= sum;
#ifdef _DEBUG
		std::cerr << weights[i] << " ";
#endif
	}
#ifdef _DEBUG
	std::cerr<<"\n" << std::endl;
#endif
}

void
GLPainterlyObj::make_average (float weights[],
			      int dim)
{
	float scale = 1.0/dim;
	for (int i = 0; i < dim; i++) {
		weights[i] = scale;
	}
}
