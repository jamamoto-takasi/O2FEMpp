#pragma once

#include "define.h"
#include "entity.h"

namespace OOFEM
{

class PointForce3D : public Entity
{
public:
    OOFEM_CLASS_POINTER_DEFINITION(PointForce3D);
    typedef Entity BaseType;
    typedef BaseType::NodesArrayType NodesArrayType;
    
    PointForce3D(Node::Pointer pNode, double ForceX, double ForceY, double ForceZ)
    : BaseType(), mForceX(ForceX), mForceY(ForceY), mForceZ(ForceZ)
    {
        GetNodes().push_back(pNode);
    }

    virtual ~PointForce3D() {}

    virtual void GetDofList(std::vector<Dof::Pointer>& rDofList) const;

    virtual void CalculateLocalSystem(Matrix& rLHSContribution, Vector& rRHSContribution);

    virtual void Print() const;

    virtual GeometryType GetGeometryType() const {return POINT_3D;}

    double ForceX() const {return mForceX;}

    double ForceY() const {return mForceY;}

    double ForceZ() const {return mForceZ;}

private:
    double mForceX, mForceY, mForceZ;
};

} // end namespace OOFEM

