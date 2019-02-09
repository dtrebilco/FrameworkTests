//======================================================================
// Runtime test
//      By Damian Trebilco
//======================================================================

#include "App.h"
#include <vector>
#include <algorithm>

#include "LogInterface.h"

extern long g_allocRequestCount;
extern long g_allocFreeCount;

BaseApp *CreateApp() { return new App(); }

bool s_mouseLeftDown = false;
bool s_mouseRightDown = false;
int32_t s_mouseDownX = 0;
int32_t s_mouseDownY = 0;

bool dummy = false;

App::App()
{
}

bool App::init()
{
  speed = 100.0f;
  return OpenGLApp::init();
}

void App::resetCamera()
{
  BaseApp::resetCamera();
  camPos = vec3(-8.05934429f, 17.5981026f, -12.2385817f);
  wx = 0.360000521f;
  wy = -0.71699959f;

}

bool App::onKey(const uint key, const bool pressed)
{
  if (pressed)
  {
    switch (key)
    {
    case '1': 
      break;
    }
  }
  return BaseApp::onKey(key, pressed);
}

bool App::load()
{
  // Set the shader version used
  ((OpenGLRenderer*)renderer)->SetShaderVersionStr("#version 130");
  
  return true;
}

bool App::onMouseButton(const int x, const int y, const MouseButton button, const bool pressed)
{
  if (button == MOUSE_LEFT)
  {
    s_mouseLeftDown = pressed;
    s_mouseDownX = x;
    s_mouseDownY = y;
  }
  if (button == MOUSE_RIGHT)
  {
    s_mouseRightDown = pressed;
    s_mouseDownX = x;
    s_mouseDownY = y;
  }
  return OpenGLApp::onMouseButton(x, y, button, pressed);
}

bool App::onMouseMove(const int x, const int y, const int deltaX, const int deltaY)
{
  if (s_mouseLeftDown ||
    s_mouseRightDown)
  {
    s_mouseDownX = x;
    s_mouseDownY = y;
  }
  return OpenGLApp::onMouseMove(x, y, deltaX, deltaY);
}

void DrawBox(const vec3& i_pos, float i_size)
{
  glColor3f(1.0f, 0.0f, 0.0f);
  glVertex3f(i_pos.x - i_size, i_pos.y - i_size, i_pos.z - i_size);
  glVertex3f(i_pos.x - i_size, i_pos.y - i_size, i_pos.z + i_size);
  glVertex3f(i_pos.x + i_size, i_pos.y - i_size, i_pos.z + i_size);
  glVertex3f(i_pos.x + i_size, i_pos.y - i_size, i_pos.z - i_size);

  glVertex3f(i_pos.x - i_size, i_pos.y + i_size, i_pos.z - i_size);
  glVertex3f(i_pos.x - i_size, i_pos.y + i_size, i_pos.z + i_size);
  glVertex3f(i_pos.x + i_size, i_pos.y + i_size, i_pos.z + i_size);
  glVertex3f(i_pos.x + i_size, i_pos.y + i_size, i_pos.z - i_size);

  glColor3f(1.0f, 1.0f, 0.0f);
  glVertex3f(i_pos.x - i_size, i_pos.y - i_size, i_pos.z - i_size);
  glVertex3f(i_pos.x - i_size, i_pos.y - i_size, i_pos.z + i_size);
  glVertex3f(i_pos.x - i_size, i_pos.y + i_size, i_pos.z + i_size);
  glVertex3f(i_pos.x - i_size, i_pos.y + i_size, i_pos.z - i_size);

  glVertex3f(i_pos.x + i_size, i_pos.y - i_size, i_pos.z - i_size);
  glVertex3f(i_pos.x + i_size, i_pos.y - i_size, i_pos.z + i_size);
  glVertex3f(i_pos.x + i_size, i_pos.y + i_size, i_pos.z + i_size);
  glVertex3f(i_pos.x + i_size, i_pos.y + i_size, i_pos.z - i_size);

  glColor3f(0.0f, 1.0f, 1.0f);
  glVertex3f(i_pos.x - i_size, i_pos.y - i_size, i_pos.z - i_size);
  glVertex3f(i_pos.x - i_size, i_pos.y + i_size, i_pos.z - i_size);
  glVertex3f(i_pos.x + i_size, i_pos.y + i_size, i_pos.z - i_size);
  glVertex3f(i_pos.x + i_size, i_pos.y - i_size, i_pos.z - i_size);

  glVertex3f(i_pos.x - i_size, i_pos.y - i_size, i_pos.z + i_size);
  glVertex3f(i_pos.x - i_size, i_pos.y + i_size, i_pos.z + i_size);
  glVertex3f(i_pos.x + i_size, i_pos.y + i_size, i_pos.z + i_size);
  glVertex3f(i_pos.x + i_size, i_pos.y - i_size, i_pos.z + i_size);
}

void DrawBox(const mat4& i_transform)
{
  vec4 pos[8] =
  {
    i_transform * vec4(-1.0f, -1.0f, -1.0f, 1.0f), // 0
    i_transform * vec4(-1.0f, -1.0f, +1.0f, 1.0f), // 1
    i_transform * vec4(+1.0f, -1.0f, +1.0f, 1.0f), // 2
    i_transform * vec4(+1.0f, -1.0f, -1.0f, 1.0f), // 3
    i_transform * vec4(-1.0f, +1.0f, -1.0f, 1.0f), // 4
    i_transform * vec4(-1.0f, +1.0f, +1.0f, 1.0f), // 5
    i_transform * vec4(+1.0f, +1.0f, +1.0f, 1.0f), // 6
    i_transform * vec4(+1.0f, +1.0f, -1.0f, 1.0f)  // 7
  };

  glColor3f(1.0f, 0.0f, 0.0f);
  glVertex4fv(value_ptr(pos[0]));
  glVertex4fv(value_ptr(pos[1]));
  glVertex4fv(value_ptr(pos[2]));
  glVertex4fv(value_ptr(pos[3]));

  glVertex4fv(value_ptr(pos[4]));
  glVertex4fv(value_ptr(pos[5]));
  glVertex4fv(value_ptr(pos[6]));
  glVertex4fv(value_ptr(pos[7]));

  glColor3f(1.0f, 1.0f, 0.0f);
  glVertex4fv(value_ptr(pos[0]));
  glVertex4fv(value_ptr(pos[1]));
  glVertex4fv(value_ptr(pos[5]));
  glVertex4fv(value_ptr(pos[4]));

  glVertex4fv(value_ptr(pos[2]));
  glVertex4fv(value_ptr(pos[3]));
  glVertex4fv(value_ptr(pos[7]));
  glVertex4fv(value_ptr(pos[6]));

  glColor3f(0.0f, 1.0f, 1.0f);
  glVertex4fv(value_ptr(pos[0]));
  glVertex4fv(value_ptr(pos[4]));
  glVertex4fv(value_ptr(pos[7]));
  glVertex4fv(value_ptr(pos[3]));

  glVertex4fv(value_ptr(pos[1]));
  glVertex4fv(value_ptr(pos[5]));
  glVertex4fv(value_ptr(pos[6]));
  glVertex4fv(value_ptr(pos[2]));
}

void DrawBox(const mat4x3& i_transform)
{
  vec3 pos[8] =
  {
    i_transform * vec4(-1.0f, -1.0f, -1.0f, 1.0f), // 0
    i_transform * vec4(-1.0f, -1.0f, +1.0f, 1.0f), // 1
    i_transform * vec4(+1.0f, -1.0f, +1.0f, 1.0f), // 2
    i_transform * vec4(+1.0f, -1.0f, -1.0f, 1.0f), // 3
    i_transform * vec4(-1.0f, +1.0f, -1.0f, 1.0f), // 4
    i_transform * vec4(-1.0f, +1.0f, +1.0f, 1.0f), // 5
    i_transform * vec4(+1.0f, +1.0f, +1.0f, 1.0f), // 6
    i_transform * vec4(+1.0f, +1.0f, -1.0f, 1.0f)  // 7
  };

  glColor3f(1.0f, 0.0f, 0.0f);
  glVertex3fv(value_ptr(pos[0]));
  glVertex3fv(value_ptr(pos[1]));
  glVertex3fv(value_ptr(pos[2]));
  glVertex3fv(value_ptr(pos[3]));

  glVertex3fv(value_ptr(pos[4]));
  glVertex3fv(value_ptr(pos[5]));
  glVertex3fv(value_ptr(pos[6]));
  glVertex3fv(value_ptr(pos[7]));

  glColor3f(1.0f, 1.0f, 0.0f);
  glVertex3fv(value_ptr(pos[0]));
  glVertex3fv(value_ptr(pos[1]));
  glVertex3fv(value_ptr(pos[5]));
  glVertex3fv(value_ptr(pos[4]));

  glVertex3fv(value_ptr(pos[2]));
  glVertex3fv(value_ptr(pos[3]));
  glVertex3fv(value_ptr(pos[7]));
  glVertex3fv(value_ptr(pos[6]));

  glColor3f(0.0f, 1.0f, 1.0f);
  glVertex3fv(value_ptr(pos[0]));
  glVertex3fv(value_ptr(pos[4]));
  glVertex3fv(value_ptr(pos[7]));
  glVertex3fv(value_ptr(pos[3]));

  glVertex3fv(value_ptr(pos[1]));
  glVertex3fv(value_ptr(pos[5]));
  glVertex3fv(value_ptr(pos[6]));
  glVertex3fv(value_ptr(pos[2]));
}

void DrawWireBox(const vec3& i_center, const vec3& i_extents)
{
  vec3 pos[8] =
  {
    i_center + vec3(i_extents.x,   i_extents.y,  i_extents.z), // 0
    i_center + vec3(i_extents.x,  -i_extents.y,  i_extents.z), // 1
    i_center + vec3(i_extents.x,   i_extents.y, -i_extents.z), // 2
    i_center + vec3(i_extents.x,  -i_extents.y, -i_extents.z), // 3
    i_center + vec3(-i_extents.x,  i_extents.y,  i_extents.z), // 4
    i_center + vec3(-i_extents.x, -i_extents.y,  i_extents.z), // 5
    i_center + vec3(-i_extents.x,  i_extents.y, -i_extents.z), // 6
    i_center + vec3(-i_extents.x, -i_extents.y, -i_extents.z)  // 7
  };

  glVertex3fv(value_ptr(pos[0]));
  glVertex3fv(value_ptr(pos[1]));
  
  glVertex3fv(value_ptr(pos[0]));
  glVertex3fv(value_ptr(pos[2]));

  glVertex3fv(value_ptr(pos[2]));
  glVertex3fv(value_ptr(pos[3]));

  glVertex3fv(value_ptr(pos[1]));
  glVertex3fv(value_ptr(pos[3]));


  glVertex3fv(value_ptr(pos[4]));
  glVertex3fv(value_ptr(pos[5]));

  glVertex3fv(value_ptr(pos[4]));
  glVertex3fv(value_ptr(pos[6]));

  glVertex3fv(value_ptr(pos[6]));
  glVertex3fv(value_ptr(pos[7]));

  glVertex3fv(value_ptr(pos[5]));
  glVertex3fv(value_ptr(pos[7]));


  glVertex3fv(value_ptr(pos[0]));
  glVertex3fv(value_ptr(pos[4]));

  glVertex3fv(value_ptr(pos[7]));
  glVertex3fv(value_ptr(pos[3]));

  glVertex3fv(value_ptr(pos[1]));
  glVertex3fv(value_ptr(pos[5]));

  glVertex3fv(value_ptr(pos[6]));
  glVertex3fv(value_ptr(pos[2]));
}

void App::drawFrame()
{

  m_projection = perspectiveMatrixX(1.5f, width, height, 0.1f, 4000);
  //mat4 modelview = scale(1.0f, 1.0f, -1.0f) * rotateXY(-wx, -wy) * translate(-camPos) * rotateX(PI * 0.5f);
  m_modelView = rotateXY(-wx, -wy) * translate(-camPos);

  glMatrixMode(GL_PROJECTION);
  glLoadMatrixf(value_ptr(m_projection));

  glMatrixMode(GL_MODELVIEW);
  glLoadMatrixf(value_ptr(m_modelView));

  float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
  renderer->clear(true, true, false, clearColor);

  renderer->reset();
  renderer->apply();

  // Floor
  glColor3f(0.0f, 1.0f, 0.0f);
  glBegin(GL_LINES);
  for (uint32_t i = 0; i <= 100; i++)
  {
    glVertex3i(i, 0, 0);
    glVertex3i(i, 0, 100);
  }
  for (uint32_t i = 0; i <= 100; i++)
  {
    glVertex3i(0, 0, i);
    glVertex3i(100, 0, i);
  }
  glEnd();

 
  // Boxes
  glBegin(GL_QUADS);

  glEnd();


  glBegin(GL_LINES);
  glEnd();

  renderer->setup2DMode(0, (float)width, 0, (float)height);

  // Draw text data to the screen 
  renderer->reset();
  //renderer->setShader(m_gridDraw);
  //renderer->setTexture("perlinTex", m_perlin);
  renderer->apply();

  renderer->reset();
  renderer->setDepthState(noDepthWrite);
  renderer->apply();

  {
    char buffer[100];
    float xPos = (float)width - 250.0f;

#ifdef _DEBUG
    sprintf(buffer, "Alloc Count %d", g_allocRequestCount);
    renderer->drawText(buffer, xPos, 138.0f, 30, 38, defaultFont, linearClamp, blendSrcAlpha, noDepthTest);

    sprintf(buffer, "Free Count %d", g_allocFreeCount);
    renderer->drawText(buffer, xPos, 168.0f, 30, 38, defaultFont, linearClamp, blendSrcAlpha, noDepthTest);

    sprintf(buffer, "Working Count %d", g_allocRequestCount - g_allocFreeCount);
    renderer->drawText(buffer, xPos, 198.0f, 30, 38, defaultFont, linearClamp, blendSrcAlpha, noDepthTest);
#endif // _DEBUG
  }
}
