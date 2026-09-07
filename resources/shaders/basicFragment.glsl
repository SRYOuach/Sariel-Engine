#version 400 core
out vec4 FragColor;

in vec3 Normals;
in vec2 TexCoord;
in vec3 fragPos;

const int MAX_TEXTURES = 16;

uniform sampler2D textures[MAX_TEXTURES];
uniform int activeTextureCount;

uniform float ambientIntensity = 0.1f;
uniform vec3 ambientColor = vec3(1.0f, 1.0f, 1.0f);

uniform bool isLight = false;

uniform vec3 diffuseLightPos;
uniform vec3 diffuseLightColor;
uniform float diffuseLightIntensity;


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
    if (!isLight)
    {
        vec4 ambient = vec4(ambientIntensity * ambientColor, 1.0f);
        vec3 lightDir = normalize(fragPos - diffuseLightPos);
        vec4 diffuse = vec4(diffuseLightIntensity * diffuseLightColor * max(dot(-lightDir, Normals), 0.0), 1.0);
        FragColor = (ambient + diffuse) * accumulatedColor;
    }
    else 
    {
      FragColor = accumulatedColor;
    }
}








