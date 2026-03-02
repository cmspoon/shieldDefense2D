#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTextureCoord;

out vec2 texture_coordinate;

flat out int textured;

uniform bool is_textured;
uniform mat4 model;

void main()
{
    if (is_textured) {
        textured = 1;
    } else {
        textured = 0;
    }
    gl_Position = model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    texture_coordinate = aTextureCoord;
}