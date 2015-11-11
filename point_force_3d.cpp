#include "point_force_3d.h"

namespace OOFEM
{

OOFEM_USE_DOF(DISPLACEMENT_X)
OOFEM_USE_DOF(DISPLACEMENT_Y)
OOFEM_USE_DOF(DISPLACEMENT_Z)

void PointForce3D::GetDofList(std::vector<Dof::Pointer>& rDofList) const
{
    rDofList.resize(3);
    rDofList[0] = GetNodes()[0]->GetDof(DISPLACEMENT_X);
    rDofList[1] = GetNodes()[0]->GetDof(DISPLACEMENT_Y);
    rDofList[2] = GetNodes()[0]->GetDof(DISPLACEMENT_Z);
}

void PointForce3D::CalculateLocalSystem(Matrix& rLHSContribution, Vector& rRHSContribution)
{
    // resize LHS if needed and initialize
    if(rLHSContribution.size1() != 3 || rLHSContribution.size2() != 3)
        rLHSContribution.resize(3, 3);
    noalias(rLHSContribution) = ZeroMatrix(3, 3);

    // resize RHS if needed and initialize
    if(rRHSContribution.size() != 3)
        rRHSContribution.resize(3);

    rRHSContribution(0) = mForceX;
    rRHSContribution(1) = mForceY;
    rRHSContribution(2) = mForceZ;
}

void PointForce3D::Print() const
{
    std::cout << "PointForce3D #" << Id() << " at Node " << GetNodes()[0]->Info()
              << ", Force = " << "(" << mForceX << "," << mForceY << "," << mForceZ << ")" << std::endl;
}

}

