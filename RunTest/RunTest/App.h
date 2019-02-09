//======================================================================
// Runtime test
//      By Damian Trebilco
//======================================================================

#include "../Framework3/OpenGL/OpenGLApp.h"


class App : public OpenGLApp {
public:

  App();

  char *getTitle() const override { return "Test"; }
  bool init() override;

  bool onKey(const uint key, const bool pressed) override;
  bool onMouseButton(const int x, const int y, const MouseButton button, const bool pressed) override;
  bool onMouseMove(const int x, const int y, const int deltaX, const int deltaY) override;

  void resetCamera() override;
  bool load() override;

  void drawFrame() override;

protected:

  mat4 m_projection; //!< The projection matrix used
  mat4 m_modelView;  //!< The model-view matrix used
};
