#include "Engine\Controlleur.hpp"
#include <iostream>
#include <assimp/cimport.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

using namespace std;
using namespace obj;

/* ---------------------------------------------------------------------------- */
void color4_to_float4(const aiColor4t<float> *c, float f[4])
{
	f[0] = c->r;
	f[1] = c->g;
	f[2] = c->b;
	f[3] = c->a;
}

/* ---------------------------------------------------------------------------- */
void set_float4(float f[4], float a, float b, float c, float d)
{
	f[0] = a;
	f[1] = b;
	f[2] = c;
	f[3] = d;
}
/* ---------------------------------------------------------------------------- */
void apply_material(const aiMaterial *mtl)
{
	float c[4];

	GLenum fill_mode;
	int ret1, ret2;
	aiColor4t<float> diffuse;
	aiColor4t<float> specular;
	aiColor4t<float> ambient;
	aiColor4t<float> emission;
	float shininess, strength;
	int two_sided;
	int wireframe;
	unsigned int max;

	set_float4(c, 0.8f, 0.8f, 0.8f, 1.0f);
	if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
		color4_to_float4(&diffuse, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, c);

	set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
	if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &specular))
		color4_to_float4(&specular, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);

	set_float4(c, 0.2f, 0.2f, 0.2f, 1.0f);
	if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &ambient))
		color4_to_float4(&ambient, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, c);

	set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
	if (AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE, &emission))
		color4_to_float4(&emission, c);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, c);

	max = 1;
	ret1 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS, &shininess, &max);
	if (ret1 == AI_SUCCESS) {
		max = 1;
		ret2 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS_STRENGTH, &strength, &max);
		if (ret2 == AI_SUCCESS)
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess * strength);
		else
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
	}
	else {
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
		set_float4(c, 0.0f, 0.0f, 0.0f, 0.0f);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);
	}

	max = 1;
	if (AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_ENABLE_WIREFRAME, &wireframe, &max))
		fill_mode = wireframe ? GL_LINE : GL_FILL;
	else
		fill_mode = GL_FILL;
	glPolygonMode(GL_FRONT_AND_BACK, fill_mode);

	max = 1;
	if ((AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_TWOSIDED, &two_sided, &max)) && two_sided)
		glDisable(GL_CULL_FACE);
	else
		glEnable(GL_CULL_FACE);
}
/* ---------------------------------------------------------------------------- */
void recursive_render(const aiScene *sc, const  aiNode* nd)
{
	unsigned int i;
	unsigned int n = 0, t;
	aiMatrix4x4t<float> m = nd->mTransformation;

	/* update transform */
	aiTransposeMatrix4(&m);
	glPushMatrix();
	glMultMatrixf((float*)&m);

	/* draw all meshes assigned to this node */
	for (; n < nd->mNumMeshes; ++n) {
		const aiMesh* mesh = sc->mMeshes[nd->mMeshes[n]];

		apply_material(sc->mMaterials[mesh->mMaterialIndex]);
		//pour toutes les faces
		for (t = 0; t < mesh->mNumFaces; ++t) {
			const aiFace* face = &mesh->mFaces[t];
			GLenum face_mode;

			//utiliser le bon type de tracé en fonction du nbr de sommets de la face
			switch (face->mNumIndices) {
			case 1: face_mode = GL_POINTS; break;
			case 2: face_mode = GL_LINES; break;
			case 3: face_mode = GL_TRIANGLES; break;
			default: face_mode = GL_POLYGON; break;
			}
			glBegin(face_mode);
			//tracer
			for (i = 0; i < face->mNumIndices; i++) {
				int index = face->mIndices[i];
				if (mesh->mColors[0] != NULL)
					glColor4fv((GLfloat*)&mesh->mColors[0][index]);
				if (mesh->mNormals != NULL)
					glNormal3fv(&mesh->mNormals[index].x);
				glVertex3fv(&mesh->mVertices[index].x);
			}
			//fin
			glEnd();
		}

	}
	/* draw all children */
	for (n = 0; n < nd->mNumChildren; ++n) {
		recursive_render(sc, nd->mChildren[n]);
	}

	glPopMatrix();
}
/* ---------------------------------------------------------------------------- */
Controlleur::Ptr Controlleur::get() {
	static  Controlleur::Ptr inst = Controlleur::Ptr(new Controlleur);
	return inst;
}
void Controlleur::drawGL() {
	//paramétrages

	//glViewport(0, 0, static_cast<int>(window.getSize().x), static_cast<int>(window.getSize().y));
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	static const auto pi = 3.1415926535897932384626433832795f;
	static const auto fov = 90.f;
	static const auto near_distance = .1f;
	static const auto far_distance = 100.f;
	static const auto aspect = 800.f / 600.f;

	auto frustum_height = std::tan(fov / 360 * pi) * near_distance;
	auto frustum_width = frustum_height * aspect;

	glFrustum(-frustum_width, frustum_width, -frustum_height, frustum_height, near_distance, far_distance);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(-.50f, -.50f, -50.f);

	//affichage spécifique
	if (objAffiche == 0) {
		drawDefault();
	}
	else {
		drawObj();
	}
}
void Controlleur::drawDefault() {
	glBegin(GL_QUADS);
	glVertex3f(0.f, 1.f, 0.f);
	glVertex3f(0.f, 0.f, 0.f);
	glVertex3f(1.f, 0.f, 0.f);
	glVertex3f(1.f, 1.f, 0.f);
	glEnd();
}

void Controlleur::drawObj() {
	glPushMatrix();
	glCallList(objAffiche);
	glPopMatrix();
}
//charge un obj et le sauvegarde ds objAffiche pour l'afficher
void Controlleur::loadObj(std::string path) {
	try {
		Assimp::Importer importer;

		const aiScene *scene = importer.ReadFile(path,
			aiProcess_CalcTangentSpace |
			aiProcess_Triangulate |
			aiProcess_JoinIdenticalVertices |
			aiProcess_SortByPType);
		objAffiche = glGenLists(1);
		glNewList(objAffiche, GL_COMPILE);

		recursive_render(scene, scene->mRootNode);
		glEndList();
	}
	catch (FileError fe) {
		std::cout << fe.what();
	}
	catch (exception e) {
		std::cout << e.what();
	}
}