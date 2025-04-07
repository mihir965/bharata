#include "../grid/grid.h"
#include "../map/map.h"
#include <SDL2/SDL.h>
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>
#include <string.h>
#include <string_view>
#include <vector>

// GLOBALS
int gScreenHeight = 480;
int gScreenWidth = 640;
SDL_Window *gGraphicsApplicationWindow = nullptr;
SDL_GLContext gOpenGLContext = nullptr;

bool gQuit = false;
unsigned int VAO;
unsigned int VBO;
// unsigned int EBO;
unsigned int vertexShader;
unsigned int fragmentShader;
unsigned int shaderProgram;

void GetOpenGLVersionInfo() {
  std::cout << "Vendor" << glGetString(GL_VENDOR) << std::endl;
  std::cout << "Renderer" << glGetString(GL_RENDERER) << std::endl;
  std::cout << "Version" << glGetString(GL_VERSION) << std::endl;
}

void InitializeProgram() {

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cout << "SDL2 could not initialize video subsystem" << std::endl;
    exit(1);
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

  gGraphicsApplicationWindow = SDL_CreateWindow(
      "Bharata", 0, 0, gScreenWidth, gScreenHeight, SDL_WINDOW_OPENGL);

  if (gGraphicsApplicationWindow == nullptr) {
    std::cout << "SDL_Window was not able to be created" << std::endl;
    exit(1);
  }

  gOpenGLContext = SDL_GL_CreateContext(gGraphicsApplicationWindow);

  if (gOpenGLContext == nullptr) {
    std::cout << "OpenGL context not available" << std::endl;
    exit(1);
  }

  if (!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
    std::cout << "glad was not initialized" << std::endl;
    exit(1);
  }

  GetOpenGLVersionInfo();
}

std::string readVertexShader() {
  std::ifstream inputVertex;
  inputVertex.open("./src/shaders/vertex.vert");
  if (!inputVertex.is_open()) {
    std::cerr << "Therre was an error" << std::endl;
  }
  std::stringstream shaderStream;
  shaderStream << inputVertex.rdbuf();
  inputVertex.close();
  return shaderStream.str();
}

std::string readFragmentShader() {
  std::ifstream inputFragment;
  inputFragment.open("./src/shaders/fragment.frag");
  if (!inputFragment.is_open()) {
    std::cerr << "Therre was an error" << std::endl;
  }
  std::stringstream shaderStream;
  shaderStream << inputFragment.rdbuf();
  inputFragment.close();
  return shaderStream.str();
}

void VertexSpecification(const std::vector<std::vector<int>> &grid) {
  std::vector<float> vertices = getVertexData(grid);
  // std::vector<int> indices = getIndices();

  // Graphics Code

  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  // glGenBuffers(1, &EBO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
               vertices.data(), GL_STATIC_DRAW);

  // Position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // Color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  std::string vertexShaderCode = readVertexShader();

  const char *vertexShaderSource = vertexShaderCode.c_str();

  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  std::string fragmentShaderCode = readFragmentShader();
  const char *fragShaderSource = fragmentShaderCode.c_str();
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragShaderSource, NULL);
  glCompileShader(fragmentShader);

  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  glDetachShader(shaderProgram, vertexShader);
  glDetachShader(shaderProgram, fragmentShader);
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  glUseProgram(shaderProgram);

  glm::mat4 projection = glm::ortho(0.0f, 640.0f, 480.0f, 0.0f, -1.0f, 1.0f);

  GLint projLoc = glGetUniformLocation(shaderProgram, "uProjection");
  glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

void CreateGraphicsPipeline() {}

void Input() {
  SDL_Event e;

  while (SDL_PollEvent(&e) != 0) {
    if (e.type == SDL_QUIT) {
      std::cout << "Goodbye" << std::endl;
      gQuit = true;
    } else if (SDL_MOUSEMOTION == e.type) {
      int x, y;
      SDL_GetMouseState(&x, &y);
      // std::cout << x << ":" << y << std::endl;
    } else if (SDL_MOUSEBUTTONDOWN) {
      int x, y;
      SDL_GetMouseState(&x, &y);
      if (SDL_MOUSEBUTTONUP) {
        std::cout << x << ":" << y << std::endl;
      }
    }
  }
}

void PreDraw() {}

void Draw() {
  glUseProgram(shaderProgram);
  glBindVertexArray(VAO);
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glDrawArrays(GL_TRIANGLES, 0, 6 * 10 * 10);
  glBindVertexArray(0);
}

void MainLoop() {
  while (!gQuit) {
    Input();

    PreDraw();

    Draw();

    SDL_GL_SwapWindow(gGraphicsApplicationWindow);
  }
}

void CleanUp() {
  SDL_DestroyWindow(gGraphicsApplicationWindow);
  SDL_Quit();
}

int main() {
  std::srand(static_cast<unsigned>(std::time(
      nullptr))); // This is for making sure that the maps are different
  std::cout << "current working directory: "
            << std::filesystem::current_path().string() << std::endl;

  std::vector<std::vector<int>> grid = grid_init(MAP_HEIGHT, MAP_WIDTH);

  InitializeProgram();

  VertexSpecification(grid);

  CreateGraphicsPipeline();

  MainLoop();

  CleanUp();

  return 0;
}
