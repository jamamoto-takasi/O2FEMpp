/*
* vtkFXRenderWindowInteractor Implementation
* 
* Author: Doug Henry (brilligent@gmail.com)
*/


#include "vtkFXRenderWindowInteractor.h"
#include "FXVTKCanvas.h"

#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkInteractorStyle.h>
#include <vtkVersion.h>
#include <vtkCommand.h>


vtkFXRenderWindowInteractor::vtkFXRenderWindowInteractor()
		: vtkRenderWindowInteractor()
		, _canvas(NULL)
{}


vtkFXRenderWindowInteractor::vtkFXRenderWindowInteractor(FXVTKCanvas *canvas)
		: vtkRenderWindowInteractor()
		, _canvas(canvas)
{}


vtkFXRenderWindowInteractor::~vtkFXRenderWindowInteractor()
{}


void vtkFXRenderWindowInteractor::setCanvas(FXVTKCanvas *canvas)
{
	if (canvas != NULL)
		_canvas = canvas;
}


void vtkFXRenderWindowInteractor::Initialize()
{
	if (!RenderWindow)
	{
		vtkErrorMacro( << "vtkFXRenderWindowInteractor::Initialize has no render window");
	}
	
	else
	{
		int *size = RenderWindow->GetSize();
		Enable();
		
		Size[0] = size[0];
		Size[1] = size[1];
		
		Initialized = 1;
	}
}


void vtkFXRenderWindowInteractor::Enable()
{
	if (!Enabled)
	{
		Enabled = 1;
		
		Modified();
	}
}


void vtkFXRenderWindowInteractor::Disable()
{
	if (Enabled)
	{
		Enabled = 0;
		
		Modified();
	}
}


void vtkFXRenderWindowInteractor::Start()
{
	vtkErrorMacro( << "vtkFXRenderWindowInteractor::Start() interactor cannot control event loop.");
}


void vtkFXRenderWindowInteractor::SetRenderWindow(vtkRenderWindow *renderer)
{
	vtkRenderWindowInteractor::SetRenderWindow(renderer);
	
	if (RenderWindow)
	{
		RenderWindow->SetSize(_canvas->getWidth(), _canvas->getHeight());
	}
}


void vtkFXRenderWindowInteractor::UpdateSize(int w, int h)
{
	if (RenderWindow != NULL)
	{
		if ((w != Size[0]) || (h != Size[1]))
		{
			Size[0] = w;
			Size[1] = h;
			
			RenderWindow->SetSize(w, h);
			
			int *pos = RenderWindow->GetPosition();
			
			if ((pos[0] != _canvas->getX()) || (pos[1] != _canvas->getY()))
			{
				RenderWindow->SetPosition(_canvas->getX(), _canvas->getY());
			}
		}
	}
}


void vtkFXRenderWindowInteractor::TerminateApp()
{}
