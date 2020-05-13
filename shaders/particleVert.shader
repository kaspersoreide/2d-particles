#version 430

layout(location = 0) in vec2 pos;
layout(location = 1) in vec2 vel;

out vec2 v;

void main() {
	v = vel;
	gl_Position = vec4(pos, 0.0, 1.0);
}