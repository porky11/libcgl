#include "texture.h"
#include "impex.h"

#include <libmcm-0.0.1/vectors.h>

#include <GL/glew.h>

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

struct texture {
	char *name;
	GLuint texid;
	GLuint target;
	bool use_mipmapping;
	GLenum param_min, param_mag, param_wrap_s, param_wrap_t;
	unsigned int width, height;
	bool bound;
};

static struct texture *textures = 0;
static unsigned int textures_allocated = 0, 
                    next_texture_index = 0;

static void allocate_texture() {
	// maintain texture table
	if (next_texture_index >= textures_allocated) {
		struct texture *old_array = textures;
		unsigned int allocate = 1.5 * (textures_allocated + 1);
		textures = malloc(sizeof(struct texture) * allocate);
		for (int i = 0; i < textures_allocated; ++i)
			textures[i] = old_array[i];
		textures_allocated = allocate;
		free(old_array);
	}
}

texture_ref make_texture(const char *name, const char *filename, int target) {
	unsigned int w, h;
	vec3f *data = load_png3f(filename, &w, &h);
	
	allocate_texture();
	texture_ref ref;
	ref.id = next_texture_index++;
	struct texture *texture = textures+ref.id;
	texture->name = malloc(strlen(name)+1);
	strcpy(texture->name, name);
	
	glGenTextures(1, &texture->texid);
	texture->target = target;
	glBindTexture(target, texture->texid);
	glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	texture->bound = false;

	texture->width = w;
	texture->height = h;
	glTexImage2D(target, 0, GL_RGBA8, texture->width, texture->height, 0, GL_RGB, GL_FLOAT, data);

	glBindTexture(target, 0);

	free(data);

	return ref;
}

texture_ref make_empty_texture(const char *name, unsigned int w, unsigned int h, int target, unsigned int internal_format) {
	allocate_texture();
	texture_ref ref;
	ref.id = next_texture_index++;
	struct texture *texture = textures+ref.id;
	texture->name = malloc(strlen(name)+1);
	strcpy(texture->name, name);
	
	glGenTextures(1, &texture->texid);
	texture->target = target;
	glBindTexture(target, texture->texid);
	glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	texture->bound = false;

	texture->width = w;
	texture->height = h;
	glTexImage2D(target, 0, internal_format, texture->width, texture->height, 0, GL_RGBA, GL_FLOAT, 0);

	glBindTexture(target, 0);
	return ref;
}

void bind_texture(texture_ref ref)   { textures[ref.id].bound = true; glBindTexture(textures[ref.id].target, textures[ref.id].texid); }
void unbind_texture(texture_ref ref) { textures[ref.id].bound = false; glBindTexture(GL_TEXTURE_2D, 0); }

void save_texture_as_rgb_png(texture_ref ref, const char *filename) {
	struct texture *texture = textures + ref.id;
	bool was_bound = false;
	if (!texture->bound)
		bind_texture(ref);
	else
		was_bound = true;

	/*
	vec4f *data = malloc(sizeof(vec4f)*texture->width*texture->height);
	glGetTexImage(texture->target, 0, GL_RGBA, GL_FLOAT, data);
	save_png4f(data, texture->width, texture->height, filename);
	*/
	vec3f *data = malloc(sizeof(vec3f)*texture->width*texture->height);
	glGetTexImage(texture->target, 0, GL_RGB, GL_FLOAT, data);
	save_png3f(data, texture->width, texture->height, filename);

	if (!was_bound)
		unbind_texture(ref);
}

int texture_id(texture_ref ref) {
	return textures[ref.id].texid;
}

