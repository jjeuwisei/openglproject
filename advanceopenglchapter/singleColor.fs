#version 330 core

out vec4 fragColor;
uniform sampler2D texture1;
in vec2 TexCoords;

void main() {
  vec3 result = texture(texture1, TexCoords).rgb;
  fragColor = vec4(result, 1.0);
}
