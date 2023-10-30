#include "Mesh.hpp"
#include "Transform.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <algorithm>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <list>
#include <memory>

class SceneGraphNode : public Mesh {

    public : 
    Transform transform;

    SceneGraphNode() : Mesh() {}

    SceneGraphNode(int id) : Mesh(id) {}

    std::vector<SceneGraphNode*> children;

    void addChild(SceneGraphNode* child) {
        children.push_back(child);
    }

    void update() {
        transform.modelMatrix = this->transform.changeModelMatrix();
    }

    void drawScene(GLuint programID) {
        this->sendDatas();
        this->drawMesh(programID);
    }

    void updateScene(std::vector<glm::vec3> indexed_vertices, std::vector<float> textureData, std::vector<unsigned short> indices, std::vector<float> normals) {
        this->updateMeshData(indexed_vertices, textureData, indices, normals);
    }

    void deleteBuffer() {
        this->deleteBuffer();
    }
};