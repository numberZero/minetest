#version 130

uniform sampler2D Render;
uniform sampler2D Tex0;
uniform sampler2D Tex1;

uniform vec2 PixelSize;
vec2 off = 2.0 * PixelSize;

float fNear = 2.5e-4;
float fFar = 1.0e-1;
float sigma = 2.0;
float coef = -1.0 / (2.0 * sigma * sigma);

void main()
{
	vec2 uv = gl_TexCoord[0].xy;
	float fDepth = texture2D(Tex1, vec2(0.5, 0.5)).x;
	float depth = texture2D(Tex1, uv).x;

	float b = log(clamp(depth, fNear, fFar)) - log(clamp(fDepth, fNear, fFar));
	float blur = 1.0 - exp(coef * b * b);

	b = 0.5 / (blur * blur);

	float bc0 = 1.0;
	vec4 bc1 = exp(-vec4(1.0, 2.0, 4.0, 5.0) * b);
	float c = bc0 + 4.0 * (bc1.x + bc1.y + bc1.z) + 8.0 * bc1.w;
	bc0 /= c;
	bc1 /= c;

	vec4 color = vec4(0.0);
	vec2 tc = uv - vec2(PixelSize.x*0.5, PixelSize.y*0.5);

	color += texture2D(Tex0, tc + vec2(off.x * -2.0, off.y * -1.0)) * bc1.w;
	color += texture2D(Tex0, tc + vec2(off.x * -2.0, off.y *  0.0)) * bc1.z;
	color += texture2D(Tex0, tc + vec2(off.x * -2.0, off.y * +1.0)) * bc1.w;
	color += texture2D(Tex0, tc + vec2(off.x * -1.0, off.y * -2.0)) * bc1.w;
	color += texture2D(Tex0, tc + vec2(off.x * -1.0, off.y * -1.0)) * bc1.y;
	color += texture2D(Tex0, tc + vec2(off.x * -1.0, off.y *  0.0)) * bc1.x;
	color += texture2D(Tex0, tc + vec2(off.x * -1.0, off.y * +1.0)) * bc1.y;
	color += texture2D(Tex0, tc + vec2(off.x * -1.0, off.y * +2.0)) * bc1.w;
	color += texture2D(Tex0, tc + vec2(off.x *  0.0, off.y * -2.0)) * bc1.z;
	color += texture2D(Tex0, tc + vec2(off.x *  0.0, off.y * -1.0)) * bc1.x;
	color += texture2D(Tex0, tc + vec2(off.x *  0.0, off.y *  0.0)) * bc0;
	color += texture2D(Tex0, tc + vec2(off.x *  0.0, off.y * +1.0)) * bc1.x;
	color += texture2D(Tex0, tc + vec2(off.x *  0.0, off.y * +2.0)) * bc1.z;
	color += texture2D(Tex0, tc + vec2(off.x * +1.0, off.y * -2.0)) * bc1.w;
	color += texture2D(Tex0, tc + vec2(off.x * +1.0, off.y * -1.0)) * bc1.y;
	color += texture2D(Tex0, tc + vec2(off.x * +1.0, off.y *  0.0)) * bc1.x;
	color += texture2D(Tex0, tc + vec2(off.x * +1.0, off.y * +1.0)) * bc1.y;
	color += texture2D(Tex0, tc + vec2(off.x * +1.0, off.y * +2.0)) * bc1.w;
	color += texture2D(Tex0, tc + vec2(off.x * +2.0, off.y * -1.0)) * bc1.w;
	color += texture2D(Tex0, tc + vec2(off.x * +2.0, off.y *  0.0)) * bc1.z;
	color += texture2D(Tex0, tc + vec2(off.x * +2.0, off.y * +1.0)) * bc1.w;

	gl_FragData[0].rgba = color;
}
