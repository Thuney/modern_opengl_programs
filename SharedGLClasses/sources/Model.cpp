#include "Model.h"

#include <GL/glew.h>

#include "stb_image.h"

#include <iostream>

/*
 * Mesh
 */

Mesh::Mesh(vector<Vertex>& vertices, vector<unsigned int>& indices)
{
	this->vertices = vertices;
	this->indices = indices;

	setupMesh();
}

void Mesh::setupMesh()
{
	//Generate all buffers
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	//Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	//Vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	//Vertex texture coordinates
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texture_coords));

	glBindVertexArray(GL_NONE);
}

Shader* TexturedMesh::phong_texture_shader = nullptr;

TexturedMesh::TexturedMesh(vector<Vertex>& vertices, vector<unsigned int>& indices, vector<ModelTexture> textures, float shininess) : Mesh(vertices, indices)
{
	this->textures = textures;
	this->shininess = shininess;
}

void TexturedMesh::Draw()
{
	/*
	 * Assigns all diffuse and specular textures within the mesh to the corresponding sampler
	 * uniforms via our naming convention and then renders the mesh
	 */

	this->phong_texture_shader->use();
	this->phong_texture_shader->setFloat("shininess", this->shininess);

	unsigned int diffuse_number = 1;
	unsigned int specular_number = 1;
	for (unsigned int i = 0; i < textures.size(); i++)
	{
		//Activate proper texture unit before binding
		glActiveTexture(GL_TEXTURE0 + i);

		//Retrieve texture number
		string number;
		string name = textures[i].type;

		if (name == "texture_diffuse")
			number = std::to_string(diffuse_number++);
		else if (name == "texture_specular")
			number = std::to_string(specular_number++);

		this->phong_texture_shader->setInt((name + number).c_str(), i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}
	glActiveTexture(GL_TEXTURE0);

	//Draw mesh
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

Shader* SolidMaterialMesh::phong_solidmaterial_shader = nullptr;

SolidMaterialMesh::SolidMaterialMesh(vector<Vertex>& vertices, vector<unsigned int>& indices, Material material) : Mesh(vertices, indices)
{
	this->material = material;
}

void SolidMaterialMesh::Draw()
{
	this->phong_solidmaterial_shader->use();

	//this->phong_solidmaterial_shader->setVec3("material.ambient_color", this->material.ambient_color);
	this->phong_solidmaterial_shader->setVec3("material.ambient_color", this->material.diffuse_color);
	this->phong_solidmaterial_shader->setVec3("material.diffuse_color", this->material.diffuse_color);
	this->phong_solidmaterial_shader->setVec3("material.specular_color", this->material.specular_color);
	this->phong_solidmaterial_shader->setFloat("material.shininess", this->material.shininess);

	//Draw mesh
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

/*
 * Model
 */

unsigned int texture_from_file(const char *path, const string &directory, bool gamma = false);

Model::Model(const char *path)
{
	loadModel(path);
}

Model::~Model()
{
	std::vector<Mesh*>::iterator m;

	for (m = meshes.begin(); m != meshes.end(); m++)
		delete (*m);
}

void Model::Draw(Shader* textured_shader, Shader* solidmaterial_shader)
{
	/*
	 * Iterates through all the meshes within the model and draws them with the provided shader
	 */
	std::vector<Mesh*>::iterator m;

	TexturedMesh::phong_texture_shader = textured_shader;
	SolidMaterialMesh::phong_solidmaterial_shader = solidmaterial_shader;

	for (m = meshes.begin(); m != meshes.end(); m++)
		(*m)->Draw();
}

void Model::loadModel(string path)
{
	/*
	 * Define an Assimp importer and import the scene from the filepath provided,
	 * then process the scene into our mesh objects via the processNode function
	 */
	Assimp::Importer import;
	const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

	//Print an error if there was an issue loading the model
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
		return;
	}

	//Store a local reference to the path to the model
	this->directory = path.substr(0, path.find_last_of('/'));

	//Recursively process the root node into our datatypes
	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode *node, const aiScene *scene)
{
	/*
	 * Recursive function to process the meshes of every node in the scene
	 */

	//Process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	//Then do the same for each of its children
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh* Model::processMesh(aiMesh *mesh, const aiScene *scene)
{
	vector<Vertex> vertices;
	vector<unsigned int> indices;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		//Process vertex positions, normals, and texture coordinates
		glm::vec3 vector;

		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.position = vector;

		//Normals are generated if not already present in the model
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.normal = vector;

		if (mesh->mTextureCoords[0]) //Does the mesh contain texture coordinates?
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.texture_coords = vec;
		}
		else
			vertex.texture_coords = glm::vec2(0.0f, 0.0f);

		vertices.push_back(vertex);
	}
	//Process indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	//vector<Texture> textures;

	//Process material and store textures for each mesh
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

		int num_textures = material->GetTextureCount(aiTextureType_DIFFUSE) + material->GetTextureCount(aiTextureType_SPECULAR);

		int i = 0;

		if (num_textures > 0)
		{
			vector<ModelTexture> textures;

			vector<ModelTexture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
			vector<ModelTexture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

			float shininess;
			aiGetMaterialFloat(material, AI_MATKEY_SHININESS, &shininess);

			return new TexturedMesh(vertices, indices, textures, shininess);
		}
		else
		{
			Material mesh_material;

			aiColor4D ambientColor;
			aiColor4D diffuseColor;
			aiColor4D specularColor;
			float shininess;

			aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &ambientColor);
			aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuseColor);
			aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &specularColor);
			aiGetMaterialFloat(material, AI_MATKEY_SHININESS, &shininess);

			mesh_material.ambient_color = glm::vec3(ambientColor.r, ambientColor.g, ambientColor.b);
			mesh_material.diffuse_color = glm::vec3(diffuseColor.r, diffuseColor.g, diffuseColor.b);
			mesh_material.specular_color = glm::vec3(specularColor.r, specularColor.g, specularColor.b);
			mesh_material.shininess = shininess;

			return new SolidMaterialMesh(vertices, indices, mesh_material);
		}
	}

	return nullptr;
}

vector<ModelTexture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName)
{
	vector<ModelTexture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded.size(); j++)
		{
			if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
			{
				textures.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{
			//If texture hasn't been loaded already, load it
			ModelTexture texture;
			texture.id = texture_from_file(str.C_Str(), directory);
			texture.type = typeName;
			texture.path = str.C_Str();

			textures.push_back(texture);
			textures_loaded.push_back(texture); //Add to loaded textures
		}
	}
	return textures;
}

unsigned int texture_from_file(const char *path, const string &directory, bool gamma)
{
	/*
	 * Loads a texture from a filepath and returns its texture name
	 */

	string filename = string(path);
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, number_of_components;
	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &number_of_components, 0);
	if (data)
	{
		GLenum format;
		if (number_of_components == 1)
			format = GL_RED;
		else if (number_of_components == 3)
			format = GL_RGB;
		else if (number_of_components == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}