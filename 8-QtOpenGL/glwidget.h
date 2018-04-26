#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
    Q_OBJECT

public:
    GLWidget(QWidget* parent = Q_NULLPTR);
    ~GLWidget();

    void setClearColor(const QColor &color);
    void setFrameData(QOpenGLTexture::PixelFormat sourceFormat,
        QOpenGLTexture::PixelType sourceType, const void *data, int width, int height);

protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    void resizeGL(int width, int height) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

private:
    void makeObject();
	void createTexture(int width, int height);

private:
    QColor clearColor;
    QOpenGLTexture* texture;
    QOpenGLShaderProgram* program;
    QOpenGLBuffer vbo;
};


#endif // GLWIDGET_H
