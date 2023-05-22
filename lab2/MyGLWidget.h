// MyGLWidget.h
#include "BL3GLWidget.h"

class MyGLWidget : public BL3GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : BL3GLWidget(parent) {}
    ~MyGLWidget();

  protected:
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void creaBuffersTerraIParet ();
    virtual void initializeGL();
	virtual void carregaShaders();
	virtual void paintGL();

    GLuint colorFocusLoc, posFocusLoc, llumAmbientLoc, fviewLoc;
	glm::vec3 colorFocus, posFocus, llumAmbient;
	glm::mat4 fview;
	bool focus;


  private:
    int printOglError(const char file[], int line, const char func[]);
};
