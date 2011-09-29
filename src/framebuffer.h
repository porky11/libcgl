#ifndef __FRAMEBUFFER_H__ 
#define __FRAMEBUFFER_H__ 

#include "texture.h"

typedef struct {
	int id;
} framebuffer_ref;

framebuffer_ref make_framebuffer(char *name, unsigned int width, unsigned int height);
void attach_texture_as_colorbuffer(framebuffer_ref ref, char *name, texture_ref texture);
void attach_texture_as_depthbuffer(framebuffer_ref ref, char *name, texture_ref texture);
void attach_depth_buffer(framebuffer_ref ref);
void check_framebuffer_setup(framebuffer_ref ref);
void bind_framebuffer(framebuffer_ref ref);
void unbind_framebuffer(framebuffer_ref ref);


#endif
