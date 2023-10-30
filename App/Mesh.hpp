#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <algorithm>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <common/texture.hpp>

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "external/stb_image.h"

class Mesh {
    public :

    GLuint vertexbuffer;
    GLuint elementbuffer;
    GLuint textureBuffer;
    GLuint normalBuffer;

    GLuint textureId;

    int id;

    std::vector<glm::vec3> indexed_vertices;
    std::vector<std::vector<unsigned short> > triangles;
    std::vector<float> textureData;
    std::vector<float> normals;

    std::vector<unsigned short> indices;

    Mesh() {
    }

    Mesh(int id) {
        this->id = id;
    }

    void sendDatas() {
        glGenBuffers(1, &vertexbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

        glGenBuffers(1, &elementbuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0] , GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
                    0,                  // attribute
                    3,                  // size
                    GL_FLOAT,           // type
                    GL_FALSE,           // normalized?
                    0,                  // stride
                    (void*)0            // array buffer offset
                    );

        glGenBuffers(1, &normalBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
        glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), &normals[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);


        glGenBuffers(1, &textureBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
        glBufferData(GL_ARRAY_BUFFER, textureData.size() * sizeof(float), &textureData[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), (void*)0);
        glEnableVertexAttribArray(2);

    }

    void drawMesh(GLuint programID) {

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, this->textureId);
        glUniform1i(glGetUniformLocation(programID, "objectTexture"), 0);

        glDrawElements(
                    GL_TRIANGLES,      // mode
                    this->indices.size(),    // count
                    GL_UNSIGNED_SHORT,   // type
                    (void*)0           // element array buffer offset
                    );
        
    }

    void deleteMeshBuffers() {
        glDeleteBuffers(1, &vertexbuffer);
        glDeleteBuffers(1, &elementbuffer);
        glDeleteBuffers(1, &normalBuffer);
        glDeleteBuffers(1, &textureBuffer);
    }

    void updateMeshData(std::vector<glm::vec3> newIndexedVertices, std::vector<float> newTextureData, std::vector<unsigned short> newIndices, std::vector<float> newNormals) {
        indexed_vertices = newIndexedVertices;
        indices = newIndices;
        normals = newNormals;
        textureData = newTextureData;
    }
};