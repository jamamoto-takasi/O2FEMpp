#include "node.h"

namespace OOFEM
{

void Node::AddDof(Dof& rDof)
{
    // find if the dof has name in list
    for(std::size_t i = 0; i < mDofList.size(); ++i)
    {
        if(rDof.Type() == mDofList[i]->Type())
        return;
    }

    // if not found then put to the list of dofs
    mDofList.push_back(rDof.Create(Id()));
}

void Node::Fix(Dof& rDof)
{
    for(std::size_t i = 0; i < mDofList.size(); ++i)
    {
        if(rDof.Type() == mDofList[i]->Type())
        {
            mDofList[i]->Fix();
        }
    }
}

void Node::Free(Dof& rDof)
{
    for(std::size_t i = 0; i < mDofList.size(); ++i)
    {
        if(rDof.Type() == mDofList[i]->Type())
        {
            mDofList[i]->Free();
        }
    }
}

bool Node::IsFixed(Dof& rDof) const
{
    for(std::size_t i = 0; i < mDofList.size(); ++i)
    {
        if(rDof.Type() == mDofList[i]->Type())
        {
            return mDofList[i]->IsFixed();
        }
    }
    return true;
}

Dof::Pointer Node::GetDof(Dof& rDof)
{
    for(std::size_t i = 0; i < mDofList.size(); ++i)
    {
        if(rDof.Type() == mDofList[i]->Type())
        {
            return mDofList[i];
        }
    }
    KRATOS_THROW_ERROR(std::logic_error, rDof.Name(), "is not set for Node")
}

void Node::Print() const
{
    std::cout << "Node " << Id() << ": (" << X0() << "," << Y0() << "," << Z0() << ")" << std::endl;
    std::cout << "Dof list:" << std::endl;
    for(std::size_t i = 0; i < mDofList.size(); ++i)
    std::cout << "" << mDofList[i]->Name() 
              << ", Fixed = " << (IsFixed(*mDofList[i])?"Yes":"No")
              << ", Value = " << mDofList[i]->GetValue()
              << std::endl;
}

std::string Node::Info() const
{
    std::stringstream ss;
    ss << "#" << Id() << " (" << X0() << "," << Y0() << "," << Z0() << ")";
    return ss.str();
}

}

