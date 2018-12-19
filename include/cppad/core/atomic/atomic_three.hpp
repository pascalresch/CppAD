# ifndef CPPAD_CORE_ATOMIC_ATOMIC_THREE_HPP
# define CPPAD_CORE_ATOMIC_ATOMIC_THREE_HPP
/* --------------------------------------------------------------------------
CppAD: C++ Algorithmic Differentiation: Copyright (C) 2003-18 Bradley M. Bell

CppAD is distributed under the terms of the
             Eclipse Public License Version 2.0.

This Source Code may also be made available under the following
Secondary License when the conditions for such availability set forth
in the Eclipse Public License, Version 2.0 are satisfied:
      GNU General Public License, Version 2.0 or later.
---------------------------------------------------------------------------- */
/*
$begin atomic_three$$
$spell
    taylor
    ctor
    afun
    arg
    jac
    hes
    CppAD
$$

$section Defining Atomic Functions: Third Generation$$

$head Under Construction$$

$head Syntax$$

$codei%
%atomic_derived% %afun%(%ctor_arg_list%)
%afun%(%ax%, %ay%)
%ok% = %afun%.forward(
    %order_low%, %order_up%, %type_x%, %type_y%, %taylor_x%, %taylor_y%
)%
%ok% = %afun%.reverse(
    %order_up%, %taylor_x%, %taylor_y%, %partial_x%, %partial_y%
)%
%ok% = %afun%.jac_sparsity(
    %dependency%, %select_x% %select_y%, %pattern_out%
)%
%ok% = %afun%.jac_sparsity(
    %dependency%, %select_x% %select_y%, %pattern_out%
)%
atomic_three<%Base%>::clear()%$$

$head See Also$$
$cref checkpoint$$

$head Purpose$$

$subhead Speed$$
In some cases, it is possible to compute derivatives of a function
$latex \[
    y = f(x) \; {\rm where} \; f : \B{R}^n \rightarrow \B{R}^m
\] $$
more efficiently than by coding it using $codei%AD<%Base%>%$$
$cref/atomic/glossary/Operation/Atomic/$$ operations
and letting CppAD do the rest.
The class $codei%atomic_three%<%Base%>%$$ is used to
create a new atomic operation corresponding to a function $latex f(x)$$
where the user specifies how to compute the derivatives
and sparsity patterns for $latex f(x)$$.

$subhead Reduce Memory$$
If the function $latex f(x)$$ is many times during the recording
of an $cref ADFun$$ object,
using an atomic version of $latex f(x)$$ removed the need for repeated
copies of the corresponding $codei%AD<%Base%>%$$ operations and variables
in the recording.

$head Virtual Functions$$
Derivatives for an $code atomic_three$$
function are implemented by defining the
following virtual functions in the $icode atomic_derived$$ class:
$comment 2DO:
1. Change name of base_atomic versions of these sections.
2. Document and implement these new sections.
3. Copy base_atomic examples to test_more/deprecated.
4. Change base_atomic examples to use base_three.
$$
$cref/forward/atomic_two_forward/$$,
$cref/reverse/atomic_two_reverse/$$,
$code jac_sparsity$$,
$code hes_sparsity$$,
These virtual functions have a default implementation
that returns $icode%ok% == false%$$.
The $code forward$$ function,
for the case $icode%order_up% == 0%$$, must be implemented.
Otherwise, only those functions and orders
required by the your calculations need to be implemented.
For example,
$icode forward$$ for the case $icode%order_up% == 2%$$ can just return
$icode%ok% == false%$$ unless you require
forward mode calculation of second derivatives.

$head Examples$$
See $cref atomic_three_example$$.

$end
-------------------------------------------------------------------------------
$begin atomic_three_example$$

$section Example Defining Atomic Functions: Third Generation$$

$head Under Construction$$

$head Getting Started$$
The file $cref atomic_get_started.cpp$$ contains an example and test
that shows the minimal amount of information required to create
a user defined atomic operation.

$head Scalar Function$$
The file $cref atomic_reciprocal.cpp$$ contains an example and test
where the user provides the code for computing derivatives.
This example is simple because the domain and range are scalars.

$head Vector Range$$
The file $cref atomic_tangent.cpp$$ contains another example
where the user provides the code for computing derivatives.
This example is more complex because the range has two components.

$head Hessian Sparsity Patterns$$
The file $cref atomic_rev_sparse_hes.cpp$$ contains an minimal example
where the user provides the code for computing Hessian sparsity patterns.

$head General Case$$
The file $cref atomic_mat_mul.cpp$$ contains a more general example
where the user provides the code for computing derivatives.
This example is more complex because both the domain and range
dimensions are arbitrary.

$end
-------------------------------------------------------------------------------
*/

# include <set>
# include <cppad/core/cppad_assert.hpp>
# include <cppad/local/sparse_internal.hpp>

// needed before one can use CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL
# include <cppad/utility/thread_alloc.hpp>

namespace CppAD { // BEGIN_CPPAD_NAMESPACE
/*!
\file atomic_three.hpp
Base class for atomic function operations.
*/

template <class Base>
class atomic_three {
// ===================================================================
public:
private:
    // ------------------------------------------------------
    // constants
    //
    /// index of this object in class_object
    const size_t index_;

    // -----------------------------------------------------
    // variables
    //
    /// temporary work space used by member functions, declared here to avoid
    // memory allocation/deallocation for each usage
    struct work_struct {
        vector<bool>               vx;
        vector<bool>               vy;
        //
        vector<Base>               tx;
        vector<Base>               ty;
        //
        vector< AD<Base> >         atx;
        vector< AD<Base> >         aty;
        //
        vector<bool>               bool_t;
        //
        vectorBool                 pack_h;
        vectorBool                 pack_r;
        vectorBool                 pack_s;
        vectorBool                 pack_u;
        //
        vector<bool>               bool_h;
        vector<bool>               bool_r;
        vector<bool>               bool_s;
        vector<bool>               bool_u;
        //
        vector< std::set<size_t> > set_h;
        vector< std::set<size_t> > set_r;
        vector< std::set<size_t> > set_s;
        vector< std::set<size_t> > set_u;
    };
    // Use pointers, to avoid false sharing between threads.
    // Not using: vector<work_struct*> work_;
    // so that deprecated atomic examples do not result in a memory leak.
    work_struct* work_[CPPAD_MAX_NUM_THREADS];
    // -----------------------------------------------------
    // static member functions
    //
    /// List of all the object in this class
    static std::vector<atomic_three *>& class_object(void)
    {   CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL;
        static std::vector<atomic_three *> list_;
        return list_;
    }
    /// List of names for each object in this class
    static std::vector<std::string>& class_name(void)
    {   CPPAD_ASSERT_FIRST_CALL_NOT_PARALLEL;
        static std::vector<std::string> list_;
        return list_;
    }
public:
    // =====================================================================
    // In User API
    // =====================================================================
    //
    // ---------------------------------------------------------------------
    // ctor: doxygen in atomic_three/ctor.hpp
    atomic_three(void);
    atomic_three(const std::string& name);

    // operator(): see doxygen in atomic_three/afun.hpp
    template <class ADVector>
    void operator()(
        const ADVector&  ax     ,
              ADVector&  ay     ,
        size_t           id = 0
    );

    // ------------------------------------------------------------------------
    // forward: see docygen in atomic_three/forward.hpp
    virtual bool forward(
        size_t                    p  ,
        size_t                    q  ,
        const vector<bool>&       vx ,
              vector<bool>&       vy ,
        const vector<Base>&       tx ,
              vector<Base>&       ty
    );
    virtual bool forward(
        size_t                    p  ,
        size_t                    q  ,
        const vector<bool>&       vx ,
              vector<bool>&       vy ,
        const vector< AD<Base> >& atx ,
              vector< AD<Base> >& aty
    );

    // ------------------------------------------------------------------------
    // reverse: see docygen in atomic_three/reverse.hpp
    virtual bool reverse(
        size_t                    q  ,
        const vector<Base>&       tx ,
        const vector<Base>&       ty ,
              vector<Base>&       px ,
        const vector<Base>&       py
    );
    virtual bool reverse(
        size_t                    q   ,
        const vector< AD<Base> >& atx ,
        const vector< AD<Base> >& aty ,
              vector< AD<Base> >& apx ,
        const vector< AD<Base> >& apy
    );

    // ------------------------------------------------------------
    // for_sparse_jac: see doxygen in atomic_three/for_sparse_jac.hpp
    virtual bool for_sparse_jac(
        size_t                                  q  ,
        const vector< std::set<size_t> >&       r  ,
              vector< std::set<size_t> >&       s  ,
        const vector<Base>&                     x
    );
    virtual bool for_sparse_jac(
        size_t                                  q  ,
        const vector<bool>&                     r  ,
              vector<bool>&                     s  ,
        const vector<Base>&                     x
    );
    virtual bool for_sparse_jac(
        size_t                                  q  ,
        const vectorBool&                       r  ,
              vectorBool&                       s  ,
        const vector<Base>&                     x
    );
    template <class InternalSparsity>
    void for_sparse_jac(
        const vector<Base>&              x            ,
        const local::pod_vector<size_t>& x_index      ,
        const local::pod_vector<size_t>& y_index      ,
        InternalSparsity&                var_sparsity
    );
    // deprecated versions
    virtual bool for_sparse_jac(
        size_t                                  q  ,
        const vector< std::set<size_t> >&       r  ,
              vector< std::set<size_t> >&       s
    );
    virtual bool for_sparse_jac(
        size_t                                  q  ,
        const vector<bool>&                     r  ,
              vector<bool>&                     s
    );
    virtual bool for_sparse_jac(
        size_t                                  q  ,
        const vectorBool&                       r  ,
              vectorBool&                       s
    );
    // ------------------------------------------------------------
    // rev_sparse_jac: see doxygen in atomic_three/rev_sparse_jac.hpp
    virtual bool rev_sparse_jac(
        size_t                                  q  ,
        const vector< std::set<size_t> >&       rt ,
              vector< std::set<size_t> >&       st ,
        const vector<Base>&                     x
    );
    virtual bool rev_sparse_jac(
        size_t                                  q  ,
        const vector<bool>&                     rt ,
              vector<bool>&                     st ,
        const vector<Base>&                     x
    );
    virtual bool rev_sparse_jac(
        size_t                                  q  ,
        const vectorBool&                       rt ,
              vectorBool&                       st ,
        const vector<Base>&                     x
    );
    template <class InternalSparsity>
    void rev_sparse_jac(
        const vector<Base>&        x            ,
        const local::pod_vector<size_t>& x_index ,
        const local::pod_vector<size_t>& y_index ,
        InternalSparsity&          var_sparsity
    );
    // deprecated versions
    virtual bool rev_sparse_jac(
        size_t                                  q  ,
        const vector< std::set<size_t> >&       rt ,
              vector< std::set<size_t> >&       st
    );
    virtual bool rev_sparse_jac(
        size_t                                  q  ,
        const vector<bool>&                     rt ,
              vector<bool>&                     st
    );
    virtual bool rev_sparse_jac(
        size_t                                  q  ,
        const vectorBool&                       rt ,
              vectorBool&                       st
    );
    // ------------------------------------------------------------
    // for_sparse_hes: see doxygen in atomic_three/for_sparse_hes.hpp
    virtual bool for_sparse_hes(
        const vector<bool>&             vx ,
        const vector<bool>&             r  ,
        const vector<bool>&             s  ,
        vector< std::set<size_t> >&     h  ,
        const vector<Base>&             x
    );
    virtual bool for_sparse_hes(
        const vector<bool>&             vx ,
        const vector<bool>&             r  ,
        const vector<bool>&             s  ,
        vector<bool>&                   h  ,
        const vector<Base>&             x
    );
    virtual bool for_sparse_hes(
        const vector<bool>&             vx ,
        const vector<bool>&             r  ,
        const vector<bool>&             s  ,
        vectorBool&                     h  ,
        const vector<Base>&             x
    );
    template <class InternalSparsity>
    void for_sparse_hes(
        const vector<Base>&              x                ,
        const local::pod_vector<size_t>& x_index          ,
        const local::pod_vector<size_t>& y_index          ,
        const InternalSparsity&          for_jac_sparsity ,
        const InternalSparsity&          rev_jac_sparsity ,
        InternalSparsity&                for_hes_sparsity
    );
    // deprecated versions
    virtual bool for_sparse_hes(
        const vector<bool>&             vx ,
        const vector<bool>&             r  ,
        const vector<bool>&             s  ,
        vector< std::set<size_t> >&     h
    );
    virtual bool for_sparse_hes(
        const vector<bool>&             vx ,
        const vector<bool>&             r  ,
        const vector<bool>&             s  ,
        vector<bool>&                   h
    );
    virtual bool for_sparse_hes(
        const vector<bool>&             vx ,
        const vector<bool>&             r  ,
        const vector<bool>&             s  ,
        vectorBool&                     h
    );
    // ------------------------------------------------------------
    // rev_sparse_hes: see doxygen in atomic_three/rev_sparse_hes.hpp
    virtual bool rev_sparse_hes(
        const vector<bool>&                     vx ,
        const vector<bool>&                     s  ,
              vector<bool>&                     t  ,
        size_t                                  q  ,
        const vector< std::set<size_t> >&       r  ,
        const vector< std::set<size_t> >&       u  ,
              vector< std::set<size_t> >&       v  ,
        const vector<Base>&                     x
    );
    virtual bool rev_sparse_hes(
        const vector<bool>&                     vx ,
        const vector<bool>&                     s  ,
              vector<bool>&                     t  ,
        size_t                                  q  ,
        const vector<bool>&                     r  ,
        const vector<bool>&                     u  ,
              vector<bool>&                     v  ,
        const vector<Base>&                     x
    );
    virtual bool rev_sparse_hes(
        const vector<bool>&                     vx ,
        const vector<bool>&                     s  ,
              vector<bool>&                     t  ,
        size_t                                  q  ,
        const vectorBool&                       r  ,
        const vectorBool&                       u  ,
              vectorBool&                       v  ,
        const vector<Base>&                     x
    );
    template <class InternalSparsity>
    void rev_sparse_hes(
        const vector<Base>&              x                ,
        const local::pod_vector<size_t>& x_index          ,
        const local::pod_vector<size_t>& y_index          ,
        const InternalSparsity&          for_jac_sparsity ,
        bool*                            rev_jac_flag     ,
        InternalSparsity&                rev_hes_sparsity
    );
    // deprecated
    virtual bool rev_sparse_hes(
        const vector<bool>&                     vx ,
        const vector<bool>&                     s  ,
              vector<bool>&                     t  ,
        size_t                                  q  ,
        const vector< std::set<size_t> >&       r  ,
        const vector< std::set<size_t> >&       u  ,
              vector< std::set<size_t> >&       v
    );
    virtual bool rev_sparse_hes(
        const vector<bool>&                     vx ,
        const vector<bool>&                     s  ,
              vector<bool>&                     t  ,
        size_t                                  q  ,
        const vector<bool>&                     r  ,
        const vector<bool>&                     u  ,
              vector<bool>&                     v
    );
    virtual bool rev_sparse_hes(
        const vector<bool>&                     vx ,
        const vector<bool>&                     s  ,
              vector<bool>&                     t  ,
        size_t                                  q  ,
        const vectorBool&                       r  ,
        const vectorBool&                       u  ,
              vectorBool&                       v
    );
    // ------------------------------------------------------------
    // clear: see doxygen in atomic_three/clear.hpp
    static void clear(void);

    // =====================================================================
    // Not in User API
    // =====================================================================

    /// Name corresponding to a base_atomic object
    const std::string& afun_name(void) const
    {   return class_name()[index_]; }

    /// destructor informs CppAD that this atomic function with this index
    /// has dropped out of scope by setting its pointer to null
    virtual ~atomic_three(void)
    {   CPPAD_ASSERT_UNKNOWN( class_object().size() > index_ );
        // change object pointer to null, but leave name for error reporting
        class_object()[index_] = CPPAD_NULL;
        //
        // free temporary work memory
        for(size_t thread = 0; thread < CPPAD_MAX_NUM_THREADS; thread++)
            free_work(thread);
    }
    /// allocates work_ for a specified thread
    void allocate_work(size_t thread)
    {   if( work_[thread] == CPPAD_NULL )
        {   // allocate the raw memory
            size_t min_bytes = sizeof(work_struct);
            size_t num_bytes;
            void*  v_ptr     = thread_alloc::get_memory(min_bytes, num_bytes);
            // save in work_
            work_[thread]    = reinterpret_cast<work_struct*>( v_ptr );
            // call constructor
            new( work_[thread] ) work_struct;
        }
        return;
    }
    /// frees work_ for a specified thread
    void free_work(size_t thread)
    {   if( work_[thread] != CPPAD_NULL )
        {   // call destructor
            work_[thread]->~work_struct();
            // return memory to avialable pool for this thread
            thread_alloc::return_memory(
                reinterpret_cast<void*>(work_[thread])
            );
            // mark this thread as not allocated
            work_[thread] = CPPAD_NULL;
        }
        return;
    }
    /// atomic_three function object corresponding to a certain index
    static atomic_three* class_object(size_t index)
    {   CPPAD_ASSERT_UNKNOWN( class_object().size() > index );
        return class_object()[index];
    }
    /// atomic_three function name corresponding to a certain index
    static const std::string& class_name(size_t index)
    {   CPPAD_ASSERT_UNKNOWN( class_name().size() > index );
        return class_name()[index];
    }

    /*!
    Set value of id (used by deprecated atomic_one class)

    This function is called just before calling any of the virtual function
    and has the corresponding id of the corresponding virtual call.
    */
    virtual void set_old(size_t id)
    { }
// ---------------------------------------------------------------------------
};
} // END_CPPAD_NAMESPACE

// functitons implemented in cppad/core/atomic_three files
# include <cppad/core/atomic/three_ctor.hpp>
# include <cppad/core/atomic/two_option.hpp>
# include <cppad/core/atomic/two_afun.hpp>
# include <cppad/core/atomic/two_forward.hpp>
# include <cppad/core/atomic/two_reverse.hpp>
# include <cppad/core/atomic/two_for_sparse_jac.hpp>
# include <cppad/core/atomic/two_rev_sparse_jac.hpp>
# include <cppad/core/atomic/two_for_sparse_hes.hpp>
# include <cppad/core/atomic/two_rev_sparse_hes.hpp>
# include <cppad/core/atomic/two_clear.hpp>

# endif