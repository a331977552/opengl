#ifndef  __MODEL__H__
#define __MODEL__H__
#define GLEW_STATIC
#include <GLFW\glfw3.h>
#include <GL\glew.h>
#include "Shader.h"
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\glm.hpp>
#include <vector>
#include "Mesh.h"
#include "Texture.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
using std::vector;
using std::string;
class  Model{
private :
	vector<Texture> textures_loaded; 
	vector<Mesh>  meshes;
	string directory;
public :
	Model(char  * path) {
		this->loadModel(path);

	}

	void draw(Shader shader) {
		for (int i = 0; i <(int) meshes.size(); i++) {
			meshes[i].draw(shader);
		}

	}

	void loadModel(string  path) {
		Assimp::Importer importer;
		const aiScene*  scene =importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
		if (!scene || !scene->mRootNode || scene->mFlags& aiProcess_Triangulate)
		{
			  cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
        return;
		}
		this->directory =  path.substr(0,path.find_last_of('/'));
		this->processNode(scene->mRootNode, scene);


	}
	void processNode(aiNode  *node, const aiScene * scene) {
		for (int i=0;i<node->mNumMeshes; i++)
		{
			 aiMesh * mesh=scene->mMeshes[node->mMeshes[i]];
			 meshes.push_back(processMesh(mesh,scene));
		}
		for (int i=0;i<node->mNumChildren; i++)
		{
			aiNode * node = node->mChildren[i];
			processNode(node, scene);
		}



	}
	Mesh processMesh(aiMesh *mesh,const aiScene *scene)
	{
		vector<Vertex> vertices;
		vector<GLuint> indices;
		vector<Texture> textures;
		for (int i = 0; i < mesh->mNumVertices; i++) {
			   Vertex vertex;
			aiVector3D  v= mesh->mVertices[i];
			vertex.position.x = v.x;
			vertex.position.y = v.y;
			vertex.position.z = v.z;
			//vertex.Normal.
			vertex.Normal.x = mesh->mNormals[i].x;
			vertex.Normal.y = mesh->mNormals[i].y;
			vertex.Normal.z = mesh->mNormals[i].z;
			if (mesh->mTextureCoords[0]) {
				vertex.texCoord.x = mesh->mTextureCoords[0][i].x;
				vertex.texCoord.y = mesh->mTextureCoords[0][i].y;

			}

			vertices.push_back(vertex);
		}
		for(GLuint i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for(GLuint j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}  

		if (mesh->mMaterialIndex >= 0) {
			aiMaterial  *material=scene->mMaterials[mesh->mMaterialIndex];
		    vector<Texture> diffuse =	loadMaterialTextures(material, aiTextureType_DIFFUSE, Texture::TextureType::DIFFUSE);
			textures.insert(textures.end(), diffuse.begin(), diffuse.end());
			vector<Texture> diffuse =	loadMaterialTextures(material, aiTextureType_SPECULAR, Texture::TextureType::SPECULAR);
			textures.insert(textures.end(), diffuse.begin(), diffuse.end());


		}
		return Mesh(vertices, textures, indices);


	}
	vector<Texture> loadMaterialTextures(aiMaterial *mat,aiTextureType type ,Texture::TextureType typeName) {
		vector<Texture>  textures;

		for (int i = 0; i < mat->GetTextureCount(type); i++) {
			aiString str;
			mat->GetTexture(type, i, &str);
			string s = (directory + str.C_Str());
		
		
		   GLboolean skip = false;
        for(GLuint j = 0; j < textures_loaded.size(); j++)
        {
            if(std::strcmp(textures_loaded[j].path.c_str(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true; 
                break;
            }
        }
        if(!skip)

        {   // If texture hasn't been loaded already, load it
          	Texture texture( s,   typeName);
			texture.pathName = str.C_Str();
			textures.push_back(texture);
        }
		
		
		}


		return textures;
	}

};
#endif