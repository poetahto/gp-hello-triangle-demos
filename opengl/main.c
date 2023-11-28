#include <stdio.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void onWindowResize(GLFWwindow* window, int width, int height);
void checkForShaderCompilationErrors(unsigned int shader);
void checkForProgramCompilationErrors(unsigned int program);

int main() {
    // Setup
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", NULL, NULL);

    if (window == NULL) {
        printf("Failed to create window.\n");
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to initialize GLAD\n");
        glfwTerminate();
        return 2;
    }

    glfwSetFramebufferSizeCallback(window, onWindowResize);
    onWindowResize(window, 800, 600);

    // Create triangle data
    const char* vertexShaderSource = 
    "#version 330 core\n"
    "layout (location = 0) in vec2 vertInPos;\n"
    "layout (location = 1) in vec3 vertInColor;\n"
    "out vec3 fragInColor;\n"
    "void main() {\n"
    "    gl_Position = vec4(vertInPos, 0.0, 1.0);\n"
    "    fragInColor = vertInColor;\n"
    "}\0";

    const char* fragmentShaderSource = 
    "#version 330 core\n"
    "in vec3 fragInColor;\n"
    "out vec4 fragOutColor;\n"
    "void main() {\n"
    "    fragOutColor = vec4(fragInColor, 1.0);\n"
    "}\0";

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    checkForShaderCompilationErrors(vertexShader);

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    checkForShaderCompilationErrors(fragmentShader);

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    checkForProgramCompilationErrors(shaderProgram);

    float vertices[] = {
        -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.5f, 0.0f, 0.0f, 1.0f,
    };
    unsigned int vao;
    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); // position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))); // color attribute
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    // Rendering
    while (!glfwWindowShouldClose(window)) {
        glUseProgram(shaderProgram);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

void onWindowResize(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void checkForShaderCompilationErrors(unsigned int shader) {
    int success;
    char infoLog[513];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        infoLog[512] = '\0';
        printf("Failed to compile shader: %s\n", infoLog);
    }
}

void checkForProgramCompilationErrors(unsigned int program) {
    int success;
    char infoLog[513];
    glGetProgramiv(program, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        infoLog[512] = '\0';
        printf("Failed to compile program: %s\n", infoLog);
    }
}