#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>
#include <array>
#include <vector>

#include "camera.hpp"
#include "model.hpp"

typedef struct {
    glm::vec3 position;
    glm::vec3 color;
} Light;

void glfw_error(const char* msg);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
GLFWwindow* create_window();
void process_input(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

float windowWidth = 800.0f;
float windowHeight = 600.0f;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

float deltaTime = 0.0f;

int main() {
    GLFWwindow* window = create_window();
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    std::vector<float> vertices { -0.5f, -0.5f, -0.5f, 0.5f, -0.5f, -0.5f, 0.5f,  0.5f, -0.5f, 0.5f,  0.5f, -0.5f, -0.5f,  0.5f, -0.5f,  -0.5f, -0.5f, -0.5f,  -0.5f, -0.5f,  0.5f, 0.5f, -0.5f,  0.5f, 0.5f,  0.5f,  0.5f, 0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f, -0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f, -0.5f,  0.5f, -0.5f,  0.5f,  0.5f, 0.5f,  0.5f,  0.5f, 0.5f,  0.5f, -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f,  0.5f, 0.5f,  0.5f,  0.5f, -0.5f, -0.5f, -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f,  0.5f, 0.5f, -0.5f,  0.5f, -0.5f, -0.5f,  0.5f, -0.5f, -0.5f, -0.5f, -0.5f,  0.5f, -0.5f, 0.5f,  0.5f, -0.5f, 0.5f,  0.5f,  0.5f, 0.5f,  0.5f,  0.5f, -0.5f,  0.5f,  0.5f, -0.5f,  0.5f, -0.5f };
    std::vector<float> normals { 0.0f,  0.0f, -1.0f, 0.0f,  0.0f, -1.0f, 0.0f,  0.0f, -1.0f, 0.0f,  0.0f, -1.0f, 0.0f,  0.0f, -1.0f, 0.0f,  0.0f, -1.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f, -1.0f,  0.0f, 0.0f,  1.0f,  0.0f, 0.0f,  1.0f,  0.0f, 0.0f,  1.0f,  0.0f, 0.0f,  1.0f,  0.0f, 0.0f,  1.0f,  0.0f, 0.0f,  1.0f,  0.0f };
    std::array<Light, 6> lights {{
        { glm::vec3(3.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f) },
        { glm::vec3(-3.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f) },
        { glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 1.0f) },
        { glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(0.96f, 1.0f, 0.0f) },
        { glm::vec3(0.0f, 3.0f, 0.0f), glm::vec3(1.0f, 0.58f, 0.0f) },
        { glm::vec3(0.0f, -3.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.9f) },
    }};

    Model cube(vertices, normals, {}, {}, "./assets/shaders/cube.vert", "./assets/shaders/cube.frag");
    Model light(vertices, {}, {}, {}, "./assets/shaders/light.vert", "./assets/shaders/light.frag");
    light.SetScale(glm::vec3(0.2f));

    float lastFrame = 0.0f;
    float rotationSpeed = 30.0f;

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        process_input(window);

        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.GetZoom()), (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);
        glm::mat4 lightRotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(rotationSpeed * deltaTime), glm::vec3(1.0f, 1.0f, 1.0f));
        glm::mat3 normalMatrix = glm::mat3(glm::transpose(glm::inverse(cube.GetMatrix())));

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        light.Begin();
        for (size_t i = 0; i < lights.size(); i++) {
            glm::vec4 rotatedPos = lightRotationMatrix * glm::vec4(lights[i].position, 1.0f);
            lights[i].position = glm::vec3(rotatedPos);

            light.SetPosition(lights[i].position);
            light.GetShader()->Set("uColor", lights[i].color);
            light.Draw(view, projection);
        }
        light.End();
        
        cube.Begin();
        cube.GetShader()->Set("uNormal", normalMatrix);
        
        cube.GetShader()->Set("uCameraPos", camera.GetPosition());
        cube.GetShader()->Set("uObjectColor", glm::vec3(1.0f, 1.0f, 1.0f));

        cube.GetShader()->Set("uLightColor1", lights[0].color);
        cube.GetShader()->Set("uLightPos1", lights[0].position);

        cube.GetShader()->Set("uLightColor2", lights[1].color);
        cube.GetShader()->Set("uLightPos2", lights[1].position);

        cube.GetShader()->Set("uLightColor3", lights[2].color);
        cube.GetShader()->Set("uLightPos3", lights[2].position);

        cube.GetShader()->Set("uLightColor4", lights[3].color);
        cube.GetShader()->Set("uLightPos4", lights[3].position);

        cube.GetShader()->Set("uLightColor5", lights[4].color);
        cube.GetShader()->Set("uLightPos5", lights[4].position);

        cube.GetShader()->Set("uLightColor6", lights[5].color);
        cube.GetShader()->Set("uLightPos6", lights[5].position);

        cube.Draw(view, projection);
        cube.End();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    exit(EXIT_SUCCESS);
}

void glfw_error(const char* msg) {
    const char* description;
    int code = glfwGetError(&description);
    std::cerr << msg << ": [" << code << "] " << (description ? description : "Unknown error") << std::endl;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    windowWidth = static_cast<float>(width);
    windowHeight = static_cast<float>(height);
    glViewport(0, 0, width, height);
}

GLFWwindow* create_window() {
    glfwSetErrorCallback([](int error, const char* description) {
        std::cerr << "GLFW Error [" << error << "]: " << description << std::endl;
    });

    if (!glfwInit()) {
        glfw_error("Failed to initialize GLFW");
        return nullptr;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(static_cast<int>(windowWidth), static_cast<float>(windowHeight), "Learn OpenGL", nullptr, nullptr);
    if (!window) {
        glfw_error("Failed to create GLFW window");
        return nullptr;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return nullptr;
    }

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    // glEnable(GL_FRAMEBUFFER_SRGB);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    int maxVertexUniforms = 0;
    glGetIntegerv(GL_MAX_VERTEX_UNIFORM_COMPONENTS, &maxVertexUniforms);
    std::cout << "Max vertex shader uniform components: " << maxVertexUniforms << std::endl;

    int maxFragmentUniforms = 0;
    glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &maxFragmentUniforms);
    std::cout << "Max fragment shader uniform components: " << maxFragmentUniforms << std::endl;

    return window;
}

void process_input(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {
    static bool firstMouse = true;
    static float lastX = windowWidth / 2.0f;
    static float lastY = windowHeight / 2.0f;

    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}