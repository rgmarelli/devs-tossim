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
#ifndef DEVS_CPP_SHUFFLE__
#define DEVS_CPP_SHUFFLE__

#include <vector>
#include <algorithm>
#include <time.h>

namespace DEVS {

template<class RandomAccessContainer>
class Shuffle
{
public:

    typedef typename RandomAccessContainer::value_type value_type;
    typedef typename RandomAccessContainer::reference reference;
    typedef typename RandomAccessContainer::pointer pointer;

    struct iterator
    {
    public:
        iterator(RandomAccessContainer& container, std::vector<int>& index, int v=0) : container_(container), index_(index), v_(v)
        {}

        iterator& operator++() {
            v_++;
            return *this;
        }

        iterator operator++(int) { 
            iterator temp(*this); 
            ++(*this);
            return temp;
        }

        bool operator==(const iterator& it) { return v_==it.v_; }
        bool operator!=(const iterator& it) { return v_!=it.v_; }

        reference operator*() { return this->container_[this->index_[v_]]; }
        pointer operator->() { return &this->container_[this->index_[v_]]; }

    private:
        RandomAccessContainer& container_;
        int v_;
        std::vector<int> index_;
    };

    Shuffle(RandomAccessContainer& container) : container_(container), index_(container.size())
    {
        std::srand(time(NULL));
        for(int i=0;i<container_.size();i++) this->index_[i]=i;
        int n = this->index_.size();
        for (int i = n-1; i > 0; --i) {
            using std::swap;
            swap(this->index_[i], this->index_[std::rand() % (i+1)]);
        }
    }

    iterator begin() { return iterator(this->container_,this->index_,0); }
    iterator end() { return iterator(this->container_,this->index_,index_.size()); }

private:
    RandomAccessContainer& container_;
    std::vector<int> index_;
};

}; //DEVS

#endif //DEVS_CPP_SHUFFLE__

