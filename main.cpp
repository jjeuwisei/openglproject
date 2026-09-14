#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.h"
#include "camera.h"
#include "model.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec3 lightPos = glm::vec3(3.0f, 2.0f, 2.0f);
glm::vec3 objectColor = glm::vec3(0.49f, 0.0f, 0.49f);
glm::vec3 objectPos = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 lightDirection = glm::vec3(-1.2f, -2.0f, -2.3f);

float deltaTime = 0.0f;
float lastFrame = 0.0f;
void process_input(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);
void framebuffer_resize_callback(GLFWwindow *window, int width, int height);
unsigned int load_texture(const char* path);

const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
bool firstMouse = true;

int main(int argc, char* argv[]) {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "opengl test", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
    
    float vertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
    };
    glm::vec3 cubePositions[] = {
    glm::vec3{ 5.00f, -2.29f,  5.00f},
    glm::vec3{-4.28f,  5.00f, -1.54f},
    glm::vec3{-5.00f,  0.56f, -5.00f},
    glm::vec3{ 1.49f, -5.00f,  3.97f},
    glm::vec3{-5.00f,  4.13f,  5.00f},
    glm::vec3{ 0.31f, -5.00f, -4.46f},
    glm::vec3{ 5.00f,  1.24f, -5.00f},
    glm::vec3{-2.74f, -5.00f,  0.93f},
    glm::vec3{ 5.00f,  5.00f, -3.38f},
    glm::vec3{-1.16f, -0.95f,  5.00f}
    };
    glm::vec3 pLight[4] = {
      glm::vec3{  1.95f, -6.65f, -3.15f },
      glm::vec3{ -3.88f,  3.31f,  2.47f }, 
      glm::vec3{  5.49f, -5.78f, -1.09f }, 
      glm::vec3{ -6.58f, -3.94f,  0.07f }  
    };
    glm::vec3 pDiff[4] = {
      glm::vec3{0.1f},
      glm::vec3{0.1f},
      glm::vec3{0.1f},
      glm::vec3{0.1f},
    };
    float pLinear[4] = {
      0.35f, 0.22f, 0.14f, 0.09f
    };
    float pQuadratic[4] = {
      0.44f, 0.20f, 0.07f, 0.032f
    };
    Shader ourShader("vertexShader.glsl", "fragmentShader.glsl");
    Shader lightShader("lightvertShader.glsl", "lightfragShader.glsl");
    Shader modelShader("modelfragshader.glsl", "modelshader.glsl");
    GLuint VAO[2];
    GLuint VBO;
    
    glGenBuffers(1, &VBO);
    glGenVertexArrays(2, VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 
        8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 
        (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    unsigned int diffuseMap, specularMap, emissionMap;
    stbi_set_flip_vertically_on_load(true);
    diffuseMap = load_texture("container2.png");
    specularMap = load_texture("container2_specular.png");
    emissionMap = load_texture("matrix.jpg");

    ourShader.use();
    ourShader.setInt("material.diffuse", 0);
    ourShader.setInt("material.specular", 1);
    ourShader.setInt("material.emission", 2);
    ourShader.setFloat("light.constant", 1.0);
    ourShader.setFloat("light.linear", 0.045);
    ourShader.setFloat("light.quadratic", 0.0075);
    ourShader.setFloat("light.cutoff", glm::cos(glm::radians(12.5f)));
    ourShader.setFloat("light.outerCutoff", glm::cos(glm::radians(25.0f)));
    for(unsigned int i = 0; i < 4; i++) {
      std::string name = "pLight[" + std::to_string(i) + "].";
      ourShader.setVec3((name + "position").c_str(), pLight[i]);
      ourShader.setVec3((name + "diffuse").c_str(), pDiff[i]);
      ourShader.setVec3((name + "ambient").c_str(), glm::vec3(0.01f));
      ourShader.setVec3((name + "specular").c_str(), glm::vec3(1.0f));
      ourShader.setFloat((name + "constant").c_str(), 1.0f);
      ourShader.setFloat((name + "linear").c_str(), pLinear[i]);
      ourShader.setFloat((name + "quadratic").c_str(), pQuadratic[i]);
    }
    ourShader.setVec3("dLight.diffuse", glm::vec3(0.1f));
    ourShader.setVec3("dLight.ambient", glm::vec3(0.01f));
    ourShader.setVec3("dLight.specular", glm::vec3(1.0f));
    ourShader.setVec3("dLight.direction", glm::vec3(-4.0f, 2.0f, -1.0f));
    
    ourShader.setVec3("spLight.diffuse", glm::vec3(1.0f));
    ourShader.setVec3("spLight.ambient", glm::vec3(0.1f));
    ourShader.setVec3("spLight.specular", glm::vec3(1.0f));
    ourShader.setFloat("spLight.linear", pLinear[0]);
    ourShader.setFloat("spLight.quadratic", pQuadratic[0]);
    ourShader.setFloat("spLight.constant", 1.0f);
    ourShader.setFloat("spLight.cutoff", glm::cos(glm::radians(12.0f)));
    ourShader.setFloat("spLight.outerCutoff", glm::cos(glm::radians(24.0f)));


    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseMap);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularMap);
        
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, emissionMap);

    glm::vec3 goldAmbient = glm::vec3(0.24725, 0.1995, 0.0745);
    glm::vec3 goldDiffuse = glm::vec3(0.75164, 0.60648, 0.22648);
    glm::vec3 goldSpecular = glm::vec3(0.62828, 0.5558, 0.36607);
    glm::vec3 plasticAmbient = glm::vec3(0.0, 0.0, 0.0);	
    glm::vec3 plasticDiffuse = glm::vec3(0.5, 0.0, 0.0);	
    glm::vec3 plasticSpecular = glm::vec3(0.7, 0.6, 0.6);	
    const float plasticShininess = 32.0;
    const float goldShininess = 51.2;
    Model ourModel("./backpack/backpack.obj");



    while (!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        //lightColor.x = sin(currentFrame) * 0.2f;
        //lightColor.y = sin(currentFrame) * 0.4f;
        //lightColor.z = sin(currentFrame) * 0.6f;
        //glm::vec3 lightAmbient = lightColor * 0.3f;
        //glm::vec3 lightDiffuse = lightColor * 0.7f;
        
        process_input(window);
        
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        glm::mat4 model = glm::mat4(1.0f);
        
        model = glm::translate(model, glm::vec3(0.3f));
        glm::mat4 projection = glm::perspective(glm::radians(90.0f), 
            (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.get_view_matrix();
        ourShader.use();
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);
        ourShader.setVec3("viewPos", camera.Position); 
        ourShader.setMat4("view", view);
        ourShader.setVec3("objectColor", objectColor);
        ourShader.setVec3("lightColor", lightColor);
         ourShader.setMat4("projection", projection);
         ourShader.setFloat("material.shininess",  goldShininess);
         ourShader.setVec3("light.position", camera.Position);
         ourShader.setVec3("light.ambient", glm::vec3(0.1f));
         ourShader.setVec3("light.diffuse", lightColor);
         ourShader.setVec3("light.specular", glm::vec3(1.0f));
         ourShader.setVec3("light.direction", camera.Front);
         ourShader.setVec3("spLight.direction", camera.Front);
         ourShader.setVec3("spLight.position", camera.Position);
         glBindVertexArray(VAO[0]);
         for(unsigned int i = 0; i < 10; i++) {
           model = glm::mat4(1.0f);
           model = glm::translate(model, cubePositions[i]);
           ourShader.setMat4("model", model);
           glDrawArrays(GL_TRIANGLES, 0, 36);
         }
          model = glm::mat4(1.0f);
          model = glm::translate(model, glm::vec3(0.5f));
          model = glm::scale(model, glm::vec3(0.4f));
          modelShader.use();
          modelShader.setMat4("model", model);
          modelShader.setMat4("projection", projection);
          modelShader.setMat4("view", view);
          ourModel.Draw(modelShader);
          lightShader.use();
          lightShader.setMat4("view", view);
          lightShader.setMat4("projection", projection);
          for(unsigned int i = 0; i < 4; i++) {
           model = glm::mat4(1.0f); 
           model = glm::translate(model, pLight[i]);
           lightShader.setMat4("model", model);
           lightShader.setVec3("lightPos", pLight[i]);
           lightShader.setVec3("lightColor", pDiff[i]);
           glBindVertexArray(VAO[1]);
           glDrawArrays(GL_TRIANGLES, 0, 36);
         }
          glfwSwapBuffers(window);
          glfwPollEvents();
    }
    glDeleteVertexArrays(2, VAO);
    glDeleteBuffers(1, &VBO);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
void process_input(GLFWwindow* window) {
  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
  if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    camera.process_keyboard(FORWARD, deltaTime);
  }
  if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    camera.process_keyboard(BACKWARD, deltaTime);
  }
  if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    camera.process_keyboard(LEFT, deltaTime);
  }
  if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    camera.process_keyboard(RIGHT, deltaTime);
  }
}

void mouse_callback(GLFWwindow* window, double xPosIn, double yPosIn) {
  float xPos = static_cast<float>(xPosIn);
  float yPos = static_cast<float>(yPosIn);
  if(firstMouse) {
    lastX = xPos;
    lastY = yPos;
    firstMouse = false;
  }
  float xoffset = xPos - lastX;
  float yoffset = lastY - yPos;
  
  lastX = xPos;
  lastY = yPos;
  camera.process_mouse_move(xoffset, yoffset);
}

void framebuffer_resize_callback(GLFWwindow* window, int width, int height) {
  glViewport(0, 0, width, height);
}

unsigned int load_texture(const char* path) {
  unsigned int textureID;
  glGenTextures(1, &textureID);

  int width, height, nrComponents;
  unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
  if(data) {
    GLenum format;
    if (nrComponents == 1)
        format = GL_RED;
    else if (nrComponents == 3)
        format = GL_RGB;
    else if (nrComponents == 4)
        format = GL_RGBA;
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
    stbi_image_free(data);
  }
  else {
    std::cout << "failed to load texture in: " << path << std::endl;
    stbi_image_free(data);
  }
  return textureID;
}
