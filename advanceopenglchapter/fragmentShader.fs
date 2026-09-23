#version 330 core 

out vec4 fragColor;

in vec2 texCoords;

uniform sampler2D texture1;

float offset = 1.0 / 300.0;

void main() 
{
  vec2 offsets[9] = vec2[]
  (
    vec2(-offset, offset),
    vec2(0.0, offset),
    vec2(offset, offset),
    vec2(-offset, 0.0),
    vec2(0.0, 0.0),
    vec2(offset, 0.0),
    vec2(-offset, -offset),
    vec2(0.0, -offset),
    vec2(offset, -offset)
  );

  float kernel[9] = float[]
  (
    1.0, 1.0, 1.0,
    1.0, -8, 1.0,
    1.0, 1.0, 1.0
  );

  vec3 sample[9];
  for(int i=0; i < 9; i++)
  {
    sample[i] = vec3(texture(texture1, texCoords.xy + offsets[i]));
  }
  vec3 color = vec3(0.0);
  for(int i=0; i < 9; i++)
  {
    color += sample[i] * kernel[i];
  }
  fragColor = vec4(color, 1.0);
}
