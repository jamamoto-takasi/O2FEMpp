/*
* FXVTKCanvas Widget Definition
* 
* This class is intended to be a drop-in replacement for FXGLCanvas, with
* the exception that it allows for VTK rendering.  This class holds an
* instance of vtkFXRenderWindowInteractor, mapping all appropriate functions
* and events, to allow for proper VTK use.
* 
* Author: Doug Henry (brilligent@gmail.com)
*/


#ifndef FXVTKCanvas_DEF
#define FXVTKCanvas_DEF


#include <fx.h>
#include <FXGLCanvas.h>

/*
 * Fix for API change in FOX 1.6 -> 1.7
 */
#if FOX_MAJOR >= 1 && FOX_MINOR > 6
#define VISUAL_DOUBLEBUFFER VISUAL_DOUBLE_BUFFER
#endif

class vtkFXRenderWindowInteractor;


class FXVTKCanvas : public FXGLCanvas
{
		FXDECLARE(FXVTKCanvas)
		
	public:
		FXVTKCanvas(FXComposite *p, FXGLVisual *vis, FXObject *tgt = NULL, FXSelector sel = 0, FXuint opts = 0, FXint x = 0, FXint y = 0, FXint w = 0, FXint h = 0);
		
		~FXVTKCanvas();
		
		vtkFXRenderWindowInteractor* getInteractor() const;
		void setInteractor(vtkFXRenderWindowInteractor *fxrwi);
		
		void create();
		void destroy();
		void render();
		
		long onPaint(FXObject *obj, FXSelector sel, void *data);
		long onResize(FXObject *obj, FXSelector sel, void *data);
		
		long onLeftButtonDown(FXObject *obj, FXSelector sel, void *data);
		long onLeftButtonUp(FXObject *obj, FXSelector sel, void *data);
		long onMiddleButtonDown(FXObject *obj, FXSelector sel, void *data);
		long onMiddleButtonUp(FXObject *obj, FXSelector sel, void *data);
		long onRightButtonDown(FXObject *obj, FXSelector sel, void *data);
		long onRightButtonUp(FXObject *obj, FXSelector sel, void *data);
		long onMotion(FXObject *obj, FXSelector sel, void *data);
		long onKeyboard(FXObject *obj, FXSelector sel, void *data);
		long onMouseWheel(FXObject *obj, FXSelector sel, void *data);
		
	protected:
		FXVTKCanvas()
		{}
		
		// not implemented (should cause compile error if object is copied)
		FXVTKCanvas(const FXVTKCanvas& FVC);
		FXVTKCanvas& operator=(const FXVTKCanvas& FVC);
		
	private:
		vtkFXRenderWindowInteractor *_fxrwi;
		void *_id;
		void *_display;
};


#endif
