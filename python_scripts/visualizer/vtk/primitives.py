import vtk
import math

def cross(u, v):
    w = [0.0, 0.0, 0.0]
    w[0] = u[1] * v[2] - u[2] * v[1]
    w[1] = u[2] * v[0] - u[0] * v[2]
    w[2] = u[0] * v[1] - u[1] * v[0]
    return w

def norm(u):
    return math.sqrt(math.pow(u[0], 2) + math.pow(u[1], 2) + math.pow(u[2], 2))

def dist(u, v):
    return math.sqrt(math.pow(u[0] - v[0], 2) + math.pow(u[1] - v[1], 2) + math.pow(u[2] - v[2], 2))

class Primitive(object):
    def __init__(self):
        self.actor_is_created = False
        pass

    # TODO
    def SetColor(self, r, g, b):
        self.GetActor().GetProperty().SetColor(r, g, b)
        pass

    # abstract method to be derived by sub-class
    def SetResolution(self, res):
        pass

    # abstract method to be derived by sub-class
    # Finalize the primitive construction
    def Finalize(self):
        pass

    def CreateActor(self):
        self.primMapper = vtk.vtkPolyDataMapper()
        self.primMapper.SetInput(self.prim.GetOutput())
        self.primActor = vtk.vtkActor()
        self.primActor.SetMapper(self.primMapper)
        self.actor_is_created = True

    def GetActor(self):
        if(not self.actor_is_created):
            self.CreateActor()
        return self.primActor

class Cone(Primitive):
    def __init__(self):
        super(Cone, self).__init__()
        self.prim = vtk.vtkConeSource()

    def SetHeight(self, height):
        self.prim.SetHeight(height)

    def SetRadius(self, radius):
        self.prim.SetRadius(radius)

    def SetCenter(self, cx, cy, cz):
        self.prim.SetCenter(cx, cy, cz)

    def SetDirection(self, dx, dy, dz):
        self.prim.SetDirection(dx, dy, dz)

    def SetAngle(self, angle):
        self.prim.SetAngle(angle)

    def SetResolution(self, res):
        self.prim.SetResolution(res)

    def Finalize(self):
        c = self.prim.GetCenter()
        h = self.prim.GetHeight()
        d = self.prim.GetDirection()
        cx = c[0] - h/2 * d[0]
        cy = c[1] - h/2 * d[1]
        cz = c[2] - h/2 * d[2]
        self.prim.SetCenter(cx, cy, cz)

class Cylinder(Primitive):
    def __init__(self):
        super(Cylinder, self).__init__()
        self.line = vtk.vtkLineSource()
        self.prim = vtk.vtkTubeFilter()
        self.prim.SetInputConnection(self.line.GetOutputPort())

    def SetPoint1(self, px, py, pz):
        self.line.SetPoint1(px, py, pz)

    def SetPoint2(self, px, py, pz):
        self.line.SetPoint2(px, py, pz)

    def SetRadius(self, radius):
        self.prim.SetRadius(radius)
    
    def SetResolution(self, res):
        self.prim.SetNumberOfSides(res) #res is the number of sides of the tube

class Arrow(Primitive):
    #REF: http://www.vtk.org/Wiki/VTK/Examples/Cxx/GeometricObjects/OrientedArrow
    def __init__(self):
        super(Arrow, self).__init__()
        self.arrow = vtk.vtkArrowSource()
        self.prim = vtk.vtkTransformPolyDataFilter()
        self.end_point = [0, 0, 0]
        self.start_point = [1, 0, 0]
        self.shaft_radius = 0.03
        self.tip_radius = 0.1
        self.length_ratio = 0.35

    def SetStartPoint(self, x, y, z):
        self.start_point[0] = x
        self.start_point[1] = y
        self.start_point[2] = z

    def SetEndPoint(self, x, y, z):
        self.end_point[0] = x
        self.end_point[1] = y
        self.end_point[2] = z

    def SetShaftRadius(self, radius):
        self.shaft_radius = radius

    def SetTipRadius(self, radius):
        self.tip_radius = radius

    # set the length ratio between the tip and the arrow
    def SetLengthRatio(self, length_ratio):
        self.length_ratio = length_ratio

    def Finalize(self):
        # adjust the arrow
        self.arrow.SetShaftRadius(0.03)
        self.arrow.SetTipRadius(0.03 / self.shaft_radius * self.tip_radius)
        self.arrow.SetTipLength(self.length_ratio)

        normalizedX = [self.end_point[0] - self.start_point[0], self.end_point[1] - self.start_point[1], self.end_point[2] - self.start_point[2]]
        length = norm(normalizedX)
        normalizedX[0] = normalizedX[0] / length
        normalizedX[1] = normalizedX[1] / length
        normalizedX[2] = normalizedX[2] / length

        arbitrary = [1.0, 0.0, 0.0]
        if(dist(arbitrary, normalizedX) < 1.0e-6):
            arbitrary = [0.0, 1.0, 0.0]
            if(dist(arbitrary, normalizedX) < 1.0e-6):
                arbitrary = [0.0, 0.0, 1.0]

        normalizedZ = cross(normalizedX, arbitrary)
        normZ = norm(normalizedZ)
        normalizedZ[0] = normalizedZ[0] / normZ;
        normalizedZ[1] = normalizedZ[1] / normZ;
        normalizedZ[2] = normalizedZ[2] / normZ;
        
        normalizedY = cross(normalizedZ, normalizedX)
        normY = norm(normalizedY)
        normalizedY[0] = normalizedY[0] / normY;
        normalizedY[1] = normalizedY[1] / normY;
        normalizedY[2] = normalizedY[2] / normY;

        # compute the transformation matrix
        mat = vtk.vtkMatrix4x4()
        mat.Identity()
        for i in range(0, 3):
            mat.SetElement(i, 0, normalizedX[i])
            mat.SetElement(i, 1, normalizedY[i])
            mat.SetElement(i, 2, normalizedZ[i])

        transform = vtk.vtkTransform()
        transform.Translate(self.start_point)
        transform.Concatenate(mat)
        transform.Scale(length, self.shaft_radius/0.03 * length, self.shaft_radius/0.03 * length)

        # create the transformation
        self.prim.SetTransform(transform)
        self.prim.SetInputConnection(self.arrow.GetOutputPort())
        
