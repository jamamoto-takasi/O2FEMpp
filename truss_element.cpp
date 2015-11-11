#include "dof.h"
#include "truss_element.h"

namespace OOFEM
{

OOFEM_USE_DOF(DISPLACEMENT_X)
OOFEM_USE_DOF(DISPLACEMENT_Y)
OOFEM_USE_DOF(DISPLACEMENT_Z)

void TrussElement::GetDofList(std::vector<Dof::Pointer>& rDofList) const
{
    rDofList.resize(6);
    rDofList[0] = GetNodes()[0]->GetDof(DISPLACEMENT_X);
    rDofList[1] = GetNodes()[0]->GetDof(DISPLACEMENT_Y);
    rDofList[2] = GetNodes()[0]->GetDof(DISPLACEMENT_Z);
    rDofList[3] = GetNodes()[1]->GetDof(DISPLACEMENT_X);
    rDofList[4] = GetNodes()[1]->GetDof(DISPLACEMENT_Y);
    rDofList[5] = GetNodes()[1]->GetDof(DISPLACEMENT_Z);
}

double TrussElement::GetLength() const
{
    double dX = GetNodes()[1]->X0() - GetNodes()[0]->X0();
    double dY = GetNodes()[1]->Y0() - GetNodes()[0]->Y0();
    double dZ = GetNodes()[1]->Z0() - GetNodes()[0]->Z0();
    return sqrt(pow(dX, 2) + pow(dY, 2) + pow(dZ, 2));
}

void TrussElement::CalculateLocalSystem(Matrix& rLHSContribution, Vector& rRHSContribution)
{
    // resize LHS if needed and initialize
    if(rLHSContribution.size1() != 6 || rLHSContribution.size2() != 6)
        rLHSContribution.resize(6, 6);
    noalias(rLHSContribution) = ZeroMatrix(6, 6);

    // resize RHS if needed and initialize
    if(rRHSContribution.size() != 6)
        rRHSContribution.resize(6);
    noalias(rRHSContribution) = ZeroVector(6);

    // form the transformation matrix
    double L = GetLength();
    Matrix T(2, 6);
    noalias(T) = ZeroMatrix(2, 6);
    T(0, 0) = (GetNodes()[1]->X0() - GetNodes()[0]->X0()) / L;
    T(0, 1) = (GetNodes()[1]->Y0() - GetNodes()[0]->Y0()) / L;
    T(0, 2) = (GetNodes()[1]->Z0() - GetNodes()[0]->Z0()) / L;

    T(1, 3) = (GetNodes()[1]->X0() - GetNodes()[0]->X0()) / L;
    T(1, 4) = (GetNodes()[1]->Y0() - GetNodes()[0]->Y0()) / L;
    T(1, 5) = (GetNodes()[1]->Z0() - GetNodes()[0]->Z0()) / L;

    // form the local stiffness
    Matrix ke(2, 2);
    double aux = mE * mA / L;
    ke(0, 0) = aux;
    ke(0, 1) = -aux;
    ke(1, 0) = -aux;
    ke(1, 1) = aux;

    // transform the local stiffness to get the element stiffness
    noalias(rLHSContribution) = prod(trans(T), Matrix(prod(ke, T)));
}

void TrussElement::Print() const
{
    std::cout << "TrussElement #" << Id()
              << ", Node 1: " << GetNodes()[0]->Info()
              << ", Node 2: " << GetNodes()[1]->Info()
              << ", E = " << mE << ", A = " << mA
              << std::endl;
}

}

