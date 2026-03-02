#version 330 core
out vec4 FragColor;

uniform vec4 set_color;
uniform sampler2D our_texture;

flat in int textured;
in vec2 texture_coordinate;

void main()
{
    if (textured > 0) {
        FragColor = texture(our_texture,texture_coordinate);
        return;
    }
    FragColor = set_color;
}