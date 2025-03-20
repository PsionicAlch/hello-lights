#version 330 core

in vec3 fPos;
in vec3 fNormal;

out vec4 oColor;

uniform vec3 uObjectColor;
uniform vec3 uCameraPos;

uniform vec3 uLightColor1;
uniform vec3 uLightPos1;

uniform vec3 uLightColor2;
uniform vec3 uLightPos2;

uniform vec3 uLightColor3;
uniform vec3 uLightPos3;

uniform vec3 uLightColor4;
uniform vec3 uLightPos4;

uniform vec3 uLightColor5;
uniform vec3 uLightPos5;

uniform vec3 uLightColor6;
uniform vec3 uLightPos6;

vec3 phong(vec3 lightColor, vec3 lightPosition) {
    float ambientStrenght = 0.1;
    vec3 ambient = ambientStrenght * lightColor;

    vec3 normal = normalize(fNormal);
    vec3 lightDir = normalize(lightPosition - fPos);
    
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    float specularStrenght = 0.5;
    vec3 viewDir = normalize(uCameraPos - fPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrenght * spec * lightColor;

    return ambient + diffuse + specular;
}

void main() {
    vec3 light1 = phong(uLightColor1, uLightPos1);
    vec3 light2 = phong(uLightColor2, uLightPos2);
    vec3 light3 = phong(uLightColor3, uLightPos3);
    vec3 light4 = phong(uLightColor4, uLightPos4);
    vec3 light5 = phong(uLightColor5, uLightPos5);
    vec3 light6 = phong(uLightColor6, uLightPos6);

    vec3 result = clamp(light1 + light2 + light3 + light4 + light5 + light6, 0.0, 1.0) * uObjectColor;
    oColor = vec4(result, 1.0);
}