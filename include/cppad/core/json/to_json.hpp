# ifndef CPPAD_CORE_JSON_TO_JSON_HPP
# define CPPAD_CORE_JSON_TO_JSON_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-19 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */

# include <cppad/core/ad_fun.hpp>
# include <cppad/local/op_code_dyn.hpp>

/*
$begin to_json$$
$spell
    Json
$$

$section Create a Json AD Graph Corresponding to an ADFun Object$$

$head Under Construction$$
This routine is under construction and subject to
change without backward compatibility.

$head Syntax$$
$codei%
    %graph% = %fun%.to_json()
%$$

$head Prototype$$
$srcfile%include/cppad/core/json/to_json.hpp%
    0%// BEGIN_PROTOTYPE%// END_PROTOTYPE%1
%$$

$head fun$$
is the $cref/ADFun/adfun/$$ object.

$head graph$$
The return value of $icode graph$$ is a
$cref json_ad_graph$$ representation of the corresponding function.

$head Base$$
is the type corresponding to this $cref/ADFun/adfun/$$ object;
i.e., its calculations are done using the type $icode Base$$.

$head RecBase$$
in the prototype above, $icode RecBase$$ is the same type as $icode Base$$.

$childtable%
    example/json/to_json.cpp
%$$
$head Example$$
The file $cref to_json.cpp$$ is an example and test of this operation.

$end
*/
// BEGIN_PROTOTYPE
template <class Base, class RecBase>
std::string CppAD::ADFun<Base,RecBase>::to_json(void)
// END_PROTOTYPE
{   using local::pod_vector;
    using local::opcode_t;
    // --------------------------------------------------------------------
    // some constants
    // --------------------------------------------------------------------
    // dynamic parameter information
    const pod_vector<opcode_t>& dyn_par_op ( play_.dyn_par_op()  );
    const pod_vector<addr_t>&   dyn_par_arg( play_.dyn_par_arg() );
    const pod_vector<addr_t>&   dyn_ind2par_ind ( play_.dyn_ind2par_ind() );
    const pod_vector<bool>&     dyn_par_is( play_.dyn_par_is() );
    //
    // number of dynamic parameters
    const size_t n_dynamic     = dyn_ind2par_ind.size();
    //
    // number of independent dynamic parameters
    const size_t n_dynamic_ind = play_.num_dynamic_ind();
    //
    // number of parameters
    const size_t n_parameter = play_.num_par_rec();
    //
    // number of constant parameters
    const size_t n_constant = n_parameter - n_dynamic - 1;
    //
    // number of independent variables
    const size_t n_independent = ind_taddr_.size();
    //
    // number of string constants
    const size_t n_string = 0;
    //
    // value of parameters
    const Base* parameter = play_.GetPar();
    //
    // number of dynamic parameter operators
    const size_t n_dynamic_op = n_dynamic - n_dynamic_ind;
    //
    // number of variables
    const size_t n_variable = play_.num_var_rec();
    //
    // number of json operators
    const size_t n_json_op = size_t( local::json::n_json_op );
    //
    // some checks
    CPPAD_ASSERT_UNKNOWN( n_dynamic_ind <= n_dynamic );
    CPPAD_ASSERT_UNKNOWN( dyn_par_is.size() == n_parameter );
    CPPAD_ASSERT_UNKNOWN( n_parameter > 0 );
    CPPAD_ASSERT_UNKNOWN( isnan( parameter[0] ) );
    CPPAD_ASSERT_UNKNOWN( ! dyn_par_is[0] );
    // -----------------------------------------------------------------------
    // is_json_op_used
    pod_vector<bool> is_json_op_used(n_json_op);
    for(size_t i = 0; i < n_json_op; ++i)
        is_json_op_used[i] = false;
    //
    std::string error_message =
    "Json conversion for following dynamic operator not yet implemented: ";
    for(size_t i_dyn = n_dynamic_ind; i_dyn < n_dynamic; ++i_dyn)
    {   // operator for this dynamic parameter
        local::op_code_dyn dyn_op = local::op_code_dyn( dyn_par_op[i_dyn] );
        //
        switch(dyn_op)
        {
            case local::add_dyn:
            is_json_op_used[local::json::add_json_op] = true;
            break;

            default:
            error_message += op_name_dyn(dyn_op);
            CPPAD_ASSERT_KNOWN( false, error_message.c_str() );
            break;
        }
    }
    // --------------------------------------------------------------------
    // count_variable_op_used, is_json_op_used
    size_t count_variable_op_used = 0;
    //
    local::play::const_sequential_iterator itr = play_.begin();
    local::OpCode var_op;
    const addr_t* arg;
    size_t        i_var;
    itr.op_info(var_op, arg, i_var);
    CPPAD_ASSERT_UNKNOWN( var_op == local::BeginOp ); // skip BeginOp
    //
    bool more_operators  = true;
    error_message        =
    "Json conversion for following variable operator not yet implemented: ";
    while(more_operators)
    {
        // next op
        (++itr).op_info(var_op, arg, i_var);
        switch( var_op )
        {
            // -------------------------------------------------------------
            // Ignore all comparison operators (for now)
            case local::EqppOp:
            case local::EqpvOp:
            case local::EqvvOp:
            case local::NeppOp:
            case local::NepvOp:
            case local::NevvOp:
            //
            case local::LtppOp:
            case local::LtpvOp:
            case local::LtvpOp:
            case local::LtvvOp:
            case local::LeppOp:
            case local::LepvOp:
            case local::LevpOp:
            case local::LevvOp:
            //
            // other operators that are ignored
            case local::InvOp:
            break;

            // -------------------------------------------------------------
            // operators that are implemented
            case local::AddvvOp:
            is_json_op_used[local::json::add_json_op] = true;
            ++count_variable_op_used;
            break;

            case local::CSumOp:
            is_json_op_used[local::json::sum_json_op] = true;
            ++count_variable_op_used;
            break;

            case local::MulvvOp:
            is_json_op_used[local::json::mul_json_op] = true;
            ++count_variable_op_used;
            break;

            // -------------------------------------------------------------
            // EndOp:
            case local::EndOp:
            more_operators = false;
            break;

            default:
            error_message += local::OpName(var_op);
            CPPAD_ASSERT_KNOWN( false, error_message.c_str() );
            break;
        }
    }
    // --------------------------------------------------------------------
    // par2node
    pod_vector<size_t> par2node(n_parameter);
    par2node[0] = 0; // invalid value
    for(size_t i = 1; i <= n_dynamic_ind; ++i)
        par2node[i] = i; // independent dynamic parameters
    for(size_t i = n_dynamic_ind + 1; i < n_parameter; ++i)
        par2node[i] = 0; // will be set later
    // ----------------------------------------------------------------------
    // start with the opening '{' for this graph
    std::string result = "{\n";

    // ----------------------------------------------------------------------
    // op_define_vec
    size_t n_define = 0;
    pod_vector<size_t> graph_code(n_json_op);
    for(size_t i = 0; i < n_json_op; ++i)
    {   graph_code[i] = 0;
        if( is_json_op_used[i] )
            graph_code[i] = ++n_define;
    }
    result += "'op_define_vec' : [ " + to_string(n_define) + ", [\n";
    size_t count_define = 0;
    for(size_t i = 0; i < n_json_op; ++i)
    {   if( is_json_op_used[i] )
        {   ++count_define;
            const std::string name = local::json::op_enum2name[i];
            size_t op_code   = graph_code[i];
            result += "{ 'op_code':" + to_string(op_code);
            result += ", 'name':'" + name + "'";
            if( i != size_t( local::json::sum_json_op ) )
                result += ", 'n_arg':2";
            result += " }";
            if( count_define < n_define )
                result += " ,\n";
        }
    }
    CPPAD_ASSERT_UNKNOWN( count_define == n_define );
    result += " ]\n] ,\n";
    //
    // initialize index of previous node in the graph
    size_t previous_node = 0;
    //
    // n_dynamic_ind
    result += "'n_dynamic_ind' : " + to_string( n_dynamic_ind ) + ",\n";
    previous_node += n_dynamic_ind;
    //
    // n_independent
    result += "'n_independent' : " + to_string( n_independent ) + ",\n";
    previous_node += n_independent;
    //
    // string_vec
    result += "'string_vec' : [ 0, [ ] ],\n";
    previous_node += n_string;
    // --------------------------------------------------------------------
    // constant_vec and par2node for constants
    local::pod_vector_maybe<Base> constant_vec;
    for(size_t i = 1; i < n_parameter; ++i)
    {   if( ! dyn_par_is[i] )
        {   // this is a constant node
            constant_vec.push_back( parameter[i] );
            par2node[i] = ++previous_node;
        }
    }
    CPPAD_ASSERT_UNKNOWN( n_constant == constant_vec.size() );
    result += "'constant_vec' : [ " + to_string(n_constant) + ", [\n";
    for(size_t i = 0; i < n_constant; ++i)
    {   result += to_string( constant_vec[i] );
        if( i + 1 < n_constant )
            result += ",\n";
    }
    result += " ] ],\n";
    // ----------------------------------------------------------------------
    // Json operators is dynamic operators plus variables operators.
    // Skip BeginOp, EndOp, and independent variables.
    size_t n_usage = n_dynamic_op + count_variable_op_used;
    result += "'op_usage_vec' : [ " + to_string(n_usage) + ", [\n";
    size_t count_usage = 0;
    // ----------------------------------------------------------------------
    // dynamic parameter operations and par2node
    // for dynamic parameters that are not constants or independent
    CPPAD_ASSERT_UNKNOWN( num_arg_dyn(local::ind_dyn) == 0 );
    size_t i_arg = 0;
    pod_vector<size_t> node_arg(2);
    for(size_t i_dyn = n_dynamic_ind; i_dyn < n_dynamic; ++i_dyn)
    {   // operator for this dynamic parameter
        local::op_code_dyn dyn_op = local::op_code_dyn( dyn_par_op[i_dyn] );
        //
        // parameter index for this dynamic parameter
        size_t i_par = size_t( dyn_ind2par_ind[i_dyn] );
        CPPAD_ASSERT_UNKNOWN( par2node[i_par] == 0 );
        par2node[i_par] = ++previous_node;
        //
        // number of arguments for operators with fixed number of arguments
        size_t n_arg = size_t( num_arg_dyn(dyn_op) );
        CPPAD_ASSERT_UNKNOWN( n_arg <= 2 );
        //
        // arguments in graph node space
        for(size_t i = 0; i < n_arg; ++i)
        {   node_arg[i] = par2node[ dyn_par_arg[i_arg + i] ];
            CPPAD_ASSERT_UNKNOWN( node_arg[i] > 0 );
        }
        //
        size_t op_code;
        switch(dyn_op)
        {
            case local::add_dyn:
            op_code = graph_code[ local::json::add_json_op ];
            CPPAD_ASSERT_UNKNOWN( op_code != 0 );
            CPPAD_ASSERT_UNKNOWN( n_arg == 2 );
            result += "[ " + to_string(op_code) + ", ";
            result += to_string(node_arg[0]) + ", ";
            result += to_string(node_arg[1]) + " ]";
            i_arg  += n_arg;
            ++count_usage;
            break;

            default:
            // This error should have been reported above
            CPPAD_ASSERT_UNKNOWN( false );
            break;
        }
        if( count_usage < n_usage )
            result += " ,\n";
    }
    CPPAD_ASSERT_UNKNOWN( count_usage == n_dynamic_op );
    // ----------------------------------------------------------------------
    // variable operators
    pod_vector<size_t> var2node(n_variable);
    var2node[0] = 0; // invalide node value
    for(size_t i = 1; i <= n_independent; ++i)
        var2node[i] = n_dynamic_ind + i;
    for(size_t i = n_independent + 1; i < n_variable; ++i)
        var2node[i] = 0; // invalid node value
    //
    itr            = play_.begin();
    more_operators = true;
    while(more_operators)
    {
        // next op
        (++itr).op_info(var_op, arg, i_var);
        size_t op_code;
        switch( var_op )
        {
            // -------------------------------------------------------------
            // Ignore all comparison operators (for now)
            case local::EqppOp:
            case local::EqpvOp:
            case local::EqvvOp:
            case local::NeppOp:
            case local::NepvOp:
            case local::NevvOp:
            //
            case local::LtppOp:
            case local::LtpvOp:
            case local::LtvpOp:
            case local::LtvvOp:
            case local::LeppOp:
            case local::LepvOp:
            case local::LevpOp:
            case local::LevvOp:
            //
            break;

            // --------------------------------------------------------------

            // AddvvOp:
            case local::AddvvOp:
            op_code = graph_code[ local::json::add_json_op ];
            CPPAD_ASSERT_UNKNOWN( op_code != 0 );
            var2node[i_var] = ++previous_node;
            result += "[ " + to_string(op_code) + ", ";
            result += to_string( var2node[ arg[0] ] ) + ", ";
            result += to_string( var2node[ arg[1] ] ) + " ]";
            ++count_usage;
            if( count_usage < n_usage )
                result += " ,\n";
            break;
            // --------------------------------------------------------------

            // CSumOp
            case local::CSumOp:
            op_code = graph_code[ local::json::sum_json_op ];
            CPPAD_ASSERT_UNKNOWN( op_code != 0 );
            var2node[i_var] = ++previous_node;
            if( (arg[1] != arg[2]) | (arg[3] != arg[4]) )
            {   error_message = "A CSumOp operator has subtraction entries.";
                CPPAD_ASSERT_KNOWN(false, error_message.c_str() );
            }
            else
            {   CPPAD_ASSERT_UNKNOWN( arg[4] > 4 );
                size_t n_arg = size_t(arg[4] - 4);
                result += "[ " + to_string(op_code) + ", 1, ";
                result += to_string(n_arg) + ", [ ";
                size_t arg_node  = par2node[ arg[0] ];
                result += to_string(arg_node) + ", ";
                for(addr_t i = 5; i < arg[1]; ++i)
                {   arg_node    = var2node[ arg[i] ];
                    CPPAD_ASSERT_UNKNOWN( arg_node > 0 );
                    result += to_string(arg_node);
                    if( i + 1 < arg[3] )
                        result += ", ";
                }
                for(addr_t i = arg[2]; i < arg[3]; ++i)
                {   arg_node  = par2node[ arg[i] ];
                    result   += to_string(arg_node);
                    if( i + 1 < arg[3] )
                        result += ", ";
                }
                result += "] ]";
            }
            itr.correct_before_increment();
            ++count_usage;
            if( count_usage < n_usage )
                result += " ,\n";
            break;
            // --------------------------------------------------------------

            // EndOp:
            case local::EndOp:
            more_operators = false;
            break;
            // --------------------------------------------------------------

            // InvOp: independent variables
            case local::InvOp:
            break;
            // --------------------------------------------------------------

            // MulvvOp:
            case local::MulvvOp:
            op_code = graph_code[ local::json::mul_json_op ];
            CPPAD_ASSERT_UNKNOWN( op_code != 0 );
            var2node[i_var] = ++previous_node;
            result += "[ " + to_string(op_code) + ", ";
            result += to_string( var2node[ arg[0] ] ) + ", ";
            result += to_string( var2node[ arg[1] ] ) + " ]";
            ++count_usage;
            if( count_usage < n_usage )
                result += " ,\n";
            break;
            // --------------------------------------------------------------

            default:
            // This error should have been reported above
            CPPAD_ASSERT_UNKNOWN(false);
            break;
        }
    }
    CPPAD_ASSERT_UNKNOWN( count_usage == n_usage );
    result += " ]\n] ,\n";
    // ----------------------------------------------------------------------
    // dependent_vec
    size_t n_dependent = dep_taddr_.size();
    result += "'dependent_vec' : [ " + to_string(n_dependent) + ", [ ";
    for(size_t i = 0; i < n_dependent; ++i)
    {   result += to_string( var2node[ dep_taddr_[i] ] );
        if( i + 1 < n_dependent )
            result += ", ";
    }
    result += " ] ]\n";
    result += "}\n";
    // ----------------------------------------------------------------------
    // Convert the single quote to double quote
    for(size_t i = 0; i < result.size(); ++i)
        if( result[i] == '\'' ) result[i] = '"';
    //
    return result;
}

# endif
