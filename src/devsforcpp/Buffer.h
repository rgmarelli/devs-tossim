/*
 * Copyright (c) 2013-2014 Ricardo Guido Marelli
 * All rights reserved.
 *
 */
#ifndef DEVS_CPP_BUFFER__
#define DEVS_CPP_BUFFER__

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <iomanip>   

namespace DEVS {

class Buffer
{
public:

    Buffer() : content_size_(0), content_(NULL)
    {}

    Buffer( const Buffer &buffer ) : content_size_(0), content_(NULL) {
        this->putContent( buffer.content(), buffer.size() ); 
    }

    virtual ~Buffer() {
        releaseContent();
    }

    void putContent(const void* content,unsigned int content_size) {
        releaseContent();
        if( content_size == 0 || content == NULL ) return;
        content_= (void*)malloc(content_size);
        if(content_ != NULL) {
            memcpy(content_,content,content_size);
            content_size_ = content_size;
        }
    }

    template <class T>
    void putContent(const T& content) {
        putContent(&content,sizeof(T));
    }

    const void* content() const { 
        return content_;
    }

    template <class T>
    T content() const {
        T retval;
        if(content_size_ != sizeof(T)) {
            std::cout << "Buffer::content() fail sanity check" << std::endl;
            exit(1);
        }
        memcpy(&retval,content_,sizeof(T));
        return retval;
    }

    unsigned int size() const { 
        return content_size_;
    }

    std::string dump() {
        std::stringstream s;
        for( int i=0;i<this->content_size_;i++ ) {
            s << std::hex << std::setfill('0') << std::setw(2) << (int)(((unsigned char*)this->content_)[i]) << " ";
        }
        return s.str();
    }

protected:
    unsigned int content_size_;
    void* content_;

    void releaseContent() 
    {
        if(content_size_>0  && content_!=NULL) {
            free(content_);
        }
        content_size_ = 0;
        content_ = NULL;
    }
}; // Buffer

}; // DEVS

#endif

