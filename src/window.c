/**
  *   @file window.c
  * 
  *   @author Guitter Julien
  *   @date 1 jan 2025
  */

#include <stdbool.h>
#include <GLFW/glfw3.h>
#include "window.h"
#include "stb_image.h"



#define MAX_OBJECTS             255
#define RECTANGLE_VERTEX_SIZE   20



static GLFWwindow* window;
static unsigned int shaderProgram;
static float vertices[RECTANGLE_VERTEX_SIZE];
static unsigned int VBO, EBO, VAO[MAX_OBJECTS];
static unsigned int texture[MAX_OBJECTS];
static int nbObjects = 0;
static int win_width, win_height;

static void (*renderCallback)(void);
static void (*mouseCallback)(double, double);


static const char* vertexshader = " \
    #version 330 core\n \
    layout (location = 0) in vec3 aPos;\n \
    layout (location = 1) in vec2 aTexCoord;\n \
    uniform vec3 uOffset;\n \
    out vec2 TexCoord;\n \
    void main()\n \
    {\n \
        gl_Position = vec4(aPos + uOffset, 1.0);\n \
        TexCoord = aTexCoord;\n \
    }\n \
";

static const char* fragmentshader = " \
    #version 330 core\n \
    out vec4 FragColor;\n \
    in vec2 TexCoord;\n \
    uniform sampler2D texture1;\n \
    uniform sampler2D texture2;\n \
    void main()\n \
    {\n \
        FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2);\n \
    }\n \
";

static const int RECTANGLE_INDICES[] = {
    0, 1, 3,
    1, 2, 3
};



void init_shader_program();
unsigned int load_texture(const char* path);
void setRectangleVertices(float vertex[], double width, double height);

float convert_to_opengl_pos_width(double pos);
float convert_to_opengl_pos_height(double pos);

void windowSizeCallback(GLFWwindow* window, int width, int height);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);





void init_window(int width, int height, bool resizable){

    win_width = width;
    win_height = height;

    // init window
    if (!glfwInit()) {
        printf("Failed to initialize GLFW\n");
        return;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GL_FALSE);

    if(!resizable){
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    }

    window = glfwCreateWindow(width, height, "Morpion", NULL, NULL);
    if (!window) {
        printf("Failed to create window\n");
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);
    glViewport(0, 0, width, height);
    glfwSetFramebufferSizeCallback(window, windowSizeCallback);
    
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    
    // Init shader
    init_shader_program();

    // Init mouse click
    glfwSetMouseButtonCallback(window, mouse_button_callback);
}

void set_render_callback(void (*callback)(void)){
    renderCallback = callback;
}

void set_mouse_callback(void (*callback)(double, double)){
    mouseCallback = callback;
}


void init_shader_program(){
    // init shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexshader, NULL);
    glCompileShader(vertexShader);
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
    }

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentshader, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n", infoLog);
    }

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", infoLog);
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

unsigned int load_texture(const char* path){
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    int width, height, nrChannels;
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data) {
        if (nrChannels == 3) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        } else if (nrChannels == 4) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        }
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        printf("Failed to load texture\n");
    }

    // Set png to be transparent
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    stbi_image_free(data);

    return texture;
}

void setRectangleVertices(float vertex[], double width, double height){
    float new_width = convert_to_opengl_pos_width(width);
    float new_height = convert_to_opengl_pos_height(height);

    float vertices[] = {
        -1.0f,              -1.0f + new_height,     0.0f,       0.0f, 1.0f,
        -1.0f,              -1.0f,                  0.0f,       0.0f, 0.0f,
        -1.0f + new_width,  -1.0f,                  0.0f,       1.0f, 0.0f,
        -1.0f + new_width,  -1.0f + new_height,     0.0f,       1.0f, 1.0f
    };
    for (int i = 0; i < RECTANGLE_VERTEX_SIZE; i++){
        vertex[i] = vertices[i];
    }
}

void free_window(){
    glDeleteProgram(shaderProgram);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    for (int i = 0; i < nbObjects; i++){
        glDeleteVertexArrays(1, &VAO[i]);
        glDeleteTextures(1, &texture[i]);
    }
    glfwDestroyWindow(window);
    glfwTerminate();
}

int windowShouldClose(){
    return glfwWindowShouldClose(window);
}



unsigned int add_object(const char* texture_path, double width, double height){
    if (nbObjects >= MAX_OBJECTS){
        printf("Max objects reached\n");
        return -1;
    }

    setRectangleVertices(vertices, width, width);

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glGenVertexArrays(1, &VAO[nbObjects]);
    glBindVertexArray(VAO[nbObjects]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(RECTANGLE_INDICES), RECTANGLE_INDICES, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //desactivate VAO and VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    texture[nbObjects] = load_texture(texture_path);

    return nbObjects++;

}

void draw_object(unsigned int object, double x, double y){
    int offsetLocation = glGetUniformLocation(shaderProgram, "uOffset");

    float new_x = convert_to_opengl_pos_width(x);
    float new_y = convert_to_opengl_pos_height(y);

    glUniform3fv(offsetLocation, 1, (float[]){new_x, new_y, 0.0f});

    glBindVertexArray(VAO[object]);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[object]);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void render_window(){
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);

    renderCallback();

    glfwSwapBuffers(window);
    glfwPollEvents();
}

float convert_to_opengl_pos_width(double pos){
    return ((float)pos / (float)win_width)*2;
}

float convert_to_opengl_pos_height(double pos){
    return ((float)pos / (float)win_height)*2;
}

void get_window_size(int* width, int* height){
    glfwGetFramebufferSize(window, width, height);
}



void windowSizeCallback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, 0, height, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    win_width = width;
    win_height = height;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) 
    {
        double xpos, ypos;
        //getting cursor position
        glfwGetCursorPos(window, &xpos, &ypos);

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        double x = xpos;
        double y = height - ypos;

        mouseCallback(x, y);
    }
}