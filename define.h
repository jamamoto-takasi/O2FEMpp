#pragma once

/* External includes */
#include "boost/smart_ptr.hpp"
#include "boost/numeric/ublas/vector.hpp"
#include "boost/numeric/ublas/vector_proxy.hpp"
#include "boost/numeric/ublas/vector_sparse.hpp"
#include "boost/numeric/ublas/vector_expression.hpp"
#include "boost/numeric/ublas/matrix.hpp"
#include "boost/numeric/ublas/matrix_sparse.hpp"
#include "boost/numeric/ublas/matrix_proxy.hpp"
#include "boost/numeric/ublas/symmetric.hpp"
#include "boost/numeric/ublas/hermitian.hpp"
#include "boost/numeric/ublas/banded.hpp"
#include "boost/numeric/ublas/triangular.hpp"
#include "boost/numeric/ublas/io.hpp"
#include "boost/numeric/ublas/operation.hpp"
#include "boost/numeric/ublas/lu.hpp"
#include "boost/numeric/ublas/operation_sparse.hpp"

#include <stdexcept>
#include <iostream>
#include <sstream>
#include <string>
#include <cmath>
#include <vector>
#include <map>
#include <utility>

#define OOFEM_CLASS_POINTER_DEFINITION(a) typedef boost::shared_ptr<a> Pointer; \
typedef boost::shared_ptr<const a> ConstPointer; \
typedef boost::weak_ptr<a> WeakPointer;


#define OOFEM_CREATE_DOF(DofName) \
class DofName##_ABC : public Dof \
{ \
public: \
    OOFEM_CLASS_POINTER_DEFINITION(DofName##_ABC) \
    DofName##_ABC(int Id) : Dof(Id, DofName##_ID) {} \
};


#define OOFEM_REGISTER_DOF(DofName) \
DofName##_ABC DofName(0);


#define OOFEM_USE_DOF(DofName) \
extern DofName##_ABC DofName;


#define OOFEM_ADD_DOF_TO_PYTHON(DofName) \
class_<DofName##_ABC, DofName##_ABC::Pointer, bases<Dof>, boost::noncopyable> \
("DofName##_ABC", no_init);


#define STRINGIFY(DofName) std::string(#DofName)


// borrow a macro from KRATOS
#define KRATOS_THROW_ERROR(ExceptionType, ErrorMessage, MoreInfo)    \
{                                                              \
std::stringstream kratos_error_buffer_12345;                                      \
kratos_error_buffer_12345 << "in " << BOOST_CURRENT_FUNCTION << " [ " << __FILE__ << " , Line " << __LINE__ << " ]" << std::endl; \
kratos_error_buffer_12345 << "\nwith subject    :  " << ErrorMessage << " " << MoreInfo; \
throw ExceptionType(kratos_error_buffer_12345.str());                             \
}


#define OOFEM_WATCH(variable) \
    std::cout << #variable << " : " << variable << std::endl;


namespace OOFEM
{
    typedef boost::numeric::ublas::vector<double> Vector;
    typedef boost::numeric::ublas::matrix<double> Matrix;
    typedef boost::numeric::ublas::compressed_matrix<double> CompressedMatrix;
    typedef boost::numeric::ublas::zero_matrix<double> ZeroMatrix;
    typedef boost::numeric::ublas::identity_matrix<double> IdentityMatrix;
    typedef boost::numeric::ublas::zero_vector<double> ZeroVector;
}

