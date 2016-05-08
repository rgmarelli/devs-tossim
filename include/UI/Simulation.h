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
#ifndef DEVS_CPP_UI_SIMULATION__
#define DEVS_CPP_UI_SIMULATION__

#include "Controller.h"
#include "../CoupledSimulator.h"
#include <sstream>

namespace DEVS {
namespace UI {

class Simulation : public DEVS::UI::Controller
{
public:

   Simulation( DEVS::CoupledSimulator *simulator) : simulator_(simulator),
                                       loop_(false),
                                       step_(0),
                                       iterations_(0),
                                       total_exec_time_(0),
                                       time_(1),
                                       compensate_time_(true)
   {}

   virtual Command_Result exec( std::string command ) {

        if( command == "dump" ) {
            simulator_->dump();
            return Command_Result( "Event queue dumped to log" );
        }
        if( command == "help" ) {
            return Command_Result( help() );
        }
        else if( command == "stop" ) {
            step_ = 0;
            loop_ = false;
            return Command_Result("Simulation stopped");
        }
        else if( command == "loop" ) {
            step_ = 0;
            loop_ = true;
            return Command_Result("Simulation loop started");
        }
        else if( command == "info" ) {
            std::stringstream  s;
            DEVS::Time nexttn = simulator_->nextTN();
            DEVS::Time avg = total_exec_time_*((float)1/ (float)(iterations_>0 ? iterations_ : 1) );


            s << "           Model : " << simulator_->model_name() << std::endl
              << "          Childs : " << simulator_->childs().size() << std::endl
              << "          nextTN : ";
            if( nexttn == DEVS::Time::infinity() ) {
                s <<  "(infinity)" << std::endl;
            }
            else {
                s << nexttn.to_string() << std::endl;
            }

            s << "Event queue size : " << simulator_->queue_size() << std::endl
              << "Total exec. time : " << total_exec_time_.to_string() << std::endl
              << " Avg. exec. time : " << avg.to_string() << " (" << avg.msec() << "msec)" << std::endl
              << "     Time factor : " << print_time_factor() << std::endl
              << "      Iterations : " << iterations_ << std::endl;
            return Command_Result(s.str());            
        }
        else if( command.substr(0, 4) == "step" ) {
            step_ = 1;
            if( command.size() > 5 ) {
                step_ = atoi( command.substr( 5 ).c_str() );
            }           
            loop_ = false;

            std::stringstream  s;
            s << "Executing " << step_ << " simulation steps...";
            return Command_Result(s.str());
        }
        else if( command.substr(0, 4) == "time" ) {
            if( command.size() > 5 ) {
                time_ = atof( command.substr( 5 ).c_str() );
            }           

            return Command_Result( "New time factor: " + print_time_factor() );
        }

        return Command_Result( "Unknown command: " + command +".", true );
    }

    virtual std::vector<std::string>* commands( std::vector<std::string> *v ) {
        v->push_back( "dump" );
        v->push_back( "help" );
        v->push_back( "info" );
        v->push_back( "loop" );
        v->push_back( "step" );
        v->push_back( "stop" );
        v->push_back( "time" );
        v->push_back( "quit" );
        return v;
    }

    virtual bool step() {
        if( loop_ || step_ > 0 ) {
            DEVS::Time tn = simulator_->nextTN();
            if( tn == DEVS::Time::infinity() ) {
                Log::write(LOG_NOTICE,"DEVS::Simulation","nextTN is infinity. Simulation stopped.");
                loop_ = false;
                step_ = 0;
                return false;
            }

            DEVS::Time t = DEVS::Time::now();
            simulator_->simulate();
            DEVS::Time diff = DEVS::Time::now() - t;
            total_exec_time_ = total_exec_time_ + diff;
            DEVS::Time nexttn = simulator_->nextTN();
            iterations_++;
            if( time_ ) {
                if( nexttn == DEVS::Time::infinity() ) {
                }
                else {
                    DEVS::Time t(0);
                    if( this->compensate_time_ ) {                       
                        t = (nexttn - tn);
                        if( diff > t ) {
                            t=0;
                            Log::write(LOG_WARNING,"DEVS::Simulation","Missing deadline");
                        }
                        else {
                            t = (t - diff)*time_;
                        }
                    }
                    else {
                        t = (nexttn - tn)*time_;
                    }
                    DEVS::Time::sleep_interval( t );
                }
            }
            if( !loop_ ) {
                step_--;
            }
        }
        return (loop_ || step_ > 0);
    }

protected:
    std::string print_time_factor() {
        std::stringstream  s;
        s << time_;
        if( time_ == 0 ) {
            s << " (as fast as possible)";    
        }
        else if( time_ == 1 ) {
            s << " (real time)";                
        }
        else {
            s << " (virtual time)";
        }
        return s.str();
    }

    virtual std::string help() {
        std::stringstream  s;
        s << "DEVS UI help:" << std::endl
          << "  dump     : dump event queue to log. " << std::endl
          << "  help     : this help. " << std::endl
          << "  info     : show some basic simulation information. " << std::endl
          << "  loop     : start an infinite simulation loop. " << std::endl
          << "  step [N] : run N simulation steps. " << std::endl
          << "  stop     : stop simulation. " << std::endl
          << "  time [T] : expands or contract time by factor T. 0 = run as fast as possible." << std::endl
          << "  quit     : exit the simulation." << std::endl;
        return s.str();
    }

protected:
    DEVS::CoupledSimulator *simulator_;
    bool loop_;
    unsigned int step_;
    unsigned int iterations_;
    DEVS::Time total_exec_time_;
    float time_;
    bool compensate_time_;
};

} //namespace UI
} //namespace DEVS

#endif //DEVS_CPP_UI_SIMULATION__

