#pragma once
#ifndef NIFTY_GRAPH_OPTIMIZATION_VISITOR_BASE_HXX
#define NIFTY_GRAPH_OPTIMIZATION_VISITOR_BASE_HXX

#include <string>
#include <initializer_list>
#include <sstream>
#include <iostream>

namespace nifty {
namespace graph {
namespace optimization{


    template<class SOLVER> 
    class VisitorBase{
    public:

        typedef SOLVER SolverType;

        // maybe the solver ptr will become a shared ptr
        virtual void begin(SolverType * solver) = 0;
        virtual bool visit(SolverType * solver) = 0;
        virtual void end(SolverType * solver) = 0;



        
        virtual void addLogNames(std::initializer_list<std::string> logNames){

        }
        virtual void setLogValue(const size_t logIndex, double logValue){

        }
    };



    template<class SOLVER> 
    class VerboseVisitor : public VisitorBase<SOLVER>{
    public:
        typedef SOLVER SolverType;

        VerboseVisitor(const int printNth = 1)
        :   printNth_(printNth),
            runOpt_(true),
            iter_(1){
        }

        virtual void begin(SolverType * ) {
            std::cout<<"begin inference\n";
        }
        virtual bool visit(SolverType * solver) {
            if(iter_%printNth_ == 0){
                std::stringstream ss;
                ss<<solver->currentBestEnergy()<<" ";
                for(size_t i=0; i<logNames_.size(); ++i){
                    ss<<logNames_[i]<<" "<<logValues_[i]<<" ";
                }
                ss<<"\n";
                std::cout<<ss.str();
            }
            ++iter_;
            return runOpt_;
        }
        virtual void end(SolverType * )   {
            std::cout<<"end inference\n";
        }
        virtual void addLogNames(std::initializer_list<std::string> logNames){
            logNames_.assign(logNames.begin(), logNames.end());
            logValues_.resize(logNames.size());
        }
        virtual void setLogValue(const size_t logIndex, double logValue){
            logValues_[logIndex] = logValue;
        }
        void stopOptimize(){
            runOpt_ = false;
        }
    private:
        bool runOpt_;
        int printNth_;
        int iter_;
        std::vector<std::string> logNames_;
        std::vector<double> logValues_;
    };



    template<class SOLVER> 
    class EmptyVisitor : public VisitorBase<SOLVER>{
    public:
        typedef SOLVER SolverType;

        virtual void begin(SolverType * solver) {}
        virtual bool visit(SolverType * solver) {return true;}
        virtual void end(SolverType * solver)   {}
    private:
    };



    template<class SOLVER>
    class VisitorProxy{
    public:
        typedef SOLVER SolverType;
        typedef VisitorBase<SOLVER> VisitorBaseTpe;
        VisitorProxy(VisitorBaseTpe * visitor)
        :   visitor_(visitor){

        }

        void addLogNames(std::initializer_list<std::string> logNames){
            if(visitor_  != nullptr){
                visitor_->addLogNames(logNames);
            }
        }
        void begin(SolverType * solver) {
            if(visitor_ != nullptr){
                visitor_->begin(solver);
            }
        }
        bool visit(SolverType * solver) {
            if(visitor_ != nullptr){
                return visitor_->visit(solver);
            }
            return true;
        }
        void end(SolverType * solver)   {
            if(visitor_ != nullptr){
                visitor_->begin(solver);
            }
        }

        void setLogValue(const size_t logIndex, const double logValue)   {
            if(visitor_ != nullptr){
                visitor_->setLogValue(logIndex, logValue);
            }
        }

    private:
        VisitorBaseTpe * visitor_;
    };





}
}
}

#endif // NIFTY_GRAPH_OPTIMIZATION_VISITOR_BASE_HXX
