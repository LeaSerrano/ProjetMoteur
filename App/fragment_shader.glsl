#version 330 core

// Ouput data
out vec4 FragColor;

uniform sampler2D objectTexture;

in vec2 o_uv0;
in vec3 FragNormal;  
in vec3 FragPos;  
in vec3 FragLightDir;

uniform vec3 lightColor = vec3(1, 1, 1);
uniform vec3 ambientColor = vec3(0.8, 0.8, 0.8);
uniform vec3 diffuseColor = vec3(0.8, 0.8, 0.8);
uniform vec3 specularColor = vec3(0, 0, 0);
uniform float shininess = 80;

void main(){

    vec3 ambient = ambientColor * texture(objectTexture, o_uv0).rgb;
    vec3 norm = normalize(FragNormal);
    vec3 lightDir = normalize(FragLightDir);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * diffuseColor * texture(objectTexture, o_uv0).rgb;
    vec3 viewDir = normalize(-vec3(gl_FragCoord));
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = spec * specularColor;
    vec3 result = ambient + diffuse + specular;

    FragColor = vec4(result, 1.0);

}