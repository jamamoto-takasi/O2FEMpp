/// Created by hbui on 20 Jul 2014

#ifndef _FX_GL_SHAPES_H_
#define _FX_GL_SHAPES_H_

#include <cstring>

#include "fx.h"

#ifndef FXGLSHAPE_H
#include "FXGLShape.h"
#endif

using namespace FX;

namespace FoxGL
{

void drawBox(GLfloat xmin, GLfloat ymin, GLfloat zmin, GLfloat xmax, GLfloat ymax, GLfloat zmax);

/// Shape object that used in the gltest example
class FXAPI FXGLDemoShape : public FXGLShape {
  FXDECLARE(FXGLDemoShape)
public:
  FXfloat angle;
protected:
  virtual void drawshape(FXGLViewer* viewer);
public:
  /// Default constructor
  FXGLDemoShape();

  /// Constructor with material
  FXGLDemoShape(const FXMaterial& mtl);

  /// Copy constructor
  FXGLDemoShape(const FXGLDemoShape& orig);

  /// Copy this object
  virtual FXGLObject* copy();

  /// Change angle
  void setAngle(FXfloat a){ angle=a; }
  FXfloat getAngle() const { return angle; }
  
  // Create new tab item for control
  void createTab(FXTabBook* tabbook, const FXString& text);
  
  /// Save to a stream
  virtual void save(FXStream& store) const;

  /// Load from a stream
  virtual void load(FXStream& store);

  /// Destroy
  virtual ~FXGLDemoShape();
};
  
}

#endif

