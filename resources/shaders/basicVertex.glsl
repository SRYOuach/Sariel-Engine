#version 400 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 Normals;
out vec3 fragPos;
out vec2 TexCoord;

uniform mat4 model = mat4(1.0f);
uniform mat4 view = mat4(1.0f);
uniform mat4 projection = mat4(1.0f);

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    Normals = aNormal;
    TexCoord = aTexCoord;
    fragPos = vec3(model * vec4(aPos, 1.0));
}
