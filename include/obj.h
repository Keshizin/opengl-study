#ifndef OBJ_H
#define OBJ_H

typedef struct {
	float x;
	float y;
	float z;
} VERT;

typedef struct {
	int total;
	int ind[4];
} FACE;

typedef struct {
	VERT *vertices;
	FACE *faces;
	int total_faces;
} OBJ;

void drawOBJ(OBJ *obj);

void drawOBJ(OBJ *obj)
{
	glBegin(GL_LINE_LOOP);
	
	for(int faces = 0; faces < obj->total_faces; ++faces)
	{
		// Percorre todos os vértices da face
		for(int vertices = 0; vertices < obj->faces[faces].total; ++vertices)
		{
			// if(textures) {
			// 	glTexCoord2f(obj->textures)
			// }

			glVertex3f(
				obj->vertices[obj->faces[faces].ind[vertices]].x,
				obj->vertices[obj->faces[faces].ind[vertices]].y,
				obj->vertices[obj->faces[faces].ind[vertices]].z);
		}			
	}

	glEnd();
}

#endif