#version 410 //130 linux - 410 mac

const int MAX_LIGHTS = 5;

in vec3 normalEye;
in vec3 normal;
in vec2 st;
in vec3 positionEye;
in vec4 shadowCoord;


in vec3 EyeDirection_tangent;
in vec3 LightDirection_tangent;
in vec3 lightDirection_eye;

in vec3 LightPositionSpots_tangent[MAX_LIGHTS];
in vec3 LightPositionSpots_eye[MAX_LIGHTS];
in vec3 LightDirectionSpots_eye[MAX_LIGHTS];
in vec3 LightDirectionSpots_tangent[MAX_LIGHTS];

out vec4 frag_colour;

uniform sampler2D basic_texture;
uniform sampler2D normal_map;
uniform sampler2DShadow shadow_map;
uniform mat4 view;

uniform vec3 lightPwr[MAX_LIGHTS];
uniform vec3 lightAngle[MAX_LIGHTS];
uniform int nLights;
uniform vec3 ks;

vec4 diffuse = vec4(0, 0, 0, 1);
float specular;
vec4 ambient = vec4(0.1, 0.1, 0.1, 1);


void main() {

	float visibility = texture( shadow_map, vec3(shadowCoord.xy, (shadowCoord.z)/shadowCoord.w) );


	vec3 normal_tan = texture (normal_map, st).rgb;
	normal_tan = normalize (normal_tan * 2.0 - 1.0);
	vec4 texel = texture (basic_texture, st);


	//MAIN DIRECTIONA LIGHT
	
	// Diffuse factor done in eye space	
	diffuse += vec4(1, 1, 1, 1) * clamp(dot(normalize(normalEye), lightDirection_eye), 0, 1);

	// Eye vector (towards the camera)
	vec3 E = normalize(EyeDirection_tangent);
	vec3 H = normalize(EyeDirection_tangent + normalize(LightDirection_tangent));
	// Direction in which the triangle reflects the light
	vec3 R = reflect(-normalize(LightDirection_tangent), normal_tan);
	float cosAlpha = clamp(dot(E, R), 0, 1);
	specular = pow(cosAlpha, 100);


	
	for(int i = 0; i < nLights; i++){
		float attenuation = 1.0f;
		vec3 direccionluz = normalize(LightPositionSpots_eye[i] - positionEye);
		float dis = distance(LightPositionSpots_eye[i], positionEye);
		float lightToSurfaceAngle = degrees(acos(dot(-direccionluz, normalize(LightDirectionSpots_eye[0]))));
		if(lightToSurfaceAngle > lightAngle[i].x){
			attenuation = 0.0;
		}else if (abs(lightToSurfaceAngle - lightAngle[i].x) <= 20.f){
			attenuation = clamp(abs(lightToSurfaceAngle - lightAngle[i].x)/4, 0, 1);
		}
		diffuse += attenuation * clamp(	dot(normalize(normalEye), direccionluz) * vec4(1,1,1, 1) , 0, 1) / (dis*0.1);
		
	}

	frag_colour = (texel * ambient) + ((texel * diffuse) + vec4(ks,1)*specular);
}
