/* Demo_02_Agent.cpp */
#include "Demo_02_Agent.h"
RepastHPCDemoAgent::RepastHPCDemoAgent(repast::AgentId id): id_(id), state(0){ } // not sure , c(100), total(200)
RepastHPCDemoAgent::RepastHPCDemoAgent(repast::AgentId id, int state, int newState): id_(id), state(newState){ }
RepastHPCDemoAgent::~RepastHPCDemoAgent(){ }
void RepastHPCDemoAgent::set(int currentRank, int state){id_.currentRank(currentRank); newState = state;} // change ID & vars corresponding to agent
/*
const char* threshold(repast::SharedContext<RepastHPCDemoAgent>* context) { // Moore Machine implementation (see diagram in-report)
// lower state corresponds to transportation mode creating the least amount of traffic (1 - least, 3 - most); thresh range e [4 (4*1), 12 (4*3)] since an agent has 4 neighbours
    if(thresh <= 7) 			        {state = 3; return "using a car or public transport";} // agent chooses to use (drive/ride in) a motorized vehicle
    else if (thresh = 8 || thresh < 11)	{state = 2; return "cycling";} // agent chooses to cycle
    else 				                {state = 1; return "on-foot";} // agent chooses to walk
} */
void RepastHPCDemoAgent::play(repast::SharedNetwork<RepastHPCDemoAgent,
                              repast::RepastEdge<RepastHPCDemoAgent>,
                              repast::RepastEdgeContent<RepastHPCDemoAgent>,
                              repast::RepastEdgeContentManager<RepastHPCDemoAgent> > *network){
    std::vector<RepastHPCDemoAgent*> agentsToPlay;
    network->successors(this, agentsToPlay);
    std::vector<RepastHPCDemoAgent*>::iterator agentToPlay = agentsToPlay.begin();
    while(agentToPlay != agentsToPlay.end()){
        agentToPlay++;
    }
}
/*
void RepastHPCDemoAgent::play(repast::SharedNetwork<RepastHPCDemoAgent, repast::RepastEdge<RepastHPCDemoAgent>, repast::RepastEdgeContent<RepastHPCDemoAgent>,
                              repast::RepastEdgeContentManager<RepastHPCDemoAgent> > *network) {
    std::set<RepastHPCDemoAgent*> agentsToThreshold; // vector into which the ptrs to the agents will be placed
    agentsToThreshold.insert(this); // create neighbouring agents to threshold vector & prohibit thresholding against self	
    //context->selectAgents(4, agentsToThreshold, true); // true removes any elements originally in the vector before it's returned
    std::set<RepastHPCDemoAgent*>::iterator agentToPlay = agentsToPlay.begin();
    while(agentToThreshold != agentsToThreshold.end()){ agentToPlay++;} // while iterator hasn't reached vector end, increment iterator   
    //sum += state; // calculate the sum of transportation mode values
}
*/  
/* Serializable Agent Package Data */
RepastHPCDemoAgentPackage::RepastHPCDemoAgentPackage(){ }
RepastHPCDemoAgentPackage::RepastHPCDemoAgentPackage(int _id, int _rank, int _type, int _currentRank, int _state, int _newState):
id(_id), rank(_rank), type(_type), currentRank(_currentRank), state(_state), newState(_newState){ }
