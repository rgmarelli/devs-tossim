/* 
 * devsCPP - a DEVS C++ library
 * Copyright (c) 2013 Ricardo Guido Marelli
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
