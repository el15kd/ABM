/* Demo_02_Agent.h */
#ifndef DEMO_02_AGENT
#define DEMO_02_AGENT
#include "repast_hpc/AgentId.h"
#include "repast_hpc/SharedContext.h"
/* Agents */
class RepastHPCDemoAgent{
private:
    repast::AgentId  id_; 
public:
    RepastHPCDemoAgent(repast::AgentId id); ~RepastHPCDemoAgent(); //RepastHPCDemoAgent(){} RepastHPCDemoAgent(repast::AgentId id);    
    /* Required Getters */
    int thresh, state=2;
    virtual repast::AgentId& getId(){return id_;}
    virtual const repast::AgentId& getId() const {return id_;}
    /* Getters specific to this kind of Agent */
    int getState(); // obtain agent state
    /* Setter */
    void set(int currentRank, int state); // set agent current rank & state
    /* Actions */
    //bool cooperate();                                                 // Will indicate whether the agent cooperates or not; probability determined by = c / total
    void play(repast::SharedContext<RepastHPCDemoAgent>* context);    // Choose three other agents from the given context and see if they cooperate or not
};
/* Serializable Agent Package */
struct RepastHPCDemoAgentPackage {
public:
    int id, rank, type, currentRank, state;
    /* Constructors */
    RepastHPCDemoAgentPackage(); RepastHPCDemoAgentPackage(int _id,int _rank,int _type,int _currentRank,int _state); // For serialization
    /* For archive packaging */
    template<class Archive>
    void serialize(Archive &ar,const unsigned int version){ar & id;ar & rank;ar & type;ar & currentRank;ar & state;}	
};
#endif
