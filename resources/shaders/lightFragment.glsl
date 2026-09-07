#version 400 core
out vec4 FragColor;

in vec3 Normals;
in vec2 TexCoord;
in vec3 fragPos;

const int MAX_TEXTURES = 16;

uniform sampler2D textures[MAX_TEXTURES];
uniform int activeTextureCount;

void main(){
    vec4 accumulatedColor = vec4(0.0);
    for (int i = 0; i < activeTextureCount; i++)
    {
        accumulatedColor += texture(textures[i], TexCoord);
    }
    if (activeTextureCount > 0)
    {
        accumulatedColor = accumulatedColor / float(activeTextureCount);
    }
    else 
    {
        accumulatedColor = vec4(1.0f);
    }

      FragColor = accumulatedColor;
}








