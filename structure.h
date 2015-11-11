#pragma once

#include "define.h"
#include "node.h"
#include "entity.h"

namespace OOFEM
{

class Structure
{
public:
    OOFEM_CLASS_POINTER_DEFINITION(Structure);

    Structure(std::string Name) : mName(Name)
    {}

    virtual ~Structure()
    {}

    Node::Pointer AddNode(double X, double Y, double Z);

    Node::Pointer GetNode(std::size_t i) {return mpNodes[i];}

//    Node::Pointer GetNode(std::size_t i) const {return mpNodes[i];}

    std::size_t NumberOfNodes() const {return mpNodes.size();}

    void AddDof(Dof& rDof);

    void AddCondition(Entity::Pointer pCond);

    void AddElement(Entity::Pointer pElem);

    Entity::Pointer GetCondition(std::size_t i) {return mpConditions[i];}

    std::size_t NumberOfConditions() const {return mpConditions.size();}

    Entity::Pointer GetElement(std::size_t i) {return mpElements[i];}

//    Entity::Pointer GetElement(std::size_t i) const {return mpElements[i];}

    std::size_t NumberOfElements() const {return mpElements.size();}

    int Solve();

    std::size_t GetEquationSystemSize() const {return mEquationSystemSize;}

    void Print(int level) const;

private:
    std::string mName;
    std::vector<Node::Pointer> mpNodes;
    std::vector<Entity::Pointer> mpConditions;
    std::vector<Entity::Pointer> mpElements;
    std::size_t mEquationSystemSize;
};

} // end namespace OOFEM

