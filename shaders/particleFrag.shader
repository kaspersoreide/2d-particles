#version 430

out vec4 theColor;

in vec3 c;

void main() {
	theColor = vec4(c, 1.);
}