#version 330 core 

out vec4 fragColor;

in vec3 Normal;
in vec3 Position;

uniform samplerCube skybox;
uniform vec3 CameraPos;

void main() 
{
  float ratio = 1.0 / 2.42;
  vec3 I = normalize(Position - CameraPos);
  vec3 R = refract(I, normalize(Normal), ratio);
  fragColor = texture(skybox, R);
}
