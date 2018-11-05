uniform sampler2D baseTexture;
uniform sampler2D normalTexture;
uniform sampler2D textureFlags;

#define solidTexture baseTexture
#define translucentTexture normalTexture
#define depthTexture textureFlags

void main(void)
{
	vec2 uv = gl_TexCoord[0].st;
	vec4 solid = texture2D(solidTexture, uv).rgba;
// 	vec4 translucent = texture2D(translucentTexture, uv).rgba;
// 	float depth = texture2D(depthTexture, uv).r;
	vec4 postfx = gl_Color;
	vec3 color = postfx.a * solid.rgb + postfx.rgb;
	gl_FragColor = vec4(color, 1.0);
}
