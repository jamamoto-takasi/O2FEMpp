import sys
import os
import math
sys.path.append(os.getcwd() + "/../libs")
sys.path.append(os.getcwd() + "/../python_scripts")

from OOFEM import *
import visualizer

lb = 15.0
r  = 457.2 / 2000
t  = 10.0 / 1000
a  = math.pi * (math.pow(r, 2) - math.pow(r - t, 2))
e  = 2.1e11

struct = Structure("Tetraeder");

n1 = struct.AddNode(0.0, 0.0, lb * math.sqrt(2.0 / 3.0));
n2 = struct.AddNode(0.0, lb / math.sqrt(3), 0.0);
n3 = struct.AddNode(-lb / 2, -lb / math.sqrt(12.0), 0.0);
n4 = struct.AddNode(lb / 2, -lb / math.sqrt(12.0), 0.0);

struct.AddDof(DISPLACEMENT_X)
struct.AddDof(DISPLACEMENT_Y)
struct.AddDof(DISPLACEMENT_Z)

force = PointForce3D(n1, 0.0, -20.0e3, -100.0e3)
struct.AddCondition(force)

n2.Fix(DISPLACEMENT_X)
n2.Fix(DISPLACEMENT_Y)
n2.Fix(DISPLACEMENT_Z)

n3.Fix(DISPLACEMENT_X)
n3.Fix(DISPLACEMENT_Y)
n3.Fix(DISPLACEMENT_Z)

n4.Fix(DISPLACEMENT_Z)

e1 = TrussElement(e, a, n1, n2)
e2 = TrussElement(e, a, n1, n3)
e3 = TrussElement(e, a, n1, n4)
e4 = TrussElement(e, a, n2, n3)
e5 = TrussElement(e, a, n3, n4)
e6 = TrussElement(e, a, n4, n2)
struct.AddElement(e1)
struct.AddElement(e2)
struct.AddElement(e3)
struct.AddElement(e4)
struct.AddElement(e5)
struct.AddElement(e6)

struct.Solve()

struct.Print(1)

viewer = visualizer.vtk.Viewer()

# plot the elements
for i in range(0, struct.NumberOfElements()):
    bar = visualizer.vtk.Cylinder()
    x1 = struct.GetElement(i).GetNode(0).X0()
    y1 = struct.GetElement(i).GetNode(0).Y0()
    z1 = struct.GetElement(i).GetNode(0).Z0()
    bar.SetPoint1(x1, y1, z1)
    x2 = struct.GetElement(i).GetNode(1).X0()
    y2 = struct.GetElement(i).GetNode(1).Y0()
    z2 = struct.GetElement(i).GetNode(1).Z0()
    bar.SetPoint2(x2, y2, z2)
    bar.SetRadius(0.1)
    bar.SetColor(0.1, 0.4, 0.4)
    viewer.AddPrimitive(bar)

# plot the constraints
for i in range(0, struct.NumberOfNodes()):
    if(struct.GetNode(i).IsFixed(DISPLACEMENT_X)):
        cone = visualizer.vtk.Cone()
        x = struct.GetNode(i).X0()
        y = struct.GetNode(i).Y0()
        z = struct.GetNode(i).Z0()
        cone.SetCenter(x, y, z)
        cone.SetDirection(1.0, 0.0, 0.0)
        cone.SetRadius(0.5)
        cone.SetHeight(0.5)
        cone.SetResolution(20)
        cone.SetColor(1, 0, 0)
        viewer.AddPrimitive(cone)

    if(struct.GetNode(i).IsFixed(DISPLACEMENT_Y)):
        cone = visualizer.vtk.Cone()
        x = struct.GetNode(i).X0()
        y = struct.GetNode(i).Y0()
        z = struct.GetNode(i).Z0()
        cone.SetCenter(x, y, z)
        cone.SetDirection(0.0, 1.0, 0.0)
        cone.SetRadius(0.5)
        cone.SetHeight(0.5)
        cone.SetResolution(20)
        cone.SetColor(1, 0, 0)
        viewer.AddPrimitive(cone)

    if(struct.GetNode(i).IsFixed(DISPLACEMENT_Z)):
        cone = visualizer.vtk.Cone()
        x = struct.GetNode(i).X0()
        y = struct.GetNode(i).Y0()
        z = struct.GetNode(i).Z0()
        cone.SetCenter(x, y, z)
        cone.SetDirection(0.0, 0.0, 1.0)
        cone.SetRadius(0.5)
        cone.SetHeight(0.5)
        cone.SetResolution(20)
        cone.SetColor(1, 0, 0)
        viewer.AddPrimitive(cone)

# plot the force
arrow = visualizer.vtk.Arrow()
force_scale = 5.0e-5
arrow.SetStartPoint(force.GetNode(0).X0() + force_scale * force.ForceX(), force.GetNode(0).Y0() + force_scale * force.ForceY(), force.GetNode(0).Z0() + force_scale * force.ForceZ())
arrow.SetEndPoint(force.GetNode(0).X0(), force.GetNode(0).Y0(), force.GetNode(0).Z0())
arrow.SetColor(0.0, 0.0, 1.0)
viewer.AddPrimitive(arrow)

# plot the displacement
displacement_scale = 1.0e4
for i in range(0, struct.NumberOfElements()):
    bar = visualizer.vtk.Cylinder()
    x1 = struct.GetElement(i).GetNode(0).X0() + struct.GetElement(i).GetNode(0).GetSolutionStepValue(DISPLACEMENT_X) * displacement_scale
    y1 = struct.GetElement(i).GetNode(0).Y0() + struct.GetElement(i).GetNode(0).GetSolutionStepValue(DISPLACEMENT_Y) * displacement_scale
    z1 = struct.GetElement(i).GetNode(0).Z0() + struct.GetElement(i).GetNode(0).GetSolutionStepValue(DISPLACEMENT_Z) * displacement_scale
    bar.SetPoint1(x1, y1, z1)
    x2 = struct.GetElement(i).GetNode(1).X0() + struct.GetElement(i).GetNode(1).GetSolutionStepValue(DISPLACEMENT_X) * displacement_scale
    y2 = struct.GetElement(i).GetNode(1).Y0() + struct.GetElement(i).GetNode(1).GetSolutionStepValue(DISPLACEMENT_Y) * displacement_scale
    z2 = struct.GetElement(i).GetNode(1).Z0() + struct.GetElement(i).GetNode(1).GetSolutionStepValue(DISPLACEMENT_Z) * displacement_scale
    bar.SetPoint2(x2, y2, z2)
    bar.SetRadius(0.1)
    bar.SetColor(1.0, 1.0, 1.0)
    viewer.AddPrimitive(bar)

viewer.Start()

