uniform sampler2D Render;
uniform sampler2D Tex0;
uniform sampler2D Tex1;

uniform vec2 PixelSize;

float Far = 20000.0;
float fLength = 12.0/Far;

void main()
{
	vec2 uv = gl_TexCoord[0].xy;
	float fDepth = texture2D(Tex1, vec2(0.5, 0.5)).x;
	float depth = texture2D(Tex1, uv).x;

	float b = depth / fDepth - 1.0;
	float blur = 1.0 - exp(-0.5 * b * b);
	blur = clamp(blur, 0.0, 1.0);

	float b1 = 1.0;
	float b2 = blur;
	float b3 = blur * blur;
	float c = b1 + 4.0 * b2 + 4.0 * b3;
	b1 /= c;
	b2 /= c;
	b3 /= c;

	vec4 color = vec4(0.0);
	vec2 tc = uv - vec2(PixelSize.x*0.5, PixelSize.y*0.5);

	color += texture2D(Tex0, tc + vec2(PixelSize.x * -2.0, PixelSize.y * -2.0)) * b3;
	color += texture2D(Tex0, tc + vec2(PixelSize.x * -2.0, PixelSize.y *  0.0)) * b2;
	color += texture2D(Tex0, tc + vec2(PixelSize.x * -2.0, PixelSize.y * +2.0)) * b3;
	color += texture2D(Tex0, tc + vec2(PixelSize.x *  0.0, PixelSize.y * -2.0)) * b2;
	color += texture2D(Tex0, tc + vec2(PixelSize.x *  0.0, PixelSize.y *  0.0)) * b1;
	color += texture2D(Tex0, tc + vec2(PixelSize.x *  0.0, PixelSize.y * +2.0)) * b2;
	color += texture2D(Tex0, tc + vec2(PixelSize.x * +2.0, PixelSize.y * -2.0)) * b3;
	color += texture2D(Tex0, tc + vec2(PixelSize.x * +2.0, PixelSize.y *  0.0)) * b2;
	color += texture2D(Tex0, tc + vec2(PixelSize.x * +2.0, PixelSize.y * +2.0)) * b3;

	gl_FragData[0].rgba = color;
}
