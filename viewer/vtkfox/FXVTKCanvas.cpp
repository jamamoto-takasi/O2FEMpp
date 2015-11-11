/*
* FXVTKCanvas Widget Implementation
* 
* Author: Doug Henry (brilligent@gmail.com)
*/


#include "FXVTKCanvas.h"
#include "vtkFXRenderWindowInteractor.h"

#include <vtkRenderWindow.h>
#include <vtkCommand.h>


FXDEFMAP(FXVTKCanvas) FXVTKCanvasMap[] =
    {
        FXMAPFUNC(SEL_PAINT, 0, FXVTKCanvas::onPaint),
        FXMAPFUNC(SEL_CONFIGURE, 0, FXVTKCanvas::onResize),
        
        FXMAPFUNC(SEL_LEFTBUTTONPRESS, 0, FXVTKCanvas::onLeftButtonDown),
        FXMAPFUNC(SEL_LEFTBUTTONRELEASE, 0, FXVTKCanvas::onLeftButtonUp),
        FXMAPFUNC(SEL_MIDDLEBUTTONPRESS, 0, FXVTKCanvas::onMiddleButtonDown),
        FXMAPFUNC(SEL_MIDDLEBUTTONRELEASE, 0, FXVTKCanvas::onMiddleButtonUp),
        FXMAPFUNC(SEL_RIGHTBUTTONPRESS, 0, FXVTKCanvas::onRightButtonDown),
        FXMAPFUNC(SEL_RIGHTBUTTONRELEASE, 0, FXVTKCanvas::onRightButtonUp),
        FXMAPFUNC(SEL_MOTION, 0, FXVTKCanvas::onMotion),
        FXMAPFUNC(SEL_KEYPRESS, 0, FXVTKCanvas::onKeyboard),
        FXMAPFUNC(SEL_MOUSEWHEEL, 0, FXVTKCanvas::onMouseWheel)
    };
    
FXIMPLEMENT(FXVTKCanvas, FXGLCanvas, FXVTKCanvasMap, ARRAYNUMBER(FXVTKCanvasMap))


/**
 * FOX compatible constructor.  Should match the constructor given in FOX itself for
 * the FXGLCanvas widget.
 */

FXVTKCanvas::FXVTKCanvas(FXComposite *p, FXGLVisual *vis, FXObject *tgt, FXSelector sel, FXuint opts, FXint x, FXint y, FXint w, FXint h)
		: FXGLCanvas(p, vis, tgt, sel, opts, x, y, w, h)
		, _fxrwi(NULL)
		, _id(NULL)
		, _display(NULL)
{
	_fxrwi = new vtkFXRenderWindowInteractor(this);
}


/**
 * Delete the interactor using the VTK Delete reference counting
 * method.
 */

FXVTKCanvas::~FXVTKCanvas()
{
	_fxrwi->Delete();
}


/**
 * Get the internally stored interactor.
 * 
 * @return pointer to vtkFOX interactor
 */

vtkFXRenderWindowInteractor* FXVTKCanvas::getInteractor() const
{
	return _fxrwi;
}


/**
 * Set the internal interactor, deleting the current interactor if one
 * exists.  This class will maintain control of the interactor (e.g. clean it up).
 * 
 * @param fxrwi pointer to a vtkFOX interactor
 */

void FXVTKCanvas::setInteractor(vtkFXRenderWindowInteractor *fxrwi)
{
	if (fxrwi != NULL)
	{
		_fxrwi->Delete();
		_fxrwi = fxrwi;
		
		_fxrwi->setCanvas(this);
	}
}


/**
 * Standard FOX create.  The window id and display id are stored
 * for use with VTK.
 */

void FXVTKCanvas::create()
{
	FXGLCanvas::create();
	
	_id = (void*)id();
	_display = getApp()->getDisplay();	
}

/**
 * Finalize the server side resources associated with VTK.
 */
void FXVTKCanvas::destroy()
{
	_fxrwi->GetRenderWindow()->Finalize();
}


/**
 * Repaint callback handler.  Sets the window and display ids and then
 * calls the VTK renderer.
 */

long FXVTKCanvas::onPaint(FXObject *obj, FXSelector sel, void *data)
{	
	render();
		
	return 1;
}


/**
 * Resize callback handler.  Tell VTK that our window size has changed.
 */

long FXVTKCanvas::onResize(FXObject *obj, FXSelector sel, void *data)
{
	_fxrwi->UpdateSize(getWidth(), getHeight());
	
	return 1;
}


/**
 * Left mouse button pressed callback handler.  Fires the appropriate
 * VTK event.
 */

long FXVTKCanvas::onLeftButtonDown(FXObject *obj, FXSelector sel, void *data)
{
	FXEvent *event = (FXEvent*)data;
	
	grab();
	
	_fxrwi->SetEventInformationFlipY(event->win_x, event->win_y);
	_fxrwi->InvokeEvent(vtkCommand::LeftButtonPressEvent, NULL);
	
	setFocus();

	if (target) {
		return (target->tryHandle(this, FXSEL(SEL_LEFTBUTTONPRESS, message), data));
	}

	return 1;
}


/**
 * Left mouse button released callback handler.  Fires the appropriate
 * VTK event.
 */

long FXVTKCanvas::onLeftButtonUp(FXObject *obj, FXSelector sel, void *data)
{
	FXEvent *event = (FXEvent*)data;
	
	_fxrwi->SetEventInformationFlipY(event->win_x, event->win_y);
	_fxrwi->InvokeEvent(vtkCommand::LeftButtonReleaseEvent, NULL);
	
	ungrab();
	
	if (target) {
		return (target->tryHandle(this, FXSEL(SEL_LEFTBUTTONRELEASE, message), data));
	}

	return 1;
}


/**
 * Middle mouse button pressed callback handler.  Fires the appropriate
 * VTK event.
 */

long FXVTKCanvas::onMiddleButtonDown(FXObject *obj, FXSelector sel, void *data)
{
	FXEvent *event = (FXEvent*)data;
	
	grab();
	
	_fxrwi->SetEventInformationFlipY(event->win_x, event->win_y);
	_fxrwi->InvokeEvent(vtkCommand::MiddleButtonPressEvent, NULL);
	
	setFocus();
	
	if (target) {
		return (target->tryHandle(this, FXSEL(SEL_MIDDLEBUTTONPRESS, message), data));
	}

	return 1;
}


/**
 * Middle mouse button released callback handler.  Fires the appropriate
 * VTK event.
 */

long FXVTKCanvas::onMiddleButtonUp(FXObject *obj, FXSelector sel, void *data)
{
	FXEvent *event = (FXEvent*)data;
	
	_fxrwi->SetEventInformationFlipY(event->win_x, event->win_y);
	_fxrwi->InvokeEvent(vtkCommand::MiddleButtonReleaseEvent, NULL);
	
	ungrab();

	if (target) {
		return (target->tryHandle(this, FXSEL(SEL_MIDDLEBUTTONRELEASE, message), data));
	}

	return 1;
}


/**
 * Right mouse button pressed callback handler.  Fires the appropriate
 * VTK event.
 */

long FXVTKCanvas::onRightButtonDown(FXObject *obj, FXSelector sel, void *data)
{
	FXEvent *event = (FXEvent*)data;
	
	grab();
	
	_fxrwi->SetEventInformationFlipY(event->win_x, event->win_y);
	_fxrwi->InvokeEvent(vtkCommand::RightButtonPressEvent, NULL);
	
	setFocus();
	
	if (target) {
		return (target->tryHandle(this, FXSEL(SEL_RIGHTBUTTONPRESS, message), data));
	}

	return 1;
}


/**
 * Right mouse button released callback handler.  Fires the appropriate
 * VTK event.
 */

long FXVTKCanvas::onRightButtonUp(FXObject *obj, FXSelector sel, void *data)
{
	FXEvent *event = (FXEvent*)data;
	
	_fxrwi->SetEventInformationFlipY(event->win_x, event->win_y);
	_fxrwi->InvokeEvent(vtkCommand::RightButtonReleaseEvent, NULL);
	
	ungrab();
	
	if (target) {
		return (target->tryHandle(this, FXSEL(SEL_RIGHTBUTTONRELEASE, message), data));
	}

	return 1;
}


/**
 * Mouse motion callback handler.  Fires the appropriate
 * VTK event.
 */

long FXVTKCanvas::onMotion(FXObject *obj, FXSelector sel, void *data)
{
	FXEvent *event = (FXEvent*)data;
	
	_fxrwi->SetEventInformationFlipY(event->win_x, event->win_y);
	_fxrwi->InvokeEvent(vtkCommand::MouseMoveEvent, NULL);
	
	return 1;
}


/**
 * Key press callback handler.  Fires the appropriate
 * VTK event.
 */

long FXVTKCanvas::onKeyboard(FXObject *obj, FXSelector sel, void *data)
{
	FXEvent *event = (FXEvent*)data;
	
	//fxmessage("keypress: %s %d %d\n", event->text.text(), event->state, event->code);
	
	if (event->text != FXString::null)
	{
		_fxrwi->SetEventInformationFlipY(event->win_x, event->win_y, event->state & CONTROLMASK, event->state & SHIFTMASK, event->code, 1);
		_fxrwi->InvokeEvent(vtkCommand::KeyPressEvent, NULL);
		_fxrwi->InvokeEvent(vtkCommand::CharEvent, NULL);
	}
	
	if (target) {
		return (target->tryHandle(this, FXSEL(SEL_KEYPRESS, message), data));
	}

	return 1;
}


/**
 * Mouse wheel callback handler.  Fires the appropriate
 * VTK event.
 */

long FXVTKCanvas::onMouseWheel(FXObject *obj, FXSelector sel, void *data)
{
	FXEvent *event = (FXEvent*)data;

	if (event->code > 0) {
	   _fxrwi->InvokeEvent(vtkCommand::MouseWheelForwardEvent, NULL);
	} else {
	   _fxrwi->InvokeEvent(vtkCommand::MouseWheelBackwardEvent, NULL);
	}

	if (target) {
		return (target->tryHandle(this, FXSEL(SEL_MOUSEWHEEL, message), data));
	}

	return 1;
}
