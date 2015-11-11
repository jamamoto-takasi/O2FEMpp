#pragma once

#include "define.h"

namespace OOFEM
{

enum DofTypeName {
    DISPLACEMENT_X_ID,
    DISPLACEMENT_Y_ID,
    DISPLACEMENT_Z_ID,
};

class Dof
{
public:
    OOFEM_CLASS_POINTER_DEFINITION(Dof);

    Dof(std::size_t Id, DofTypeName TypeName) : mId(Id), mType(TypeName), mIsFixed(false), mValue(0.0)
    {}

    virtual ~Dof()
    {}

    Dof::Pointer Create(int Id) {return Dof::Pointer(new Dof(Id, Type()));}

    const DofTypeName Type() const {return mType;}

    const std::string Name() const {std::stringstream ss; ss << mType; return ss.str();}

    void SetEquationId(std::size_t EquationId) {mEquationId = EquationId;}

    std::size_t EquationId() const {return mEquationId;}

    std::size_t Id() const {return mId;}

    void Fix() {mIsFixed = true;}

    void Free() {mIsFixed = false;}

    bool IsFixed() const {return mIsFixed;}

    void SetValue(double Value) {mValue = Value;}

    double GetValue() const {return mValue;}

    void PrintData(std::ostream& rOStream) const
    {
        rOStream << "Dof Id = " << Id() << ", Type = " << Type()
                 << ", IsFixed = " << IsFixed()
                 << ", EquationId = " << EquationId();
    }

private:
    std::size_t mId;
    DofTypeName mType;
    std::size_t mEquationId;
    bool mIsFixed;
    double mValue;
};

/**
 * output stream function
 */
inline std::ostream& operator << (std::ostream& rOStream, const Dof& rThis)
{
    rThis.PrintData(rOStream);
    return rOStream;
}

OOFEM_CREATE_DOF(DISPLACEMENT_X)
OOFEM_CREATE_DOF(DISPLACEMENT_Y)
OOFEM_CREATE_DOF(DISPLACEMENT_Z)

} // end namespace OOFEM

