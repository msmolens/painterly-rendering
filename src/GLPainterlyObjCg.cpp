#include "GLPainterlyObj.h"

// Loads the vertex and fragment profiles.
void
GLPainterlyObj::load_profiles ()
{
	if ((vertex_profile = cgGLGetLatestProfile (CG_GL_VERTEX)) == CG_PROFILE_UNKNOWN) {
		std::cerr << "ERROR: cannot load vertex profile." << std::endl;
		exit (EXIT_FAILURE);
	}
	cgGLSetOptimalOptions (vertex_profile);

//	if ((fragment_profile = cgGLGetLatestProfile (CG_GL_FRAGMENT)) == CG_PROFILE_UNKNOWN) {
//		std::cerr << "ERROR: cannot load fragment profile." << std::endl;
//		exit (EXIT_FAILURE);
//	}
	fragment_profile = CG_PROFILE_FP30;
	cgGLSetOptimalOptions (fragment_profile);
}

// Compiles and loads vertex and fragment programs.
void
GLPainterlyObj::load_cg_programs ()
{
	if (!cgIsContext (context)) {
		std::cerr <<  "ERROR: invalid Cg context" << std::endl;
		exit (EXIT_FAILURE);
	}

	// Load passthrough vertex and fragment programs.
	vp_passthrough = cgCreateProgramFromFile (
		context,
		CG_SOURCE,
		"passthroughv.cg",
		vertex_profile,
		NULL,
		NULL);
	if (!cgIsProgramCompiled (vp_passthrough)) {
		cgCompileProgram (vp_passthrough);
	}
	cgGLLoadProgram (vp_passthrough);

	fp_passthrough = cgCreateProgramFromFile (
		context,
		CG_SOURCE,
		"passthroughf.cg",
		fragment_profile,
		NULL,
		NULL);
	if (!cgIsProgramCompiled (fp_passthrough)) {
		cgCompileProgram (fp_passthrough);
	}
	cgGLLoadProgram (fp_passthrough);

	// Load difference fragment program.
	fp_difference = cgCreateProgramFromFile (
		context,
		CG_SOURCE,
		"difference.cg",
		fragment_profile,
		NULL,
		NULL);
	if (!cgIsProgramCompiled (fp_difference)) {
		cgCompileProgram (fp_difference);
	}
	cgGLLoadProgram (fp_difference);

	// Load Sobel vertex and fragment programs.
	vp_sobel = cgCreateProgramFromFile (
		context,
		CG_SOURCE,
		"sobelv.cg",
		vertex_profile,
		NULL,
		NULL);
	if (!cgIsProgramCompiled (vp_sobel)) {
		cgCompileProgram (vp_sobel);
	}
	cgGLLoadProgram (vp_sobel);

	fp_sobel = cgCreateProgramFromFile (
		context,
		CG_SOURCE,
		"sobelf.cg",
		fragment_profile,
		NULL,
		NULL);
	if (!cgIsProgramCompiled (fp_sobel)) {
		cgCompileProgram (fp_sobel);
	}
	cgGLLoadProgram (fp_sobel);

	// Load convolution fragment programs.
	char filename[20];
	for (int i = 0; i < 4; i++) {
		
		// Load horizontal passes.
		sprintf (filename, "filterf_%d_h.cg", i+1);
		fp_filter_h[i] =
			cgCreateProgramFromFile (
				context,
				CG_SOURCE,
				filename,
				fragment_profile,
				NULL,
				NULL);
		if (!cgIsProgramCompiled (fp_filter_h[i])) {
			cgCompileProgram (fp_filter_h[i]);
		}
		cgGLLoadProgram (fp_filter_h[i]);

		// Load vertical passes.
		sprintf (filename, "filterf_%d_v.cg", i+1);
		fp_filter_v[i] =
			cgCreateProgramFromFile (
				context,
				CG_SOURCE,
				filename,
				fragment_profile,
				NULL,
				NULL);
		if (!cgIsProgramCompiled (fp_filter_v[i])) {
			cgCompileProgram (fp_filter_v[i]);
		}
		cgGLLoadProgram (fp_filter_v[i]);
	}
}
