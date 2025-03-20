#include "model.hpp"

Model::Model(const std::vector<float>& vertices, const std::vector<float>& normals, const std::vector<float>& colors, const std::vector<float>& texCoords, const char* vertexPath, const char* fragmentPath) : m_VAO(0), m_VBO(0), m_indexCount(static_cast<GLsizei>(vertices.size())), m_Shader(std::make_shared<Shader>(vertexPath, fragmentPath)), m_Position(glm::vec3(0.0f)), m_Rotation(0.0f), m_RotationAxis(glm::vec3(1.0f, 1.0f, 1.0f)), m_Scale(glm::vec3(1.0f)) {
    setupModel(vertices, normals, colors, texCoords);
}

Model::Model(const std::vector<float>& vertices, const std::vector<float>& normals, const std::vector<float>& colors, const std::vector<float>& texCoords, std::shared_ptr<Shader> shader) : m_VAO(0), m_VBO(0), m_indexCount(static_cast<GLsizei>(vertices.size())), m_Shader(shader), m_Position(glm::vec3(0.0f)), m_Rotation(0.0f), m_RotationAxis(glm::vec3(1.0f, 0.3f, 0.5f)), m_Scale(glm::vec3(1.0f)) {
    setupModel(vertices, normals, colors, texCoords);
}

Model::~Model() {
    GL_CHECK(glDeleteVertexArrays(1, &m_VAO));
    GL_CHECK(glDeleteBuffers(1, &m_VBO));
}

void Model::Begin() const {
    GL_CHECK(glBindVertexArray(m_VAO));
    m_Shader->Use();
}

void Model::Draw(const glm::mat4& view, const glm::mat4& projection) const {
    m_Shader->Set("uModel", GetMatrix());
    m_Shader->Set("uView", view);
    m_Shader->Set("uProjection", projection);

    GL_CHECK(glDrawArrays(GL_TRIANGLES, 0, m_indexCount));
}

void Model::End() const {
    GL_CHECK(glBindVertexArray(0));
}

std::shared_ptr<Shader> Model::GetShader() const {
    return m_Shader;
}

void Model::SetPosition(float x, float y, float z) {
    m_Position = glm::vec3(x, y, z);
}

void Model::SetPosition(const glm::vec3& pos) {
    m_Position = glm::vec3(pos);
}

glm::vec3 Model::GetPosition() const {
    return m_Position;
}

void Model::SetRotationDeg(float deg) {
    m_Rotation = glm::radians(deg);
}

void Model::SetRotationRad(float rad) {
    m_Rotation = rad;
}

void Model::SetRotationAxis(float x, float y, float z) {
    m_RotationAxis = glm::vec3(x, y, z);
}

void Model::SetRotationAxis(const glm::vec3& axis) {
    m_RotationAxis = glm::vec3(axis);
}

void Model::SetScale(float x, float y, float z) {
    m_Scale = glm::vec3(x, y, z);
}

void Model::SetScale(const glm::vec3& scale) {
    m_Scale = glm::vec3(scale);
}

glm::mat4 Model::GetMatrix() const {
    glm::mat4 matrix(1.0f);
    matrix = glm::translate(matrix, m_Position);
    matrix = glm::rotate(matrix, m_Rotation, m_RotationAxis);
    matrix = glm::scale(matrix, m_Scale);

    return matrix;
}

void Model::setupModel(const std::vector<float>& vertices, const std::vector<float>& normals, const std::vector<float>& colors, const std::vector<float>& texCoords) {
    GLsizei stride = 3 * sizeof(float);

    // Determine if the colors and texCoords exist.
    bool hasNormals = !normals.empty();
    bool hasColors = !colors.empty();
    bool hasTexCoords = !texCoords.empty();

    if (hasNormals && normals.size() / 3 != vertices.size() / 3) {
        std::cerr << "Error: Mismatch in vertex and normal data size!\n";
        return;
    }

    if (hasColors && colors.size() / 3 != vertices.size() / 3) {
        std::cerr << "Error: Mismatch in vertex and color data size!\n";
        return;
    }

    if (hasTexCoords && texCoords.size() / 2 != vertices.size() / 3) {
        std::cerr << "Error: Mismatch in vertex and texture coordinates data size!\n";
        return;
    }

    // Compute stride.
    if (hasNormals) stride += 3 * sizeof(float);
    if (hasColors) stride += 3 * sizeof(float);
    if (hasTexCoords) stride += 2 * sizeof(float);

    std::vector<float> interleavedData;
    interleavedData.reserve(vertices.size() + (hasColors ? colors.size() : 0) + (hasTexCoords ? texCoords.size() : 0));

    size_t numVertices = vertices.size() / 3;
    for (size_t i = 0; i < numVertices; i++) {
        interleavedData.push_back(vertices[i * 3]);
        interleavedData.push_back(vertices[i * 3 + 1]);
        interleavedData.push_back(vertices[i * 3 + 2]);

        if (hasNormals) {
            interleavedData.push_back(normals[i * 3]);
            interleavedData.push_back(normals[i * 3 + 1]);
            interleavedData.push_back(normals[i * 3 + 2]);
        }

        if (hasColors) {
            interleavedData.push_back(colors[i * 3]);
            interleavedData.push_back(colors[i * 3 + 1]);
            interleavedData.push_back(colors[i * 3 + 2]);
        }

        if (hasTexCoords) {
            interleavedData.push_back(texCoords[i * 2]);
            interleavedData.push_back(texCoords[i * 2 + 1]);
        }
    }

    // OpenGL buffer setup.
    GL_CHECK(glGenVertexArrays(1, &m_VAO));
    GL_CHECK(glBindVertexArray(m_VAO));

    GL_CHECK(glGenBuffers(1, &m_VBO));
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
    GL_CHECK(glBufferData(GL_ARRAY_BUFFER, interleavedData.size() * sizeof(float), interleavedData.data(), GL_STATIC_DRAW));

    // Set up vertex attribute pointers.
    size_t offset = 0;
    GLuint vertexArrayIndex = 0;

    GL_CHECK(glVertexAttribPointer(vertexArrayIndex, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offset)));
    GL_CHECK(glEnableVertexAttribArray(vertexArrayIndex));
    
    offset += 3 * sizeof(float);
    vertexArrayIndex += 1;

    if (hasNormals) {
        GL_CHECK(glVertexAttribPointer(vertexArrayIndex, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offset)));
        GL_CHECK(glEnableVertexAttribArray(vertexArrayIndex));
        offset += 3 * sizeof(float);
        vertexArrayIndex += 1;
    }

    if (hasColors) {
        GL_CHECK(glVertexAttribPointer(vertexArrayIndex, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offset)));
        GL_CHECK(glEnableVertexAttribArray(vertexArrayIndex));
        offset += 3 * sizeof(float);
        vertexArrayIndex += 1;
    }

    if (hasTexCoords) {
        GL_CHECK(glVertexAttribPointer(vertexArrayIndex, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offset)));
        GL_CHECK(glEnableVertexAttribArray(vertexArrayIndex));
    }

    std::cout << interleavedData.size() * sizeof(float) << " bytes of data used to create model\n";

    // Clean up by undbinding the necessary buffers and objects.
    GL_CHECK(glBindVertexArray(0));
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, 0));
}