#include "structure.h"
#include "solver/superlu_solver.h"

namespace OOFEM
{

Node::Pointer Structure::AddNode(double X, double Y, double Z)
{
    std::size_t lastId = mpNodes.size();
    Node::Pointer pNode = Node::Pointer(new Node(++lastId, X, Y, Z));
    mpNodes.push_back(pNode);
    return pNode;
}

void Structure::AddDof(Dof& rDof)
{
    for(std::size_t i = 0; i < NumberOfNodes(); ++i)
        mpNodes[i]->AddDof(rDof);
}

void Structure::AddCondition(Entity::Pointer pCond)
{
    std::size_t lastId = mpConditions.size();
    pCond->SetId(++lastId);
    mpConditions.push_back(pCond);
}

void Structure::AddElement(Entity::Pointer pElem)
{
    std::size_t lastId = mpElements.size();
    pElem->SetId(++lastId);
    mpElements.push_back(pElem);
}

void Structure::Print(int level) const
{
    if(level > 0)
    {
        // print nodes
        std::cout << "List of Nodes:" << std::endl;
        for(std::size_t i = 0; i < NumberOfNodes(); ++i)
            mpNodes[i]->Print();
    }

    if(level > 1)
    {
        std::cout << "================================================" << std::endl;
        std::cout << "List of Elements:" << std::endl;
        // print elements
        for(std::size_t i = 0; i < NumberOfElements(); ++i)
            mpElements[i]->Print();

        std::cout << "================================================" << std::endl;
        std::cout << "List of Conditions:" << std::endl;
        // print conditions
        for(std::size_t i = 0; i < NumberOfConditions(); ++i)
            mpConditions[i]->Print();
    }
}

int Structure::Solve()
{
    // extract dofs from elements and conditions
    // this scheme ensures every dofs will be accounted once
    typedef std::pair<std::size_t, std::size_t> Key_t;
    typedef std::map<Key_t, Dof::Pointer> DofSetContainer_t;
    DofSetContainer_t DofSet;
    std::vector<Dof::Pointer> DofList;
    for(std::size_t i = 0; i < NumberOfElements(); ++i)
    {
        GetElement(i)->GetDofList(DofList);
        for(std::size_t i = 0; i < DofList.size(); ++i)
        {
            Key_t key(DofList[i]->Id(), DofList[i]->Type());
            DofSet[key] = DofList[i];
        }
    }
    for(std::size_t i = 0; i < NumberOfConditions(); ++i)
    {
        GetCondition(i)->GetDofList(DofList);
        for(std::size_t i = 0; i < DofList.size(); ++i)
        {
            Key_t key(DofList[i]->Id(), DofList[i]->Type());
            DofSet[key] = DofList[i];
        }
    }

    // assign for each dof a unique equation id
    std::size_t free_id = 0;
    std::size_t fixed_id = DofSet.size();
    for(DofSetContainer_t::iterator it = DofSet.begin(); it != DofSet.end(); ++it)
    {
        if(it->second->IsFixed())
            it->second->SetEquationId(--fixed_id);
        else
            it->second->SetEquationId(free_id++);
    }
    mEquationSystemSize = fixed_id;

//    for(DofSetContainer_t::iterator it = DofSet.begin(); it != DofSet.end(); ++it)
//        std::cout << *(it->second) << std::endl;

    // assemble the stiffness matrix & external force vector
    CompressedMatrix K;
    K.resize(mEquationSystemSize, mEquationSystemSize);
    noalias(K) = ZeroMatrix(mEquationSystemSize, mEquationSystemSize);

    Vector R;
    R.resize(mEquationSystemSize);
    noalias(R) = ZeroVector(mEquationSystemSize);

    Matrix LHSlocal;
    Vector RHSlocal;
    for(std::size_t i = 0; i < NumberOfElements(); ++i)
    {
        GetElement(i)->GetDofList(DofList);
        GetElement(i)->CalculateLocalSystem(LHSlocal, RHSlocal);

        std::size_t local_size = DofList.size();
        std::size_t row, col;
        for(std::size_t i = 0; i < local_size; ++i)
            for(std::size_t j = 0; j < local_size; ++j)
            {
                row = DofList[i]->EquationId();
                col = DofList[j]->EquationId();
                if(row < mEquationSystemSize && col < mEquationSystemSize)
                    K(row, col) += LHSlocal(i, j);
            }

        for(std::size_t i = 0; i < local_size; ++i)
        {
            row = DofList[i]->EquationId();
            if(row < mEquationSystemSize)
                R(row) += RHSlocal(i);
        }
    }

    for(std::size_t i = 0; i < NumberOfConditions(); ++i)
    {
        GetCondition(i)->GetDofList(DofList);
        GetCondition(i)->CalculateLocalSystem(LHSlocal, RHSlocal);

        std::size_t local_size = DofList.size();
        std::size_t row, col;
        for(std::size_t i = 0; i < local_size; ++i)
            for(std::size_t j = 0; j < local_size; ++j)
            {
                row = DofList[i]->EquationId();
                col = DofList[j]->EquationId();
                if(row < mEquationSystemSize && col < mEquationSystemSize)
                    K(row, col) += LHSlocal(i, j);
            }

        for(std::size_t i = 0; i < local_size; ++i)
        {
            row = DofList[i]->EquationId();
            if(row < mEquationSystemSize)
                R(row) += RHSlocal(i);
        }
    }

    OOFEM_WATCH(K)
    OOFEM_WATCH(R)

    // solve the linear system
    Vector X;
    X.resize(mEquationSystemSize);
    noalias(X) = ZeroVector(mEquationSystemSize);
    Kratos::SuperLUSolver solver;
    solver.Solve(K, X, R);

    OOFEM_WATCH(X)

    // update the values at d.o.f
    for(DofSetContainer_t::iterator it = DofSet.begin(); it != DofSet.end(); ++it)
    {
        std::size_t EqnId = it->second->EquationId();
        if(EqnId < mEquationSystemSize)
            it->second->SetValue(X(EqnId));
    }
}

}

