/* Demo_02_Agent.h */
#ifndef DEMO_02_AGENT
#define DEMO_02_AGENT
#include "repast_hpc/AgentId.h"
#include "repast_hpc/SharedContext.h"
#include "repast_hpc/SharedNetwork.h"
#include "Demo_02_Network.h"
/* Agents */
class RepastHPCDemoAgent{
private:
    repast::AgentId   id_;
    double c, total;	
public:
    RepastHPCDemoAgent(repast::AgentId id);
	RepastHPCDemoAgent(){}
    RepastHPCDemoAgent(repast::AgentId id, double newC, double newTotal);
    ~RepastHPCDemoAgent();
    /* Required Getters */
    virtual repast::AgentId& getId(){return id_;}
    virtual const repast::AgentId& getId() const {return id_;}
    /* Getters specific to this kind of Agent */
    double getC(){return c;} double getTotal(){return total;}
    /* Setter */
    void set(int currentRank, double newC, double newTotal);
    /* Actions */
    bool cooperate(); // Will indicate whether the agent cooperates or not; probability determined by = c / total
    void play(repast::SharedNetwork<RepastHPCDemoAgent, DemoModelCustomEdge<RepastHPCDemoAgent>,
              DemoModelCustomEdgeContent<RepastHPCDemoAgent>, DemoModelCustomEdgeContentManager<RepastHPCDemoAgent> > *network);
};
/* Serializable A	gent Package */
struct RepastHPCDemoAgentPackage {
public:
    int id, rank, type, currentRank; double c, total;
    /* Constructors */
    RepastHPCDemoAgentPackage(); // For serialization
    RepastHPCDemoAgentPackage(int _id, int _rank, int _type, int _currentRank, double _c, double _total);
    /* For archive packaging */
    template<class Archive>
    void serialize(Archive &ar, const unsigned int version){ar & id; ar & rank; ar & type; ar & currentRank; ar & c; ar & total;}
};
#endif
