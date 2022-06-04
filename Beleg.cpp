/* ---------------------------------------------------------------
   name:           Beleg.cpp
   purpose:        Beleg Template
   version:	   STARTER CODE
   author:         katrin lang
                   htw berlin
   ------------------------------------------------------------- */

#include "GLIncludes.hpp"

#include <iostream> 

#include "Beleg.hpp"
#include "Context.hpp"
#include "Input.hpp"

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"



// use this with care
// might cause name collisions
using namespace glm;
using namespace std;

// field of view
GLfloat Beleg::fov= 45.0;
GLfloat Beleg::cameraY = 10;
GLfloat Beleg::cameraZ= 3;

mat4 Beleg::projectionMatrix, Beleg::viewMatrix, Beleg::modelMatrix(1);
TriangleMesh Beleg::mesh;
glsl::Shader Beleg::diffuseShader;
float Beleg::scaling = 2.0; // scale

LightSource Beleg::lightSource={
    // position
  glm::vec4(20, 200, 0, 1),
  // ambient color
  glm::vec4(1.0, 1.0, 1.0, 1.0),
  // diffuse color
  glm::vec4(1.0, 1.0, 1.0, 1.0),
  // specular color
  glm::vec4(1.0, 1.0, 1.0, 1.0),
};

glm::vec3 translationA(3,0,0);
glm::vec3 translationC(2, 0, -2);
glm::vec3 translationE(1, 0, -4);

void Beleg::init(){
    mesh.load("meshes/cube.off");

    const std::string version = "#version 120\n";

    diffuseShader.addVertexShader(version);
    diffuseShader.loadVertexShader("shaders/diffuse.vert");
    diffuseShader.compileVertexShader();
    diffuseShader.addFragmentShader(version);
    diffuseShader.loadFragmentShader("shaders/diffuse.frag");
    diffuseShader.compileFragmentShader();
    diffuseShader.bindVertexAttrib("position", TriangleMesh::attribPosition);
    diffuseShader.bindVertexAttrib("normal", TriangleMesh::attribNormal);
    diffuseShader.link();
  // set background color to black
  glClearColor(0.0,0.0,0.0,1.0);
  glPolygonMode(GL_FRONT, GL_FILL);
    
  // enable depth test (z-buffer)
  glDepthFunc(GL_LESS);
  glEnable(GL_DEPTH_TEST);

  // enable antialiasing
  glEnable(GL_MULTISAMPLE);
}

// adjust to new window size
void Beleg::reshape(){
    
  // viewport
  glViewport(0, 0, (GLsizei) window->width(), (GLsizei) window->height());

  computeViewMatrix();
  computeProjectionMatrix();
}

void Beleg::computeViewMatrix(void){

  viewMatrix= glm::lookAt(vec3(0,10,5), vec3(2, 0, -2), vec3(0,1,0));
}

void Beleg::computeProjectionMatrix(void){

  // reshaped window aspect ratio
  float aspect= (float) window->width() / (float) window->height();
  
  // compute near and far plane
  float nearPlane=cameraZ/10.0f; 
  float farPlane= cameraZ*10.0f;
  
  projectionMatrix= glm::perspective(radians(fov), aspect, nearPlane, farPlane);
}

// this is where the drawing happens
void Beleg::display(void){
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  diffuseShader.bind();

  {
      glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
      glm::mat4 mvp = projectionMatrix * viewMatrix * model;
      diffuseShader.setUniform("transformation", mvp);
      diffuseShader.setUniform("color", vec3(1, 1, 1));
      diffuseShader.setUniform("lightPosition", inverse(modelMatrix) * lightSource.position);
      mesh.draw();
  }
 
  {
      glm::mat4 model = glm::translate(glm::mat4(1.0f), translationC);
      glm::mat4 mvp = projectionMatrix * viewMatrix * model;
      diffuseShader.setUniform("transformation", mvp);
      diffuseShader.setUniform("color", vec3(1, 1, 1));
      diffuseShader.setUniform("lightPosition", inverse(modelMatrix) * lightSource.position);
      mesh.draw();
  }
 
  {
      glm::mat4 model = glm::translate(glm::mat4(1.0f), translationE);
      glm::mat4 mvp = projectionMatrix * viewMatrix * model;
      diffuseShader.setUniform("transformation", mvp);
      diffuseShader.setUniform("color", vec3(1, 1, 1));
      diffuseShader.setUniform("lightPosition", inverse(modelMatrix) * lightSource.position);
      mesh.draw();
  }
  for (int x = 0; x < 5; x++) {
      if (x == 0 || x == 4) {
          for (int i = 0; i < 5; i++) {
              
           glm::vec3 translation(x, 0, -i);
           glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
           glm::mat4 mvp = projectionMatrix * viewMatrix * model;
           diffuseShader.setUniform("transformation", mvp);
           diffuseShader.setUniform("color", vec3(1, 1, 1));
           diffuseShader.setUniform("lightPosition", inverse(modelMatrix) * lightSource.position);
           mesh.draw();                     
              
             
          }
      }
      else continue;
  }
  
  
  
  diffuseShader.unbind();
  
  // display back buffer
  window->swapBuffers();
}

// keyboard control
void Beleg::keyPressed(){
  
  // rotate selected node around 
  // x,y and z axes with keypresses
  switch(keyboard->key){
    
  case 'q':
  case 'Q':
    exit(0);

  default:
    break;
  }
}

// keyboard callback for special keys
// (arrow keys for node selection)
void Beleg::specialKey(){

  // rotate selected node around 
  // x,y and z axes with keypresses
  switch(keyboard->code) {

  case Keyboard::Code::UP:
    
    break;
  case Keyboard::Code::DOWN:
    
    break;
  case Keyboard::Code::LEFT:
    
    break;
  case Keyboard::Code::RIGHT:
    
    break;
  default:
    break;
  }
}

vector< pair < int, string > > Beleg::menuEntries{{Menu::QUIT, "quit"}};

// mouse menu control
void Beleg::menu(int id){
  
  switch (id) {
  case Menu::QUIT: 
    exit(0);

  default:
    break;
  }
}

int main(int argc, char** argv){

  // initialize OpenGL context
  OpenGLContext<Beleg>::init(&argc, argv);

  // some diagnostic output
  std::cout << "GPU: " << glGetString(GL_RENDERER) << ", OpenGL version: " << glGetString(GL_VERSION) << ", GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    
  //start event loop
  InputManager<Beleg>::startEventLoop();
  
  return 0;
}
