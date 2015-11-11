#pragma once

#include "define.h"
#include "entity.h"

namespace OOFEM
{

class TrussElement : public Entity
{
public:
    OOFEM_CLASS_POINTER_DEFINITION(TrussElement);
    typedef Entity BaseType;
    typedef BaseType::NodesArrayType NodesArrayType;

    TrussElement(double E, double A, Node::Pointer Node1, Node::Pointer Node2)
    : BaseType(), mE(E), mA(A)
    {
        GetNodes().push_back(Node1);
        GetNodes().push_back(Node2);
    }

    virtual ~TrussElement()
    {}

    virtual void GetDofList(std::vector<Dof::Pointer>& rDofList) const;

    virtual void CalculateLocalSystem(Matrix& rLHSContribution, Vector& rRHSContribution);

    double GetLength() const;

    virtual void Print() const;

    virtual GeometryType GetGeometryType() const {return LINE_3D_2;}

private:
    double mE, mA;
};

} // end namespace OOFEM

