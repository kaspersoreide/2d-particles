#version 430

layout(points) in;
layout(line_strip, max_vertices = 2) out;

in vec2 v[];

out vec3 c;

void main() {
	float vel = length(v[0]);
	c = vec3(1.0, 20.0 * vel, 0.0);
	vec4 p = gl_in[0].gl_Position;
	gl_Position = p;
	EmitVertex();
	c = vec3(0.3, 0.0, 0.0);
	gl_Position = p + vec4(v[0], .0, .0);
	EmitVertex();
	EndPrimitive();
}