/* -----------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-06 Bradley M. Bell

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
------------------------------------------------------------------------ */

/*
$begin Compare.cpp$$
$spell
	Cpp
$$

$section AD Binary Comparison Operators: Example and Test$$

$index compare, AD example$$
$index example, AD compare$$
$index test, AD compare$$

$index <, example$$
$index <=, example$$
$index >, example$$
$index >=, example$$
$index ==, example$$
$index !=, example$$

$code
$verbatim%Example/Compare.cpp%0%// BEGIN PROGRAM%// END PROGRAM%1%$$
$$

$end
*/
// BEGIN PROGRAM
# include <CppAD/CppAD.h>

bool Compare(void)
{	bool ok = true;
	using CppAD::AD;
	using CppAD::NearEqual;

	// declare independent variables and start tape recording
	size_t n  = 2;
	double x0 = 0.5;
	double x1 = 1.5;
	CppADvector< AD<double> > x(n);
	x[0]      = x0; 
	x[1]      = x1; 
	CppAD::Independent(x);

	// some binary comparision operations
	AD<double> p;
	if( x[0] < x[1] )
		p = x[0];   // values in x choose this case
	else	p = x[1];
	if( x[0] <= x[1] )
		p *= x[0];  // values in x choose this case
	else	p *= x[1];
	if( x[0] >  x[1] )
		p *= x[0]; 
	else	p *= x[1];  // values in x choose this case
	if( x[0] >= x[1] )
		p *= x[0]; 
	else	p *= x[1];  // values in x choose this case
	if( x[0] == x[1] )
		p *= x[0]; 
	else	p *= x[1];  // values in x choose this case
	if( x[0] != x[1] )
		p *= x[0];  // values in x choose this case
	else	p *= x[1]; 

	// dependent variable vector 
	size_t m = 1;
	CppADvector< AD<double> > y(m);
	y[0] = p;

	// create f: x -> y and stop tape recording
	CppAD::ADFun<double> f(x, y); 

	// check value 
	ok &= NearEqual(y[0] , x0*x0*x1*x1*x1*x0,  1e-10 , 1e-10);

	// forward computation of partials w.r.t. x[0]
	CppADvector<double> dx(n);
	CppADvector<double> dy(m);
	dx[0] = 1.;
	dx[1] = 0.;
	dy    = f.Forward(1, dx);
	ok   &= NearEqual(dy[0], 3.*x0*x0*x1*x1*x1, 1e-10, 1e-10);

	// forward computation of partials w.r.t. x[1]
	dx[0] = 0.;
	dx[1] = 1.;
	dy    = f.Forward(1, dx);
	ok   &= NearEqual(dy[0], 3.*x0*x0*x1*x1*x0, 1e-10, 1e-10);

	// reverse computation of derivative of y[0]
	CppADvector<double> w(m);
	w[0]  = 1.;
	dx    = f.Reverse(1, w);
	ok   &= NearEqual(dx[0], 3.*x0*x0*x1*x1*x1, 1e-10, 1e-10);
	ok   &= NearEqual(dx[1], 3.*x0*x0*x1*x1*x0, 1e-10, 1e-10);

	return ok;
}

// END PROGRAM
