// $Id$
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-16 Bradley M. Bell

CppAD is distributed under multiple licenses. This distribution is under
the terms of the
                    Eclipse Public License Version 1.0.

A copy of this license is included in the COPYING file of this distribution.
Please visit http://www.coin-or.org/CppAD/ for information on other licenses.
-------------------------------------------------------------------------- */
/*
$begin optimize_forward_active.cpp$$

$section Example Optimization and Forward Activity Analysis$$

$code
$srcfile%example/optimize/forward_active.cpp%0%// BEGIN C++%// END C++%1%$$
$$

$end
*/
// BEGIN C++
# include <cppad/cppad.hpp>
namespace {
	struct tape_size { size_t n_var; size_t n_op; };

	template <class Vector> void fun(
		const Vector& x, Vector& y, tape_size& before, tape_size& after
	)
	{	typedef typename Vector::value_type scalar;

		// phantom variable with index 0 and independent variables
		// begin operator, independent variable operators and end operator
		before.n_var = 1 + x.size(); before.n_op  = 2 + x.size();
		after.n_var  = 1 + x.size(); after.n_op   = 2 + x.size();

		// adding the constant zero does not take any operations
		scalar zero   = 0.0 + x[0];
		before.n_var += 0; before.n_op  += 0;
		after.n_var  += 0; after.n_op   += 0;

		// multiplication by the constant one does not take any operations
		scalar one    = 1.0 * x[1];
		before.n_var += 0; before.n_op  += 0;
		after.n_var  += 0; after.n_op   += 0;

		// multiplication by the constant zero does not take any operations
		// and results in the constant zero.
		scalar two    = 0.0 * x[0];

		// operations that only involve constants are not recorded
		scalar three  = (1.0 + two) * 3.0;
		before.n_var += 0; before.n_op  += 0;
		after.n_var  += 0; after.n_op   += 0;

		// This is the same as adding (x[0] + x[1]) * ( 0.0 + 3.0 )
		// (If we had used all additions here, optimization would convert this
		// to one cumulative summation operator.)
		scalar four   = (zero + one) * ( two + three );
		before.n_var += 2; before.n_op  += 2;
		after.n_var  += 2; after.n_op   += 2;

		// results for this operation sequence
		y[0] = four;
		before.n_var += 0; before.n_op  += 0;
		after.n_var  += 0; after.n_op   += 0;
	}
}

bool forward_active(void)
{	bool ok = true;
	using CppAD::AD;

	// domain space vector
	size_t n  = 2;
	CPPAD_TESTVECTOR(AD<double>) ax(n);
	ax[0] = 0.5;
	ax[1] = 1.5;

	// declare independent variables and start tape recording
	CppAD::Independent(ax);

	// range space vector
	size_t m = 1;
	CPPAD_TESTVECTOR(AD<double>) ay(m);
	tape_size before, after;
	fun(ax, ay, before, after);

	// create f: x -> y and stop tape recording
	CppAD::ADFun<double> f(ax, ay);
	ok &= f.size_var() == before.n_var;
	ok &= f.size_op()  == before.n_op;

	// Optimize the operation sequence
	// Note that, for this case, all the optimization was done during
	// the recording and there is no benifit to the optimization.
	f.optimize();
	ok &= f.size_var() == after.n_var;
	ok &= f.size_op()  == after.n_op;

	// check zero order forward with different argument value
	CPPAD_TESTVECTOR(double) x(n), y(m), check(m);
	for(size_t i = 0; i < n; i++)
		x[i] = double(i + 2);
	y    = f.Forward(0, x);
	fun(x, check, before, after);
	ok  &= y[0] == check[0];

	return ok;
}
// END C++
