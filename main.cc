#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <chrono>
#include <thread>

#include "glad/glad.h"
#include <GLFW/glfw3.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "BOX.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#define W 800
#define H 600

GLuint loadShader(GLuint shaderType, const std::string path) {
    std::ifstream file(path);    
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string bufferStr = buffer.str();
    const char* cStr = bufferStr.c_str();

    std::cout << "Loading shader... " << std::endl << bufferStr << std::endl;

    GLuint shader = glCreateShader(shaderType);    
    glShaderSource(shader, 1, &cStr, NULL);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR compiling shader: " << std::endl << infoLog << std::endl;
        return -1;
    }

    return shader;
    
}

GLuint vao, 
    posVBO,
    colorVBO, 
    normalVBO, 
    texCoordVBO,
    vShader,
    fShader,
    program,
    uModelViewProjMat,
    inPos,
    inColor,
    triangleIndexVBO;
glm::mat4 view, model, proj;

void InitProgramAndSetUniforms() {
    program = glCreateProgram();
    vShader = loadShader(GL_VERTEX_SHADER, "../shaders/vertex.vert");
    fShader = loadShader(GL_FRAGMENT_SHADER, "../shaders/fragment.frag");

    glAttachShader(program, vShader);
    glAttachShader(program, fShader);

    glLinkProgram(program);
    int linked;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked)
    {
        //Calculamos una cadena de error
        GLint logLen;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);
        char* logString = new char[logLen];
        glGetProgramInfoLog(program, logLen, NULL, logString);
        std::cout << "Error: " << logString << std::endl;
        delete[] logString;
        exit(-1);
    }
    
    std::cout << "Program linked!";

    // Load uniforms & Atribs
    uModelViewProjMat = glGetUniformLocation(program, "modelViewProjMat");

    inPos = glGetAttribLocation(program, "inPos");
    inColor = glGetAttribLocation(program, "inColor");    
}

void InitObj() {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &posVBO);
    glGenBuffers(1, &colorVBO);
    glGenBuffers(1, &normalVBO);
    glGenBuffers(1, &texCoordVBO);

    glBindBuffer(GL_ARRAY_BUFFER, posVBO);
    glBufferData(GL_ARRAY_BUFFER, cubeNVertex * sizeof(float) * 3,
        cubeVertexPos, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
    glBufferData(GL_ARRAY_BUFFER, cubeNVertex * sizeof(float) * 3,
        cubeVertexColor, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    if (inPos != -1)
        glEnableVertexAttribArray(inPos);

    if (inColor != -1)
        glEnableVertexAttribArray(inColor);
    
    glGenBuffers(1, &triangleIndexVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleIndexVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        cubeNTriangleIndex * sizeof(unsigned int) * 3, cubeTriangleIndex,
        GL_STATIC_DRAW);

    view = glm::lookAt(glm::vec3(0, 0, -6), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));          
    model = glm::mat4(1.0f);
}

float angle = 0;
float angleInc = 1;

void DrawScene() {   
    glUseProgram(program);

    angle += angleInc / 100;
    glm::mat4 newModel = glm::rotate(model, angle, glm::vec3(1, 0, 1));

    glm::mat4 modelView = view * newModel;
    glm::mat4 modelViewProj = proj * modelView;
    glm::mat4 normal = glm::transpose(glm::inverse(modelView));

    if (uModelViewProjMat != -1)
        glUniformMatrix4fv(uModelViewProjMat, 1, GL_FALSE,
            &(modelViewProj[0][0]));

    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, cubeNTriangleIndex * 3,
        GL_UNSIGNED_INT, (void*)0);
}

void DrawImGui() {
    // render your GUI
    ImGui::Begin("Control window");
    ImGui::SliderFloat("root speed", &angleInc, 0.0f, 10.0f);
    ImGui::End();

    // Render dear imgui into screen
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void WindowResized(GLFWwindow* window, int width, int height) {    
    glViewport(0, 0, width, height);
    proj = glm::perspective(glm::radians(60.0f), float(width) / float(height), 0.1f, 50.0f);

}


int main() {
  if (!glfwInit()) {
    std::cerr << "Error initializing GLfW";
    exit(-1);
  }

  GLFWwindow* window = glfwCreateWindow(W, H, "TestGL", NULL, NULL);

  if (!window) {
    glfwTerminate();
    std::cerr << "Error creating window";
    exit(-1);
  }

  glfwMakeContextCurrent(window);
  gladLoadGL();

  glEnable(GL_DEPTH_TEST);
  glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

  glFrontFace(GL_CCW);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glEnable(GL_CULL_FACE);

  glfwSetFramebufferSizeCallback(window, WindowResized);
  WindowResized(window, W, H);

  InitProgramAndSetUniforms();
  InitObj();

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  // Setup Platform/Renderer bindings  
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 330 core");
  // Setup Dear ImGui style
  ImGui::StyleColorsDark();

  double refTime = glfwGetTime();
    
  while (!glfwWindowShouldClose(window)) {
      if (glfwGetTime() - refTime < 0.0016) {
          continue;
      }
      
      glfwPollEvents();

      glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // feed inputs to dear imgui, start new frame
      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplGlfw_NewFrame();
      ImGui::NewFrame();
      
      DrawScene();
      DrawImGui();
     
      glfwSwapBuffers(window);      

      refTime = glfwGetTime();
  }

  glfwDestroyWindow(window);
  glfwTerminate();
}
