#include <boost/python.hpp>

#include "dof.h"
#include "node.h"
#include "entity.h"
#include "point_force_3d.h"
#include "truss_element.h"
#include "structure.h"

using namespace boost::python;

namespace OOFEM
{

OOFEM_USE_DOF(DISPLACEMENT_X)
OOFEM_USE_DOF(DISPLACEMENT_Y)
OOFEM_USE_DOF(DISPLACEMENT_Z)

BOOST_PYTHON_MODULE(OOFEM)
{
    class_<Dof, Dof::Pointer, boost::noncopyable>
    ("Dof", init<std::size_t, DofTypeName>())
    ;

    OOFEM_ADD_DOF_TO_PYTHON(DISPLACEMENT_X)
    scope().attr("DISPLACEMENT_X") = boost::ref(DISPLACEMENT_X);
    OOFEM_ADD_DOF_TO_PYTHON(DISPLACEMENT_Y)
    scope().attr("DISPLACEMENT_Y") = boost::ref(DISPLACEMENT_Y);
    OOFEM_ADD_DOF_TO_PYTHON(DISPLACEMENT_Z)
    scope().attr("DISPLACEMENT_Z") = boost::ref(DISPLACEMENT_Z);

    class_<Node, Node::Pointer, boost::noncopyable>
    ("Node", init<std::size_t, double, double, double>())
    .def("X0", &Node::X0)
    .def("Y0", &Node::Y0)
    .def("Z0", &Node::Z0)
    .def("AddDof", &Node::AddDof)
    .def("Fix", &Node::Fix)
    .def("Free", &Node::Free)
    .def("IsFixed", &Node::IsFixed)
    .def("GetSolutionStepValue", &Node::GetSolutionStepValue)
    .def(self_ns::str(self))
    ;

    class_<Entity, Entity::Pointer, boost::noncopyable>
    ("Entity", init<>())
    .def("Id", &Entity::Id)
    .def("NumberOfNodes", &Entity::NumberOfNodes)
    .def("GetNode", &Entity::GetNode)
    ;

    class_<PointForce3D, PointForce3D::Pointer, bases<Entity>, boost::noncopyable>
    ("PointForce3D", init<Node::Pointer, double, double, double>())
    .def("ForceX", &PointForce3D::ForceX)
    .def("ForceY", &PointForce3D::ForceY)
    .def("ForceZ", &PointForce3D::ForceZ)
    ;

    class_<TrussElement, TrussElement::Pointer, bases<Entity>, boost::noncopyable>
    ("TrussElement", init<double, double, Node::Pointer, Node::Pointer>())
    ;

    class_<Structure, Structure::Pointer, boost::noncopyable>
    ("Structure", init<std::string>())
    .def("AddDof", &Structure::AddDof)
    .def("AddNode", &Structure::AddNode)
    .def("GetNode", &Structure::GetNode)
    .def("AddElement", &Structure::AddElement)
    .def("GetElement", &Structure::GetElement)
    .def("AddCondition", &Structure::AddCondition)
    .def("GetCondition", &Structure::GetCondition)
    .def("NumberOfNodes", &Structure::NumberOfNodes)
    .def("NumberOfConditions", &Structure::NumberOfConditions)
    .def("NumberOfElements", &Structure::NumberOfElements)
    .def("Solve", &Structure::Solve)
    .def("Print", &Structure::Print)
    ;
}

}

