#version 430
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

struct Data {
	vec2 pos;
	vec2 vel;
};

layout(std430, binding = 0) buffer InBuf 
{
	Data din[];
};

layout(std430, binding = 1) buffer OutBuf
{
	Data dout[];
};

uniform vec2 origin;
uniform float G;

void main()
{
	uint i = gl_GlobalInvocationID.x;
	dout[i].pos = din[i].pos + din[i].vel;
	if (abs(dout[i].pos.x) > 1.) dout[i].pos.x = -dout[i].pos.x;
	if (abs(dout[i].pos.y) > 1.) dout[i].pos.y = -dout[i].pos.y;
	vec2 r = normalize(din[i].pos - origin);
	dout[i].vel = 0.99 * din[i].vel; 			//friction
	dout[i].vel -= G * r; 			 			//gravity
	dout[i].vel += .06 * G * vec2(r.y, -r.x);	//spin
}