/* Demo_02_Agent.cpp */
#include "Demo_02_Agent.h"
RepastHPCDemoAgent::RepastHPCDemoAgent(repast::AgentId id): id_(id){ }
//RepastHPCDemoAgent::RepastHPCDemoAgent(repast::AgentId id{ }
RepastHPCDemoAgent::~RepastHPCDemoAgent(){ }
void RepastHPCDemoAgent::set(int currentRank,int state){id_.currentRank(currentRank); state=state;}
//bool RepastHPCDemoAgent::cooperate(){return repast::Random::instance()->nextDouble() < c/total;}
int RepastHPCDemoAgent::getState(){return state;}
void RepastHPCDemoAgent::play(repast::SharedContext<RepastHPCDemoAgent>* context){
    std::set<RepastHPCDemoAgent*> agentsToPlay;	
//    agentsToPlay.insert(this); // Prohibit playing against self	
//    context->selectAgents(3, agentsToPlay, true);
    std::set<RepastHPCDemoAgent*>::iterator agentToPlay = agentsToPlay.begin();
    while(agentToPlay != agentsToPlay.end()){
        if(thresh <= 7)                    {state = 3;} // agent chooses to use (drive/ride in) a car
    	else if(thresh = 8 || thresh < 11) {state = 2;} // agent chooses to cycle
    	else                               {state = 1;} // agent chooses to walk
        agentToPlay++;
    }
}
/* Serializable Agent Package Data */
RepastHPCDemoAgentPackage::RepastHPCDemoAgentPackage(){ }
RepastHPCDemoAgentPackage::RepastHPCDemoAgentPackage(int _id,int _rank,int _type,int _currentRank,int _state):
id(_id),rank(_rank),type(_type),currentRank(_currentRank),state(_state){ }
/*
const char* treshold(int thresh) {
// 
    if(thresh <= 7) 			{state = 3; return "using a car or public transport";} // agent chooses to use (drive/ride in) a motorized vehicle
    else if (thresh = 8 || thresh < 11)	{state = 2; return "cycling";} // agent chooses to cycle
    else 				{state = 1; return "on-foot";} // agent chooses to walk
} // ^ lower state corresponds to transportation mode creating the least amount of traffic (1 - least, 3 - most); thresh range e [4 (4*1), 12 (4*3)] since an agent has 4 neighbours
int main(int argc, char** argv){
*/