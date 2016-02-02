/*
*  Simple trackball-like motion adapted (ripped off) from projtex.c
*  (written by David Yu and David Blythe).  See the SIGGRAPH '96
*  Advanced OpenGL course notes.
*/

#include "Engine\Trackball.hpp"

Trackball::Trackball()
{
	tbInit();
}
#include <iostream>
using namespace std;
char c;//machin pour tout délay et tracer flow d'exécution
/************************************************* functions  protected*************************************************/

void Trackball::_tbPointToVector(int x, int y, int width, int height, float v[3])
{
	float d, a;
	/* project x, y onto a hemi-sphere centered within width, height. */
	v[0] = (float)(2.0 * x - width) / width;
	v[1] = (float)(height - 2.0 * y) / height;
	d = (float)sqrt(v[0] * v[0] + v[1] * v[1]);//norme
	v[2] = (float)cos((3.14159265 / 2.0) * ((d < 1.0) ? d : 1.0));
	a = 1.0 / (float)sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	v[0] *= a;
	v[1] *= a;
	v[2] *= a;
}
/************************************************* functions  public*************************************************/

void Trackball::tbInit()
{
	tb_angle = 0.0;

	/* put the identity in the trackball transform */
	glPushMatrix();
	glLoadIdentity();
	glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *)tb_transform);
	glPopMatrix();
}

void Trackball::tbStart(int x, int y) {
	active = true;
	_tbPointToVector(x, y, tb_width, tb_height, tb_lastposition);
	tbMotion(x, y);
}
void Trackball::tbStop() {
	active = false;
	glPushMatrix();
		glLoadIdentity();
		glRotatef(tb_angle, tb_axis[0], tb_axis[1], tb_axis[2]);
		glMultMatrixf((GLfloat *)tb_transform);
		glGetFloatv(GL_MODELVIEW_MATRIX, (GLfloat *)tb_transform);
	glPopMatrix();
}
void Trackball::tbMatrix()
{
	if (active) {
		glRotatef(tb_angle, tb_axis[0], tb_axis[1], tb_axis[2]);
	}
	glMultMatrixf((GLfloat *)tb_transform);

}

void Trackball::tbReshape(int width, int height)
{
	tb_width = width;
	tb_height = height;
}
void Trackball::tbMotion(int x, int y)
{
	GLfloat current_position[3], dx, dy, dz;

	_tbPointToVector(x, y, tb_width, tb_height, current_position);

	/* calculate the angle to rotate by (directly proportional to the
	length of the mouse movement) */
	dx = current_position[0] - tb_lastposition[0];
	dy = current_position[1] - tb_lastposition[1];
	dz = current_position[2] - tb_lastposition[2];
	tb_angle = 90.0 * sqrt(dx * dx + dy * dy + dz * dz);

	/* calculate the axis of rotation (produit vectoriel) */
	tb_axis[0] = tb_lastposition[1] * current_position[2] -
		tb_lastposition[2] * current_position[1];
	tb_axis[1] = tb_lastposition[2] * current_position[0] -
		tb_lastposition[0] * current_position[2];
	tb_axis[2] = tb_lastposition[0] * current_position[1] -
		tb_lastposition[1] * current_position[0];
}
