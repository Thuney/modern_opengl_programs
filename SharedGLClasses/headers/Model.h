#ifndef MODEL_H
#define MODEL_H

using namespace std;
#include <string>
#include <vector>

#include "glm/glm.hpp"
#include "Shader.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

//Struct for data defining a vertex
struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texture_coords;
};

//Struct for data defining a texture
struct ModelTexture
{
	unsigned int id;
	string type;
	string path; //Path variable for checking if texture is already loaded
};

struct Material
{
	glm::vec3 ambient_color;
	glm::vec3 diffuse_color;
	glm::vec3 specular_color;
	float shininess;
};

class Mesh
{
public:
	/*
	 *	Mesh Data
	 */
	vector<Vertex> vertices;
	vector<unsigned int> indices;

	/*
	 *	Functions
	 */
	Mesh(vector<Vertex>& vertices, vector<unsigned int>& indices);
	virtual void Draw()
	{
	};

protected:
	/*
	 *	Render data
	 */
	unsigned int vao, vbo, ebo;
	/*
	 *	Functions
	 */
	void setupMesh();
};

class SolidMaterialMesh : public Mesh
{
public:
	/*
	 *	Mesh Data
	 */
	Material material;
	static Shader* phong_solidmaterial_shader;

	/*
	 *	Functions
	 */
	SolidMaterialMesh(vector<Vertex>& vertices, vector<unsigned int>& indices, Material material);
	void Draw();

private:

};

class TexturedMesh : public Mesh
{
public:
	/*
	 *	Mesh Data
	 */
	vector<ModelTexture> textures;
	static Shader* phong_texture_shader;
	float shininess;

	 /*
	  *	Functions
	  */
	TexturedMesh(vector<Vertex>& vertices, vector<unsigned int>& indices, vector<ModelTexture> textures, float shininess);

	void Draw();

private:

};

//----------------------------------------------------------

class Model
{
public:
	/*
	 *	Functions
	 */
	Model(const char* path);
	~Model();

	void Draw(Shader* textured_shader, Shader* solidmaterial_shader);

private:
	/*
	 *	Model data
	 */
	vector<Mesh*> meshes;
	vector<ModelTexture> textures_loaded;
	string directory;
	/*
	 *	Functions
	 */
	void loadModel(string path);
	void processNode(aiNode *node, const aiScene *scene);
	Mesh* processMesh(aiMesh *mesh, const aiScene *scene);
	vector<ModelTexture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);
};

#endif