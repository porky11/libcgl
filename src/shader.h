#ifndef __SHADER_H__ 
#define __SHADER_H__ 

#include <stdbool.h>

#include <libmcm/vectors.h>
#include <libmcm/matrix.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef struct {
	int id;
} shader_ref;


shader_ref make_shader(const char *name, int input_vars);
void add_vertex_source(shader_ref ref, const char *src);
void add_fragment_source(shader_ref ref, const char *src);
void add_geometry_source(shader_ref ref, const char *src);
void add_tesselation_control_source(shader_ref ref, const char *src);
void add_tesselation_evaluation_source(shader_ref ref, const char *src);
bool add_shader_input(shader_ref ref, const char *varname, unsigned int index);
bool add_shader_uniform(shader_ref ref, const char *name);
bool modify_shader_input_index(shader_ref ref, const char *varname, unsigned int new_index);
bool compile_and_link_shader(shader_ref ref);
void bind_shader(shader_ref ref);
void unbind_shader(shader_ref ref);

const char* vertex_shader_info_log(shader_ref ref);
const char* fragment_shader_info_log(shader_ref ref);
const char* geometry_shader_info_log(shader_ref ref);
const char* shader_info_log(shader_ref ref);

int gl_shader_object(shader_ref ref);
shader_ref find_shader(const char *name);
bool valid_shader_ref(shader_ref ref);
shader_ref make_invalid_shader(void);
const char* shader_name(shader_ref ref);

// these functions handle the uniforms by retrieving the internally stored location.
int shader_uniforms(shader_ref ref);
const char* shader_uniform_name_by_id(shader_ref ref, unsigned int id);
int shader_uniform_location_by_id(shader_ref ref, unsigned int id);

// compute shader specifics.
vec3i compute_shader_local_workgroup_size(shader_ref ref);

// the following functions retrieve the uniform location for each call.
int uniform_location(shader_ref ref, const char *name);
void uniform3f(shader_ref ref, const char *name, float x, float y, float z);
void uniform4f(shader_ref ref, const char *name, float x, float y, float z, float w);
void uniform3fv(shader_ref ref, const char *name, float *v);
void uniform4fv(shader_ref ref, const char *name, float *v);
void uniform_matrix4x4f(shader_ref ref, const char *name, matrix4x4f *m);


#ifdef __cplusplus
}
#endif

#endif

