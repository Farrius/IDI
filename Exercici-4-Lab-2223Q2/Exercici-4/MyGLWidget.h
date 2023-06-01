// MyGLWidget.h
#include "LL4GLWidget.h"

class MyGLWidget : public LL4GLWidget {
  Q_OBJECT
  public:
    MyGLWidget(QWidget *parent=0) : LL4GLWidget(parent) {}
    ~MyGLWidget();
  protected:
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void keyPressEvent(QKeyEvent* event);
	virtual void initializeGL();
	virtual void paintGL();
	virtual void modelTransformDiscoBall();
	virtual void modelTransformMorty();
	virtual void modelTransformFantasma(float posX);
	void carregaShaders();
	void gira();
  private:
    int printOglError(const char file[], int line, const char func[]);
	float angleDisco, angleBailarins;
	GLuint posFocusGrocLoc, posFocusVermellLoc, posFocusBlauLoc, posFocusVerdLoc;
	GLuint colFocusBlancaLoc, colFocusLoc[4];
	glm::vec3 discoSCA, posVermell, posBlau, posVerd, posGroc;
	glm::vec3 colBlanca, colNegre, col[4];
	bool llumBlanca, llumActiva[4];
	int apagaLlums;
	QTimer *timer;
};
