# ifndef CPPAD_EXAMPLE_ABS_NORMAL_QP_BOX_HPP
# define CPPAD_EXAMPLE_ABS_NORMAL_QP_BOX_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin qp_box$$
$spell
	const
	col
	xout
	yout
	sout
	cols
	prog
	maxitr
	xin
	qp
$$
$section Solve a Quadratic Program With Box Constraints$$

$head Under Construction$$

$head Syntax$$
$icode%ok% = qp_box(
	%a%, %b%, %c%, %C%, %g%, %G%, %epsilon%, %maxitr%, %xin%, %xout%
)%$$
see $cref/prototype/qp_box/Prototype/$$

$head Problem$$
We are given
$latex a \in \B{R}^n$$,
$latex b \in \B{R}^n$$,
$latex c \in \B{R}^m$$,
$latex C \in \B{R}^{m \times n}$$,
$latex g \in \B{R}^n$$,
$latex G \in \B{R}^{n \times n}$$,
where $latex G$$ is positive semi-definite.
This routine solves the problem
$latex \[
\begin{array}{rl}
\R{minimize} &
\frac{1}{2} x^T G x + g^T x \; \R{w.r.t} \; x \in \B{R}^n
\\
\R{subject \; to} & C x + c \leq 0 \; \R{and} \; a \leq x \leq b
\end{array}
\] $$

$head Vector$$
The type $icode Vector$$ is a
simple vector with elements of type $code double$$.

$head Row-major$$
A vector $icode v$$ is a matrix $latex M \in \B{R}^{m \times n}$$
in row major order, if the size of $icode v$$ is $latex m  \times n$$ and for
$latex i = 0 , \ldots , m-1$$,
$latex j = 0 , \ldots , n-1$$,
$latex \[
	M_{i, j} = v[ i \times m + j ]
\] $$

$head a$$
This is the vector of lower limits for $latex x$$ in the problem.

$head b$$
This is the vector of upper limits for $latex x$$ in the problem.

$head c$$
This is the value of the inequality constraint function at $latex x = 0$$.

$head C$$
This is the matrix in the inequality constraint function
in row-major order.

$head g$$
This is the gradient of the objective function.

$head G$$
This is the Hessian of the objective function.

$head epsilon$$
This argument is the convergence criteria;
see $cref/KKT conditions/qp_box/KKT Conditions/$$ below.
It must be greater than zero.

$head maxitr$$
This is the maximum number of newton iterations to try before giving up
on convergence.

$head xin$$
This argument has size $icode n$$ and is the initial point for the algorithm.
It must strictly satisfy the constraints; i.e.,
$codei%
	%a% < %xin%,  %xin% < %b%,  %C% * %xin% - %c% < 0
%$$

$head xout$$
This argument has size is $icode n$$ and
the input value of its elements does no matter.
Upon return it is the primal variables
$latex x$$ corresponding to the problem solution.

$head ok$$
If the return value $icode ok$$ is true, convergence is obtained; i.e.,
$latex \[
	| F ( x , y_a, s_a, y_b, s_b, y_c, s_c ) | < \varepsilon
\] $$
where $latex |v|$$ is the Euclidean norm of the vector $latex v$$,
$latex \varepsilon$$ is $icode epsilon$$,
$latex x$$ is equal to $icode xout$$,
$latex y_a, s_a \in \B{R}_+^n$$,
$latex y_b, s_b \in \B{R}_+^n$$ and
$latex y_c, s_c \in \B{R}_+^m$$.

$head KKT Conditions$$
Give a vector $latex v \in \B{R}^m$$ we define
$latex D(v) \in \B{R}^{m \times m}$$ as the corresponding diagonal matrix.
We also define $latex 1_m \in \B{R}^m$$ as the vector of ones.
We define
$latex \[
F ( x , y_a, s_a, y_b, s_b, y_c, s_c )
=
\left(
\begin{array}{c}
g + G x - y_a + y_b + y_c^T C         \\
a + s_a - x                           \\
x + s_b - b                           \\
C x + c + s_c                         \\
D(s_a) D(y_a) 1_m                     \\
D(s_b) D(y_b) 1_m                     \\
D(s_c) D(y_c) 1_m
\end{array}
\right)
\] $$
where
$latex x \in \B{R}^n$$,
$latex y_a, s_a \in \B{R}_+^n$$,
$latex y_b, s_b \in \B{R}_+^n$$ and
$latex y_c, s_c \in \B{R}_+^m$$.
The KKT conditions for a solution of this problem is
$latex \[
	F ( x , y_a, s_a, y_b, s_b, y_c, s_c ) = 0
\] $$

$children%example/abs_normal/qp_box.cpp
%$$
$head Example$$
The file $cref qp_box.cpp$$ contains an example and test of
$code qp_box$$.
It returns true if the test passes and false otherwise.

$head Prototype$$
$srcfile%example/abs_normal/qp_box.hpp%
	0%// BEGIN PROTOTYPE%// END PROTOTYPE%
1%$$

$end
-----------------------------------------------------------------------------
*/
# include "qp_interior.hpp"

namespace CppAD { // BEGIN_CPPAD_NAMESPACE

// BEGIN PROTOTYPE
template <class Vector>
bool qp_box(
	const Vector& a       ,
	const Vector& b       ,
	const Vector& c       ,
	const Vector& C       ,
	const Vector& g       ,
	const Vector& G       ,
	double        epsilon ,
	size_t        maxitr  ,
	const Vector& xin     ,
	Vector&       xout    )
// END PROTOTYPE
{	size_t n = a.size();
	size_t m = c.size();
	//
	CPPAD_ASSERT_KNOWN( b.size() == n, "qp_box: size of b is not n" );
	CPPAD_ASSERT_KNOWN( C.size() == m * n, "qp_box: size of C is not m * n" );
	CPPAD_ASSERT_KNOWN( g.size() == n, "qp_box: size of g is not n" );
	CPPAD_ASSERT_KNOWN( G.size() == n * n, "qp_box: size of G is not n * n" );
	//
	// C_all and c_all define the extended constraints
	Vector C_all((m + 2 * n) * n ), c_all(m + 2 * n);
	for(size_t i = 0; i < C_all.size(); i++)
		C_all[i] = 0.0;
	//
	// put C * x + c <= 0 in C_all, c_all
	for(size_t i = 0; i < m; i++)
	{	c_all[i] = c[i];
		for(size_t j = 0; j < n; j++)
			C_all[i * n + j] = C[i * n + j];
	}
	//
	// put I * x - b <= 0 in C_all, c_all
	for(size_t j = 0; j < n; j++)
	{	c_all[m + j]            = - b[j];
		C_all[(m + j) * n + j]  = 1.0;
	}
	//
	// put a - I * x <= 0 in C_all, c_all
	for(size_t j = 0; j < n; j++)
	{	c_all[m + n + j]           = a[j];
		C_all[(m + n + j) * n + j] = -1.0;
	}
	Vector yout(m + 2 * n), sout(m + 2 * n);
	bool ok = qp_interior(
		c_all, C_all, g, G, epsilon, maxitr, xin, xout, yout, sout
	);
	return ok;
}

} // END_CPPAD_NAMESPACE

# endif