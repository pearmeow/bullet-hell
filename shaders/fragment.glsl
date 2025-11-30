#version 330

const float RED_LUM_CONSTANT = 0.2126;
const float GREEN_LUM_CONSTANT = 0.7152;
const float BLUE_LUM_CONSTANT = 0.0722;

uniform sampler2D texture0;
uniform int invert;

in vec2 fragTexCoord;
in vec2 fragPosition;

out vec4 finalColor;

void main()
{
    vec4 color = texture(texture0, fragTexCoord);
    if (invert > 0) {
        finalColor = vec4(1 - color.r, 1 - color.g, 1 - color.b, color.a);
    } else {
        finalColor = color;
    }
}
