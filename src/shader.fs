#version 330

in vec2 fragTexCoord;
out vec4 fragColor;

uniform sampler2D texture0;
uniform vec4 color;

void main()
{
    vec4 texel = texture(texture0, fragTexCoord);
    fragColor = vec4(texel.rgb * color.rgb, texel.a);
}