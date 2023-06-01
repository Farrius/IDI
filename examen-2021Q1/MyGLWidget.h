#include "ExamGLWidget.h"

class MyGLWidget:public ExamGLWidget
{
	Q_OBJECT

	public:
		MyGLWidget(QWidget *parent=0) : ExamGLWidget(parent) {}
		~MyGLWidget();

	protected:
		virtual void paintGL ();
		virtual void keyPressEvent(QKeyEvent* event);
		virtual void modelTransformCub (float escala, float angle);
		virtual void modelTransformPatricio ();
		virtual void projectTransform ();
		virtual void viewTransform ();
		virtual void initializeGL();
		public slots:
			void perspective();
			void orthogonal();
			void cub1();
			void cub2();
			void cub3();

	private:
		int printOglError(const char file[], int line, const char func[]);
		float angleCub[3];
		int anglePatri;
		bool cubs, blanca;
		glm::vec3 colorFocus;
};
