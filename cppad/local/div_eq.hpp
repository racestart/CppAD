# ifndef CPPAD_DIV_EQ_INCLUDED
# define CPPAD_DIV_EQ_INCLUDED

/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-07 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the 
                    Common Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */

//  BEGIN CppAD namespace
namespace CppAD {

template <class Base>
AD<Base>& AD<Base>::operator /= (const AD<Base> &right)
{	ADTape<Base> *tape = tape_ptr();
	bool var_left, var_right;
# ifdef NDEBUG
	if( tape == CPPAD_NULL )
	{	var_left =  false;
		var_right = false;
	}
	else
	{
		var_left  = id_       == tape->id_;
		var_right = right.id_ == tape->id_;
	}
# else
	var_left  = Variable(*this);
	var_right = Variable(right);
	CPPAD_ASSERT_KNOWN(
		(! var_left) || id_ == tape->id_ ,
		"/= left operand is a variable for a different thread"
	);
	CPPAD_ASSERT_KNOWN(
		(! var_right) || right.id_ == tape->id_ ,
		"/= right operand is a variable for a different thread"
	);
# endif
	Base left;
	left    = value_;
	value_ /= right.value_;

	if( var_left )
	{	if( var_right )
		{	// this = variable / variable
			CPPAD_ASSERT_UNKNOWN( NumVar(DivvvOp) == 1 );
			CPPAD_ASSERT_UNKNOWN( NumInd(DivvvOp) == 2 );

			// put operand addresses in tape
			tape->Rec.PutInd(taddr_, right.taddr_);
			// put operator in the tape
			taddr_ = tape->Rec.PutOp(DivvvOp);
			// make this a variable
			CPPAD_ASSERT_UNKNOWN( id_ = tape->id_ );
		}
		else if( IdenticalOne( right.value_ ) )
		{	// this = variable * 1
		}
		else 
		{	// this = variable / parameter
			CPPAD_ASSERT_UNKNOWN( NumVar(DivvpOp) == 1 );
			CPPAD_ASSERT_UNKNOWN( NumInd(DivvpOp) == 2 );

			// put operand addresses in tape
			size_t p = tape->Rec.PutPar(right.value_);
			tape->Rec.PutInd(taddr_, p);
			// put operator in the tape
			taddr_ = tape->Rec.PutOp(DivvpOp);
			// make this a variable
			CPPAD_ASSERT_UNKNOWN( id_ == tape->id_);
		}
	}
	else if( var_right  )
	{	if( IdenticalZero(left) )
		{	// this = 0 / variable
		}
		else
		{	// this = parameter / variable
			CPPAD_ASSERT_UNKNOWN( NumVar(DivpvOp) == 1 );
			CPPAD_ASSERT_UNKNOWN( NumInd(DivpvOp) == 2 );

			// put operand addresses in tape
			size_t p = tape->Rec.PutPar(left);
			tape->Rec.PutInd(p, right.taddr_);
			// put operator in the tape
			taddr_ = tape->Rec.PutOp(DivpvOp);
			// make this a variable
			id_ = tape->id_;
		}
	}
	return *this;
}

CPPAD_FOLD_ASSIGNMENT_OPERATOR(/=)

} // END CppAD namespace

# endif 
