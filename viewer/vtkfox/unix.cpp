/*
* FXVTKCanvas Unix Specific Implementation
* 
* Author: Doug Henry (brilligent@gmail.com)
*/


#include "FXVTKCanvas.h"
#include "vtkFXRenderWindowInteractor.h"

#include <vtkRenderWindow.h>
#include <vtkCommand.h>


void FXVTKCanvas::render()
{
	makeCurrent();
	
	_fxrwi->GetRenderWindow()->SetWindowId(_id);
	_fxrwi->GetRenderWindow()->SetDisplayId(_display);
	_fxrwi->Render();
	
	makeNonCurrent();
}
