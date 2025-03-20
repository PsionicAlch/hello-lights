#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <iostream>
#include <vector>
#include <memory>

#include "utility.hpp"
#include "shader.hpp"

class Model {
public:
    Model(const std::vector<float>& vertices, const std::vector<float>& normals, const std::vector<float>& colors, const std::vector<float>& texCoords, const char* vertexPath, const char* fragmentPath);
    Model(const std::vector<float>& vertices, const std::vector<float>& normals, const std::vector<float>& colors, const std::vector<float>& texCoords, std::shared_ptr<Shader> shader);
    ~Model();

    void Begin() const;
    void Draw(const glm::mat4& view, const glm::mat4& projection) const;
    void End() const;

    std::shared_ptr<Shader> GetShader() const;

    void SetPosition(float x, float y, float z);
    void SetPosition(const glm::vec3& pos);

    glm::vec3 GetPosition() const;

    void SetRotationDeg(float deg);
    void SetRotationRad(float rad);

    void SetRotationAxis(float x, float y, float z);
    void SetRotationAxis(const glm::vec3& axis);

    void SetScale(float x, float y, float z);
    void SetScale(const glm::vec3& scale);

    glm::mat4 GetMatrix() const;

private:
    GLuint m_VAO, m_VBO;
    GLsizei m_indexCount;
    
    std::shared_ptr<Shader> m_Shader;

    glm::vec3 m_Position;
    float m_Rotation;
    glm::vec3 m_RotationAxis;
    glm::vec3 m_Scale;

    void setupModel(const std::vector<float>& vertices, const std::vector<float>& normals, const std::vector<float>& colors, const std::vector<float>& texCoords);
};