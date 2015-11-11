#pragma once

#include "define.h"
#include "dof.h"

namespace OOFEM
{

class Node
{
public:
    OOFEM_CLASS_POINTER_DEFINITION(Node);

    Node(std::size_t Id, double X, double Y, double Z) : mId(Id), mX(X), mY(Y), mZ(Z)
    {}

    virtual ~Node()
    {}

    std::size_t Id() const {return mId;}
    double X0() const {return mX;}
    double Y0() const {return mY;}
    double Z0() const {return mZ;}

    void AddDof(Dof& rDof);

    void Fix(Dof& rDof);

    void Free(Dof& rDof);

    bool IsFixed(Dof& rDof) const;

    Dof::Pointer GetDof(Dof& rDof);

    double GetSolutionStepValue(Dof& rDof) {return GetDof(rDof)->GetValue();}

    void Print() const;

    std::string Info() const;

private:
    double mX, mY, mZ;
    std::size_t mId;
    std::vector<typename Dof::Pointer> mDofList;
};

/**
 * output stream function
 */
inline std::ostream& operator << (std::ostream& rOStream, const Node& rThis)
{
    rOStream << rThis.Info();
    return rOStream;
}

} // end namespace OOFEM

