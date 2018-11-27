/* Demo_02_Model.cpp */
#include <stdio.h>
#include <vector>
#include <boost/mpi.hpp> // includes all Boost.MPI library headers
#include "repast_hpc/AgentId.h" // Agent identity info, unique for each agent
#include "repast_hpc/RepastProcess.h" // Encapsulates the process in which repast is running and manages interprocess communication
#include "repast_hpc/Utilities.h"
#include "repast_hpc/Properties.h" // Map type object containing key, value(string) properties
#include "repast_hpc/initialize_random.h" // provides RNG seed
#include "repast_hpc/SVDataSetBuilder.h" // Used to build SVDataSets to record data in plain text tabular format
#include "Demo_02_Model.h"
BOOST_CLASS_EXPORT_GUID(repast::SpecializedProjectionInfoPacket<repast::RepastEdgeContent<RepastHPCDemoAgent> >, "SpecializedProjectionInfoPacket_EDGE");
RepastHPCDemoAgentPackageProvider::RepastHPCDemoAgentPackageProvider(repast::SharedContext<RepastHPCDemoAgent>* agentPtr): agents(agentPtr){ }
/*provide pkg*/
void RepastHPCDemoAgentPackageProvider::providePackage(RepastHPCDemoAgent * agent, std::vector<RepastHPCDemoAgentPackage>& out){
    repast::AgentId id = agent->getId();
    RepastHPCDemoAgentPackage package(id.id(), id.startingRank(), id.agentType(), id.currentRank()); // getters agent->getC(), agent->getTotal()
    out.push_back(package); // ^ need to package Agent ID & agent getters
}
/*provide content*/
void RepastHPCDemoAgentPackageProvider::provideContent(repast::AgentRequest req, std::vector<RepastHPCDemoAgentPackage>& out){
    std::vector<repast::AgentId> ids = req.requestedAgents();
    for(size_t i = 0; i < ids.size(); i++){providePackage(agents->getAgent(ids[i]), out);}
}
/*Receive pkg*/
RepastHPCDemoAgentPackageReceiver::RepastHPCDemoAgentPackageReceiver(repast::Sharedf<RepastHPCDemoAgent>* agentPtr): agents(agentPtr){}
RepastHPCDemoAgent * RepastHPCDemoAgentPackageReceiver::createAgent(RepastHPCDemoAgentPackage package){
    repast::AgentId id(package.id, package.rank, package.type, package.currentRank);
    return new RepastHPCDemoAgent(id, package.state, package.newState);
}
/*Update Agent w/ pkg*/
void RepastHPCDemoAgentPackageReceiver::updateAgent(RepastHPCDemoAgentPackage package){
    repast::AgentId id(package.id, package.rank, package.type);
    RepastHPCDemoAgent * agent = agents->getAgent(id);
    agent->set(package.currentRank, package.state);
}
/*Connect Agent Net*/
void RepastHPCDemoModel::connectAgentNetwork(){
	repast::SharedContext<RepastHPCDemoAgent>::const_local_iterator iter    = context.localBegin();
	repast::SharedContext<RepastHPCDemoAgent>::const_local_iterator iterEnd = context.localEnd();
	while(iter != iterEnd) {
		RepastHPCDemoAgent* ego = &**iter; std::vector<RepastHPCDemoAgent*> agents;
		agents.push_back(ego); context.selectAgents(4, agents, true); // Omit self & choose 4 other agents randomly
		for(size_t i = 0; i < agents.size(); i++){ // Make an undirected connection
         	    // std::cout << "CONNECTING: " << ego->getId() << " to " << agents[i]->getId() << std::endl; // uncomment to see agents are being connected
  	  	    agentNetwork->addEdge(ego, agents[i]);	
		}
		iter++;
	}	
}
/*create model/world*/
RepastHPCDemoModel::RepastHPCDemoModel(std::string propsFile, int argc, char** argv, boost::mpi::communicator* comm): context(comm){
	props = new repast::Properties(propsFile, argc, argv, comm);
	stopAt = repast::strToInt(props->getProperty("stop.at")); countOfAgents = repast::strToInt(props->getProperty("count.of.agents")); // read 'properties' file inputs
	initializeRandom(*props, comm);
	if(repast::RepastProcess::instance()->rank() == 0) props->writeToSVFile("./output/record.csv");
	provider = new RepastHPCDemoAgentPackageProvider(&context); receiver = new RepastHPCDemoAgentPackageReceiver(&context);
	agentNetwork = new repast::SharedNetwork<RepastHPCDemoAgent, repast::RepastEdge<RepastHPCDemoAgent>,
        repast::RepastEdgeContent<RepastHPCDemoAgent>, repast::RepastEdgeContentManager<RepastHPCDemoAgent> >("agentNetwork", false, &edgeContentManager);
	context.addProjection(agentNetwork);
	// Data collection
	// Create the data set builder
	std::string fileOutputName("./output/agent_state_data.csv");
	repast::SVDataSetBuilder builder(fileOutputName.c_str(), ",", repast::RepastProcess::instance()->getScheduleRunner().schedule());
	// Create the individual data sets to be added to the builder
	// Use the builder to create the data set
	agentValues = builder.createDataSet();
}
/*
DataSource_AgentState* agentState_DataSource = new DataSource_AgentState(&context);
builder.addDataSource(createSVDataSource("State", agentState_DataSource, std::plus<int>()));
DataSource_AgentNewState* agentNewState_DataSource = new DataSource_AgentNewState(&context);
builder.addDataSource(createSVDataSource("NewState", agentNewState_DataSource, std::plus<int>()));
//getState
DataSource_AgentState::DataSource_AgentState(repast::SharedContext<RepastHPCDemoAgent>* state) : context(state){ }
int DataSource_AgentState::getData(){
	int sum = 0;
	repast::SharedContext<RepastHPCDemoAgent>::const_local_iterator iter    = context->localBegin();
	repast::SharedContext<RepastHPCDemoAgent>::const_local_iterator iterEnd = context->localEnd();
	while( iter != iterEnd) {sum+= (*iter)->getState(); iter++;}
	return sum;
}
//getNewState
DataSource_AgentNewState::DataSource_AgentNewState(repast::SharedContext<RepastHPCDemoAgent>* newState) : context(newState){ }
int DataSource_AgentNewState::getData(){
	int sum = 0;
	repast::SharedContext<RepastHPCDemoAgent>::const_local_iterator iter    = context->localBegin();
	repast::SharedContext<RepastHPCDemoAgent>::const_local_iterator iterEnd = context->localEnd();
	while( iter != iterEnd) {sum+= (*iter)->getNewState(); iter++;}
	return sum;
}
*/
/*kill model*/
RepastHPCDemoModel::~RepastHPCDemoModel(){delete props; delete provider; delete receiver; delete agentValues;}
/*init model*/
void RepastHPCDemoModel::init(){
	int rank = repast::RepastProcess::instance()->rank();
	for(int i = 0; i < countOfAgents; i++){
		repast::AgentId id(i, rank, 0);
		id.currentRank(rank);
		RepastHPCDemoAgent* agent = new RepastHPCDemoAgent(id);
		context.addAgent(agent);
	}
}
/*requestAgents*/
void RepastHPCDemoModel::requestAgents(){
	int rank = repast::RepastProcess::instance()->rank();
	int worldSize= repast::RepastProcess::instance()->worldSize();
	repast::AgentRequest req(rank);
	for(int i = 0; i < worldSize; i++){ // For each process ...
		if(i != rank){ // ... except this one
			std::vector<RepastHPCDemoAgent*> agents;        
			context.selectAgents(4, agents); // Choose 4 local agents randomly
			for(size_t j = 0; j < agents.size(); j++){
				repast::AgentId local = agents[j]->getId(); // Transform each local agent's id into a matching non-local one
				repast::AgentId other(local.id(), i, 0);
				other.currentRank(i);
				req.addRequest(other); // Add it to the agent request
			}
		}
	}
    repast::RepastProcess::instance()->requestAgents<RepastHPCDemoAgent, RepastHPCDemoAgentPackage, RepastHPCDemoAgentPackageProvider, RepastHPCDemoAgentPackageReceiver>(context, req, *provider, *receiver, *receiver);
}
/*cancelAgentRequests*/
void RepastHPCDemoModel::cancelAgentRequests(){
	int rank = repast::RepastProcess::instance()->rank();
	if(rank == 0) std::cout << "CANCELING AGENT REQUESTS" << std::endl;
	repast::AgentRequest req(rank);	
	repast::SharedContext<RepastHPCDemoAgent>::const_state_aware_iterator non_local_agents_iter  = context.begin(repast::SharedContext<RepastHPCDemoAgent>::NON_LOCAL);
	repast::SharedContext<RepastHPCDemoAgent>::const_state_aware_iterator non_local_agents_end   = context.end(repast::SharedContext<RepastHPCDemoAgent>::NON_LOCAL);
	while(non_local_agents_iter != non_local_agents_end){
		req.addCancellation((*non_local_agents_iter)->getId());
		non_local_agents_iter++;
	}
    repast::RepastProcess::instance()->requestAgents<RepastHPCDemoAgent, RepastHPCDemoAgentPackage, RepastHPCDemoAgentPackageProvider, RepastHPCDemoAgentPackageReceiver>(context, req, *provider, *receiver, *receiver);
	std::vector<repast::AgentId> cancellations = req.cancellations();
	std::vector<repast::AgentId>::iterator idToRemove = cancellations.begin();
	while(idToRemove != cancellations.end()){
		context.importedAgentRemoved(*idToRemove); idToRemove++;
	}
}
/*remove local agents*/
void RepastHPCDemoModel::removeLocalAgents(){
	int rank = repast::RepastProcess::instance()->rank();
	if(rank == 0) std::cout << "REMOVING LOCAL AGENTS" << std::endl;
	for(int i = 0; i < 4; i++){
		repast::AgentId id(i, rank, 0); repast::RepastProcess::instance()->agentRemoved(id); context.removeAgent(id);
	}
    repast::RepastProcess::instance()->synchronizeAgentStatus<RepastHPCDemoAgent, RepastHPCDemoAgentPackage, RepastHPCDemoAgentPackageProvider, RepastHPCDemoAgentPackageReceiver>(context, *provider, *receiver, *receiver);
}
/*Move agents*/
void RepastHPCDemoModel::moveAgents(){
	int rank = repast::RepastProcess::instance()->rank();
// moveAgent() notifies 'this' RepastProcess the specified agent should be moved from 'this' process to a specified process; moveAgents() does this for multiple agents & processes
	if(rank == 0){ 
		repast::AgentId agent0(0, 0, 0); repast::AgentId agent1(1, 0, 0); repast::AgentId agent2(2, 0, 0); repast::AgentId agent3(3, 0, 0);
		repast::AgentId agent4(4, 0, 0); repast::RepastProcess::instance()->moveAgent(agent0, 1); repast::RepastProcess::instance()->moveAgent(agent1, 2);
		repast::RepastProcess::instance()->moveAgent(agent2, 3); repast::RepastProcess::instance()->moveAgent(agent3, 3); repast::RepastProcess::instance()->moveAgent(agent4, 1);
	} 
    repast::RepastProcess::instance()->synchronizeAgentStatus<RepastHPCDemoAgent, RepastHPCDemoAgentPackage, RepastHPCDemoAgentPackageProvider, RepastHPCDemoAgentPackageReceiver>(context, *provider, *receiver, *receiver);
}
/*Schedule Ability*/
void RepastHPCDemoModel::doSomething(){
	int whichRank = 0;
	if(repast::RepastProcess::instance()->rank() == whichRank) std::cout << " TICK " << repast::RepastProcess::instance()->getScheduleRunner().currentTick() << std::endl;
	if(repast::RepastProcess::instance()->rank() == whichRank){
		std::cout << "LOCAL AGENTS:" << std::endl;
		for(int r = 0; r < 4; r++){
			for(int i = 0; i < 10; i++){
				repast::AgentId toDisplay(i, r, 0); RepastHPCDemoAgent* agent = context.getAgent(toDisplay);
				if((agent != 0) && (agent->getId().currentRank() == whichRank)) std::cout << agent->getId() << " " <<  std::endl;
			}
		}	
		std::cout << "NON LOCAL AGENTS:" << std::endl;
		for(int r = 0; r < 4; r++){
			for(int i = 0; i < 10; i++){
				repast::AgentId toDisplay(i, r, 0); RepastHPCDemoAgent* agent = context.getAgent(toDisplay);
				if((agent != 0) && (agent->getId().currentRank() != whichRank)) std::cout << agent->getId() << " " << std::endl;
			}
		}
	}
	std::vector<RepastHPCDemoAgent*> agents; context.selectAgents(repast::SharedContext<RepastHPCDemoAgent>::LOCAL, countOfAgents, agents);
	std::vector<RepastHPCDemoAgent*>::iterator it = agents.begin();
	while(it != agents.end()){
		(*it)->play(&context); it++; //&context agentNetwork
    }
	repast::RepastProcess::instance()->synchronizeAgentStates<RepastHPCDemoAgentPackage, RepastHPCDemoAgentPackageProvider, RepastHPCDemoAgentPackageReceiver>(*provider, *receiver);
}
/*init schedule*/
void RepastHPCDemoModel::initSchedule(repast::ScheduleRunner& runner){
	runner.scheduleEvent(1, repast::Schedule::FunctorPtr(new repast::MethodFunctor<RepastHPCDemoModel> (this, &RepastHPCDemoModel::requestAgents)));
	runner.scheduleEvent(1.1, repast::Schedule::FunctorPtr(new repast::MethodFunctor<RepastHPCDemoModel> (this, &RepastHPCDemoModel::connectAgentNetwork)));
	runner.scheduleEvent(2, 1, repast::Schedule::FunctorPtr(new repast::MethodFunctor<RepastHPCDemoModel> (this, &RepastHPCDemoModel::doSomething)));
	runner.scheduleEvent(3, repast::Schedule::FunctorPtr(new repast::MethodFunctor<RepastHPCDemoModel> (this, &RepastHPCDemoModel::moveAgents)));
	runner.scheduleEndEvent(repast::Schedule::FunctorPtr(new repast::MethodFunctor<RepastHPCDemoModel> (this, &RepastHPCDemoModel::recordResults)));
	runner.scheduleStop(stopAt);
	// Data collection
	runner.scheduleEvent(1.5, 5, repast::Schedule::FunctorPtr(new repast::MethodFunctor<repast::DataSet>(agentValues, &repast::DataSet::record)));
	runner.scheduleEvent(10.6, 10, repast::Schedule::FunctorPtr(new repast::MethodFunctor<repast::DataSet>(agentValues, &repast::DataSet::write)));
	runner.scheduleEndEvent(repast::Schedule::FunctorPtr(new repast::MethodFunctor<repast::DataSet>(agentValues, &repast::DataSet::write)));
}
/*record results*/
void RepastHPCDemoModel::recordResults(){
	if(repast::RepastProcess::instance()->rank() == 0){
		props->putProperty("Result","Passed");
		std::vector<std::string> keyOrder; keyOrder.push_back("RunNumber"); keyOrder.push_back("stop.at"); keyOrder.push_back("Result");
		props->writeToSVFile("./output/results.csv", keyOrder);
    }
}