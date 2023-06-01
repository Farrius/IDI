// MyGLWidget.cpp
#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>
#include <QRadioButton>

#define printOpenGLError() printOglError(__FILE__, __LINE__)
#define CHECK() printOglError(__FILE__, __LINE__,__FUNCTION__)
#define DEBUG() std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << std::endl;

int MyGLWidget::printOglError(const char file[], int line, const char func[]) 
{
	GLenum glErr;
	int    retCode = 0;

	glErr = glGetError();
	const char * error = 0;
	switch (glErr)
	{
		case 0x0500:
			error = "GL_INVALID_ENUM";
			break;
		case 0x501:
			error = "GL_INVALID_VALUE";
			break;
		case 0x502: 
			error = "GL_INVALID_OPERATION";
			break;
		case 0x503:
			error = "GL_STACK_OVERFLOW";
			break;
		case 0x504:
			error = "GL_STACK_UNDERFLOW";
			break;
		case 0x505:
			error = "GL_OUT_OF_MEMORY";
			break;
		default:
			error = "unknown error!";
	}
	if (glErr != GL_NO_ERROR)
	{
		printf("glError in file %s @ line %d: %s function: %s\n",
				file, line, error, func);
		retCode = 1;
	}
	return retCode;
}

MyGLWidget::~MyGLWidget() {
}

void MyGLWidget::paintGL ()   // Mètode que has de modificar
{
	// En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
	// useu els paràmetres que considereu (els que hi ha són els de per defecte)
	int mn = ample;
	if (alt < mn) mn = alt;
	glViewport (ample/2 - mn/2, alt/2 - mn/2, mn, mn);

	// Esborrem el frame-buffer i el depth-buffer
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (blanca) colorFocus = glm::vec3(1.0, 1.0, 1.0);
	else colorFocus = glm::vec3(1.0, 1.0, 0.0);

	glUniform3fv(colfocusLoc, 1, &colorFocus[0]);

	// Pintem el terra
	glBindVertexArray (VAO_Terra);
	modelTransformTerra ();
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	// Pintem el Patricio
	if (not cubs) {
		glBindVertexArray (VAO_Patr);
		modelTransformPatricio ();
		glDrawArrays(GL_TRIANGLES, 0, patr.faces().size()*3);
	} else {

		// Pintem el cub
		glBindVertexArray(VAO_Cub);
		modelTransformCub (2.0/0.5, angleCub[0]);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		modelTransformCub (2.5/0.5, angleCub[1]);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		modelTransformCub (3.0/0.5, angleCub[2]);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	glBindVertexArray(0);
}

void MyGLWidget::modelTransformCub (float escala, float angle) 
{
	// En aquest mètode has de substituir aquest codi per construir la 
	// transformació geomètrica (TG) del cub usant els paràmetres adientment
	TG = glm::rotate(glm::mat4(1.f), angle, glm::vec3(0, 1, 0));
	TG = glm::translate(TG, glm::vec3(5, 0, 0));
	TG = glm::scale(TG, glm::vec3(escala, escala, escala));
	glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformPatricio ()    // Mètode que has de modificar
{
	TG = glm::rotate(glm::mat4(1.f), angleCub[anglePatri], glm::vec3(0, 1, 0));
	TG = glm::translate(TG, glm::vec3(5, 0, 0));
	TG = glm::rotate(TG, -float(M_PI/2.0), glm::vec3(0, 1, 0));
	TG = glm::scale(TG, glm::vec3(2*escala, 2*escala, 2*escala));
	TG = glm::translate(TG, -centreBasePat);

	glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::viewTransform ()    // Mètode que has de modificar
{
	if (!camPlanta)
		ExamGLWidget::viewTransform();
	else
	{
		glm::mat4 VM = glm::lookAt(glm::vec3(0.f, zn*2, 0.f), glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 0.f, 0.f));
		glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &VM[0][0]);
	}
}

void MyGLWidget::projectTransform ()
{
	if (!camPlanta)
		ExamGLWidget::projectTransform();
	else
	{
		glm::mat4 Proj;
		Proj = glm::ortho(-10.f, 10.f, -10.f, 10.f, zn, zf);
		glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
	}
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) {
	makeCurrent();
	switch (event->key()) {
		case Qt::Key_V: {
							cubs = not cubs;
							break;
						}
		case Qt::Key_1: {
							anglePatri = 0;
							break;
						}
		case Qt::Key_2: {
							anglePatri = 1;
							break;
						}
		case Qt::Key_3: {
							anglePatri = 2;
							break;
						}
		case Qt::Key_F: {
							blanca = not blanca;
							break;
						}
		case Qt::Key_C: {
							camPlanta = not camPlanta;
							viewTransform();
							projectTransform();
							break;
						}
		case Qt::Key_Right: {
								float aux = angleCub[0];
								angleCub[0] = angleCub[1];
								angleCub[1] = angleCub[2];
								angleCub[2] = aux;
								break;
							}
		case Qt::Key_Left: {
							   float aux = angleCub[0];
							   angleCub[0] = angleCub[2];
							   angleCub[2] = angleCub[1];
							   angleCub[1] = aux;
							   break;
						   }
		case Qt::Key_R: {
							initializeGL();
							break;
						}
		default: ExamGLWidget::keyPressEvent(event); break;
	}
	update();
}

void MyGLWidget::initializeGL() {
	ExamGLWidget::initializeGL();
	anglePatri = 0;
	cubs = true;
	blanca = true;
	angleCub[0] = 0.0;
	angleCub[1] = (2.0/3.0)*M_PI;
	angleCub[2] = (4.0/3.0)*M_PI;
}

void MyGLWidget::perspective() {
	makeCurrent();
	camPlanta = false;
							viewTransform();
							projectTransform();
	update();
}
void MyGLWidget::orthogonal() {
	makeCurrent();
	camPlanta = true;
							viewTransform();
							projectTransform();
	update();
}

void MyGLWidget::cub1() {
	anglePatri = 0;
	update();
}
void MyGLWidget::cub2() {
	anglePatri = 1;
	update();
}
void MyGLWidget::cub3() {
	anglePatri = 2;
	update();
}

