/*
 * Copyright (c) 2013-2014 Ricardo Guido Marelli
 * All rights reserved.
 *
 */
#ifndef DEVS_CPP_UI_OPTIONAL__
#define DEVS_CPP_UI_OPTIONAL__

namespace DEVS {
namespace UI {

template <class E>
class Optional
{
public:
    Optional( const E &e ) : e_(e), empty_(false)
    {}

    Optional() : empty_(true)
    {}

    E value() { return e_; }

    operator bool() const { return !empty_; }

private:
    bool empty_;
    E e_;
};

} //namespace UI
} //namespace DEVS

#endif //DEVS_CPP_UI_OPTIONAL__
