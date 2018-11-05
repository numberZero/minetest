uniform sampler2D baseTexture;
uniform sampler2D normalTexture;
uniform sampler2D textureFlags;

#define solidTexture baseTexture
#define translucentTexture normalTexture
#define depthTexture textureFlags

uniform vec4 skyBgColor;
uniform float fogDistance;

const float fogStart = FOG_START;
const float fogShadingParameter = 1 / (1 - fogStart);

void main(void)
{
	vec2 uv = gl_TexCoord[0].st;
	vec4 solid = texture2D(solidTexture, uv).rgba;
// 	vec4 translucent = texture2D(translucentTexture, uv).rgba;
	float depth = texture2D(depthTexture, uv).r;
	depth = 1.0 / (1.0 - depth);
	vec4 postfx = gl_Color;
	vec3 color = postfx.a * solid.rgb + postfx.rgb;

	float clarity = clamp(fogShadingParameter
		- fogShadingParameter * depth / fogDistance, 0.0, 1.0);
	color = mix(skyBgColor.rgb, color, clarity);

	gl_FragColor = vec4(color, 1.0);
// 	gl_FragColor = vec4(depth, depth, depth, 1.0);
}
