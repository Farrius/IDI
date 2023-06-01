// MyGLWidget.cpp
#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>
#include <QTimer>

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

void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
  makeCurrent();
  // Aqui cal que es calculi i s'apliqui la rotacio o el zoom com s'escaigui...
  if (DoingInteractive == ROTATE)
  {
    // Fem la rotació
    angleY += (e->x() - xClick) * M_PI / ample;
    viewTransform ();
  }

  xClick = e->x();
  yClick = e->y();

  update ();
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) {
  makeCurrent();
  switch (event->key()) {
  case Qt::Key_A: {
	  angleBailarins += 45.0;
    break;
	}
  case Qt::Key_D: {
	  angleBailarins -= 45.0;
    break;
	}
  case Qt::Key_E: {
	  llumBlanca = !llumBlanca;
    break;
	}
  case Qt::Key_B: {
	  llumActiva[apagaLlums] = !llumActiva[apagaLlums];
	  apagaLlums = (apagaLlums + 1)%4;
    break;
	}	
  case Qt::Key_R: {
	  angleDisco += 5.0;
    break;
	}
  case Qt::Key_S: {
	  if (timer->isActive()) timer->stop();
	  else timer->start(100);
    break;
	}	
  default: LL4GLWidget::keyPressEvent(event); break;
  }
  update();
}

void MyGLWidget::initializeGL() {
	// Cal inicialitzar l'ús de les funcions d'OpenGL
	LL4GLWidget::initializeGL();
	carregaShaders();

	angleDisco = 0.0;
	angleBailarins = 0.0;

	discoSCA = glm::vec3(5, 5, 5);

	colBlanca = glm::vec3(0.4, 0.4, 0.4);
	colNegre = glm::vec3(0, 0, 0);
	col[0] = glm::vec3(0.4, 0, 0);
	col[1] = glm::vec3(0, 0.4, 0);
	col[2] = glm::vec3(0, 0, 0.4);
	col[3] = glm::vec3(0.4, 0.4, 0);
	llumBlanca = true;
	apagaLlums = 0;
	for (int i = 0; i < 4; ++i) {
		llumActiva[i] = true;
	}

	timer = new QTimer(this);
	connect(timer, &QTimer::timeout, this, &MyGLWidget::gira);
}

void MyGLWidget::paintGL() {
  // En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
  // useu els paràmetres que considereu (els que hi ha són els de per defecte)
  // glViewport (0, 0, ample, alt);
  
  // Esborrem el frame-buffer i el depth-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //LLums
  if (llumBlanca) glUniform3fv(colFocusBlancaLoc, 1, &colBlanca[0]);
  else glUniform3fv(colFocusBlancaLoc, 1, &colNegre[0]);

  for (int i = 0; i < 4; ++i) {
	  if (llumActiva[i]) glUniform3fv(colFocusLoc[i], 1, &col[i][0]);
	  else glUniform3fv(colFocusLoc[i], 1, &colNegre[0]);
  }
  //Posicions
  float radDisco = glm::radians(angleDisco);

  posVermell = glm::vec3(1.2*cos(radDisco), 0, 1.2*sin(radDisco)) + discoSCA;
  glUniform3fv(posFocusVermellLoc, 1, &posVermell[0]);

  posVerd = glm::vec3(-1.2*cos(radDisco), 0, -1.2*sin(radDisco)) + discoSCA;
  glUniform3fv(posFocusVerdLoc, 1, &posVerd[0]);

  posBlau = glm::vec3(-1.2*sin(radDisco), 0, 1.2*cos(radDisco)) + discoSCA;
  glUniform3fv(posFocusBlauLoc, 1, &posBlau[0]);

  posGroc = glm::vec3(1.2*sin(radDisco), 0, -1.2*cos(radDisco)) + discoSCA;
  glUniform3fv(posFocusGrocLoc, 1, &posGroc[0]);

  // TERRA
  glBindVertexArray (VAO_Terra);
  modelTransformTerra ();
  glDrawArrays(GL_TRIANGLES, 0, 30);
  
  // MORTY
  glBindVertexArray (VAO_Morty);
  modelTransformMorty ();
  glDrawArrays(GL_TRIANGLES, 0, morty.faces().size()*3);
  
  // FANTASMA
  glBindVertexArray (VAO_Fantasma);
  modelTransformFantasma (1.0f);
  glDrawArrays(GL_TRIANGLES, 0, fantasma.faces().size()*3);
  modelTransformFantasma (9.0f);
  glDrawArrays(GL_TRIANGLES, 0, fantasma.faces().size()*3);

  // DISCO BALL
  glBindVertexArray (VAO_DiscoBall);
  modelTransformDiscoBall ();
  glDrawArrays(GL_TRIANGLES, 0, discoBall.faces().size()*3);
  
  glBindVertexArray(0);
}

void MyGLWidget::modelTransformDiscoBall ()
{
  glm::mat4 TG;
  TG = glm::translate(glm::mat4(1.f), glm::vec3(5,5,5));
  TG = glm::rotate(TG, glm::radians(angleDisco), glm::vec3(0, 1, 0));
  TG = glm::scale(TG, glm::vec3(escalaDiscoBall, escalaDiscoBall, escalaDiscoBall));
  TG = glm::translate(TG, -centreBaseDiscoBall);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformMorty ()
{
  glm::mat4 TG;
  TG = glm::translate(glm::mat4(1.f), glm::vec3(5,0,5));
  TG = glm::rotate(TG, glm::radians(angleBailarins), glm::vec3(0, 1, 0));
  TG = glm::scale(TG, glm::vec3(escalaMorty, escalaMorty, escalaMorty));
  TG = glm::translate(TG, -centreBaseMorty);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformFantasma (float posX)
{
  glm::mat4 TG;
  TG = glm::translate(glm::mat4(1.0f), glm::vec3(posX,0.5,5));
  TG = glm::rotate(TG, glm::radians(angleBailarins), glm::vec3(0, 1, 0));
  TG = glm::scale(TG, glm::vec3(escalaFantasma, escalaFantasma, escalaFantasma));
  TG = glm::translate(TG, -centreBaseFantasma);

  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::carregaShaders() {
	posFocusVermellLoc = glGetUniformLocation (program->programId(), "posFocusVermell");
	posFocusVerdLoc = glGetUniformLocation (program->programId(), "posFocusVerd");
	posFocusBlauLoc = glGetUniformLocation (program->programId(), "posFocusBlau");
	posFocusGrocLoc = glGetUniformLocation (program->programId(), "posFocusGroc");

	colFocusBlancaLoc = glGetUniformLocation (program->programId(), "colFocusBlanca");
	colFocusLoc[0] = glGetUniformLocation (program->programId(), "colFocusVermell");
	colFocusLoc[1] = glGetUniformLocation (program->programId(), "colFocusVerd");
	colFocusLoc[2] = glGetUniformLocation (program->programId(), "colFocusBlau");
	colFocusLoc[3] = glGetUniformLocation (program->programId(), "colFocusGroc");
}

void MyGLWidget::gira() {
	angleDisco += 5.0;
	llumActiva[apagaLlums] = !llumActiva[apagaLlums];
	apagaLlums = (apagaLlums + 1)%4;
	update();
}
