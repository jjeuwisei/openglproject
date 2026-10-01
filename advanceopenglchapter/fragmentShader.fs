#version 330 core 

out vec4 fragColor;

in vec3 Normal;
in vec3 Position;

uniform samplerCube skybox;
uniform vec3 CameraPos;

void main() 
{
  vec3 I = normalize(Position - CameraPos);
  vec3 R = reflect(I, normalize(Normal));
  fragColor = texture(skybox, R);
}
