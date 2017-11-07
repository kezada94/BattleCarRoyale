#version 130

//layout(location = 0) in vec3 vertexPosition_model; //
//layout(location = 1) in vec3 vertexNormal_model;   //* PARA MAC
//layout(location = 2) in vec2 texture_coord;   //
//layout(location = 3) in vec3 vertexTangent_model;
//layout(location = 4) in vec3 vertexBitangent_model;

in vec3 vertexPosition_model;    //
in vec3 vertexNormal_model;      //* PARA LINUX
in vec2 texture_coord;      //
in vec4 vertexTangent_model;				//
in vec4 vertexBitangent_model;

float inverse(float m);
mat2 inverse(mat2 m);
mat3 inverse(mat3 m);
mat4 inverse(mat4 m);




uniform mat4 view, proj, model;

out vec2 st;
out vec3 normal;
out vec3 positionEye;
out vec3 normalEye;


out vec3 EyeDirection_tangent;
out vec3 LightDirection_tangent;


void main() {
	st = texture_coord;
	normal = vertexNormal_model;
	normalEye = normalize(vec3(view * model * vec4(vertexNormal_model, 0.0)));
	positionEye = vec3(view * model * vec4(vertexPosition_model, 1.0));

	vec3 lightPosition_world = vec3(30, 100, 0);

	vec3 vertexPosition_eye = ( view * model * vec4(vertexPosition_model, 1)).xyz;
	vec3 cameraDirection_eye = vec3(0, 0, 0) - vertexPosition_eye;

	vec3 lightPosition_eye = (view * vec4(lightPosition_world, 1)).xyz;
	vec3 lightDirection_eye = lightPosition_eye + cameraDirection_eye;


	vec3 vertexNormal_eye = (view * model * vec4(normalize(vertexNormal_model), 0)).xyz;
  vec3 vertexTangent_eye = (view * model * normalize(vertexTangent_model)).xyz;
  vec3 vertexBitangent_eye = (view * model * normalize(vertexBitangent_model)).xyz;

	mat3 TBN = transpose(mat3(vertexTangent_eye, vertexBitangent_eye, vertexNormal_eye));

	LightDirection_tangent = TBN * lightDirection_eye;
	EyeDirection_tangent =  TBN * cameraDirection_eye;

	gl_Position = proj * view * model * vec4 (vertexPosition_model, 1.0);
}



float inverse(float m) {
    return 1.0 / m;
}

mat2 inverse(mat2 m) {
    return mat2(m[1][1],-m[0][1], -m[1][0], m[0][0]) / (m[0][0]*m[1][1] - m[0][1]*m[1][0]);
}

mat3 inverse(mat3 m) {
    float a00 = m[0][0], a01 = m[0][1], a02 = m[0][2];
    float a10 = m[1][0], a11 = m[1][1], a12 = m[1][2];
    float a20 = m[2][0], a21 = m[2][1], a22 = m[2][2];

    float b01 = a22 * a11 - a12 * a21;
    float b11 = -a22 * a10 + a12 * a20;
    float b21 = a21 * a10 - a11 * a20;

    float det = a00 * b01 + a01 * b11 + a02 * b21;

    return mat3(b01, (-a22 * a01 + a02 * a21), (a12 * a01 - a02 * a11),
                b11, (a22 * a00 - a02 * a20), (-a12 * a00 + a02 * a10),
                b21, (-a21 * a00 + a01 * a20), (a11 * a00 - a01 * a10)) / det;
}

mat4 inverse(mat4 m) {
  float a00 = m[0][0], a01 = m[0][1], a02 = m[0][2], a03 = m[0][3],
        a10 = m[1][0], a11 = m[1][1], a12 = m[1][2], a13 = m[1][3],
        a20 = m[2][0], a21 = m[2][1], a22 = m[2][2], a23 = m[2][3],
        a30 = m[3][0], a31 = m[3][1], a32 = m[3][2], a33 = m[3][3],

      b00 = a00 * a11 - a01 * a10,
      b01 = a00 * a12 - a02 * a10,
      b02 = a00 * a13 - a03 * a10,
      b03 = a01 * a12 - a02 * a11,
      b04 = a01 * a13 - a03 * a11,
      b05 = a02 * a13 - a03 * a12,
      b06 = a20 * a31 - a21 * a30,
      b07 = a20 * a32 - a22 * a30,
      b08 = a20 * a33 - a23 * a30,
      b09 = a21 * a32 - a22 * a31,
      b10 = a21 * a33 - a23 * a31,
      b11 = a22 * a33 - a23 * a32,

      det = b00 * b11 - b01 * b10 + b02 * b09 + b03 * b08 - b04 * b07 + b05 * b06;

  return mat4(
      a11 * b11 - a12 * b10 + a13 * b09,
      a02 * b10 - a01 * b11 - a03 * b09,
      a31 * b05 - a32 * b04 + a33 * b03,
      a22 * b04 - a21 * b05 - a23 * b03,
      a12 * b08 - a10 * b11 - a13 * b07,
      a00 * b11 - a02 * b08 + a03 * b07,
      a32 * b02 - a30 * b05 - a33 * b01,
      a20 * b05 - a22 * b02 + a23 * b01,
      a10 * b10 - a11 * b08 + a13 * b06,
      a01 * b08 - a00 * b10 - a03 * b06,
      a30 * b04 - a31 * b02 + a33 * b00,
      a21 * b02 - a20 * b04 - a23 * b00,
      a11 * b07 - a10 * b09 - a12 * b06,
      a00 * b09 - a01 * b07 + a02 * b06,
      a31 * b01 - a30 * b03 - a32 * b00,
      a20 * b03 - a21 * b01 + a22 * b00) / det;
}