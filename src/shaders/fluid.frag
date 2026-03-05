#version 330 core
in vec3 FragPos;
in vec3 Normal;
in vec3 LocalPos;

out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main() {
    if (abs(LocalPos.x) > 0.99 || abs(LocalPos.y) > 0.99 || abs(LocalPos.z) > 0.99)
        discard;

    vec3 waterColor = vec3(0.0, 0.45, 0.75);

    vec3 ambient = 0.3 * waterColor;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 norm = normalize(Normal);
    if (norm.y < 0.0) norm = -norm;

    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * waterColor;

    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16.0);
    vec3 specular = spec * vec3(0.3);

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 0.85);
}