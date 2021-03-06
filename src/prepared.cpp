#include "prepared.h"

#include "scheme.h"

#include <libmcm/camera-matrices.h>

#include <list>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

extern "C" {

mesh_ref make_quad(const char *name, matrix4x4f *trafo) {
	vec3f v[] = { {-1, -1, 0},
	              {-1,  1, 0},
				  { 1,  1, 0},
				  { 1,  1, 0},
				  { 1, -1, 0},
				  {-1, -1, 0} };
	if (trafo)
		for (int i = 0; i < 6; ++i) {
			vec4f cur = { v[i].x, v[i].y, v[i].z, 1 };
			vec4f res;
			multiply_matrix4x4f_vec4f(&res, trafo, &cur);
			v[i].x = res.x; v[i].y = res.y; v[i].z = res.z;
		}

	mesh_ref mesh = make_mesh(name, 1);
	bind_mesh_to_gl(mesh);
	add_vertex_buffer_to_mesh(mesh, "vt", GL_FLOAT, 6, 3, v, GL_STATIC_DRAW);
	unbind_mesh_from_gl(mesh);
	return mesh;
}

mesh_ref make_quad_with_tc(const char *name, matrix4x4f *trafo) {
	vec3f v[] = { {-1, -1, 0},
	              {-1,  1, 0},
				  { 1,  1, 0},
				  { 1,  1, 0},
				  { 1, -1, 0},
				  {-1, -1, 0} };
	vec2f t[] = { {0, 0},
	              {0, 1},
				  {1, 1},
				  {1, 1},
				  {1, 0},
				  {0, 0} };
	if (trafo)
		for (int i = 0; i < 6; ++i) {
			vec4f cur = { v[i].x, v[i].y, v[i].z, 1 };
			vec4f res;
			multiply_matrix4x4f_vec4f(&res, trafo, &cur);
			v[i].x = res.x; v[i].y = res.y; v[i].z = res.z;
		}
	mesh_ref mesh = make_mesh(name, 2);
	bind_mesh_to_gl(mesh);
	add_vertex_buffer_to_mesh(mesh, "vt", GL_FLOAT, 6, 3, v, GL_STATIC_DRAW);
	add_vertex_buffer_to_mesh(mesh, "tc", GL_FLOAT, 6, 2, t, GL_STATIC_DRAW);
	unbind_mesh_from_gl(mesh);
	return mesh;
}

mesh_ref make_quad_with_normal(const char *name, matrix4x4f *trafo) {
	vec3f v[] = { {-1, -1, 0},
	              {-1,  1, 0},
				  { 1,  1, 0},
				  { 1,  1, 0},
				  { 1, -1, 0},
				  {-1, -1, 0} };
	vec3f n[] = { { 0,  0, 1}, {0,0,1}, {0,0,1}, {0,0,1}, {0,0,1}, {0,0,1} };
	if (trafo) {
		matrix4x4f tmp, norm;
		invert_matrix4x4f(&tmp, trafo);
		transpose_matrix4x4f(&norm, &tmp);
		for (int i = 0; i < 6; ++i) {
			// transform v
			vec4f cur = { v[i].x, v[i].y, v[i].z, 1 };
			vec4f res;
			multiply_matrix4x4f_vec4f(&res, trafo, &cur);
			v[i].x = res.x; v[i].y = res.y; v[i].z = res.z;
			// transform n
			cur.x = n[i].x; cur.y = n[i].y; cur.z = n[i].z; cur.w = 0;
			multiply_matrix4x4f_vec4f(&res, &norm, &cur);
			n[i].x = res.x; n[i].y = res.y; n[i].z = res.z;
		}
	}
	mesh_ref mesh = make_mesh(name, 2);
	bind_mesh_to_gl(mesh);
	add_vertex_buffer_to_mesh(mesh, "vt", GL_FLOAT, 6, 3, v, GL_STATIC_DRAW);
	add_vertex_buffer_to_mesh(mesh, "vn", GL_FLOAT, 6, 3, n, GL_STATIC_DRAW);
	unbind_mesh_from_gl(mesh);
	return mesh;
}

mesh_ref make_quad_with_normal_and_tc(const char *name, matrix4x4f *trafo) {
	vec3f v[] = { {-1, -1, 0},
	              {-1,  1, 0},
				  { 1,  1, 0},
				  { 1,  1, 0},
				  { 1, -1, 0},
				  {-1, -1, 0} };
	vec2f t[] = { {0, 0},
	              {0, 1},
				  {1, 1},
				  {1, 1},
				  {1, 0},
				  {0, 0} };
	vec3f n[] = { { 0,  0, 1}, {0,0,1}, {0,0,1}, {0,0,1}, {0,0,1}, {0,0,1} };
	if (trafo) {
		matrix4x4f tmp, norm;
		invert_matrix4x4f(&tmp, trafo);
		transpose_matrix4x4f(&norm, &tmp);
		for (int i = 0; i < 6; ++i) {
			// transform v
			vec4f cur = { v[i].x, v[i].y, v[i].z, 1 };
			vec4f res;
			multiply_matrix4x4f_vec4f(&res, trafo, &cur);
			v[i].x = res.x; v[i].y = res.y; v[i].z = res.z;
			// transform n
			cur.x = n[i].x; cur.y = n[i].y; cur.z = n[i].z; cur.w = 0;
			multiply_matrix4x4f_vec4f(&res, &norm, &cur);
			n[i].x = res.x; n[i].y = res.y; n[i].z = res.z;
		}
	}
	mesh_ref mesh = make_mesh(name, 3);
	bind_mesh_to_gl(mesh);
	add_vertex_buffer_to_mesh(mesh, "vt", GL_FLOAT, 6, 3, v, GL_STATIC_DRAW);
	add_vertex_buffer_to_mesh(mesh, "vn", GL_FLOAT, 6, 3, n, GL_STATIC_DRAW);
	add_vertex_buffer_to_mesh(mesh, "tc", GL_FLOAT, 6, 2, t, GL_STATIC_DRAW);
	unbind_mesh_from_gl(mesh);
	return mesh;
}

mesh_ref make_circle(const char *name, int n, matrix4x4f *trafo) {
	vec3f v[n+2];
	v[0].x = v[0].y = v[0].z = 0;
	for (int i = 0; i < n; ++i) {
		v[i+1].x = cosf(i*2*M_PI/float(n));
		v[i+1].y = sinf(i*2*M_PI/float(n));
		v[i+1].z = 0;
	}
	v[n+1].x = cosf(0);
	v[n+1].y = sinf(0);

	if (trafo)
		for (int i = 0; i < n+2; ++i) {
			vec4f cur = { v[i].x, v[i].y, v[i].z, 1 };
			vec4f res;
			multiply_matrix4x4f_vec4f(&res, trafo, &cur);
			v[i].x = res.x; v[i].y = res.y; v[i].z = res.z;
		}

	mesh_ref mesh = make_mesh(name, 1);
	bind_mesh_to_gl(mesh);
	add_vertex_buffer_to_mesh(mesh, "vt", GL_FLOAT, n+2, 3, v, GL_STATIC_DRAW);
	set_mesh_primitive_type(mesh, GL_TRIANGLE_FAN);
	unbind_mesh_from_gl(mesh);
	return mesh;
}


mesh_ref make_cylinder(const char *name, int n, matrix4x4f *trafo) {
	vec3f v[2*n+2];
	vec3f norm[2*n+2];
	for (int i = 0; i < n+1; ++i) {
		v[2*i].x = v[2*i+1].x = cosf(i*2*M_PI/float(n));
		v[2*i].y = v[2*i+1].y = sinf(i*2*M_PI/float(n));
		v[2*i].z = 1;
		v[2*i+1].z = -1;
		norm[2*i].x = norm[2*i+1].x = v[2*i].y;
		norm[2*i].y = norm[2*i+1].y = v[2*i].x;
		norm[2*i].z = norm[2*i+1].z = 0;
	}
	if (trafo) {
		matrix4x4f tmp, nm;
		invert_matrix4x4f(&tmp, trafo);
		transpose_matrix4x4f(&nm, &tmp);
		for (int i = 0; i < 2*n+2; ++i) {
			// transform v
			vec4f cur = { v[i].x, v[i].y, v[i].z, 1 };
			vec4f res;
			multiply_matrix4x4f_vec4f(&res, trafo, &cur);
			v[i].x = res.x; v[i].y = res.y; v[i].z = res.z;
			// transform n
			cur.x = norm[i].x; cur.y = norm[i].y; cur.z = norm[i].z; cur.w = 0;
			multiply_matrix4x4f_vec4f(&res, &nm, &cur);
			norm[i].x = res.x; norm[i].y = res.y; norm[i].z = res.z;
		}
	}

	mesh_ref mesh = make_mesh(name, 2);
	bind_mesh_to_gl(mesh);
	add_vertex_buffer_to_mesh(mesh, "vt", GL_FLOAT, 2*n+2, 3, v, GL_STATIC_DRAW);
	add_vertex_buffer_to_mesh(mesh, "vn", GL_FLOAT, 2*n+2, 3, norm, GL_STATIC_DRAW);
	set_mesh_primitive_type(mesh, GL_TRIANGLE_STRIP);
	unbind_mesh_from_gl(mesh);
	return mesh;
}

void make_cube_vertices_and_normals(vec3f **verts, vec3f **norms, int *N, matrix4x4f *trafo) {
	static vec3f v[] = { {-1, -1,  1}, { 1, -1,  1}, { 1,  1,  1}, { 1,  1,  1}, {-1,  1,  1}, {-1, -1,  1}, // FRONT
	                     {-1, -1, -1}, {-1,  1, -1}, { 1,  1, -1}, { 1,  1, -1}, { 1, -1, -1}, {-1, -1, -1}, // BACK
	                     {-1, -1,  1}, {-1,  1,  1}, {-1,  1, -1}, {-1,  1, -1}, {-1, -1, -1}, {-1, -1,  1}, // LEFT
	                     { 1, -1, -1}, { 1,  1, -1}, { 1,  1,  1}, { 1,  1,  1}, { 1, -1,  1}, { 1, -1, -1}, // RIGHT
	                     {-1,  1,  1}, { 1,  1,  1}, { 1,  1, -1}, { 1,  1, -1}, {-1,  1, -1}, {-1,  1,  1}, // TOP
	                     {-1, -1,  1}, {-1, -1, -1}, { 1, -1, -1}, { 1, -1, -1}, { 1, -1,  1}, {-1, -1,  1}, // BOTTOM
	};
	static vec3f n[] = { { 0, 0, 1 },  { 0, 0, 1 },  { 0, 0, 1 },  { 0, 0, 1 },  { 0, 0, 1 },  { 0, 0, 1 }, 
	                     { 0, 0,-1 },  { 0, 0,-1 },  { 0, 0,-1 },  { 0, 0,-1 },  { 0, 0,-1 },  { 0, 0,-1 },
	                     {-1, 0, 0 },  {-1, 0, 0 },  {-1, 0, 0 },  {-1, 0, 0 },  {-1, 0, 0 },  {-1, 0, 0 },
	                     { 1, 0, 0 },  { 1, 0, 0 },  { 1, 0, 0 },  { 1, 0, 0 },  { 1, 0, 0 },  { 1, 0, 0 },
	                     { 0, 1, 0 },  { 0, 1, 0 },  { 0, 1, 0 },  { 0, 1, 0 },  { 0, 1, 0 },  { 0, 1, 0 },
	                     { 0,-1, 0 },  { 0,-1, 0 },  { 0,-1, 0 },  { 0,-1, 0 },  { 0,-1, 0 },  { 0,-1, 0 } 
	};
	*verts = (vec3f*)malloc(sizeof(vec3f)*36);
	*norms = (vec3f*)malloc(sizeof(vec3f)*36);
	for (int i = 0; i < 36; ++i) {
		(*verts)[i] = v[i];
		(*norms)[i] = n[i];
	}
	*N = 36;
	if (trafo) {
		matrix4x4f tmp, norm;
		invert_matrix4x4f(&tmp, trafo);
		transpose_matrix4x4f(&norm, &tmp);
		for (int i = 0; i < 36; ++i) {
			// transform v
			vec4f cur = { (*verts)[i].x, (*verts)[i].y, (*verts)[i].z, 1 };
			vec4f res;
			multiply_matrix4x4f_vec4f(&res, trafo, &cur);
			(*verts)[i].x = res.x; (*verts)[i].y = res.y; (*verts)[i].z = res.z;
			// transform n
			cur.x = (*norms)[i].x; cur.y = (*norms)[i].y; cur.z = (*norms)[i].z; cur.w = 0;
			multiply_matrix4x4f_vec4f(&res, &norm, &cur);
			(*norms)[i].x = res.x; (*norms)[i].y = res.y; (*norms)[i].z = res.z;
		}
	}
}

mesh_ref make_cube(const char *name, matrix4x4f *trafo) {
	vec3f v[] = { {-1, -1,  1}, { 1, -1,  1}, { 1,  1,  1}, { 1,  1,  1}, {-1,  1,  1}, {-1, -1,  1}, // FRONT
	              {-1, -1, -1}, {-1,  1, -1}, { 1,  1, -1}, { 1,  1, -1}, { 1, -1, -1}, {-1, -1, -1}, // BACK
	              {-1, -1,  1}, {-1,  1,  1}, {-1,  1, -1}, {-1,  1, -1}, {-1, -1, -1}, {-1, -1,  1}, // LEFT
	              { 1, -1, -1}, { 1,  1, -1}, { 1,  1,  1}, { 1,  1,  1}, { 1, -1,  1}, { 1, -1, -1}, // RIGHT
	              {-1,  1,  1}, { 1,  1,  1}, { 1,  1, -1}, { 1,  1, -1}, {-1,  1, -1}, {-1,  1,  1}, // TOP
	              {-1, -1,  1}, {-1, -1, -1}, { 1, -1, -1}, { 1, -1, -1}, { 1, -1,  1}, {-1, -1,  1}, // BOTTOM
	};
	vec3f n[] = { { 0, 0, 1 },  { 0, 0, 1 },  { 0, 0, 1 },  { 0, 0, 1 },  { 0, 0, 1 },  { 0, 0, 1 }, 
	              { 0, 0,-1 },  { 0, 0,-1 },  { 0, 0,-1 },  { 0, 0,-1 },  { 0, 0,-1 },  { 0, 0,-1 },
	              {-1, 0, 0 },  {-1, 0, 0 },  {-1, 0, 0 },  {-1, 0, 0 },  {-1, 0, 0 },  {-1, 0, 0 },
	              { 1, 0, 0 },  { 1, 0, 0 },  { 1, 0, 0 },  { 1, 0, 0 },  { 1, 0, 0 },  { 1, 0, 0 },
	              { 0, 1, 0 },  { 0, 1, 0 },  { 0, 1, 0 },  { 0, 1, 0 },  { 0, 1, 0 },  { 0, 1, 0 },
	              { 0,-1, 0 },  { 0,-1, 0 },  { 0,-1, 0 },  { 0,-1, 0 },  { 0,-1, 0 },  { 0,-1, 0 } 
	};
	vec2f t[] = { {0,0},    {1,0},    {1,1},    {1,1},    {0,1},    {0,0}, 
	              {0,0},    {0,1},    {1,1},    {1,1},    {1,0},    {0,0}, 
	              {0,1},    {1,1},    {1,0},    {1,0},    {0,0},    {0,1}, 
	              {0,0},    {1,0},    {1,1},    {1,1},    {0,1},    {0,0}, 
	              {0,1},    {1,1},    {1,0},    {1,0},    {0,0},    {0,1}, 
				  {0,1},    {0,0},    {1,0},    {1,0},    {1,1},    {0,1}, 
	};
	if (trafo) {
		matrix4x4f tmp, norm;
		invert_matrix4x4f(&tmp, trafo);
		transpose_matrix4x4f(&norm, &tmp);
		for (int i = 0; i < 36; ++i) {
			// transform v
			vec4f cur = { v[i].x, v[i].y, v[i].z, 1 };
			vec4f res;
			multiply_matrix4x4f_vec4f(&res, trafo, &cur);
			v[i].x = res.x; v[i].y = res.y; v[i].z = res.z;
			// transform n
			cur.x = n[i].x; cur.y = n[i].y; cur.z = n[i].z; cur.w = 0;
			multiply_matrix4x4f_vec4f(&res, &norm, &cur);
			n[i].x = res.x; n[i].y = res.y; n[i].z = res.z;
		}
	}
	mesh_ref mesh = make_mesh(name, 3);
	bind_mesh_to_gl(mesh);
	add_vertex_buffer_to_mesh(mesh, "vt", GL_FLOAT, 36, 3, v, GL_STATIC_DRAW);
	add_vertex_buffer_to_mesh(mesh, "vn", GL_FLOAT, 36, 3, n, GL_STATIC_DRAW);
	add_vertex_buffer_to_mesh(mesh, "tc", GL_FLOAT, 36, 2, t, GL_STATIC_DRAW);
	unbind_mesh_from_gl(mesh);
	return mesh;

}

mesh_ref make_general_wire_furstum(const char *name, vec3f *near_ll, vec3f *near_lr, vec3f *near_ur, vec3f *near_ul, 
                                   vec3f *far_ll, vec3f *far_lr, vec3f *far_ur, vec3f *far_ul) {
	vec3f v[8] = { *near_ll, *near_lr, *near_ur, *near_ul, *far_ll, *far_lr, *far_ur, *far_ul };
	unsigned int indices[24] = { 0, 1,   1, 2,   2, 3,   3, 0,
	                             0, 4,   1, 5,   2, 6,   3, 7,
	                             4, 5,   5, 6,   6, 7,   7, 4 };
	mesh_ref mesh = make_mesh(name, 1);
	set_mesh_primitive_type(mesh, GL_LINES);
	bind_mesh_to_gl(mesh);
	add_vertex_buffer_to_mesh(mesh, "vt", GL_FLOAT, 8, 3, v, GL_STATIC_DRAW);
	add_index_buffer_to_mesh(mesh, 24, indices, GL_STATIC_DRAW);
	unbind_mesh_from_gl(mesh);
	return mesh;
}

/*! \brief make an icosahedron and subdivide it n times.
 * 	\param subdiv how often to subdivde (0 produces a plain icosahedron).
 *
 * 	\note this produces redundant vertices which may be taken out later on.
 *
 * 	based on http://www.fho-emden.de/~hoffmann/ikos27042002.pdf, 
 * 	found at http://sarvanz.blogspot.de/2013/07/sphere-triangulation-using-icosahedron.html
 */
vec3f* make_isosphere_vertices(int subdiv, int *resulting_verts) {
	vec3f base[12];

	float theta = 26.56505117707799 * M_PI / 180.0; // refer paper for theta value

	float stheta = sinf(theta);
	float ctheta = cosf(theta);

	// bottom vertex
	make_vec3f(base+0, 0.0f, -1.0f, 0.0f);

	// lower pentagon
	float phi = M_PI / 5.0f;
	for (int i = 1; i < 6; ++i) {
		make_vec3f(base+i, ctheta * cosf(phi), -stheta, ctheta * sinf(phi));
		phi += 2.0f * M_PI / 5.0f;
	}
	
	// the upper pentagon
	phi = 0.0f;
	for (int i = 6; i < 11; ++i) {
		make_vec3f(base+i, ctheta * cosf(phi), stheta, ctheta * sinf(phi));
		phi += 2.0f * M_PI / 5.0f;
	}

	make_vec3f(base+11, 0.0f, 1.0f, 0.0f); // the upper vertex

	// piece together (each line is a triangle)
	std::list<vec3f> curr, next;
	curr.push_back(base[0]);  curr.push_back(base[1]); curr.push_back(base[2]);  
	curr.push_back(base[0]);  curr.push_back(base[2]); curr.push_back(base[3]);  
	curr.push_back(base[0]);  curr.push_back(base[3]); curr.push_back(base[4]);  
	curr.push_back(base[0]);  curr.push_back(base[4]); curr.push_back(base[5]);  
	curr.push_back(base[0]);  curr.push_back(base[5]); curr.push_back(base[1]);  
	curr.push_back(base[1]);  curr.push_back(base[7]); curr.push_back(base[2]);  
	curr.push_back(base[2]);  curr.push_back(base[8]); curr.push_back(base[3]);  
	curr.push_back(base[3]);  curr.push_back(base[9]); curr.push_back(base[4]);  
	curr.push_back(base[4]);  curr.push_back(base[10]);curr.push_back(base[5]);  
	curr.push_back(base[5]);  curr.push_back(base[6]); curr.push_back(base[1]);  
	curr.push_back(base[1]);  curr.push_back(base[6]); curr.push_back(base[7]);  
	curr.push_back(base[2]);  curr.push_back(base[7]); curr.push_back(base[8]);  
	curr.push_back(base[3]);  curr.push_back(base[8]); curr.push_back(base[9]);  
	curr.push_back(base[4]);  curr.push_back(base[9]); curr.push_back(base[10]); 
	curr.push_back(base[5]);  curr.push_back(base[10]);curr.push_back(base[6]);  
	curr.push_back(base[6]);  curr.push_back(base[11]);curr.push_back(base[7]);  
	curr.push_back(base[7]);  curr.push_back(base[11]);curr.push_back(base[8]);  
	curr.push_back(base[8]);  curr.push_back(base[11]);curr.push_back(base[9]);  
	curr.push_back(base[9]);  curr.push_back(base[11]);curr.push_back(base[10]); 
	curr.push_back(base[10]); curr.push_back(base[11]);curr.push_back(base[6]);  

	// subdivide
	for (int s = 0; s < subdiv; ++s) {
		vec3f v[6];
		while (curr.size()) {
			v[0] = curr.front(); curr.pop_front();
			v[1] = curr.front(); curr.pop_front();
			v[2] = curr.front(); curr.pop_front();

			add_components_vec3f(v+3, v+0, v+1); normalize_vec3f(v+3);
			add_components_vec3f(v+4, v+1, v+2); normalize_vec3f(v+4);
			add_components_vec3f(v+5, v+2, v+0); normalize_vec3f(v+5);

			next.push_back(v[0]); next.push_back(v[3]); next.push_back(v[5]);
			next.push_back(v[1]); next.push_back(v[4]); next.push_back(v[3]);
			next.push_back(v[2]); next.push_back(v[5]); next.push_back(v[4]);
			next.push_back(v[4]); next.push_back(v[5]); next.push_back(v[3]);
		}
		curr.swap(next);
	}
	
	*resulting_verts = curr.size();
	vec3f *data = (vec3f*)malloc(sizeof(vec3f) * *resulting_verts);
	int i = 0;
	for (std::list<vec3f>::iterator it = curr.begin(); it != curr.end(); ++it, ++i)
		data[i] = *it;

	return data;
}

void compute_flat_icosphere(int subdiv, vec3f **v_out, vec3f **n_out, int *N, matrix4x4f *trafo) {
	*v_out = make_isosphere_vertices(subdiv, N);
	*n_out = (vec3f*)malloc(sizeof(vec3f) * *N);
	vec3f *v = *v_out,
		  *n = *n_out;
	for (int i = 0; i < *N; i+=3) {
		vec3f e1, e2;
		sub_components_vec3f(&e1, v+i, v+i+1);
		sub_components_vec3f(&e2, v+i, v+i+2);
		cross_vec3f(n+i, &e1, &e2);
		normalize_vec3f(n+i);
		n[i+1] = n[i+2] = n[i];
	}

	if (trafo) {
		matrix4x4f tmp, norm;
		invert_matrix4x4f(&tmp, trafo);
		transpose_matrix4x4f(&norm, &tmp);
		for (int i = 0; i < *N; ++i) {
			// transform v
			vec4f cur = { v[i].x, v[i].y, v[i].z, 1 };
			vec4f res;
			multiply_matrix4x4f_vec4f(&res, trafo, &cur);
			v[i].x = res.x; v[i].y = res.y; v[i].z = res.z;
			// transform n
			cur.x = n[i].x; cur.y = n[i].y; cur.z = n[i].z; cur.w = 0;
			multiply_matrix4x4f_vec4f(&res, &norm, &cur);
			n[i].x = res.x; n[i].y = res.y; n[i].z = res.z;
		}
	}
}

mesh_ref make_flat_icosphere(const char *name, int subdiv, matrix4x4f *trafo) {
	vec3f *v, *n;
	int N;
	compute_flat_icosphere(subdiv, &v, &n, &N, trafo);

	mesh_ref mesh = make_mesh(name, 2);
	bind_mesh_to_gl(mesh);
	add_vertex_buffer_to_mesh(mesh, "vt", GL_FLOAT, N, 3, v, GL_STATIC_DRAW);
	add_vertex_buffer_to_mesh(mesh, "vn", GL_FLOAT, N, 3, n, GL_STATIC_DRAW);
	set_mesh_primitive_type(mesh, GL_TRIANGLES);
	unbind_mesh_from_gl(mesh);

	free(v);
	free(n);

	return mesh;
}

/*
void compute_furstum_base(const vec3f *dir, const vec3f *up, vec3f *W, vec3f *U, vec3f *V) {
	vec3f TxW;
	div_vec3f_by_scalar(W, dir, length_of_vec3f(dir));
	cross_vec3f(&TxW, up, W);
	div_vec3f_by_scalar(U, &TxW, length_of_vec3f(&TxW));
	cross_vec3f(V, W, U);
}

void print_matrix(const matrix4x4f *m) {
	printf("    %3.6f\t%3.6f\t%3.6f\t%3.6f\n", m->col_major[0*4+0], m->col_major[1*4+0], m->col_major[2*4+0], m->col_major[3*4+0]);
	printf("    %3.6f\t%3.6f\t%3.6f\t%3.6f\n", m->col_major[0*4+1], m->col_major[1*4+1], m->col_major[2*4+1], m->col_major[3*4+1]);
	printf("    %3.6f\t%3.6f\t%3.6f\t%3.6f\n", m->col_major[0*4+2], m->col_major[1*4+2], m->col_major[2*4+2], m->col_major[3*4+2]);
	printf("    %3.6f\t%3.6f\t%3.6f\t%3.6f\n", m->col_major[0*4+3], m->col_major[1*4+3], m->col_major[2*4+3], m->col_major[3*4+3]);
}
*/

vec3f v3(vec4f v) {
	div_vec4f_by_scalar(&v, &v, v.w);
	vec3f r = { v.x, v.y, v.z };
	return r;
}

mesh_ref make_simple_wire_frustum(const char *name, const vec3f *pos, const vec3f *dir, const vec3f *up, float angle, float aspect, float near, float far) {
	matrix4x4f proj, la, view, pv, inv;
	make_projection_matrixf(&proj, angle, aspect, near, far);
	make_lookat_matrixf(&la, pos, dir, up);
	make_gl_viewing_matrixf(&view, &la);
	multiply_matrices4x4f(&pv, &proj, &view);
	invert_matrix4x4f(&inv, &pv);
	matrix4x4f tmp;
	multiply_matrices4x4f(&tmp, &pv, &inv);
	vec4f cs_n_ll = {-1,-1,-1, 1}, cs_n_lr = { 1,-1,-1, 1}, cs_n_ur = { 1, 1,-1, 1}, cs_n_ul = {-1, 1,-1, 1},
	      cs_f_ll = {-1,-1, 1, 1}, cs_f_lr = { 1,-1, 1, 1}, cs_f_ur = { 1, 1, 1, 1}, cs_f_ul = {-1, 1, 1, 1},
		  res;
	vec3f nll, nlr, nur, nul, fll, flr, fur, ful;

	multiply_matrix4x4f_vec4f(&res, &inv, &cs_n_ll); 	nll = v3(res);
	multiply_matrix4x4f_vec4f(&res, &inv, &cs_n_lr); 	nlr = v3(res);
	multiply_matrix4x4f_vec4f(&res, &inv, &cs_n_ur); 	nur = v3(res);
	multiply_matrix4x4f_vec4f(&res, &inv, &cs_n_ul); 	nul = v3(res);

	multiply_matrix4x4f_vec4f(&res, &inv, &cs_f_ll); 	fll = v3(res);
	multiply_matrix4x4f_vec4f(&res, &inv, &cs_f_lr); 	flr = v3(res);
	multiply_matrix4x4f_vec4f(&res, &inv, &cs_f_ur); 	fur = v3(res);
	multiply_matrix4x4f_vec4f(&res, &inv, &cs_f_ul); 	ful = v3(res);

	return make_general_wire_furstum(name, &nll, &nlr, &nur, &nul, &fll, &flr, &fur, &ful);
}


/*
 *   tangent space stuff
 */


float triangle_area(const vec3f *a, const vec3f *b, const vec3f *c) {
        vec3f ab, ac, cross;
        sub_components_vec3f(&ab, b, a);
        sub_components_vec3f(&ac, c, a);
        cross_vec3f(&cross, &ab, &ac);
        return 0.5 * dot_vec3f(&cross, &cross);
}

vec4f* generate_tangent_space_from_tri_mesh(const vec3f *vertex, const vec3f *normal, const vec2f *tc, unsigned int vertex_data_len, const unsigned int *index, unsigned int indices) {
        if (indices % 3 != 0) return 0;
        unsigned int triangles = indices/3;
        struct tb {
                vec3f t, b;
        };
        tb *per_triangle = new tb[triangles];
        std::list<int> *per_vertex = new std::list<int>[vertex_data_len];
        for (int i = 0; i < triangles; ++i) {
                int vi1 = index[3*i], vi2 = index[3*i+1], vi3 = index[3*i+2];
                int ti1 = index[3*i], ti2 = index[3*i+1], ti3 = index[3*i+2];
                vec3f q1, q2;
                sub_components_vec3f(&q1, vertex+vi2, vertex+vi1);
                sub_components_vec3f(&q2, vertex+vi3, vertex+vi1);
                vec2f uv1, uv2;
                sub_components_vec2f(&uv1, tc+ti2, tc+ti1);
                sub_components_vec2f(&uv2, tc+ti3, tc+ti1);

                vec2f inv_t_row = { uv2.y, -uv1.y },
                          inv_s_row = { -uv2.x, uv1.x };
                vec2f qx_col = { q1.x, q2.x },
                          qy_col = { q1.y, q2.y },
                          qz_col = { q1.z, q2.z };

                vec3f t = { dot_vec2f(&inv_t_row, &qx_col),
                            dot_vec2f(&inv_t_row, &qy_col),
                            dot_vec2f(&inv_t_row, &qz_col) },
                          b = { dot_vec2f(&inv_s_row, &qx_col),
                            dot_vec2f(&inv_s_row, &qy_col),
                            dot_vec2f(&inv_s_row, &qz_col) };

                float normalize = uv1.x*uv2.y - uv2.x*uv1.y;
                div_vec3f_by_scalar(&t, &t, normalize);
                div_vec3f_by_scalar(&b, &b, normalize);

                per_triangle[i].t = t;
                per_triangle[i].b = b;

                per_vertex[vi1].push_back(i);  // register the triangle with all vertices for back reference.
                per_vertex[vi2].push_back(i);
                per_vertex[vi3].push_back(i);
        }
        vec4f *tangents = new vec4f[vertex_data_len];
        for (int i = 0; i < vertex_data_len; ++i) {
                float area = 0.0f;
                vec3f accum_t = {0,0,0};
                vec3f accum_b = {0,0,0};
                vec4f result = {0,0,0,0};
                for (std::list<int>::iterator it = per_vertex[i].begin(); it != per_vertex[i].end(); ++it) {
                        int tri = 3**it;
                        float a = triangle_area(vertex+index[tri+0], vertex+index[tri+1], vertex+index[tri+2]);
                        vec3f t, b;
                        mul_vec3f_by_scalar(&t, &per_triangle[*it].t, a);
                        mul_vec3f_by_scalar(&b, &per_triangle[*it].b, a);
                        add_components_vec3f(&accum_t, &accum_t, &t);
                        add_components_vec3f(&accum_b, &accum_b, &b);
                        area += a;
                }
                if (area > 0) {
                        div_vec3f_by_scalar(&accum_t, &accum_t, area);
                        div_vec3f_by_scalar(&accum_b, &accum_b, area);
                        
						/* not required, not tested, included for the sake of completeness
						float dot = dot_vec3f(&accum_t, &accum_b);
                        result.x = accum_t.x;
                        result.y = accum_t.y;
                        result.z = accum_t.z;
                        result.w = (dot < 0 ? -1 : 1);
						*/

                        vec3f newt, newb, tmp, tmp2; 
                        const vec3f *norm = normal+i;
                        mul_vec3f_by_scalar(&tmp, norm, dot_vec3f(norm, &accum_t));
                        sub_components_vec3f(&newt, &accum_t, &tmp);
                        normalize_vec3f(&newt);

                        cross_vec3f(&tmp, norm, &newt);
                        float dot = dot_vec3f(&tmp, &accum_b);
                        result.x = newt.x;
                        result.y = newt.y;
                        result.z = newt.z;
                        result.w = (dot < 0 ? -1 : 1);
                }
                tangents[i] = result;
        }

        delete [] per_triangle;
        delete [] per_vertex;

        return tangents;
}


shader_ref make_shader_from_strings(const char *name, const char *vert, const char *frag, int number_of_inputs, ...) {
	va_list ap;
	const char **input_name = new const char*[number_of_inputs];
	va_start(ap, number_of_inputs);
	for (int i = 0; i < number_of_inputs; ++i)
		input_name[i] = va_arg(ap, char*);
	va_end(ap);
	
	shader_ref shader = make_shader(name, number_of_inputs);
	add_vertex_source(shader, vert);
	add_fragment_source(shader, frag);
	for (int i = 0; i < number_of_inputs; ++i)
		add_shader_input(shader, input_name[i], i);
	if (!compile_and_link_shader(shader)) {
		const char *vl = vertex_shader_info_log(shader);
		const char *fl = fragment_shader_info_log(shader);
		const char *sl = shader_info_log(shader);
		if (vl) print_error_message("Vertex SHADER ERROR %s\n---------------\n%s\n-------------------\n", name, vl);
		if (fl) print_error_message("Fragment SHADER ERROR %s\n---------------\n%s\n-------------------\n", name, fl);
		if (sl) print_error_message("Linker SHADER ERROR %s\n---------------\n%s\n-------------------\n", name, sl);
	}

	delete [] input_name;
	return shader;
}


}


#ifdef WITH_GUILE
#include <libguile.h>
extern "C" {
    SCM_DEFINE(s_make_quad_with_tc, "make-quad-with-tc", 1, 0, 0, (SCM name), "") {
        char *n = scm_to_locale_string(name);
        mesh_ref ref = make_quad_with_tc(n, 0);
        free(n);
        return scm_from_int(ref.id);
    }


    SCM_DEFINE(s_make_quad, "make-quad", 1, 0, 0, (SCM name), "") {
        char *n = scm_to_locale_string(name);
        mesh_ref ref = make_quad(n, 0);
        free(n);
        return scm_from_int(ref.id);
    }

    SCM_DEFINE(s_make_circle, "make-circle", 2, 0, 0, (SCM name, SCM segments), "") {
        char *n = scm_to_locale_string(name);
		int s = scm_to_int(segments);
        mesh_ref ref = make_circle(n, s, 0);
        free(n);
        return scm_from_int(ref.id);
    }

    SCM_DEFINE(s_make_cube, "make-cube", 1, 0, 0, (SCM name), "") {
        char *n = scm_to_locale_string(name);
        mesh_ref ref = make_cube(n, 0);
        free(n);
        return scm_from_int(ref.id);
    }

	SCM_DEFINE(s_make_simple_wire_frustum, "make-simple-wire-frustum", 8, 0, 0, 
	           (SCM name, SCM pos, SCM dir, SCM up, SCM angle, SCM aspect, SCM near, SCM far), "") {
		char *m = scm_to_locale_string(name);
		vec3f p = scm_vec_to_vec3f(pos),
			  d = scm_vec_to_vec3f(dir),
			  u = scm_vec_to_vec3f(up);
		float a = scm_to_double(angle),
			  b = scm_to_double(aspect),
			  n = scm_to_double(near),
			  f = scm_to_double(far);
		mesh_ref ref = make_simple_wire_frustum(m, &p, &d, &u, a, b, n, f);
		free(m);
		return scm_from_int(ref.id);
	}



    void register_scheme_functions_for_prepared() {
    #ifndef SCM_MAGIC_SNARFER
    #include "prepared.x"
    #endif
    }

}

#endif


