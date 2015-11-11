from Tkinter import *
import sys
import vtk
from vtk.tk.vtkTkRenderWindowInteractor import vtkTkRenderWindowInteractor

class Viewer:
    def __init__(self):
        ## create VTK renderer
        self.render = vtk.vtkRenderer()
        self.render.SetBackground(0.3, 0.4, 0.5)
        self.render.ResetCameraClippingRange()

        ## create VTK render window
        self.renWindow = vtk.vtkRenderWindow()
        self.renWindow.AddRenderer(self.render)

        ## create Tkinter GUI
        self.root = Tk()
        self.root.title("OOFEM Viewer")
        gui_frame = Frame(self.root)
        gui_frame.pack(fill=BOTH, expand=1, side=TOP)

        self.render_frame = Frame(gui_frame)
        self.render_frame.pack(fill=BOTH, expand=1, side=LEFT)

        self.control_frame = Frame(gui_frame)
        self.control_frame.pack(fill=BOTH, expand=1, side=RIGHT)
        self.button_surface = Button(self.control_frame, text="Surface", width=10, command=self.Surface)
        self.button_surface.pack(side=TOP)
        self.button_wireframe = Button(self.control_frame, text="Wireframe", width=10, command=self.Wireframe)
        self.button_wireframe.pack(side=TOP)
        self.button_fit = Button(self.control_frame, text="Fit", width=10, command=self.ResetView)
        self.button_fit.pack(side=TOP)
        self.button_exit = Button(self.control_frame, text="Exit", width=10, command=gui_frame.quit)
        self.button_exit.pack(side=TOP)

    ###########################################################
    # reset view
    def ResetView(self):
        self.render.ResetCamera()
        self.render.ResetCameraClippingRange()
        self.renWindow.Render()

    # set the representation of all actors to wireframe.
    def Wireframe(self):
        actors = self.render.GetActors()
        actors.InitTraversal()
        actor = actors.GetNextItem()
        while actor:
            actor.GetProperty().SetRepresentationToWireframe()
            actor = actors.GetNextItem()
        self.renWindow.Render()

    # set the representation of all actors to surface.
    def Surface(self):
        actors = self.render.GetActors()
        actors.InitTraversal()
        actor = actors.GetNextItem()
        while actor:
            actor.GetProperty().SetRepresentationToSurface()
            actor = actors.GetNextItem()
        self.renWindow.Render()

    # add a primitive to the VTK renderer. It needs to be an actor
    def AddPrimitive(self, prim):
        prim.Finalize()
        self.render.AddActor(prim.GetActor())

    # start the render loop
    def Start(self):
        ## create VTK interactor (this is the most important object)
        # this need a Tkinter GUI object (render_frame) and a VTK render window (renWindow) to work
        self.renWinInteractor = vtkTkRenderWindowInteractor(self.render_frame, rw=self.renWindow, width=600, height=600)
        self.renWinInteractor.SetInteractorStyle(vtk.vtkInteractorStyleTrackballCamera()) # set the trackball behaviour
        # we can add additional configurations for this interactor here
        axes = vtk.vtkAxesActor()
        marker = vtk.vtkOrientationMarkerWidget()
        marker.SetOutlineColor(0.93, 0.57, 0.13)
        marker.SetOrientationMarker(axes)
        marker.SetInteractor(self.renWinInteractor)
        marker.SetViewport(0.0, 0.0, 0.4, 0.4)
        marker.SetEnabled(1)
        marker.InteractiveOff()

        ## initialize the interactor and align it to the frame
        self.renWinInteractor.Initialize()
        self.renWinInteractor.pack(side='top', fill='both', expand=1)
        self.renWindow.Render()
        self.renWinInteractor.Start()

        ## main loop
        self.root.mainloop()

