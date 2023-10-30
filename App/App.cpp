// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <map>
#include <iostream>
#include <algorithm>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

using namespace glm;

#include <common/shader.hpp>
#include <common/objloader.hpp>
#include <common/vboindexer.hpp>
#include <common/texture.hpp>

#include <string.h>
#include <sstream>
#include <fstream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "SceneGraphNode.hpp"

void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
glm::vec3 camera_position   = glm::vec3(3.5f, 1.0f, 10.0f);
glm::vec3 camera_target = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 camera_up    = glm::vec3(0.0f, 1.0f,  0.0f);

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

float timeSpeed = 1.0;

//rotation
float angle = 0.;
float zoom = 1.;

bool display_cameraOrbitale = false;

bool PkeyIsPressed = false;
bool SemicolonkeyIsPressed = false;
bool CkeyIsPressed = false;
bool UpKeyIsPressed = false;
bool DownKeyIsPressed = false;
bool LeftKeyIsPressed = false;
bool RightKeyIsPressed = false;

bool shouldRenderFireball = false;
bool shouldRenderCat = true;

bool isPowerOn = false;

float timeCst = 0;

float vitesse = 4.f;
float verticalSpeed = 4.f;
float verticalSpeed2 = 4.f;
float rightVerticalSpeed = 0.f;

const float gravity = 10.f;

bool pressSpace = false;
bool pressRightArrow = false;
bool pressLeftArrow = false;
bool pressE = false;

float jumpSpeed = 0.0f; // vitesse initiale de saut
bool isJumping = false; // état de saut


std::vector<glm::vec3> indexed_vertices;
std::vector<std::vector<unsigned short> > triangles;
std::vector<float> textureData;
std::vector<unsigned short> indices;
std::vector<float> normals;

SceneGraphNode* rootNodeMovement = new SceneGraphNode();
SceneGraphNode* chat = new SceneGraphNode();
SceneGraphNode* background = new SceneGraphNode();
SceneGraphNode* bouleFeu = new SceneGraphNode();
SceneGraphNode* ennemi1 = new SceneGraphNode();
SceneGraphNode* ennemi2 = new SceneGraphNode();
SceneGraphNode* power1 = new SceneGraphNode();
SceneGraphNode* power2 = new SceneGraphNode();

std::map<SceneGraphNode*, std::vector<float>> ennemisList;
std::map<SceneGraphNode*, bool> powerList;

std::map<SceneGraphNode*, std::string> terrainTypeObjets;
std::map<SceneGraphNode*, bool> afficheCarton;

float objectHeight = 0.0f;
float vitesseFlottement = 0.005f; 


bool collisionAuSolSaut = false;
bool collisionAuPlafondSaut = false;
bool collisionAuMurDroiteSaut = false;
bool collisionAuMurGaucheSaut = false;
bool collisionAuMurDroiteMarche = false;
bool collisionAuMurGaucheMarche = false;
bool collisionAuSolMarche = false;
bool collisionAuSolBouleFeu = false;
bool catIsKilled = false;

float velocite = 0.2f;

std::vector<SceneGraphNode*> terrainObjectList;

std::vector terrainPoints = {
    /*Terre*/ glm::vec2(0, 0), glm::vec2(2, 0), glm::vec2(4, 0), glm::vec2(8, 0), glm::vec2(10, 0), glm::vec2(20, 0), glm::vec2(22, 0), glm::vec2(24, 0), glm::vec2(26, 2), glm::vec2(30, 2), glm::vec2(44, 8), glm::vec2(68, 4), glm::vec2(70, 4), glm::vec2(82, 8), glm::vec2(84, 6), glm::vec2(86, 4), glm::vec2(88, 4), glm::vec2(92, 4), glm::vec2(94, 4), glm::vec2(100, 4), glm::vec2(102, 4), glm::vec2(104, 4),
    /*Terre2*/ glm::vec2(26, 0), glm::vec2(28, 0), glm::vec2(30, 0), glm::vec2(82, 6), glm::vec2(82, 4), glm::vec2(84, 4),
    /*Brique*/ glm::vec2(12, 0), glm::vec2(14, 2), glm::vec2(14, 0), glm::vec2(16, 4), glm::vec2(16, 2), glm::vec2(16, 0), glm::vec2(18, 6), glm::vec2(18, 4), glm::vec2(18, 2), glm::vec2(18, 0), glm::vec2(72, 4), glm::vec2(74, 4), glm::vec2(74, 6), glm::vec2(76, 4), glm::vec2(76, 6), glm::vec2(76, 8), glm::vec2(78, 4), glm::vec2(78, 6), glm::vec2(78, 8), glm::vec2(78, 10), glm::vec2(80, 4), glm::vec2(80, 6), glm::vec2(80, 8), glm::vec2(80, 10),
    /*Carton*/ glm::vec2(20, 6), glm::vec2(98, 12),
    /*Eau*/ glm::vec2(28, 2), glm::vec2(66, 4),
    /*Pierre*/ glm::vec2(32, 4), glm::vec2(36, 4),
    /*Pierre2*/ glm::vec2(32, 2), glm::vec2(32, 0), glm::vec2(32, -2), glm::vec2(36, 2), glm::vec2(36, 0), glm::vec2(36, -2),
    /*Or*/ glm::vec2(38, 6),
    /*Grattoir*/ glm::vec2(52, 8), glm::vec2(54, 8), glm::vec2(56, 8),
    /*Glace*/ glm::vec2(60, 4), glm::vec2(62, 4), glm::vec2(64, 4),
    /*Bloc UR*/ glm::vec2(86, 14), glm::vec2(92, 12),
    /*Bloc UG*/ glm::vec2(88, 14), glm::vec2(96, 12)};

/*******************************************************************************/

bool loadObjFile(const char* filename, std::vector<glm::vec3>& vertices, std::vector<float>& texcoords, std::vector<unsigned short>& indices, std::vector<float>& normals)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        return false;
    }

    aiMesh* mesh = scene->mMeshes[0];

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        aiVector3D vertex = mesh->mVertices[i];
        aiVector3D normal = mesh->mNormals[i];
        aiVector3D texcoord = mesh->mTextureCoords[0][i];

        glm::vec3 vertexVec3 = glm::vec3(vertex.x, vertex.y, vertex.z);
        vertices.push_back(vertexVec3);

        normals.push_back(normal.x);
        normals.push_back(normal.y);
        normals.push_back(normal.z);

        texcoords.push_back(texcoord.x);
        texcoords.push_back(texcoord.y);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    return true;
}


GLuint loadTexture(const char* filename) {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    int width, height, numComponents;
    unsigned char* imageData = stbi_load(filename, &width, &height, &numComponents, 0);

        if (imageData == NULL) {
            std::cerr << "Erreur lors du chargement de l'image" << std::endl;
        }

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        size_t length = strlen(filename);
        const char* lastThreeChars = &filename[length - 3];

        
         if (strcmp(lastThreeChars, "jpg") == 0) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
         }
         else if (strcmp(lastThreeChars, "png") == 0) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
         }


        stbi_image_free(imageData);

        glBindTexture(GL_TEXTURE_2D, textureID);
        glEnable(GL_TEXTURE_2D);


    return textureID;
}

void createTerrain() {

    for (int i = 0; i < 76; ++i) {
        terrainObjectList.push_back(new SceneGraphNode(i));

        if (i < 22) {
            loadObjFile("cube_test.obj",indexed_vertices, textureData, indices, normals);
            terrainObjectList[i]->updateMeshData(indexed_vertices, textureData, indices, normals);
            terrainObjectList[i]->textureId = loadTexture("texture_terre.png");
            terrainObjectList[i]->transform.position.x = terrainPoints[i][0];
            terrainObjectList[i]->transform.position.y = terrainPoints[i][1];
            terrainTypeObjets.insert(std::make_pair(terrainObjectList[i], "bloc"));
        }
        else if (i < 28) {
            loadObjFile("cube_test.obj",indexed_vertices, textureData, indices, normals);
            terrainObjectList[i]->updateMeshData(indexed_vertices, textureData, indices, normals);
            terrainObjectList[i]->textureId = loadTexture("texture_terre_2.png");
            terrainObjectList[i]->transform.position.x = terrainPoints[i][0];
            terrainObjectList[i]->transform.position.y = terrainPoints[i][1];
            terrainTypeObjets.insert(std::make_pair(terrainObjectList[i], "bloc"));
        }
        else if (i < 52) {
            loadObjFile("cube_test.obj",indexed_vertices, textureData, indices, normals);
            terrainObjectList[i]->updateMeshData(indexed_vertices, textureData, indices, normals);
            terrainObjectList[i]->textureId = loadTexture("texture_terre_2.png");
            terrainObjectList[i]->transform.position.x = terrainPoints[i][0];
            terrainObjectList[i]->transform.position.y = terrainPoints[i][1];
            terrainObjectList[i]->textureId = loadTexture("texture_brique.png");
            terrainTypeObjets.insert(std::make_pair(terrainObjectList[i], "bloc"));
        }
        else if (i < 54) {
            loadObjFile("cube_test.obj",indexed_vertices, textureData, indices, normals);
            terrainObjectList[i]->updateMeshData(indexed_vertices, textureData, indices, normals);
            terrainObjectList[i]->textureId = loadTexture("texture_carton.png");
            terrainObjectList[i]->transform.position.x = terrainPoints[i][0];
            terrainObjectList[i]->transform.position.y = terrainPoints[i][1];
            terrainTypeObjets.insert(std::make_pair(terrainObjectList[i], "blocCarton"));
            afficheCarton.insert(std::make_pair(terrainObjectList[i], true));
        }
        else if (i < 56) {
            loadObjFile("cube_test.obj",indexed_vertices, textureData, indices, normals);
            terrainObjectList[i]->updateMeshData(indexed_vertices, textureData, indices, normals);
            terrainObjectList[i]->textureId = loadTexture("texture_terre_eau.png");
            terrainObjectList[i]->transform.position.x = terrainPoints[i][0];
            terrainObjectList[i]->transform.position.y = terrainPoints[i][1];
            terrainTypeObjets.insert(std::make_pair(terrainObjectList[i], "blocEau"));
        }
        else if (i < 58) {
            loadObjFile("cube_test.obj",indexed_vertices, textureData, indices, normals);
            terrainObjectList[i]->updateMeshData(indexed_vertices, textureData, indices, normals);
            terrainObjectList[i]->textureId = loadTexture("texture_brique_grise.png");
            terrainObjectList[i]->transform.position.x = terrainPoints[i][0];
            terrainObjectList[i]->transform.position.y = terrainPoints[i][1];
            terrainTypeObjets.insert(std::make_pair(terrainObjectList[i], "bloc"));
        }
        else if (i < 64) {
            loadObjFile("cube_test.obj",indexed_vertices, textureData, indices, normals);
            terrainObjectList[i]->updateMeshData(indexed_vertices, textureData, indices, normals);
            terrainObjectList[i]->textureId = loadTexture("texture_brique_grise_2.png");
            terrainObjectList[i]->transform.position.x = terrainPoints[i][0];
            terrainObjectList[i]->transform.position.y = terrainPoints[i][1];
            terrainTypeObjets.insert(std::make_pair(terrainObjectList[i], "bloc"));
        }
        else if (i < 65) {
            loadObjFile("cubeor.obj",indexed_vertices, textureData, indices, normals);
            terrainObjectList[i]->updateMeshData(indexed_vertices, textureData, indices, normals);
            terrainObjectList[i]->textureId = loadTexture("texture_or.png");
            terrainObjectList[i]->transform.position.x = terrainPoints[i][0];
            terrainObjectList[i]->transform.position.y = terrainPoints[i][1];
            terrainTypeObjets.insert(std::make_pair(terrainObjectList[i], "blocOr"));
        }
        else if (i < 68) {
            loadObjFile("cube_test.obj",indexed_vertices, textureData, indices, normals);
            terrainObjectList[i]->updateMeshData(indexed_vertices, textureData, indices, normals);
            terrainObjectList[i]->textureId = loadTexture("texture_poisson.png");
            terrainObjectList[i]->transform.position.x = terrainPoints[i][0];
            terrainObjectList[i]->transform.position.y = terrainPoints[i][1];
            terrainTypeObjets.insert(std::make_pair(terrainObjectList[i], "blocRalentissement"));
        }
        else if (i < 71) {
            loadObjFile("glacon.obj",indexed_vertices, textureData, indices, normals);
            terrainObjectList[i]->updateMeshData(indexed_vertices, textureData, indices, normals);
            terrainObjectList[i]->textureId = loadTexture("texture_glacon.png");
            terrainObjectList[i]->transform.position.x = terrainPoints[i][0];
            terrainObjectList[i]->transform.position.y = terrainPoints[i][1];
            terrainTypeObjets.insert(std::make_pair(terrainObjectList[i], "blocGlacon"));
        }
        else if (i < 73) {
            loadObjFile("cubeU.obj",indexed_vertices, textureData, indices, normals);
            terrainObjectList[i]->updateMeshData(indexed_vertices, textureData, indices, normals);
            terrainObjectList[i]->textureId = loadTexture("texture_U_gris.png");
            terrainObjectList[i]->transform.position.x = terrainPoints[i][0];
            terrainObjectList[i]->transform.position.y = terrainPoints[i][1];
            terrainTypeObjets.insert(std::make_pair(terrainObjectList[i], "bloc"));
        }
        else if (i < 75) {
            loadObjFile("cubeU.obj",indexed_vertices, textureData, indices, normals);
            terrainObjectList[i]->updateMeshData(indexed_vertices, textureData, indices, normals);
            terrainObjectList[i]->textureId = loadTexture("texture_U_gris.png");
            terrainObjectList[i]->transform.position.x = terrainPoints[i][0];
            terrainObjectList[i]->transform.position.y = terrainPoints[i][1];
            terrainTypeObjets.insert(std::make_pair(terrainObjectList[i], "bloc"));
        }
        else if (i < 76) {
            loadObjFile("cube_test.obj",indexed_vertices, textureData, indices, normals);
            terrainObjectList[i]->updateMeshData(indexed_vertices, textureData, indices, normals);
            terrainObjectList[i]->textureId = loadTexture("texture_slime.png");
            terrainObjectList[i]->transform.position.x = terrainPoints[i][0];
            terrainObjectList[i]->transform.position.y = terrainPoints[i][1];
            terrainTypeObjets.insert(std::make_pair(terrainObjectList[i], "blocSaut"));
        }

        terrainObjectList[i]->transform.rotation.y += 270;
        terrainObjectList[i]->transform.position.z = 6;

        rootNodeMovement->addChild(terrainObjectList[i]);

        indexed_vertices.clear();
        textureData.clear();
        indices.clear();
        normals.clear();
    }


}


bool checkCollisionSol(vec3 object1Position, vec3 object2Position) {
    if (object1Position.y < 1 + object2Position.y && object1Position.y > object2Position.y + 0.2 && object1Position.x < 1.5 + object2Position.x && object1Position.x > object2Position.x - 1.5) {
        return true;
    }

    return false;
}

bool checkCollisionPlafond(vec3 object1Position, vec3 object2Position) {
    if (object1Position.y < object2Position.y && object1Position.y > object2Position.y - 3 && object1Position.x < 1.5 + object2Position.x && object1Position.x > object2Position.x - 1.5) {
        return true;
    }

    return false;
}

bool checkCollisionMurDroite(vec3 object1Position, vec3 object2Position) {
    if (object1Position.x + 1.5 >= object2Position.x  - 0.5 && object1Position.x + 1.5 <= object2Position.x && object1Position.y + 2 <= object2Position.y + 1 && object1Position.y + 2 >= object2Position.y) {
        //std::cout << object1Position.x << " " << object2Position.x << " " << object1Position.y << " " << object2Position.y << std::endl;
        return true;
    }

    return false;
}

bool checkCollisionMurGauche(vec3 object1Position, vec3 object2Position) {
    if (object1Position.x - 1.5 <= object2Position.x + 0.5 && object1Position.x - 1.5 >= object2Position.x && object1Position.y + 1.5 <= object2Position.y + 1 && object1Position.y + 1.5 >= object2Position.y) {
        return true;
    }

    return false;
}

bool checkCollisionBetweenPlayerAndEnnemi(const SceneGraphNode *p, const SceneGraphNode *e) {

    if (p->transform.position.x > e->transform.position.x - 1.75 && 
        p->transform.position.x < e->transform.position.x + 1.75 &&
        p->transform.position.y > e->transform.position.y - 1.5 && 
        p->transform.position.y < e->transform.position.y + 1.5
        ) {
            return true;
        }
        
    return false;
}


int main( void )
{
    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow( 1024, 768, "Projet - GLFW", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    // Hide the mouse and enable unlimited mouvement

    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(window, 1024/2, 768/2);

    glClearColor(0.1, 0.1, 0.1, 1.0);
    //glClearColor(0.8f, 0.8f, 0.8f, 0.0f);


    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // Cull triangles which normal is not towards the camera
    //glEnable(GL_CULL_FACE);


    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // Create and compile our GLSL program from the shaders
    GLuint programID = LoadShaders("vertex_shader.glsl", "fragment_shader.glsl" );

    //Terrain
    createTerrain();

    int blocOr_dir = 1;
    float blocOr_dist = 0.f;

    indexed_vertices.clear();
    textureData.clear();
    indices.clear();
    normals.clear();

    //Image de fond
    loadObjFile("Background_v3.obj", indexed_vertices, textureData, indices, normals);
    background->updateMeshData(indexed_vertices, textureData, indices, normals);
    background->transform.position.x += -80;
    background->transform.position.y += -2;
    background->transform.position.z += 0;
    background->textureId = loadTexture("Background_fond_v3_small.png");
    rootNodeMovement->addChild(background);

    indexed_vertices.clear();
    textureData.clear();
    indices.clear();
    normals.clear();

    //Joueur
    loadObjFile("chat_omabuarts_skinfeuv3.obj", indexed_vertices, textureData, indices, normals);
    chat->updateMeshData(indexed_vertices, textureData, indices, normals);
    chat->transform.scale /= 2;
    chat->transform.position.x = 0;
    chat->transform.position.z = 6;
    chat->transform.rotation.y += 90;
    chat->transform.position.y = terrainObjectList[1]->transform.position.y + 1;
    chat->textureId = loadTexture("chat_omabuarts_v2.png");
    rootNodeMovement->addChild(chat);

    indexed_vertices.clear();
    textureData.clear();
    indices.clear();
    normals.clear();

    //Boule de feu quand elle est lancée
    loadObjFile("bouledefeu.obj", indexed_vertices, textureData, indices, normals);
    bouleFeu->updateMeshData(indexed_vertices, textureData, indices, normals);
    bouleFeu->transform.scale /= 5;
    bouleFeu->transform.rotation.y += 90;
    bouleFeu->textureId = loadTexture("bouledefeu.png");
    rootNodeMovement->addChild(bouleFeu);

    indexed_vertices.clear();
    textureData.clear();
    indices.clear();
    normals.clear();

    //Pouvoir à récupérer
    loadObjFile("bouledefeu.obj", indexed_vertices, textureData, indices, normals);
    power1->updateMeshData(indexed_vertices, textureData, indices, normals);
    power1->transform.scale /= 5;
    power1->transform.position.x = 10;
    power1->transform.position.y = 2 - 1.25;
    power1->transform.position.z = 6;
    power1->textureId = loadTexture("bouledefeu.png");
    rootNodeMovement->addChild(power1);

    indexed_vertices.clear();
    textureData.clear();
    indices.clear();
    normals.clear();

    loadObjFile("bouledefeu.obj", indexed_vertices, textureData, indices, normals);
    power2->updateMeshData(indexed_vertices, textureData, indices, normals);
    power2->transform.scale /= 5;
    power2->transform.position.x = 56;
    power2->transform.position.y = 10 - 1.25;
    power2->transform.position.z = 6;
    power2->textureId = loadTexture("bouledefeu.png");
    rootNodeMovement->addChild(power2);

    indexed_vertices.clear();
    textureData.clear();
    indices.clear();
    normals.clear();

    powerList.insert(std::make_pair(power1, true));
    powerList.insert(std::make_pair(power2, true));


    //Ennemis
     loadObjFile("cucumber_3.obj", indexed_vertices, textureData, indices, normals);
    ennemi1->updateMeshData(indexed_vertices, textureData, indices, normals);
    ennemi1->transform.position.x = 22;
    ennemi1->transform.position.y = 0;
    ennemi1->transform.position.z = 6;
    ennemi1->textureId = loadTexture("texture_cucumber.png");
    rootNodeMovement->addChild(ennemi1);

    indexed_vertices.clear();
    textureData.clear();
    indices.clear();
    normals.clear();

    loadObjFile("cucumber_3.obj", indexed_vertices, textureData, indices, normals);
    ennemi2->updateMeshData(indexed_vertices, textureData, indices, normals);
    ennemi2->transform.position.x = 68;
    ennemi2->transform.position.y = 6;
    ennemi2->transform.position.z = 6;
    ennemi2->textureId = loadTexture("texture_cucumber.png");
    rootNodeMovement->addChild(ennemi2);

    indexed_vertices.clear();
    textureData.clear();
    indices.clear();
    normals.clear();

    ennemisList.insert(std::make_pair(ennemi1, std::vector<float>{1.f, 1.f, 2.f, 4.f}));
    ennemisList.insert(std::make_pair(ennemi2, std::vector<float>{1.f, 1.f, 0.f, 2.f}));

    // For speed computation
    double lastTime = glfwGetTime();
    int nbFrames = 0;
    bool ischatLoaded = true;
    bool isCubeLoaded = false;

    float dist_bloc_spe = 0.f;
    int dir_bloc_spe = 1;

    do{

        // Measure speed
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);


        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use our shader
        glUseProgram(programID);

        glm::mat4 modelMatrix, viewMatrix, projectionMatrix;

        viewMatrix = glm::lookAt(glm::vec3(chat->transform.position.x, 4, chat->transform.position.z + 20.0), glm::vec3(chat->transform.position.x, 4, chat->transform.position.z + 20.0) + camera_target, glm::vec3(0.0f, 1.0f, 0.0f));
        projectionMatrix = glm::perspective(glm::radians(45.0f), 4.0f/3.0f, 0.1f, 100.0f);

        glUniformMatrix4fv(glGetUniformLocation(programID, "view"), 1 , GL_FALSE, &viewMatrix[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(programID, "projection"), 1 , GL_FALSE, &projectionMatrix[0][0]);

        if (shouldRenderCat) {
            glUniformMatrix4fv(glGetUniformLocation(programID, "model"), 1, false, &chat->transform.changeModelMatrix()[0][0]);
            chat->drawScene(programID);
        }
        else {
            chat->transform.position.x = -0.5;
            chat->transform.position.z = 6;
            chat->transform.position.y = terrainObjectList[1]->transform.position.y + 1;

            rightVerticalSpeed = 0.0f;

            for (auto it = afficheCarton.begin(); it != afficheCarton.end(); ++it) {
                afficheCarton[it->first] = true;
            }

            glUniformMatrix4fv(glGetUniformLocation(programID, "model"), 1, false, &chat->transform.changeModelMatrix()[0][0]);
            chat->drawScene(programID);

            catIsKilled = false;

            shouldRenderCat = true;
        }

        for (auto it = terrainTypeObjets.begin(); it != terrainTypeObjets.end(); ++it) {
            if ((it->second == "blocCarton" && afficheCarton[it->first] == true) || it->second != "blocCarton") {
                glUniformMatrix4fv(glGetUniformLocation(programID, "model"), 1, false, &(it->first)->transform.changeModelMatrix()[0][0]);
                (it->first)->drawScene(programID);
            }
            
            if (it->second == "blocOr") {
                if (blocOr_dir == 1 && blocOr_dist > 14.f) {
                    blocOr_dir = -1;
                }
                else if (blocOr_dir == -1 && blocOr_dist < 0.f) {
                    blocOr_dir = 1;
                }

                if (blocOr_dir == 1) {
                    it->first->transform.position.x += 0.05f;
                    blocOr_dist += 0.05f;            
                }
                else {
                    it->first->transform.position.x -= 0.05f;
                    blocOr_dist -= 0.05f;  
                }
            }
        }

        glUniformMatrix4fv(glGetUniformLocation(programID, "model"), 1, false, &background->transform.changeModelMatrix()[0][0]);
        background->drawScene(programID);


        for (auto it = ennemisList.begin(); it != ennemisList.end(); ++it) {

            if (it->second[0]) {
                if (checkCollisionBetweenPlayerAndEnnemi(chat, (it->first))) {

                    if (chat->transform.position.x <= (it->first)->transform.position.x) {
                        chat->transform.position.x -= 0.1;
                    }
                    else {
                        chat->transform.position.x += 0.1;
                    }
                    if (isPowerOn) {
                        chat->textureId = loadTexture("chat_texture.jpg");
                        isPowerOn = false;
                        shouldRenderFireball = false;
                    }
                    else {
                        catIsKilled = true;
                    }
                }

                if (it->second[1] == 1 && it->second[2] > 1.f) {
                    it->second[1] = -1;
                }
                else {
                    if (it->second[1] == -1 && it->second[2] < 0.f) {
                        it->second[1] = 1;
                    }
                }

                if (it->second[1] == 1) {
                    it->first->transform.position.x += 0.1f;
                    it->second[2] += 0.1f;
                }
                else {
                    it->first->transform.position.x -= 0.1f;
                    it->second[2] -= 0.1f;
                }
                
                glUniformMatrix4fv(glGetUniformLocation(programID, "model"), 1, false, &(it->first)->transform.changeModelMatrix()[0][0]);
                (it->first)->drawScene(programID);
            }
        }

        for (auto it = powerList.begin(); it != powerList.end(); ++it) {

            if (it->second) {
                if (chat->transform.position.x >= (it->first)->transform.position.x - 0.8 && chat->transform.position.x <= (it->first)->transform.position.x + 0.8) {
                    chat->textureId = loadTexture("chat_omabuarts_v3.1.png");
                    it->second = false;
                    isPowerOn = true;
                }
                
                glUniformMatrix4fv(glGetUniformLocation(programID, "model"), 1, false, &(it->first)->transform.changeModelMatrix()[0][0]);
                (it->first)->drawScene(programID);
            }
        }

        if (shouldRenderFireball) {
            glUniformMatrix4fv(glGetUniformLocation(programID, "model"), 1, false, &bouleFeu->transform.changeModelMatrix()[0][0]);
            bouleFeu->drawScene(programID);
        }
        else {
            if (chat->transform.rotation.y == -90) {
                bouleFeu->transform.position.x = chat->transform.position.x - 1.5f;
            }
            else {
                bouleFeu->transform.position.x = chat->transform.position.x + 0.5;
            }
            bouleFeu->transform.position.y = chat->transform.position.y + 0.5;
            bouleFeu->transform.position.z = chat->transform.position.z;
        }

        if(chat->transform.position.y < -3 || catIsKilled) {
            shouldRenderCat = false;
            chat->textureId = loadTexture("chat_texture.jpg");
            isPowerOn = false;
            shouldRenderFireball = false;

            for (auto it = powerList.begin(); it != powerList.end(); ++it) {
                it->second = true;
            }

            for (auto it = ennemisList.begin(); it != ennemisList.end(); ++it) {
                it->second[0] = true;
            }

        }

        if (pressSpace) {
            auto indexCollisionAuSol = terrainTypeObjets.begin();

            for (auto it = terrainTypeObjets.begin(); it != terrainTypeObjets.end(); ++it) {
                if (checkCollisionSol(chat->transform.position, (it->first)->transform.position)) {
                    collisionAuSolSaut = true;
                    indexCollisionAuSol = it;
                    break;
                }
                else if (checkCollisionPlafond(chat->transform.position, (it->first)->transform.position)) {
                    collisionAuPlafondSaut = true;
                    break;
                }
            }

            if (collisionAuSolSaut) {
                verticalSpeed = vitesse;
                chat->transform.position.y += verticalSpeed * deltaTime;
                pressSpace = false;
                collisionAuSolSaut = false;

                if (indexCollisionAuSol->second == "blocCarton") {
                    afficheCarton[indexCollisionAuSol->first] = false;
                    rightVerticalSpeed -= gravity * deltaTime;
                    chat->transform.position.y += rightVerticalSpeed * deltaTime;
                    
                }
            }
            else if (collisionAuPlafondSaut) {
                verticalSpeed -= gravity * (deltaTime * 4);
                chat->transform.position.y += verticalSpeed * (deltaTime * 4);
                collisionAuPlafondSaut = false;
            }
            else {
                verticalSpeed -= gravity * deltaTime;
                chat->transform.position.y += verticalSpeed * (deltaTime * 4);
            }

            for (auto it = terrainTypeObjets.begin(); it != terrainTypeObjets.end(); ++it) {
                if (checkCollisionMurDroite(chat->transform.position, (it->first)->transform.position)) {
                    collisionAuMurDroiteSaut = true;
                    break;
                }
                else if (checkCollisionMurGauche(chat->transform.position, (it->first)->transform.position) ) {
                    collisionAuMurGaucheSaut = true;
                    break;
                }
                
            }

            if (collisionAuMurDroiteSaut) {
                chat->transform.position.x -= 0.2;
                collisionAuMurDroiteSaut = false;
            }
            else if (collisionAuMurGaucheSaut) {
                chat->transform.position.x += 0.2;
                collisionAuMurGaucheSaut = false;
            }
        }

        if (pressRightArrow || pressLeftArrow) {
            if (pressRightArrow) {
                chat->transform.position.x += velocite;
                chat->transform.rotation.y = 90;
            }
            else if (pressLeftArrow) {
                chat->transform.position.x -= velocite;
                chat->transform.rotation.y = -90.f;
            }

            auto indexCollisionAuSol = terrainTypeObjets.begin();

            for (auto it = terrainTypeObjets.begin(); it != terrainTypeObjets.end(); ++it) {
                if (checkCollisionMurDroite(chat->transform.position, (it->first)->transform.position)) {
                    collisionAuMurDroiteMarche = true;
                    break;
                }
                else if (checkCollisionMurGauche(chat->transform.position, (it->first)->transform.position) ) {
                    collisionAuMurGaucheMarche = true;
                    break;
                }
                
            }

            if (collisionAuMurDroiteMarche) {
                chat->transform.position.x -= 0.2;
                collisionAuMurDroiteMarche = false;
            }
            else if (collisionAuMurGaucheMarche) {
                chat->transform.position.x += 0.2;
                collisionAuMurGaucheMarche = false;
            }

            indexCollisionAuSol = terrainTypeObjets.begin();

            for (auto it = terrainTypeObjets.begin(); it != terrainTypeObjets.end(); ++it) {
                if (checkCollisionSol(chat->transform.position, (it->first)->transform.position)) {
                    collisionAuSolMarche = true;
                    indexCollisionAuSol = it;
                    break;
                }
            }

            if (collisionAuSolMarche) {
                collisionAuSolMarche = false;

                if (indexCollisionAuSol->second == "blocGlacon") {
                    if (pressRightArrow) {
                        chat->transform.position.x += velocite * 2;
                    }
                    else if (pressLeftArrow) {
                        chat->transform.position.x -= velocite * 2;
                    }
                }
                else if (indexCollisionAuSol->second == "blocRalentissement") {
                    if (pressRightArrow) {
                        chat->transform.position.x -= velocite/1.5;
                    }
                    else if (pressLeftArrow) {
                        chat->transform.position.x += velocite/1.5;
                    }
                }
                else if (indexCollisionAuSol->second == "blocCarton") {
                    afficheCarton[indexCollisionAuSol->first] = false;
                    rightVerticalSpeed -= 0 * deltaTime;
                    chat->transform.position.y += rightVerticalSpeed * deltaTime;
                }
                else if (indexCollisionAuSol->second == "blocRebondissant") {
                    pressSpace = true;
                    verticalSpeed = vitesse * 2;
                    chat->transform.position.y += verticalSpeed * deltaTime;
                    collisionAuSolSaut = false;
                }
                else if (indexCollisionAuSol->second == "blocEau") {
                    chat->textureId = loadTexture("chat_texture.jpg");
                    isPowerOn = false;
                    shouldRenderFireball = false;
                }

                pressLeftArrow = false;
                pressRightArrow = false;

            }
            else {
                rightVerticalSpeed -= gravity * deltaTime;
                chat->transform.position.y += rightVerticalSpeed * deltaTime;
            }

        }

        //std::cout << chat->transform.position.x << " " << chat->transform.position.y << " " << terrainObjectList[28]->transform.position.x << " " << terrainObjectList[28]->transform.position.y << std::endl;

        if (pressE && isPowerOn) {
            shouldRenderFireball = true;

            for (auto it = terrainTypeObjets.begin(); it != terrainTypeObjets.end(); ++it) {
                if (checkCollisionSol(bouleFeu->transform.position, (it->first)->transform.position) || bouleFeu->transform.position.y < -3) {
                    collisionAuSolBouleFeu = true;
                    break;
                }
            }

            if (collisionAuSolBouleFeu) {
                float dist = vitesse * deltaTime;
                bouleFeu->transform.position.x += dist;

                verticalSpeed2 = vitesse;

                const float airFriction = 0.01f;
                float airFrictionForce = airFriction * verticalSpeed2;
                verticalSpeed2 -= airFrictionForce;

                const float attenuationCoefficient = 0.8f;
                verticalSpeed2 *= attenuationCoefficient;
                
                bouleFeu->transform.position.y += verticalSpeed2 * deltaTime;

                shouldRenderFireball = false;
                pressE = false;

                collisionAuSolBouleFeu = false;
            }
            else {
                float dist;
                if (chat->transform.rotation.y == -90) {
                    dist = -vitesse * deltaTime;
                }
                else {
                    dist = vitesse * deltaTime;
                }
                bouleFeu->transform.position.x += dist;

                verticalSpeed2 -= gravity * deltaTime;
                bouleFeu->transform.position.y += verticalSpeed2 * deltaTime;
            }

            for (auto it = ennemisList.begin(); it != ennemisList.end(); ++it) {
                if (bouleFeu->transform.position.x >= (it->first)->transform.position.x - 1.5 && bouleFeu->transform.position.x <= (it->first)->transform.position.x + 1.5 && it->second[0]) {
                    it->second[0] = false;
                }
            }

        }

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );

    // Cleanup VBO and shader
    glDeleteProgram(programID);
    glDeleteVertexArrays(1, &VertexArrayID);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}


// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    //Camera zoom in and out
    float cameraSpeed = 20 * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) { //touche W en azerty
        camera_position += cameraSpeed * camera_target;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) { 
        camera_position -= cameraSpeed * camera_target;
    }
    //TODO add translations

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) { //touche A
        camera_position -= glm::normalize(glm::cross(camera_position, camera_up)) * cameraSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera_position += glm::normalize(glm::cross(camera_position, camera_up)) * cameraSpeed;
    }


    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        pressLeftArrow = true;
    }

    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        pressRightArrow = true;
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        pressSpace = true;
    }

    if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        pressE = true;
    }

}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}