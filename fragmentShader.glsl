#version 330 core

struct Material {
  sampler2D emission;
  sampler2D diffuse;
  sampler2D specular;
  float shininess;
};
uniform Material material;

struct pointLight {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  vec3 position;

  float constant;
  float linear;
  float quadratic;
};
#define NO_OF_PLIGHTS 4
uniform pointLight pLight[NO_OF_PLIGHTS];

struct dirLight {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
  vec3 direction;
};
uniform dirLight dLight;

struct Light {
  vec3 position;
  vec3 direction;
  
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  float constant;
  float linear;
  float quadratic;

  float cutoff;
  float outerCutoff;
};
uniform Light light;

struct spotLight {
  vec3 position;
  vec3 direction;
  float cutoff;
  float outerCutoff;

  float constant;
  float linear;
  float quadratic;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};
uniform spotLight spLight;

vec3 calculate_plight_color(pointLight light, vec3 normal, vec3 viewDir);
vec3 calculate_dlight_color(dirLight light, vec3 normal, vec3 viewDir);
vec3 calculate_spotlight_color(spotLight light, vec3 normal);

uniform vec3 objectColor;
uniform vec3 viewPos;
uniform float time;

in vec3 fragPos;
in vec3 normal;
in vec2 texCoords;

out vec4 fragColor;



void main() {
    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(viewPos - fragPos);

    vec3 result = calculate_dlight_color(dLight, norm, viewDir);    
    for(int i = 0; i < NO_OF_PLIGHTS; i++) {
      result += calculate_plight_color(pLight[i], norm, viewDir);
    }
    result += calculate_spotlight_color(spLight, norm);
    fragColor = vec4(result, 1.0);    
};

vec3 calculate_dlight_color(dirLight light, vec3 normal, vec3 viewDir) {
  vec3 lightDir = normalize(light.direction);
  
  float diff = max(dot(-lightDir, normal), 0.0);
  
  vec3 reflDir = reflect(lightDir, normal);
  float spec = pow(max(dot(reflDir, viewDir), 0.0), material.shininess);
  
  vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoords)); 
  vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoords));
  vec3 specular = spec * light.specular * vec3(texture(material.specular, texCoords));
  
  return ambient + diffuse + specular;
}

vec3 calculate_plight_color(pointLight light, vec3 normal, vec3 viewDir) {
  vec3 lightDir = light.position - fragPos;
  vec3 lightDirNorm = normalize(lightDir);
  
  float diff = max(dot(lightDirNorm, normal), 0.0);
  
  vec3 reflDir = reflect(-lightDirNorm, normal);
  float spec = pow(max(dot(reflDir, viewDir), 0.0), material.shininess); 
  
  float dist = length(lightDir);
  float attenuation = 1.0 / 
    (light.constant + light.linear * dist + light.quadratic * (dist * dist));
  
  vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoords));
  vec3 diffuse = diff * light.diffuse * vec3(texture(material.diffuse, texCoords));
  vec3 specular = spec * light.specular * vec3(texture(material.specular, texCoords));
  
  return (ambient + diffuse + specular) * attenuation;
}

vec3 calculate_spotlight_color(spotLight light, vec3 normal) {
  vec3 lightDir = normalize(light.position - fragPos);
  
  float diff = max(dot(lightDir, normal), 0.0);

  vec3 viewDir = normalize(viewPos - fragPos);
  vec3 reflDir = reflect(-lightDir, normal);
  float spec = pow(max(dot(reflDir, viewDir), 0.0), material.shininess);

  vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoords));
  vec3 diffuse = light.diffuse * vec3(texture(material.diffuse, texCoords));
  vec3 specular = spec * light.specular * vec3(texture(material.specular, texCoords));
  
  float dist = length(lightDir);
  float attenuation = 1.0 / 
    (light.constant + light.linear * dist + light.quadratic * (dist * dist));
  
  float theta = dot(lightDir, normalize(-light.direction));
  float epsilon = light.cutoff - light.outerCutoff;
  float intensity = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);
  
  return (ambient + diffuse + specular) * intensity * attenuation;
}
