#version 130

uniform sampler2D Render;
uniform sampler2D Tex0;
uniform sampler2D Tex1;

uniform vec2 PixelSize;
vec2 off = 2.0 * PixelSize;

uniform float dofstrength = 0.1;

const float fNear = 2.5e-4;
const float fFar = 1.0e-1;

void main()
{
	vec2 uv = gl_TexCoord[0].xy;
	float fDepth = texture2D(Tex1, vec2(0.5, 0.5)).x;
	float depth = texture2D(Tex1, uv).x;

	float b = log(clamp(depth, fNear, fFar)) - log(clamp(fDepth, fNear, fFar));
	b = 0.5 / (dofstrength * b * b);

	float bc0 = 1.0;
	float c = bc0;

	float bc1 = exp(-1.0 * b);
	float bc2 = exp(-2.0 * b);
	c += 4.0 * bc1 + 4.0 * bc2;

#ifdef EXTENDED_DOF_LIMIT
	float bc4 = exp(-4.0 * b);
	float bc5 = exp(-5.0 * b);
	c += 4.0 * bc4 + 8.0 * bc5;
#endif

#ifdef INSANE_DOF_LIMIT
	float bc8 = exp(-8.0 * b);
	float bc9 = exp(-9.0 * b);
	float bc10 = exp(-10.0 * b);
	c += 4.0 * bc8 + 4.0 * bc9 + 8.0 * bc10;
#endif

	bc0 /= c;
	bc1 /= c;
	bc2 /= c;

#ifdef EXTENDED_DOF_LIMIT
	bc4 /= c;
	bc5 /= c;
#endif

#ifdef INSANE_DOF_LIMIT
	bc8 /= c;
	bc9 /= c;
	bc10 /= c;
#endif

	vec4 color = vec4(0.0);
	vec2 tc = uv - vec2(PixelSize.x*0.5, PixelSize.y*0.5);

	color += texture2D(Tex0, tc + vec2(off.x *  0.0, off.y *  0.0)) * bc0;

	color += texture2D(Tex0, tc + vec2(off.x * -1.0, off.y *  0.0)) * bc1;
	color += texture2D(Tex0, tc + vec2(off.x * +1.0, off.y *  0.0)) * bc1;
	color += texture2D(Tex0, tc + vec2(off.x *  0.0, off.y * -1.0)) * bc1;
	color += texture2D(Tex0, tc + vec2(off.x *  0.0, off.y * +1.0)) * bc1;

	color += texture2D(Tex0, tc + vec2(off.x * -1.0, off.y * -1.0)) * bc2;
	color += texture2D(Tex0, tc + vec2(off.x * -1.0, off.y * +1.0)) * bc2;
	color += texture2D(Tex0, tc + vec2(off.x * +1.0, off.y * -1.0)) * bc2;
	color += texture2D(Tex0, tc + vec2(off.x * +1.0, off.y * +1.0)) * bc2;

#ifdef EXTENDED_DOF_LIMIT
	color += texture2D(Tex0, tc + vec2(off.x * -2.0, off.y *  0.0)) * bc4;
	color += texture2D(Tex0, tc + vec2(off.x *  0.0, off.y * -2.0)) * bc4;
	color += texture2D(Tex0, tc + vec2(off.x *  0.0, off.y * +2.0)) * bc4;
	color += texture2D(Tex0, tc + vec2(off.x * +2.0, off.y *  0.0)) * bc4;

	color += texture2D(Tex0, tc + vec2(off.x * -2.0, off.y * -1.0)) * bc5;
	color += texture2D(Tex0, tc + vec2(off.x * -2.0, off.y * +1.0)) * bc5;
	color += texture2D(Tex0, tc + vec2(off.x * -1.0, off.y * -2.0)) * bc5;
	color += texture2D(Tex0, tc + vec2(off.x * -1.0, off.y * +2.0)) * bc5;
	color += texture2D(Tex0, tc + vec2(off.x * +1.0, off.y * -2.0)) * bc5;
	color += texture2D(Tex0, tc + vec2(off.x * +1.0, off.y * +2.0)) * bc5;
	color += texture2D(Tex0, tc + vec2(off.x * +2.0, off.y * -1.0)) * bc5;
	color += texture2D(Tex0, tc + vec2(off.x * +2.0, off.y * +1.0)) * bc5;
#endif

#ifdef INSANE_DOF_LIMIT
	color += texture2D(Tex0, tc + vec2(off.x * -2.0, off.y * -2.0)) * bc8;
	color += texture2D(Tex0, tc + vec2(off.x * -2.0, off.y * +2.0)) * bc8;
	color += texture2D(Tex0, tc + vec2(off.x * +2.0, off.y * -2.0)) * bc8;
	color += texture2D(Tex0, tc + vec2(off.x * +2.0, off.y * +2.0)) * bc8;

	color += texture2D(Tex0, tc + vec2(off.x * -3.0, off.y *  0.0)) * bc9;
	color += texture2D(Tex0, tc + vec2(off.x *  0.0, off.y * -3.0)) * bc9;
	color += texture2D(Tex0, tc + vec2(off.x *  0.0, off.y * +3.0)) * bc9;
	color += texture2D(Tex0, tc + vec2(off.x * +3.0, off.y *  0.0)) * bc9;

	color += texture2D(Tex0, tc + vec2(off.x * -3.0, off.y * -1.0)) * bc10;
	color += texture2D(Tex0, tc + vec2(off.x * -3.0, off.y * +1.0)) * bc10;
	color += texture2D(Tex0, tc + vec2(off.x * -1.0, off.y * -3.0)) * bc10;
	color += texture2D(Tex0, tc + vec2(off.x * -1.0, off.y * +3.0)) * bc10;
	color += texture2D(Tex0, tc + vec2(off.x * +1.0, off.y * -3.0)) * bc10;
	color += texture2D(Tex0, tc + vec2(off.x * +1.0, off.y * +3.0)) * bc10;
	color += texture2D(Tex0, tc + vec2(off.x * +3.0, off.y * -1.0)) * bc10;
	color += texture2D(Tex0, tc + vec2(off.x * +3.0, off.y * +1.0)) * bc10;
#endif

	gl_FragData[0].rgba = color;
}
