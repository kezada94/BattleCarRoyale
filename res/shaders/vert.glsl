#version 130

//layout(location = 0) in vec3 vertex_position; //
//layout(location = 1) in vec3 vertex_normal;   //* PARA MAC
//layout(location = 2) in vec2 texture_coord;   //
//layout(location = 3) in vec4 vtangent;

in vec3 vertex_position;    //
in vec3 vertex_normal;      //* PARA LINUX
in vec2 texture_coord;      //
in vec4 vtangent;				//
uniform mat4 view, proj, model;

out vec3 normalEye;
out vec2 st;
out vec3 positionEye;
out vec4 test_tan;
out vec3 view_dir_tan;
out vec3 light_dir_tan;
out vec3 normal;

void main() {
	st = texture_coord;
	test_tan = vtangent;
	normal = vertex_normal;
	normalEye = normalize(vec3(view * model * vec4(vertex_normal, 0.0)));
	positionEye = vec3(view * model * vec4(vertex_position, 1.0));

	/* a hacky way to get the camera position out of the view matrix instead of
	using another uniform variable */
	vec3 cam_pos_wor = (inverse(view) * vec4 (0.0, 0.0, 0.0, 1.0)).xyz;
	vec3 light_pos_wor = vec3(0, 60, 0);
	vec3 light_dir_wor = normalize(light_pos_wor - cam_pos_wor);
	/* work out bi-tangent as cross product of normal and tangent. also multiply
		by the determinant, which we stored in .w to correct handedness
	*/ 
	vec3 bitangent = cross (vertex_normal, vtangent.xyz) * vtangent.w;
	
	/* transform our camera and light uniforms into local space */
	vec3 cam_pos_loc = vec3 (inverse (model) * vec4 (cam_pos_wor, 1.0));
	vec3 light_dir_loc = vec3 (inverse (model) * vec4 (light_dir_wor, 0.0));
	// ...and work out view _direction_ in local space
	vec3 view_dir_loc = normalize (cam_pos_loc - vertex_position);
	
	/* this [dot,dot,dot] is the same as making a 3x3 inverse tangent matrix, and
		 doing a matrix*vector multiplication.
	*/
	// work out view direction in _tangent space_
	view_dir_tan = vec3 (
		dot (vtangent.xyz, view_dir_loc),
		dot (bitangent, view_dir_loc),
		dot (vertex_normal, view_dir_loc)
	);
	// work out light direction in _tangent space_
	light_dir_tan = vec3 (
		dot (vtangent.xyz, light_dir_loc),
		dot (bitangent, light_dir_loc),
		dot (vertex_normal, light_dir_loc)
	);


	gl_Position = proj * view * model * vec4 (vertex_position, 1.0);
}
