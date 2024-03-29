#version 330 core

// note: add ability for multiple lights and lmao. pointlight need to be struct and so should direc and spotlight tbh.
// https://gamedev.stackexchange.com/questions/19575/is-there-a-way-to-use-an-arbitrary-number-of-lights-in-a-fragment-shader

// Output colors in RGBA
out vec4 FragColor;

//input normals and current viewer pos from vertex shader
in vec3 curPos;
in vec3 Normal;

// take in "color" and texCoords from vert shader
in vec3 color;
in vec2 texCoords;

//basically just an empty texture object. diffuse0 is for diffuse, specular0 is specular. 
uniform sampler2D diffuse0;
uniform sampler2D specular0;


//take in the color of light, the position of the light, and the position of the camera from the light object and camera object in main.cpp so we can calculate all of the neccessary light info
uniform vec4 lightColor;	//1, .55, 0 for a goodish torch color. This should be point/spotlight specific ONLY. It is a srs issue that it effects what is supposed to be the sun.  
uniform vec3 lightPos;
uniform vec3 camPos;

struct PointLight {
	vec3 position;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
//i dont like this but its going to have to do. for multiple lights, make an array of light vec3 positions, create a bunch of light objects, and then change this value to match how many lights ur rendering. 
#define NUM_POINT_LIGHTS 1		//changing this number defines how many lights are generated in the scene!!!! more lights than you have in the scene is bad bc the lights will still generate. 
uniform PointLight pointLights[NUM_POINT_LIGHTS];

vec4 pointLight(PointLight light) //this is the code for a point light. emits light from a specific point in space. In this game, stuff like torches. 
{
	//get distance to light. dist = length(vec lightPos - curPos)
	vec3 lightVec = light.position - curPos;
	//calculate the intensity of the light based on how far away the light is. In real life this has an inverse square relationship, but in graphics we use quadratic formuler
	//where a is the quadratic term and b is the linear term. Or, how fast it dies out, and how far it reaches. The last 1.0 is the constant 
	float dist = length(lightVec);
	float a = 1.0;	//these two could go in pointLight struct but im lazy lol 
	float b = 0.9;
	float inten = 1.0f / (a * dist * dist + b * dist + 1.0f); 

	float ambient = 0.00f; //change this when you want the ambient light to be brighter :)
	
	//calculate diffuse - tells the computer that the intensity gets less as the angle increases. called diffuse lighting. dot = cosine of angle between them. we take the max value between that and 0
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightVec);
	float diffuse = max(dot(normal, lightDirection), 0.0f);  

	//calculate specular light to add to the diffuse lighting in order to complete phong shading algo
	float specularLight = 0.5f; //max specular intensity. .5 default.
	vec3 viewDirection = normalize(camPos - curPos);
	//lightdirection is negative so that its toward the plane its reflecting and not away from it
	vec3 reflectionDirection = reflect(-lightDirection, normal); 
	
	//larger the angle between view and reflectDirection, the weaker we want specular light to be. The higher the power, the more point-like the specular light will be.
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16); 
	float specular = specAmount * specularLight;

	return (texture(diffuse0, texCoords) * (diffuse * inten + ambient) + texture(specular0, texCoords).r * specular * inten) * lightColor; //first calculate diffuse and ambient, then add specular values from map based on the angle of view etc. 
}

vec4 direcLight()	//light emitting from infinity away, meaning the light rays are all essentially parallel, like the sun.
{
	float ambient = 0.2f; 
	
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(vec3(1.0f, 1.0f, 0.0f)); //this should point in the opposite direction you want it to. eg you want it pointing down then it must go up

	//calculate diffuse - tells the computer that the intensity gets less as the angle increases. called diffuse lighting. dot = cosine of angle between them. we take the max value between that and 0
	float diffuse = max(dot(normal, lightDirection), 0.0f);  

	//calculate specular light to add to the diffuse lighting in order to complete phong lighting algo
	float specularLight = 0.5f; //maximum intensity of specular light. Default .5
	vec3 viewDirection = normalize(camPos - curPos);
	//lightdirection is negative so that its toward the plane its reflecting and not away from it
	vec3 reflectionDirection = reflect(-lightDirection, normal); 
	//larger the angle between view and reflectDirection, the weaker we want specular light to be. The higher the power, the more point-like the specular light will be.
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16); //last value is shininess
	float specular = specAmount * specularLight;

	vec4 sunColor = vec4(1.0f, 1.0f, 0.8f, 1.0f);

	return (texture(diffuse0, texCoords) * (diffuse + ambient) + texture(specular0, texCoords).r * specular) * sunColor; //first calculate diffuse and ambient, then add specular values from map based on the angle of view etc. 

}
vec4 spotLight()	//lights a conic area. creates two cones by taking the cosine values of two angles - the angle between inner cone and the direction of the light, and the angle between outer cone and lightdirec
{
	//two cones are used to create a nice gradient from light to dark. these are the cosine values - just hardcode them and dont base them off an angle
	//how big the area of the spotlight is: 
	float innerCone = 0.90f;
	float outerCone = 0.95f;
	
	float ambient = 0.20f;
	
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightPos - curPos);
	//calculate diffuse - tells the computer that the intensity gets less as the angle increases. called diffuse lighting. dot = cosine of angle between them. we take the max value between that and 0
	float diffuse = max(dot(normal, lightDirection), 0.0f);  

	//calculate specular light to add to the diffuse lighting in order to complete phong shading algo
	float specularLight = 0.50f; //maximum intensity of specular light//this should be .5 as a default i just turned it down for the wood
	vec3 viewDirection = normalize(camPos - curPos);
	
	//lightdirection is negative so that its toward the plane its reflecting and not away from it
	vec3 reflectionDirection = reflect(-lightDirection, normal); 
	
	//larger the angle between view and reflectDirection, the weaker we want specular light to be. The higher the power, the more point-like the specular light will be.
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16); 
	float specular = specAmount * specularLight;

	//calculate intensity based on angle to the center of the light cone
	float angle = dot(vec3(0.0f, -1.0f, 0.0f), -lightDirection);
	float inten = clamp((angle - outerCone) / (outerCone - innerCone), 0.0f, 1.0f); //clamp the two cone angle values between 0-1 to values between 0-1. 

	return (texture(diffuse0, texCoords) * (diffuse * inten + ambient) + texture(specular0, texCoords).r * specular * inten) * lightColor;
}

void main()
{
	vec4 Lightoutput = vec4(0.0f);
	Lightoutput += direcLight();

	for(int i = 0; i < NUM_POINT_LIGHTS; i++)
        Lightoutput += pointLight(pointLights[i]);    

	FragColor = Lightoutput;
}