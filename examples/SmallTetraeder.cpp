#include "structure.h"
#include "visualizer.h"
#include "point_force_3d.h"
#include "truss_element.h"

#include "fx.h"
#include "fx3d.h"
#include "viewer/gl_view_window.h"
#include "viewer/vtk_view_window.h"

#define PI 3.141592653590

namespace OOFEM
{

OOFEM_USE_DOF(DISPLACEMENT_X)
OOFEM_USE_DOF(DISPLACEMENT_Y)
OOFEM_USE_DOF(DISPLACEMENT_Z)

Structure::Pointer createStructure()
{
    // create the structure pointer
    Structure::Pointer p_struct = Structure::Pointer(new Structure("Tetraeder"));
    
    // structure parameters
    double lb = 15.0;
    double r  = 457.2 / 2000;
    double t  = 10.0 / 1000;
    double a  = PI * (pow(r, 2) - pow(r - t, 2));
    double e  = 2.1e11;
    
    // create nodes
    Node::Pointer p_n1 = p_struct->AddNode(0.0, 0.0, lb * sqrt(2.0 / 3.0));
    Node::Pointer p_n2 = p_struct->AddNode(0.0, lb / sqrt(3), 0.0);
    Node::Pointer p_n3 = p_struct->AddNode(-lb / 2, -lb / sqrt(12.0), 0.0);
    Node::Pointer p_n4 = p_struct->AddNode(lb / 2, -lb / sqrt(12.0), 0.0);
    
    // add degree of freedoms to nodes
    p_struct->AddDof(DISPLACEMENT_X);
    p_struct->AddDof(DISPLACEMENT_Y);
    p_struct->AddDof(DISPLACEMENT_Z);

//    // boundary condition
    Entity::Pointer p_force = Entity::Pointer(new PointForce3D(p_n1, 0.0, -20.0e3, -100.0e3));
    p_struct->AddCondition(p_force);

    p_n2->Fix(DISPLACEMENT_X);
    p_n2->Fix(DISPLACEMENT_Y);
    p_n2->Fix(DISPLACEMENT_Z);

    p_n3->Fix(DISPLACEMENT_X);
    p_n3->Fix(DISPLACEMENT_Y);
    p_n3->Fix(DISPLACEMENT_Z);

    p_n4->Fix(DISPLACEMENT_Z);

    // elements
    Entity::Pointer p_e1 = Entity::Pointer(new TrussElement(e, a, p_n1, p_n2));
    p_struct->AddElement(p_e1);
    Entity::Pointer p_e2 = Entity::Pointer(new TrussElement(e, a, p_n1, p_n3));
    p_struct->AddElement(p_e2);
    Entity::Pointer p_e3 = Entity::Pointer(new TrussElement(e, a, p_n1, p_n4));
    p_struct->AddElement(p_e3);
    Entity::Pointer p_e4 = Entity::Pointer(new TrussElement(e, a, p_n2, p_n3));
    p_struct->AddElement(p_e4);
    Entity::Pointer p_e5 = Entity::Pointer(new TrussElement(e, a, p_n3, p_n4));
    p_struct->AddElement(p_e5);
    Entity::Pointer p_e6 = Entity::Pointer(new TrussElement(e, a, p_n4, p_n2));
    p_struct->AddElement(p_e6);

    return p_struct;
}

}

using namespace OOFEM;

int main(int argc, char** argv)
{
    // create and solve the structure
    Structure::Pointer p_struct = createStructure();
    
    p_struct->Solve();
    
    p_struct->Print(2);

//    return 0;

    // start the viewer
    FXApp application("GLViewer", "OOFEM (c) Giang H. Bui 2015");
    application.init(argc, argv);
    new FoxGL::GLViewWindow(&application);
//    new FoxGL::VTKViewWindow(&application);
//    new Visualizer("Structural Visualizer", p_struct, &application);
    application.create();
    return application.run();
}

