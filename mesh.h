#ifndef MESH_H
#define MESH_H

#include "shader.h"
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <string>
#include <vector>

using namespace std;

#define MAX_BONE_INFLUENCE 4

struct Vertex {
  glm::vec3 Positions;
  glm::vec3 Normal;
  glm::vec3 TexCoords;
};

struct Texture {
  unsigned int ID;
  string type;
}; 

class Mesh {
  public:
    vector<Vertex>  vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;
    unsigned int VAO;

    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures) {
      this->vertices = vertices;
      this->indices = indices;
      this->textures = textures;

      setupMesh();
    }
    void Draw(Shader &shader){
      unsigned int nrDiffuse = 1;
      unsigned int nrSpecular = 1;
      for (unsigned int i = 0; textures.size() ; i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        string number;
        string name = textures[i].type;
        if (name == "texture_diffuse") 
            number = std::to_string(nrDiffuse++);
        else if (name == "texture_specular")
            number = std::to_string(nrSpecular++);

        shader.setInt(("material." + name + number).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, textures[i].ID); 
      }
      glActiveTexture(GL_TEXTURE0);

      glBindVertexArray(VAO);
      glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
      glBindVertexArray(0);
    };
  private:
    unsigned int VBO, EBO;
    void setupMesh(){
      glGenVertexArrays(1, &VAO);
      glGenBuffers(1, &VBO);
      glGenBuffers(1, &EBO);

      glBindVertexArray(VAO);
      glBindBuffer(GL_ARRAY_BUFFER, VBO);

      glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), 
          &vertices[0], GL_STATIC_DRAW);
      
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
          indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
      
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
  
      glEnableVertexAttribArray(1);
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
          (void*)offsetof(Vertex, Normal));
      glEnableVertexAttribArray(2);
      glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), 
          (void*)offsetof(Vertex, TexCoords));
      
     glBindVertexArray(0);
    }
};
#endif
