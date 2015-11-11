#include "xincs.h"
#include "fxver.h"
#include "fxdefs.h"
#include "FXHash.h"
#include "FXThread.h"
#include "FXStream.h"
#include "FXVec2f.h"
#include "FXVec3f.h"
#include "FXVec4f.h"
#include "FXQuatf.h"
#include "FXMat4f.h"
#include "FXRangef.h"
#include "FXString.h"
#include "FXSize.h"
#include "FXPoint.h"
#include "FXRectangle.h"
#include "FXRegistry.h"
#include "FXAccelTable.h"
#include "FXObjectList.h"
#include "FXApp.h"
#include "FXGLViewer.h"
#include "fx_gl_shapes.h"

using namespace FX;

namespace FoxGL {

/********************************************************************
FXGLDemoShape Implementation
********************************************************************/
// Object implementation
FXIMPLEMENT(FXGLDemoShape,FXGLShape,NULL,0)


// Create shape
FXGLDemoShape::FXGLDemoShape():angle(0.0f){
  FXTRACE((100,"FXGLDemoShape::FXGLDemoShape\n"));
}


// Create shape with material
FXGLDemoShape::FXGLDemoShape(const FXMaterial& mtl){
  FXTRACE((100,"FXGLDemoShape::FXGLDemoShape\n"));
}


// Copy constructor
FXGLDemoShape::FXGLDemoShape(const FXGLDemoShape& orig):FXGLShape(orig){
  FXTRACE((100,"FXGLDemoShape::FXGLDemoShape\n"));
  angle=orig.angle;
}


void FXGLDemoShape::createTab(FXTabBook* tabbook, const FXString& text) {
  FXTabItem *newtab = new FXTabItem(tabbook,text);
  FXVerticalFrame *settings=new FXVerticalFrame(tabbook,FRAME_THICK|FRAME_RAISED|LAYOUT_FILL_Y|LAYOUT_CENTER_X|LAYOUT_TOP|LAYOUT_LEFT,0,0,0,0,10,10,10,10);
//    new FXCheckButton(settings,"Lighting",mdiclient,FXGLViewer::ID_LIGHTING,ICON_BEFORE_TEXT);
//    new FXCheckButton(settings,"Fog",mdiclient,FXGLViewer::ID_FOG,ICON_BEFORE_TEXT);
//    new FXCheckButton(settings,"Dither",mdiclient,FXGLViewer::ID_DITHER,ICON_BEFORE_TEXT);
//    new FXCheckButton(settings,"Turbo",mdiclient,FXGLViewer::ID_TURBO,ICON_BEFORE_TEXT);
  newtab->create();
  settings->create();
  tabbook->layout(); //update the tab book immediately
}
  

// Draw
void FXGLDemoShape::drawshape(FXGLViewer* glview){
#ifdef HAVE_GL_H
  const GLfloat lightPosition[]={15.,10.,5.,1.};
  const GLfloat lightAmbient[]={.1f,.1f,.1f,1.};
  const GLfloat lightDiffuse[]={.9f,.9f,.9f,1.};
  const GLfloat redMaterial[]={1.,0.,0.,1.};
  const GLfloat blueMaterial[]={0.,0.,1.,1.};

  GLdouble width = glview->getWidth();
  GLdouble height = glview->getHeight();
  GLdouble aspect = height>0 ? width/height : 1.0;

//  glClearColor(1.0,1.0,1.0,1.0);
//  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
//  glEnable(GL_DEPTH_TEST);

//  glDisable(GL_DITHER);

  // TODO: study this 2 those comment command below to enable better rendering
//  glMatrixMode(GL_PROJECTION);
//  glLoadIdentity();
//  gluPerspective(30.,aspect,1.,100.);

//  glMatrixMode(GL_MODELVIEW);
//  glLoadIdentity();
//  gluLookAt(5.,10.,15.,0.,0.,0.,0.,1.,0.);

  glShadeModel(GL_SMOOTH);
  glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
  glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHTING);

  glMaterialfv(GL_FRONT, GL_AMBIENT, blueMaterial);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, blueMaterial);

  glPushMatrix();
  glRotated(angle, 0., 1., 0.);
  drawBox(-1, -1, -1, 1, 1, 1);

  glMaterialfv(GL_FRONT, GL_AMBIENT, redMaterial);
  glMaterialfv(GL_FRONT, GL_DIFFUSE, redMaterial);

  glPushMatrix();
  glTranslated(0.,1.75,0.);
  glRotated(angle, 0., 1., 0.);
  drawBox(-.5,-.5,-.5,.5,.5,.5);
  glPopMatrix();

  glPushMatrix();
  glTranslated(0.,-1.75,0.);
  glRotated(angle, 0., 1., 0.);
  drawBox(-.5,-.5,-.5,.5,.5,.5);
  glPopMatrix();

  glPushMatrix();
  glRotated(90., 1., 0., 0.);
  glTranslated(0.,1.75,0.);
  glRotated(angle, 0., 1., 0.);
  drawBox(-.5,-.5,-.5,.5,.5,.5);
  glPopMatrix();

  glPushMatrix();
  glRotated(90., -1., 0., 0.);
  glTranslated(0.,1.75,0.);
  glRotated(angle, 0., 1., 0.);
  drawBox(-.5,-.5,-.5,.5,.5,.5);
  glPopMatrix();

  glPushMatrix();
  glRotated(90., 0., 0., 1.);
  glTranslated(0.,1.75,0.);
  glRotated(angle, 0., 1., 0.);
  drawBox(-.5,-.5,-.5,.5,.5,.5);
  glPopMatrix();

  glPushMatrix();
  glRotated(90., 0., 0., -1.);
  glTranslated(0.,1.75,0.);
  glRotated(angle, 0., 1., 0.);
  drawBox(-.5,-.5,-.5,.5,.5,.5);
  glPopMatrix();

  glPopMatrix();
#endif
}


// Copy this object
FXGLObject* FXGLDemoShape::copy(){
  return new FXGLDemoShape(*this);
}


// Save object to stream
void FXGLDemoShape::save(FXStream& store) const {
  FXGLShape::save(store);
  store << angle;
}


// Load object from stream
void FXGLDemoShape::load(FXStream& store){
  FXGLShape::load(store);
  store >> angle;
}


// Destroy
FXGLDemoShape::~FXGLDemoShape(){
  FXTRACE((100,"FXGLDemoShape::~FXGLDemoShape\n"));
}

/********************************************************************
End of FXGLDemoShape Implementation
********************************************************************/



// Draws a simple box using the given corners
void drawBox(GLfloat xmin, GLfloat ymin, GLfloat zmin, GLfloat xmax, GLfloat ymax, GLfloat zmax) {
  glBegin(GL_TRIANGLE_STRIP);
    glNormal3f(0.,0.,-1.);
    glVertex3f(xmin, ymin, zmin);
    glVertex3f(xmin, ymax, zmin);
    glVertex3f(xmax, ymin, zmin);
    glVertex3f(xmax, ymax, zmin);
  glEnd();

  glBegin(GL_TRIANGLE_STRIP);
    glNormal3f(1.,0.,0.);
    glVertex3f(xmax, ymin, zmin);
    glVertex3f(xmax, ymax, zmin);
    glVertex3f(xmax, ymin, zmax);
    glVertex3f(xmax, ymax, zmax);
  glEnd();

  glBegin(GL_TRIANGLE_STRIP);
    glNormal3f(0.,0.,1.);
    glVertex3f(xmax, ymin, zmax);
    glVertex3f(xmax, ymax, zmax);
    glVertex3f(xmin, ymin, zmax);
    glVertex3f(xmin, ymax, zmax);
  glEnd();

  glBegin(GL_TRIANGLE_STRIP);
    glNormal3f(-1.,0.,0.);
    glVertex3f(xmin, ymin, zmax);
    glVertex3f(xmin, ymax, zmax);
    glVertex3f(xmin, ymin, zmin);
    glVertex3f(xmin, ymax, zmin);
  glEnd();

  glBegin(GL_TRIANGLE_STRIP);
    glNormal3f(0.,1.,0.);
    glVertex3f(xmin, ymax, zmin);
    glVertex3f(xmin, ymax, zmax);
    glVertex3f(xmax, ymax, zmin);
    glVertex3f(xmax, ymax, zmax);
  glEnd();

  glBegin(GL_TRIANGLE_STRIP);
    glNormal3f(0.,-1.,0.);
    glVertex3f(xmax, ymin, zmax);
    glVertex3f(xmax, ymin, zmin);
    glVertex3f(xmin, ymin, zmax);
    glVertex3f(xmin, ymin, zmin);
  glEnd();
}

}

