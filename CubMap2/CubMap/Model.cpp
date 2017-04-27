#include "stdafx.h"
#include "Model.h"


Model::Model(string path)
{

	loadModel(path);
}

	
void Model::loadModel(string path) {
	Assimp::Importer importer;
	const aiScene  *scene=importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	 if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
    {
        cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
        return;
    }
	this->directory = path.substr(0, path.find_last_of('/'))+'/';
	processNode(scene->mRootNode, scene);


}
void Model::processNode(aiNode *node, const aiScene *scene) {
	for (GLuint i = 0; i < node->mNumMeshes; i++)
	{

		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshs.push_back(this->processMesh(mesh, scene));
	}
	for (GLuint i = 0; i < node->mNumChildren; i++)
	{
		aiNode *node2=	node->mChildren[i];
		processNode(node2, scene);
	}





}
Mesh Model::processMesh( aiMesh* mesh,const aiScene *scene) {


	vector<Mesh::Vertex>  vectices;
	vector<GLuint> indices;
	vector<Mesh::texture> textures;

	for (int i = 0; i < mesh->mNumVertices; i++) {
		Mesh::Vertex  vec;
		aiVector3D  v=mesh->mVertices[i];
		glm::vec3 pos;
		pos.x=v.x;
		pos.y=v.y;
		pos.z = v.z;
		vec.position = pos;
		if (mesh->mTextureCoords[0]) {
			aiVector3D   c=mesh->mTextureCoords[0][i];		
			glm::vec2 texCoords;
			texCoords.x = c.x;
			texCoords.y = c.y;
			vec.TexCoords = texCoords;
		}
		glm::vec3 normal;
		normal.x = mesh->mNormals[i].x;
		normal.y = mesh->mNormals[i].y;
		normal.z = mesh->mNormals[i].z;
		vec.normal = normal;  
		vectices.push_back(vec);
}
	for (int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face=mesh->mFaces[i];
	  
		for (GLuint j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}


	}
	if (mesh->mMaterialIndex >= 0) {
		aiMaterial  * mater=scene->mMaterials[mesh->mMaterialIndex];
		vector<Mesh::texture>  diffuses=loadMaterial(mater,aiTextureType::aiTextureType_DIFFUSE,"texture_diffuse");
		vector<Mesh::texture>  specualrs=loadMaterial(mater,aiTextureType::aiTextureType_SPECULAR,"texture_specular");
	  textures.insert(textures.end(), diffuses.begin(), diffuses.end());
	  textures.insert(textures.end(), specualrs.begin(), specualrs.end());

	}

	return Mesh(vectices,indices,textures);
}
vector<Mesh::texture> Model::loadMaterial(aiMaterial * mater, aiTextureType type, string typeName) {
     int count=	mater->GetTextureCount(type);
	 vector<Mesh::texture>  textures;
	 for (int i = 0; i < count; i++) {
		 bool skip=false;
		aiString str;
        mater->GetTexture(type, i, &str);
        for(GLuint j = 0; j < loadedTex.size(); j++)
        {
            if(std::strcmp(loadedTex[j].path.c_str(), str.C_Str()) == 0)
            {
                textures.push_back(loadedTex[j]);
                skip = true; 
                break;
            }
        }
		if (!skip) {
		mater->GetTexture(type, i, &str);
		Mesh::texture tex;
		tex.path = str.C_Str();
		tex.texid= TextureFromFile(str.C_Str(), this->directory);
		tex.type = typeName;
		textures.push_back(tex);
		loadedTex.push_back(tex);
		}
  }


	 return textures;
}

GLuint Model::TextureFromFile(const char * name, string path) {
	int width, height, channel;
	unsigned char * data = SOIL_load_image((path + name).c_str(), &width, &height, &channel, SOIL_LOAD_AUTO);
	if (!data) {
		REMINDER("fail to load  image:", path.c_str());
		return  -1;
	}
	GLuint type = GL_RGBA;
	if (channel == 3) {
		type = GL_RGB;

	}
	GLuint texID;
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(data);
	glBindTexture(GL_TEXTURE_2D, 0);
	return texID;
	
}









void Model::Draw(Shader shader){
	while (meshs.begin() != meshs.end()) {
		Mesh  mesh= * meshs.begin();
		mesh.Draw(shader);
		meshs.begin()++;
	}
}
Model::~Model()
{
}
