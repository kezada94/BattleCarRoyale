#version 130 //130 linux - 410 mac

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
uniform sampler2D shadowMap;


uniform mat4 view;

uniform vec3 lightPwr[MAX_LIGHTS];
uniform vec3 lightAngle[MAX_LIGHTS];
uniform int nLights;
uniform vec3 ks;

vec4 diffuse = vec4(0, 0, 0, 1);
vec4 diffuse2 = vec4(0, 0, 0, 1);
float specular;
vec4 ambient = vec4(0.1, 0.1, 0.1, 1);


vec2 poissonDisk[4] = vec2[](
  vec2( -0.94201624, -0.39906216 ),
  vec2( 0.94558609, -0.76890725 ),
  vec2( -0.094184101, -0.92938870 ),
  vec2( 0.34495938, 0.29387760 )
);

float eval_shadow (in vec4 texcoods) {
	// constant that you can use to slightly tweak the depth comparison
	float epsilon = 0.003; //el famoso BIAS
	float visibility = 1.0;
	if (texcoods.x > 1.0 || texcoods.x < 0.0 || texcoods.y > 1.0 || texcoods.y < 0.0 || texcoods.w < 0.0) {
		return 1.0; // do not add shadow/ignore
	}
	
	for (int i=0;i<4;i++){
  		if ( texture( shadowMap, texcoods.xy + poissonDisk[i]/800.0 ).z  <  texcoods.z-epsilon ){
    		visibility-=0.25;
  		}
	}
	return visibility; // not shadowed
}


void main() {
	vec4 coord_lightspace = shadowCoord;
	coord_lightspace.xyz /= coord_lightspace.w;
	coord_lightspace.xyz += 1.0;
	coord_lightspace.xyz *= 0.5;
		

	float shadow_factor = 1.0;
	vec3 normal_tan = texture (normal_map, st).rgb;
	normal_tan = normalize (normal_tan * 2.0 - 1.0);
	vec4 texel = texture (basic_texture, st);


	//MAIN DIRECTIONA LIGHT
	
	// Diffuse factor done in eye space	
	float dotp = dot(normalize(normalEye), lightDirection_eye);
	diffuse += vec4(1, 1, 1, 1) * clamp(dotp, 0, 1);

	if(dotp > 0.5){
	shadow_factor = eval_shadow (coord_lightspace);

	}

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
			continue;
		}else if (abs(lightToSurfaceAngle - lightAngle[i].x) <= 20.f){
			attenuation = clamp(abs(lightToSurfaceAngle - lightAngle[i].x)/4, 0, 1);
		}
		diffuse += attenuation * clamp(	dot(normalize(normalEye), direccionluz) * vec4(1,1,1, 1) , 0, 1) / (dis*0.4);
		float att = 1.0*attenuation;
		shadow_factor = clamp(att+shadow_factor, 0, 1);
	}
	frag_colour = ambient*texel+(shadow_factor)*((texel * diffuse) + vec4(ks,1)*specular);

}
