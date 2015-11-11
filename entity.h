#pragma once

#include "define.h"
#include "node.h"

namespace OOFEM
{

enum GeometryType{
        POINT_3D,
        LINE_3D_2,
        LINE_3D_3,
        NUMBER_OF_GEOMETRIES
    };

class Entity
{
public:

    OOFEM_CLASS_POINTER_DEFINITION(Entity);
    typedef std::vector<Node::Pointer> NodesArrayType;

    Entity() : mId(0)
    {}

    Entity(const NodesArrayType& NodesArray) : mNodesArray(NodesArray)
    {}

    virtual ~Entity()
    {}

    const NodesArrayType& GetNodes() const {return mNodesArray;}

    std::size_t NumberOfNodes() const {return mNodesArray.size();}

    std::size_t Id() const {return mId;}

    void SetId(std::size_t Id) {mId = Id;}

    Node::Pointer GetNode(std::size_t i) {return mNodesArray[i];}

    virtual void GetDofList(std::vector<Dof::Pointer>& rDofList) const
    {}

    virtual void CalculateLocalSystem(Matrix& rLHSContribution, Vector& rRHSContribution)
    {}

    virtual void Print() const
    {}

    virtual GeometryType GetGeometryType() const {return NUMBER_OF_GEOMETRIES;}

protected:
    NodesArrayType& GetNodes() {return mNodesArray;}

private:
    std::size_t mId;
    NodesArrayType mNodesArray;
};

} // end namespace OOFEM

