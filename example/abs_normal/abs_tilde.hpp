# ifndef CPPAD_EXAMPLE_ABS_NORMAL_ABS_TILDE_HPP
# define CPPAD_EXAMPLE_ABS_NORMAL_ABS_TILDE_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-17 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin abs_tilde$$
$spell
	jac
	Jacobian
$$
$section Abs-normal First Order Approximation$$

$head Under Construction$$

$head Syntax$$
$icode%g_tilde% = abs_tilde(%n%, %m%, %s%, %g_hat%, %g_jac%, %delta_x%)%$$
$pre
$$
see $cref/prototype/qp_box/Prototype/$$

$head Purpose$$
Given a current point
$latex \hat{x} \in \B{R}^n$$,
and a $latex \Delta x \in \B{R}^n$$.
this routine evaluates the abs-normal
$cref/approximation for f(x)
	/abs_normal
	/Abs-normal Approximation
	/Approximating f(x)
/$$.

$head f$$
We use the notation $icode f$$ for the original function; see
$cref/f/abs_normal/f/$$.

$head n$$
This is the dimension of the domain space for $icode f$$; see
$cref/n/abs_normal/f/n/$$.

$head m$$
This is the dimension of the range space for $icode f$$; see
$cref/m/abs_normal/f/m/$$.

$head s$$
This is the number of absolute value terms in $icode f$$; see

$head g$$
We use the notation $icode g$$ for the abs-normal representation of $icode f$$;
see $cref/g/abs_normal/g/$$.

$head g_hat$$
This vector has size $icode%m% + %s%$$ and is the value of
$icode g(x, u)$$ at $latex x = \hat{x}$$ and $latex u = a( \hat{x} )$$.

$head g_jac$$
This vector has size $codei%(%m% + %s%) * (%n% + %s%)%$$ and is the Jacobian of
$latex g(x, u)$$ at $latex x = \hat{x}$$ and $latex u = a( \hat{x} )$$.

$head delta_x$$
This vector has size $icode n$$ and is the difference
$latex \Delta x = x - \hat{x}$$,
where $latex x$$ is the point that we are approximating $latex f(x)$$.

$head g_tilde$$
This vector has size $codei%m% + %s%$$ and is a the
first order approximation for
$cref/g/abs_normal/g/$$
that corresponds to the point
$latex x = \hat{x} + \Delta x$$ and $latex u = a(x)$$.

$children%example/abs_normal/abs_tilde.cpp
%$$
$head Example$$
The file $cref abs_tilde.cpp$$ contains an example and test of
$code abs_tilde$$.
It returns true if the test passes and false otherwise.

$head Prototype$$
$srcfile%example/abs_normal/abs_tilde.hpp%
	0%// BEGIN PROTOTYPE%// END PROTOTYPE%
1%$$

$end
-----------------------------------------------------------------------------
*/

namespace CppAD { // BEGIN_CPPAD_NAMESPACE

// BEGIN PROTOTYPE
template <class Vector>
Vector abs_tilde(
	size_t        n       ,
	size_t        m       ,
	size_t        s       ,
	const Vector& g_hat   ,
	const Vector& g_jac   ,
	const Vector& delta_x )
// END PROTOTYPE
{	using std::fabs;
	//
	CPPAD_ASSERT_KNOWN(
		delta_x.size() == n,
		"abs_tilde: size of delta_x not equal to n"
	);
	CPPAD_ASSERT_KNOWN(
		g_hat.size() == m + s,
		"abs_tilde: size of g_hat not equal to m + s"
	);
	CPPAD_ASSERT_KNOWN(
		g_jac.size() == (m + s) * (n + s),
		"abs_tilde: size of g_jac not equal to (m + s)*(n + s)"
	);
# ifndef NDEBUG
	// Check that partial_u z(x, u) is strictly lower triangular
	for(size_t i = 0; i < s; i++)
	{	for(size_t j = i; j < s; j++)
		{	// index in g_jac of partial of z_i w.r.t u_j
			// (note that g_jac has n + s elements in each row)
			size_t index = (m + i) * (n + s) + (n + j);
			CPPAD_ASSERT_KNOWN(
				g_jac[index] == 0.0,
				"abs_tilde: partial z_i w.r.t u_j non-zero for i <= j"
			);
		}
	}
# endif
	// initialize g_tilde as equalt to g_hat
	Vector g_tilde(m + s);
	//
	// compute z_tilde, the last s components of g_tilde
	for(size_t i = 0; i < s; i++)
	{	// start at z_hat_i
		g_tilde[m + i] = g_hat[m + i];
		// contribution for change x
		for(size_t j = 0; j < n; j++)
		{	// index in g_jac of partial of z_i w.r.t x_j
			size_t index = (m + i) * (n + s) + j;
			// add contribution for delta_x_j to z_tilde_i
			g_tilde[m + i] += g_jac[index] * delta_x[j];
		}
		// contribution for change in u_j for j < i
		for(size_t j = 0; j < i; j++)
		{	// approixmation for change in absolute value
			double delta_a_j = fabs(g_tilde[m + j]) - fabs(g_hat[m + j]);
			// index in g_jac of partial of z_i w.r.t u_j
			size_t index = (m + i) * (n + s) + n + j;
			// add constribution for delta_a_j to s_tilde_i
			g_tilde[m + i] += g_jac[index] * delta_a_j;
		}
	}
	//
	// compute y_tilde, the first m components of g_tilde
	for(size_t i = 0; i < s; i++)
	{	// start at y_hat_i
		g_tilde[i] = g_hat[i];
		// contribution for change x
		for(size_t j = 0; j < n; j++)
		{	// index in g_jac of partial of y_i w.r.t x_j
			size_t index = i * (n + s) + j;
			// add contribution for delta_x_j to y_tilde_i
			g_tilde[i] += g_jac[index] * delta_x[j];
		}
		// contribution for change in u_j
		for(size_t j = 0; j < m; j++)
		{	// approximation for change in absolute value
			double delta_a_j = fabs(g_tilde[m + j]) - fabs(g_hat[m + j]);
			// index in g_jac of partial of y_i w.r.t u_j
			size_t index = i * (n + s) + n + j;
			// add constribution for delta_a_j to s_tilde_i
			g_tilde[i] += g_jac[index] * delta_a_j;
		}
	}
	return g_tilde;
}
} // END_CPPAD_NAMESPACE

# endif