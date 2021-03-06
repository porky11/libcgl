#ifndef __GLUT_H__ 
#define __GLUT_H__ 

/*! \file glut.h
 *
 * The GLUT bindings.
 *
 * You may wonder why the glut interface is wrapped away by yet another
 * layer...  And you would have a valid point.
 *
 * I think that using the gl-lib from another language might be easier if it is
 * not necessary to look for ported glut/gl headers of the correct verion, or
 * port them yourself. The hope is that this layer will not be too large and
 * simpler to port. Jugde yourself :)
 */

#include <libmcm-0.0.1/matrix.h>

#include "cgl.h"

#ifdef __cplusplus
extern "C" {
#endif


void startup_glut(const char *title, int argc, char **argv, int gl_maj, int gl_min, int res_x, int res_y);

void register_display_function(     void (*fn)());
void register_idle_function(        void (*fn)());
void register_keyboard_function(    void (*fn)(unsigned char, int, int));
void register_keyboard_up_function( void (*fn)(unsigned char, int, int));
void register_mouse_motion_function(void (*fn)(int, int));
void register_mouse_function(       void (*fn)(int, int, int, int));
void register_resize_function(      void (*fn)(int, int));

// these are the standard functions which you can call from your own handlers
// to get some default behaviour.
void standard_keyboard(unsigned char key, int x, int y);
void standard_mouse_motion(int x, int y);
void standard_mouse_func(int button, int state, int x, int y);
void standard_resize_func(int w, int h);

// ok, this is getting strange...
void swap_buffers();
void enter_glut_main_loop();

extern float cgl_cam_move_factor;	// we should prefix global variables (at least). if only C had namespaces...
extern bool cgl_use_fix_up_vector;
extern vec3f cgl_up_vector;

void enable_glut_multisampling();

#ifdef __cplusplus
}
#endif

#endif

