#pragma once
#include <iostream>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include "Mesh.h"
using namespace std;

class Model
{
public:
	string directory;
	vector<Mesh> meshs;
	vector<Mesh::texture> loadedTex;
	void Draw(Shader shader);
	Model(string path);
	~Model();
	void loadModel(string path);
	void processNode(aiNode *node,const  aiScene * scene);
	Mesh Model::processMesh( aiMesh* mesh, const aiScene *scene);
	 GLuint TextureFromFile(const  char * name, string path);
	vector<Mesh::texture>  Model::loadMaterial(aiMaterial * mater, aiTextureType type, string typeName);
};

