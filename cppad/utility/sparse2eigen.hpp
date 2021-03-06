# ifndef CPPAD_UTILITY_SPARSE2EIGEN_HPP
# define CPPAD_UTILITY_SPARSE2EIGEN_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-18 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

/*
$begin sparse2eigen$$
$spell
	CppAD
	Eigen
	cppad.hpp
	const
	Ptr
	nnz
$$

$section Convert A CppAD Sparse Matrix to an Eigen Sparse Matrix$$

$head Syntax$$
$codei%# include <cppad/utility/sparse2eigen.hpp>
%$$
$codei%sparse2eigen(%source%, %destination%)%$$

$head Prototype$$
$srcfile%cppad/utility/sparse2eigen.hpp%0
	%// BEGIN_PROTOTYPE%// END_PROTOTYPE%
1%$$

$head Eigen$$
This routine is only available when
$cref eigen_prefix$$ is specified.

$head SizeVector$$
We use $cref/SizeVector/sparse_rc/SizeVector/$$ to denote a
$cref SimpleVector$$ class with elements of $code size_t$$.

$head ValueVector$$
We use $icode ValueVector$$ to denote a
$cref SimpleVector$$ class with elements of type $icode value_type$$.

$head Options$$
We use $icode Options$$ to denote either
$code Eigen::RowMajor$$ of $code Eigen::ColMajor$$.

$head value_type$$
The type of elements of elements in $icode source$$ and $icode destination$$
must be the same. We use $icode value_type$$ to denote this type.

$head source$$
This is the CppAD sparse matrix that is being converted to eigen format.

$head destination$$
This is the Eigen sparse matrix that is the result of the conversion.

$head Compressed$$
The result matrix $icode destination$$
is in compressed format. For example, let
$codei%
	size_t %%           %nnz%       = %source%.nnz();
	const %s_vector%&   %s_value%   = %source%.val();
	const %value_type%* %d_value%   = %destination%.valuePtr();
	const %s_vector%&   %row_major% = %source%.row_major();
	const %s_vector%&   %col_major% = %source%.col_major();
%$$
It follows that, for $icode%k% = 0 , %...%, %nnz%$$:
If $icode Options$$ is $code Eigen::RowMajor$$,
$codei%
	%d_value%[%k%] == %s_value%[ %row_major%[%k%] ]
%$$
If $icode Options$$ is $code Eigen::ColMajor$$,
$codei%
	%d_value%[%k%] == %s_value%[ %col_major%[%k%] ]
%$$

$children%example/sparse/sparse2eigen.cpp
%$$

$head Example$$
The file $cref sparse2eigen.cpp$$ contains an example and test
of $code sparse2eigen.cpp$$ It return true if the test passes
and false otherwise.

$end
*/
# include <cppad/configure.hpp>
# if CPPAD_HAS_EIGEN

# include <Eigen/Sparse>
# include <cppad/utility/sparse_rcv.hpp>
# include <cppad/utility/vector.hpp>

namespace CppAD { // BEGIN CPPAD_NAMESPACE

// BEGIN_PROTOTYPE
template <typename SizeVector, typename ValueVector, int Options>
void sparse2eigen(
const CppAD::sparse_rcv<SizeVector, ValueVector>&               source       ,
Eigen::SparseMatrix<typename ValueVector::value_type, Options>& destination  )
// END_PROTOTYPE
{	using Eigen::Index;
	typedef typename ValueVector::value_type value_type;
	typedef Eigen::Triplet<value_type>       triplet;
	std::vector<triplet> vec( source.nnz() );
	//
	const SizeVector&  row = source.row();
	const SizeVector&  col = source.col();
	const ValueVector& val = source.val();
	//
	for(size_t k = 0; k < source.nnz(); k++)
		vec[k] = triplet( int(row[k]), int(col[k]), val[k] );
	//
	size_t nr = source.nr();
	size_t nc = source.nc();
	destination.resize( Index(nr), Index(nc) );
	destination.setFromTriplets(vec.begin(), vec.end());
	//
	CPPAD_ASSERT_UNKNOWN( destination.isCompressed() );
	//
	return;
}

}       // END_CPPAD_NAMESPACE
# endif // CPPAD_HAS_EIGEN
# endif
