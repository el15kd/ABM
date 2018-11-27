/* Demo_02_Agent.h */
#ifndef DEMO_02_AGENT
#define DEMO_02_AGENT
#include "repast_hpc/AgentId.h" // Agent identity info, unique for each agent
#include "repast_hpc/SharedContext.h" // collection in which agents will exist during sim runs
/* Agents */
class RepastHPCDemoAgent{
private:
    repast::AgentId   id_; int thresh = 0, state = 0, newState = 0;
public:
    RepastHPCDemoAgent(repast::AgentId id);
	RepastHPCDemoAgent(){}
    RepastHPCDemoAgent(repast::AgentId id, int newState); ~RepastHPCDemoAgent();
    /* 2 (Required) Getters; 1 Setter; 1 Action */
    virtual repast::AgentId& getId(){return id_;}
    virtual const repast::AgentId& getId() const {return id_;}
    double getState(){return state;} double getNewState(){return newState;}	
    void set(int currentRank, int newState);
    // Choose 4 neighbouring agents from the given context and calculate the sum of their parameters corresponding to transport mode choice
    //const char* threshold(repast::SharedContext<RepastHPCDemoAgent>* context);
    void play(repast::SharedNetwork<RepastHPCDemoAgent, repast::RepastEdge<RepastHPCDemoAgent>, repast::RepastEdgeContent<RepastHPCDemoAgent>,
              repast::RepastEdgeContentManager<RepastHPCDemoAgent> > *network); 
};
/* Serializable Agent Package */
struct RepastHPCDemoAgentPackage {
public:
    int id, rank, type, currentRank, state, newState;
    /* Constructors */
    RepastHPCDemoAgentPackage(); RepastHPCDemoAgentPackage(int _id, int _rank, int _type, int _currentRank, int _state, double _newState); // For serialization
    /* For archive packaging */
    template<class Archive>
    void serialize(Archive &ar, const unsigned int version){ar & id; ar & rank; ar & type; ar & currentRank; ar & state; ar & newState;}	
};
#endif