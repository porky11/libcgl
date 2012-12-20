#ifndef __MESH_H__ 
#define __MESH_H__ 

#include <stdbool.h>
#include "cgl.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef struct {
	int id;
} mesh_ref;

inline bool equal_mesh_refs(mesh_ref a, mesh_ref b) {
	return a.id == b.id;
}

/*! you can add a vertex buffer to a mesh in two different ways.
 *  first: add it by passing cpu data -> a vbo will be created.
 *  second: add an existing vbo -> it will be used. (create the mesh with make_mesh("", 0).
 *  \attention don't mix the two methods!
 */

mesh_ref make_mesh(const char *name, unsigned int vertex_buffers);
void bind_mesh_to_gl(mesh_ref mr);
void unbind_mesh_from_gl(mesh_ref mr);
bool add_vertex_buffer_to_mesh(mesh_ref mr, const char *name, GLenum content_type, unsigned int vertices, unsigned int element_dim, const void *data, GLenum usage_hint);
bool add_existing_vertex_buffer_to_mesh(mesh_ref mr, const char *name, GLenum content_type, unsigned int vertices, unsigned int element_dim, GLuint vboid);
bool change_vertex_buffer_data(mesh_ref mr, const char *name, GLenum content_type, unsigned int element_dim, const void *data, GLenum usage_hint);
void add_index_buffer_to_mesh(mesh_ref mr, unsigned int number_of_indices, const unsigned int *data, GLenum usage_hint);
unsigned int index_buffer_length_of_mesh(mesh_ref mr);
unsigned int vertex_buffer_length_of_mesh(mesh_ref mr);
char* mesh_name(mesh_ref mr);
mesh_ref find_mesh(const char *name);
void draw_mesh(mesh_ref ref);//!< attention: very general, your own draw call might be faster.
void draw_mesh_as(mesh_ref mr, GLenum primitive_type);//!< ditto.
GLenum mesh_primitive_type(mesh_ref mr);
void set_mesh_primitive_type(mesh_ref mr, GLenum type);


#ifdef __cplusplus
}
#endif

#endif

