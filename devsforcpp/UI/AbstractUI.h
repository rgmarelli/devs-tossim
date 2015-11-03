/*
 * Copyright (c) 2013-2014 Ricardo Guido Marelli
 * All rights reserved.
 *
 */
#ifndef DEVS_CPP_UI_ABSTRACT_UI__
#define DEVS_CPP_UI_ABSTRACT_UI__

#include "Blocking_Buffer.h"
#include "Controller.h"

#include <string>

namespace DEVS {
namespace UI {

class AbstractUI 
{
public:
    AbstractUI( Controller* controller ) : controller_(controller)
    {

    }


    static void* loop( void* arg ) {
        AbstractUI *ui = (AbstractUI*)arg;
        bool run_next = false;
        Controller::Command_Result command_result;
        while( 1 ) {
            Optional<std::string> command = ui->buffer_in_.get( !run_next );
            if( command ) {
                if( command.value() == ui->command_exit() ) {
                    ui->buffer_out_.put( "exiting..." );
                    break;
                }

                command_result = ui->controller_->exec( command.value() );
                ui->buffer_out_.put( command_result.value );
            }
            if( (!command_result.err && command ) || run_next ) {
                run_next = ui->controller_->step();
            }
         }
    }

    void run() {
        pthread_t thread;
        pthread_create(&thread, NULL, &AbstractUI::loop, (void*)this);

        this->do_run();

        pthread_join( thread, NULL );
    }

protected:

    virtual void do_run() = 0;

    virtual std::string command_exit() { return controller_->command_exit(); }

    Optional<std::string> send_command( std::string command ) {
        buffer_in_.put( command );
        return buffer_out_.get( true );
    }

    Blocking_Buffer<std::string> buffer_in_;
    Blocking_Buffer<std::string> buffer_out_;
    Controller *controller_;
};

} //namespace UI
} //namespace DEVS

#endif //DEVS_CPP_UI_ABSTRACT_UI__
