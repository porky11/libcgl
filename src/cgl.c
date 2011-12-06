#include "cgl.h"

#include "glut.h"
#include "scheme.h"

#include <stdio.h>
#include <stdlib.h>

#ifdef WITH_GUILE
#include <libguile.h>
#endif

static void hop(void *data, int argc, char **argv) {
	load_snarfed_definitions();
	if (argv[0]) load_configfile(argv[0]);
	start_console_thread();

	((void(*)())data)();	// run the user supplied 'inner main'
}

static void* cfg_only(void *data) {
	load_snarfed_definitions();
	if (data) load_configfile((char*)data);
	return 0;
}

void startup_cgl(const char *window_title, int gl_major, int gl_minor, int argc, char **argv, int res_x, int res_y, void (*call)(), int use_guile, bool verbose, const char *initfile) {
	startup_glut(window_title, argc, argv, gl_major, gl_minor, res_x, res_y);
	
#if CGL_GL_VERSION == GL3
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	ignore_gl_error("glew-init");
	
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}
	if (verbose) {
		dump_gl_info();
		printf("Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
	}
#endif

#ifdef WITH_GUILE
	if (use_guile == with_guile) {
		char *p[2] = { (char*)initfile, 0 };
		scm_boot_guile(initfile?1:0, p, hop, (void*)call);
	}
	else if (use_guile == guile_cfg_only) {
		scm_with_guile(cfg_only, (char*)initfile);
		call();
	}
	else
		call();
#else
	call();
#endif
}

// actually, this should not be necessary as soon as we use the debugging extension...

static error_handler_t error_handler = standard_error_handler;

void register_error_handler(error_handler_t h) {
	error_handler = h;
}

void check_for_gl_errors(const char *where)
{
	GLenum error;
	while ((error = glGetError()) != GL_NO_ERROR)
		error_handler(error, where);
}

void standard_error_handler(GLenum error, const char *where) {
	fprintf(stderr, "GL error %d detected in %s\n", error, where);
	exit(-1);
}

void ignore_gl_error(const char *function)
{
	GLenum error = glGetError();
#if CGL_GL_VERSION == GL3
	fprintf(stderr, "Ignoring GL error %s (for %s).\n", gluErrorString(error), function);
#else
	fprintf(stderr, "Ignoring GL error %d (for %s) (is glu supported here?).\n", error, function);
#endif
}

void dump_gl_info(void)
{
	check_for_gl_errors("pre dump-info");
	printf("Vendor: %s\n", glGetString (GL_VENDOR));
	printf("Renderer: %s\n", glGetString (GL_RENDERER));
	printf("Version: %s\n", glGetString (GL_VERSION));
	printf("GLSL: %s\n", glGetString (GL_SHADING_LANGUAGE_VERSION));
	check_for_gl_errors("dump-info");
}

void quit(int status) {
#ifdef WITH_GUILE
	scm_c_eval_string("(cancel-thread repl-thread)");
	scm_c_eval_string("(join-thread repl-thread)");
#endif
	exit(status);
}


char* gl_enum_string(GLenum e) {
#define c(X) case X: return #X;
	switch (e) {
	// textures
	c(GL_TEXTURE_2D)
	c(GL_RGB)
	c(GL_RGBA)
	c(GL_DEPTH_COMPONENT)
#if CGL_GL_VERSION == GL3
	c(GL_RGB8)
	c(GL_RGBA8)
	c(GL_DEPTH_COMPONENT24)
	c(GL_DEPTH_COMPONENT32)
	c(GL_DEPTH_COMPONENT32F)
#endif
	c(GL_FLOAT)
	c(GL_FRAMEBUFFER_COMPLETE)
	c(GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT)
	c(GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT)
	c(GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER)
	c(GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER)
	c(GL_FRAMEBUFFER_UNSUPPORTED)
	}
	return "not handled yet";
}

