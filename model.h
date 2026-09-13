#ifndef MODEL_H
#define MODEL_H
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model {
  public:
    Model(char *path) {
      loadModel(path);
    }
    void Draw(Shader &shader) {
      for(unsigned int i = 0; i < meshes.size() ; i++)
        meshes[i].Draw(shader);
    }

};

#endif
