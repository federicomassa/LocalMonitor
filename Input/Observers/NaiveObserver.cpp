#include "NaiveObserver.h"
#include "Utility/LogFunctions.h"
#include "Automation/DynamicModel.h"
#include "Automation/ControlModel.h"
#include "Dynamics/DynamicModels.h"
#include "Dynamics/StateConversions.h"
#include "Automation/Automatons/Automatons.h"
#include "Automation/Controllers/Controllers.h"
#include "Automation/Sensors/Sensors.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <experimental/filesystem>

using namespace std;
using namespace LogFunctions;
using namespace nlohmann;

// TODO: there is no check that automaton's transitions are among maneuvers in control model 
NaiveObserver::NaiveObserver(const std::string& name) : Observer(name), logger(output)
{
	simulTimeSteps = -1;
	predictionTimeSpan = -1;
	lastPredictionStartTime = -1;
	lastPredictedTime = -1;
	lastUpdateTime = -1;
	now = -1;
}

NaiveObserver::~NaiveObserver()
{
}

void NaiveObserver::Run(const double& currentTime)
{
	Require(currentTime >= 0, "NaiveObserver::Run", "currentTime must be > 0");
	now = currentTime;

	if (!std::experimental::filesystem::exists(outputDir)) {
	    std::experimental::filesystem::create_directories(outputDir);
	  }
	
	// ====== Setup output file
	if (output.is_open())
		output.close();
	
	string outputPath = outputDir + "log" + ToStringWithPrecision(lastPredictionStartTime, 6) + ".txt";
	output.open(outputPath.c_str());
	
	Require(output.is_open(), "NaiveObserver::Run", string("Error opening file \'") + outputPath + "\'");
	// =========================

	
	// Explicit const cast
	const TimedContainer<Agent>& selfTraj = selfTrajectory;
	const TimedContainer<AgentVector>& othersTraj = othersTrajectory;
	const TimedContainer<EnvironmentParameters>& envTraj = environmentTrajectory;
	
	const double& mostRecentDataTime = selfTraj.begin().time();
	
	if (mostRecentDataTime >= (lastPredictedTime) && lastPredictedTime >= 0 && lastUpdateTime < lastPredictedTime)
	{		
		
		Require(selfTraj.size() == 3 && othersTraj.size() == 3 && envTraj.size() == 3,
				"NaiveObserver::Run", string("Need 3 points, one for the beginning of prediction and two close to the end of prediction to interpolate, instead there is only ") + ToStringWithPrecision(selfTraj.size(), 1) + "! Maybe sensor frequency is too low compared to prediction span?");
		
		Agent interpolatedSelf = InterpolateSelf(++(selfTraj.begin()), selfTraj.begin());
		AgentVector interpolatedOthers = InterpolateOthers(++othersTraj.begin(), othersTraj.begin());
		EnvironmentParameters interpolatedEnv = InterpolateEnvironment(++envTraj.begin(), envTraj.begin());
		
		lastUpdateTime = mostRecentDataTime;
		UpdatePhase(interpolatedSelf, interpolatedOthers, interpolatedEnv);
	}
	if (lastUpdateTime >= lastPredictedTime)
	{		
		lastPredictionStartTime = mostRecentDataTime;
		lastPredictedTime = mostRecentDataTime + predictionTimeSpan;
		PredictPhase();
	}
	
}

void NaiveObserver::PreConfigure(const nlohmann::json& j)
{
	// ========= CHECK MANDATORY ENTRIES ============
	vector<string> mandatoryEntries;
	mandatoryEntries.push_back("world_agent_features");
	mandatoryEntries.push_back("output_dir");

	
	for (auto itr = mandatoryEntries.begin();  itr != mandatoryEntries.end(); itr++)
	{
		try
		{
			j.at(*itr);
		}
		catch(out_of_range&)
		{
			Error("NaiveObserver::PreConfigure", string("Mandatory entry ") + *itr + " not found in observer configuration file"); 
		}
	}
	// =============================================
	
	json worldAgentJ = j.at("world_agent_features");
	
	if (!worldAgentJ.is_array())
		Error("NaiveObserver::ReadDynamicModel", "\'world_agent_features\' should be an array");
	
	for (auto itr = worldAgentJ.begin(); itr != worldAgentJ.end(); itr++)
		worldAgentVars.push_back(itr->get<string>());
	
	outputDir = j.at("output_dir").get<string>();
}


void NaiveObserver::Configure(const nlohmann::json& observingJson)
{
	
	// ========= CHECK MANDATORY ENTRIES ============
	vector<string> mandatoryEntries;
	mandatoryEntries.push_back("id");
	mandatoryEntries.push_back("type");
	mandatoryEntries.push_back("dynamic_model");
	mandatoryEntries.push_back("control_model");
	mandatoryEntries.push_back("simulation_steps");
	mandatoryEntries.push_back("prediction_time_span");
	mandatoryEntries.push_back("hidden_range");
	mandatoryEntries.push_back("hidden_resolution");
	mandatoryEntries.push_back("hidden_parameters");
	mandatoryEntries.push_back("tolerances");

	if (!observingJson.at("id").is_string())
		Error("NaiveObserver::Configure", "\'id\' entry should be a string");
			
	for (auto itr = mandatoryEntries.begin();  itr != mandatoryEntries.end(); itr++)
	{
		try
		{
			observingJson.at(*itr);
		}
		catch(out_of_range&)
		{
			Error("NaiveObserver::Configure", string("Mandatory entry ") + *itr + " not found in observer configuration file, inside observing entry of agent " + GetObservedID()); 
		}
	}
	// =============================================
	
	nlohmann::json dynJson = observingJson.at("dynamic_model");
	if (!dynJson.is_array())
		Error("NaiveObserver::Configure", "\'dynamic_model\' entry must be a JSON array");
	
	for (auto itr = dynJson.begin(); itr != dynJson.end(); itr++)
	{
		if (!itr->is_object())
			Error("NaiveObserver::Configure", "Elements of \'dynamic_model\' must be JSON objects");
		
		ReadDynamicModel(*itr);
	}
	
	// Ensure that a default dynamic model has been defined
	try
	{
		pLayer("__default__");
	}
	catch(out_of_range&)
	{
		Error("NaiveObserver::Configure", "You must specified a dynamic model for ID \'__default__\'");
	}
	

	nlohmann::json controlJson = observingJson.at("control_model");
	if (!controlJson.is_array())
		Error("NaiveObserver::Configure", "\'control_model\' entry must be a JSON array");
	
	for (auto itr = controlJson.begin(); itr != controlJson.end(); itr++)
	{
		if (!itr->is_object())
			Error("NaiveObserver::Configure", "Elements of \'control_model\' must be JSON objects");
		
		ReadControlModel(*itr);
	}

	try
	{
		controlModels("__default__");
	}
	catch(out_of_range&)
	{
		Error("NaiveObserver::Configure", "You must specify a control model for ID \'__default__\'");
	}

	if (!observingJson.at("simulation_steps").is_number_integer())
		Error("NaiveObserver::Configure", "\'simulation_steps\' must be an integer");
		
	simulTimeSteps = observingJson.at("simulation_steps").get<int>();
	Require(simulTimeSteps > 0, "NaiveObserver::Configure", "\'simulation_steps\' must be > 0");
	
	if (!observingJson.at("prediction_time_span").is_number())
		Error("NaiveObserver::Configure", "\'prediction_time_span\' must be a number");
	
	predictionTimeSpan = observingJson.at("prediction_time_span").get<double>();
	
	for (auto itr = pLayer.begin(); itr != pLayer.end(); itr++)
		itr->second.SetSimulationTimeStep(predictionTimeSpan/double(simulTimeSteps));
	
	nlohmann::json rangeJson = observingJson.at("hidden_range");
	if (!rangeJson.is_object())
		Error("NaiveObserver::Configure", "\'hidden_range\' entry must be a json object");
	
	ReadRange(rangeJson);
	
	
	nlohmann::json resJson = observingJson.at("hidden_resolution");
	if (!rangeJson.is_object())
		Error("NaiveObserver::Configure", "\'hidden_resolution\' entry must be a json object");
	
	ReadResolution(resJson);
	
	nlohmann::json parJson = observingJson.at("hidden_parameters");
	if (!parJson.is_object())
		Error("NaiveObserver::Configure", "\'hidden_parameters\' entry must be a json object");
	
	ReadParameters(parJson);
	
	
	nlohmann::json tolJson = observingJson.at("tolerances");
	if (!tolJson.is_object())
		Error("NaiveObserver::Configure", "\'tolerances\' entry must be a json object");
	
	ReadTolerances(tolJson);
	
	// EXTERNAL SENSORS
	try
	{
		nlohmann::json extSensJson = observingJson.at("external_sensors");
		if (!extSensJson.is_array())
			Error("NaiveObserver::Configure", "\'external_sensors\' entry must be a json array of strings");
	
		if (extSensJson.size() == 0)
			throw out_of_range("\'external_sensors\'");
		
		for (auto itr = extSensJson.begin(); itr != extSensJson.end(); itr++)
			extSensors.push_back(shared_ptr<ExternalSensor>(InstantiateExternalSensor(itr->get<string>())));
	}
	catch(out_of_range&)
	{
		Info("NaiveObserver::Configure", string("No external sensors defined in \'observing\' agent with ID \'") + observingJson.at("id").get<string>() + "\'."); 
	}
	

	// INTERNAL SENSORS
	try
	{
		nlohmann::json intSensJson = observingJson.at("internal_sensors");
		if (!intSensJson.is_array())
			Error("NaiveObserver::Configure", "\'external_sensors\' entry must be a json array of strings");
	
		if (intSensJson.size() == 0)
			throw out_of_range("\'internal_sensors\'");
		
		for (auto itr = intSensJson.begin(); itr != intSensJson.end(); itr++)
			intSensors.push_back(shared_ptr<InternalSensor>(InstantiateInternalSensor(itr->get<string>())));
	}
	catch(out_of_range&)
	{
		Info("NaiveObserver::Configure", string("No internal sensors defined in \'observing\' agent with ID \'") + observingJson.at("id").get<string>() + "\'."); 
	}

	
	
	// FIXME Someday this will be computed for each run because it might depend on visibility
	for (auto itr = pLayer(GetObservedID()).GetDynamicModel().GetStateVariables().begin(); 
		 itr != pLayer(GetObservedID()).GetDynamicModel().GetStateVariables().end(); itr++)
		 nPredictions[*itr] = floor((varRange[*itr].second - varRange[*itr].first)/varResolution[*itr] + 1);

	InitializeHiddenState();
}

void NaiveObserver::ReadDynamicModel(const nlohmann::json& dynJ)
{	
	// ========= CHECK MANDATORY ENTRIES ============	
	vector<string> mandatoryEntries;
	mandatoryEntries.push_back("id");
	mandatoryEntries.push_back("name");
	mandatoryEntries.push_back("state_variables");
	mandatoryEntries.push_back("dynamics");
	mandatoryEntries.push_back("control_variables");
	mandatoryEntries.push_back("world_conversion_function");
	mandatoryEntries.push_back("state_estimation_function");
	
	for (auto itr = mandatoryEntries.begin();  itr != mandatoryEntries.end(); itr++)
	{
		try
		{
			dynJ.at(*itr);
		}
		catch(out_of_range&)
		{
			Error("NaiveObserver::ReadDynamicModel", string("Mandatory entry ") + *itr + " not found in observer configuration file, inside observing entry of agent " + GetObservedID() + ", in dynamic_model"); 
		}
	}
	// =============================================

	DynamicModel dynModel;
	
	dynModel.SetName(dynJ.at("name").get<string>());
	
	vector<string> stateVars;
	json stateVarsJ = dynJ.at("state_variables");
	
	if (!stateVarsJ.is_array())
		Error("NaiveObserver::ReadDynamicModel", "state_variables should be an array");
	
	for (auto itr = stateVarsJ.begin(); itr != stateVarsJ.end(); itr++)
		stateVars.push_back(itr->get<string>());
		
	dynModel.SetStateVariables(stateVars);
	
	dynModel.SetDynamicsFunctionName(dynJ.at("dynamics").get<string>());	dynModel.SetDynamicsFunction(GetDynamicsFunction(dynJ.at("dynamics").get<string>()));
	
	vector<string> controlVars;
	json controlVarsJ = dynJ.at("control_variables");
	
	if (!controlVarsJ.is_array())
		Error("NaiveObserver::ReadDynamicModel", "control_variables should be an array");
	
	for (auto itr = controlVarsJ.begin(); itr != controlVarsJ.end(); itr++)
		controlVars.push_back(itr->get<string>());
	
	dynModel.SetControlVariables(controlVars);
	dynModel.SetStateConversionToWorldName(dynJ.at("world_conversion_function").get<string>());	dynModel.SetStateConversionToWorld(GetStateConversionToWorld(dynJ.at("world_conversion_function").get<string>()));
	dynModel.SetStateConversionToStateName(dynJ.at("state_estimation_function").get<string>());	dynModel.SetStateConversionToState(GetStateConversionToState(dynJ.at("state_estimation_function").get<string>()));
	
	
	
	pLayer[dynJ.at("id").get<string>()].SetDynamicModel(dynModel);
}

void NaiveObserver::ReadControlModel(const nlohmann::json& controlJ)
{	
	// ========= CHECK MANDATORY ENTRIES ============
	vector<string> mandatoryEntries;
	mandatoryEntries.push_back("id");
	mandatoryEntries.push_back("name");
	mandatoryEntries.push_back("maneuvers");
	mandatoryEntries.push_back("controller");
	mandatoryEntries.push_back("control_variables");
	
	try
	{
		if (controlJ.at("id").get<string>() == GetObservedID())
			mandatoryEntries.push_back("automaton");
	}
	catch (out_of_range&)
	{
		Error("NaiveObserver::ReadControlModel", string("Mandatory entry \'id\' not found in observer configuration file, inside observing entry of agent ") + GetObservedID() + ", in \'control_model\'");
	}
	
	for (auto itr = mandatoryEntries.begin();  itr != mandatoryEntries.end(); itr++)
	{
		try
		{
			controlJ.at(*itr);
		}
		catch(out_of_range&)
		{
			Error("NaiveObserver::ReadControlModel", string("Mandatory entry \'") + *itr + "\' not found in observer configuration file, inside observing entry of agent " + GetObservedID() + ", in \'control_model\'"); 
		}
	}
	// =============================================
	string ID = controlJ.at("id").get<string>();
	
	controlModels[ID].SetName(controlJ.at("name").get<string>());
	
	controlModels[ID].SetController(controlJ.at("controller").get<string>());
	
	if (GetObservedID() == controlJ.at("id").get<string>())
		controlModels[ID].SetAutomaton(controlJ.at("automaton").get<string>());
	
	vector<string> controlVars;
	json controlVarsJ = controlJ.at("control_variables");
	
	if (!controlVarsJ.is_array())
		Error("NaiveObserver::ReadControlModel", "control_variables should be an array");
	
	for (auto itr = controlVarsJ.begin(); itr != controlVarsJ.end(); itr++)
		controlVars.push_back(itr->get<string>());
	
	controlModels[ID].SetControlVariables(controlVars);
	Require(pLayer.at(controlJ.at("id").get<string>()).GetDynamicModel().GetControlVariables() == controlVars, "NaiveObserver::ReadControlModel", "control_model and dynamic_model must have same control variables");
	

	vector<Maneuver> possibleMan;
	json possibleManJ = controlJ.at("maneuvers");
	
	if (!possibleManJ.is_array())
		Error("NaiveObserver::ReadControlModel", "control_variables should be an array");
	
	for (auto itr = possibleManJ.begin(); itr != possibleManJ.end(); itr++)
		possibleMan.push_back(Maneuver(itr->get<string>()));
	
	controlModels[ID].SetManeuvers(possibleMan);
	
	// When defining models of other agents (not the observed one, you 
	// must specified a single maneuver that remains the same (the automaton is not actually run). This is a simplified model 
	if (controlJ.at("id").get<string>() != GetObservedID())
	{
		Require(possibleMan.size() == 1, "NaiveObserver::ReadControlModel", "Agents different from the observed one must have a simple control model with ONE maneuver");
	}	
	
}

void NaiveObserver::ReadRange(const nlohmann::json& rangeJ)
{
	// ========= CHECK MANDATORY ENTRIES ============
	
	// There must be a range specified for each state variable of the model
	vector<string> mandatoryEntries;
	for (auto itr = pLayer(GetObservedID()).GetDynamicModel().GetStateVariables().begin();
		 itr != pLayer(GetObservedID()).GetDynamicModel().GetStateVariables().end(); itr++)
		 mandatoryEntries.push_back(*itr);
	
	for (auto itr = mandatoryEntries.begin();  itr != mandatoryEntries.end(); itr++)
	{
		try
		{
			nlohmann::json varJ = rangeJ.at(*itr);
			if (!varJ.is_array() || varJ.size() != 2)
				Error("NaiveObserver::ReadRange", "In \'range\' entry, for each state variable there must be an array of two elements, min and max value");
			
			if (varJ.at(0).get<double>() > varJ.at(1).get<double>())
				Error("NaiveObserver::ReadRange", "In \'range\' entry there must be a min and max value, the first being smaller than the second.");
			
			varRange[*itr] = std::make_pair(varJ.at(0).get<double>(), varJ.at(1).get<double>());
			
		}
		catch(out_of_range&)
		{
			Error("NaiveObserver::ReadControlModel", string("Mandatory entry \'") + *itr + "\' not found in observer configuration file, inside observing entry of agent " + GetObservedID() + ", in \'range\' entry"); 
		}
	}
	// =============================================
}


void NaiveObserver::ReadResolution(const nlohmann::json& resJ)
{
	// ========= CHECK MANDATORY ENTRIES ============
	
	// There must be a range specified for each state variable of the model
	vector<string> mandatoryEntries;
	for (auto itr = pLayer(GetObservedID()).GetDynamicModel().GetStateVariables().begin();
		 itr != pLayer(GetObservedID()).GetDynamicModel().GetStateVariables().end(); itr++)
		 mandatoryEntries.push_back(*itr);
	
	for (auto itr = mandatoryEntries.begin();  itr != mandatoryEntries.end(); itr++)
	{
		try
		{
			nlohmann::json varJ = resJ.at(*itr);
			if (!varJ.is_number())
				Error("NaiveObserver::ReadRange", "In \'resolution\' entry, for each state variable there must be a float that indicates the resolution of the hidden agent mapping");
			
			varResolution[*itr] = varJ.get<double>();
			
		}
		catch(out_of_range&)
		{
			Error("NaiveObserver::ReadControlModel", string("Mandatory entry \'") + *itr + "\' not found in observer configuration file, inside observing entry of agent " + GetObservedID() + ", in \'resolution\' entry"); 
		}
	}
	// =============================================
}


void NaiveObserver::ReadParameters(const nlohmann::json& parJ)
{
	for (auto itr = parJ.begin(); itr != parJ.end(); itr++)
	{
		Require(itr.value().is_number(), "NaiveObserver::ReadParameters", "\'parameters\' entry must contain parameters");
		parameters[itr.key()] = itr.value().get<double>();
	}
}

void NaiveObserver::ReadTolerances(const nlohmann::json& tolJ)
{
	for (auto itr = tolJ.begin(); itr != tolJ.end(); itr++)
	{
		Require(itr.value().is_number(), "NaiveObserver::ReadParameters", "\'tolerances\' entry must contain numbers");
		tolerances[itr.key()] = itr.value().get<double>();
	}
}

// TODO When deploying this code, remember that this method is not thread safe (if run and receivesensoroutput are run in different threads, it might lead to data races)
void NaiveObserver::ReceiveSensorOutput(const SensorOutput& sensorOutput, const double& currentTime)
{
	// !!! Sensor data has to be reorganized so that 'self' is observed agent and 'others' are its neighbors. NB Trajectories are stored in world coordinates, but for the prediction they have to be converted to state variables, which is the state estimation phase !!!
		
	const Agent& currentSelf = sensorOutput.RetrieveSelfData();
	const AgentVector& currentOthers = sensorOutput.RetrieveOtherAgentsData();
	const EnvironmentParameters& currentEnv = sensorOutput.RetrieveEnvironmentData();
	
	// if last prediction time is smaller than update time it means that it has updated but not started a new prediction
	if (lastPredictionStartTime < lastUpdateTime)
	{
		selfTrajectory.clear();
		othersTrajectory.clear();
		environmentTrajectory.clear();
	}
	
	
	// These are the 'truth' values of the trajectories that the observed agent sees. They will be filtered by observed agent's sensors.
	Agent trueSelf;
	AgentVector trueOthers;
	const EnvironmentParameters& trueEnv = currentEnv;
	
	// Look for observed agent in 'others', that will be the true self
	for (auto itr = currentOthers.begin(); itr != currentOthers.end(); itr++)
	{
		if (itr->second.GetID() == GetObservedID())
			trueSelf = itr->second;
		else
			trueOthers[itr->first] = itr->second;
	}
	
	// Add observer to the neighborhood of the observed
	trueOthers[currentSelf.GetID()] = currentSelf;
	
	// Keep two more measurements (other than the one at prediction time) to
	// interpolate during update phase
	if (selfTrajectory.size() >= 3)
	{
		// Erase oldest measure (not at prediction time)
		selfTrajectory.erase(++selfTrajectory.begin());
		othersTrajectory.erase(++othersTrajectory.begin());
		environmentTrajectory.erase(++environmentTrajectory.begin());
	}
	
	// Now that we have the truth values, we have to insert them. They will be filtered by the observer in the NaiveEnvironment object during predict phase
	selfTrajectory.insert(currentTime, trueSelf);
	
	othersTrajectory.insert(currentTime, trueOthers);
	
	environmentTrajectory.insert(currentTime, trueEnv);
}

void NaiveObserver::InitializeHiddenState()
{
	int totPredictions = 1;
	for (auto itr = nPredictions.begin(); itr != nPredictions.end(); itr++)
		totPredictions *= itr->second;

	for (int i = 0; i < totPredictions; i++)
		hiddenInitState.push_back(GenerateHiddenState(i));
	
}

State NaiveObserver::GenerateHiddenState(const int& i)
{
	// Convert to base nPredictions
	IMap<int> iNewBase;
	
	int quotient = i;
	for (auto itr = nPredictions.rbegin(); itr != nPredictions.rend(); itr++)
	{
		iNewBase.AddEntry(itr->first, quotient % itr->second);
		quotient = floor(double(quotient)/double(itr->second));
	}
	
	// Generate state
	State generatedQ;
	
	// Use default model for hidden agent
	for (auto itr = pLayer("__default__").GetDynamicModel().GetStateVariables().begin();
		 itr != pLayer("__default__").GetDynamicModel().GetStateVariables().end(); itr++)
		 {
			generatedQ.AddStateVariable(*itr);
			generatedQ(*itr) = varRange(*itr).first + varResolution(*itr)*iNewBase(*itr);
		 }
		
	return generatedQ;

}

void NaiveObserver::PredictPhase()
{
	// FIXME for now, every time it resets
	environments.clear();
	
	Agent initSelf = selfTrajectory.begin().value();
	AgentVector initOthers = othersTrajectory.begin().value();
	EnvironmentParameters initEnv = environmentTrajectory.begin().value();
	
	// Build first hypothesis of no hidden agents
	const std::vector<Maneuver>& possibleManeuvers = controlModels(GetObservedID()).GetManeuvers();
	
	for (auto itr = possibleManeuvers.begin(); itr != possibleManeuvers.end();
		 itr++)
	environments.push_back(NaiveEnvironment(this, initSelf, *itr, initOthers, initEnv, extSensors, intSensors, false));
	
	for (auto itr = hiddenInitState.begin(); itr != hiddenInitState.end(); itr++)
	{
		AgentVector totInitOthers = initOthers;
		
		// Build hidden agent
		Agent hidden;
		hidden.SetID("__hidden__");
		hidden.SetState(*itr);
		hidden.SetParameters(parameters);

		
		Agent hiddenWorld;
		hiddenWorld.SetID(hidden.GetID());
		// Convert hidden to world variables using your max model of knowledge (all vars that you measure of the observed agent)
		hiddenWorld.SetState(pLayer("__default__").GetDynamicModel().GetWorldState(hidden, initSelf.GetState()));
		
		// Add hidden agent to the others
		totInitOthers["__hidden__"] = hiddenWorld;
		
		for (auto itr = possibleManeuvers.begin(); itr != possibleManeuvers.end();
		 itr++)
		environments.push_back(NaiveEnvironment(this, initSelf, *itr, totInitOthers, initEnv, extSensors, intSensors));
	}
	
	for (auto itr = environments.begin(); itr != environments.end(); itr++)
		itr->Predict(predictionTimeSpan);
}

void NaiveObserver::UpdatePhase(const Agent& newSelf, const AgentVector& newOthers, const EnvironmentParameters& newEnv)
{
	
	bool erasedFirstElement = false;
	for (auto itr = environments.begin(); itr != environments.end(); itr++)
	{		
		// Trick to erase iterator inside its own for loop
		if (erasedFirstElement)
		{
			itr--;
			environments.erase(itr);
			erasedFirstElement = false;
		}
		
		// Compare prediction with real data
		State diff = itr->GetSelf().GetState() - newSelf.GetState();
		
		if (!itr->hasHiddenAgent)
			logger << "DIFF! " << itr->GetSelf().GetState() << newSelf.GetState() << diff << logger.EndL();
		
		bool needErase = false;
		
		for (auto stateVar = diff.begin(); stateVar != diff.end(); stateVar++)
		{
			logger << fabs(stateVar->second) << '\t' << tolerances(stateVar->first) << logger.EndL();
			// If difference is larger than tolerance, erase hypothesis
			if (fabs(stateVar->second) >= tolerances(stateVar->first))
			{
				needErase = true;
				break;
			}	
		}
		
		if (needErase)
		{
			logger << "erasing" << logger.EndL();
			
			if (!(itr == environments.begin()) && environments.size() != 0)
			{
				environments.erase(itr);
				itr--;
			}
			else if (environments.size() == 1)
			{
				environments.clear();
				break;
			}
			else
				erasedFirstElement = true;
		}
		
		
		
	}
	
	// ========= LOGGING =============
	logger << "Update at time " << now << "..." << "Agent " << GetObserverID() << " observing agent " << GetObservedID() << logger.EndL() << logger.EndL();
	
	logger << "Measured (interpolated) value: " << newSelf.GetState() << logger.EndL();
	logger << "Predictions..." << logger.EndL();
	
	for (auto itr = environments.begin(); itr != environments.end(); itr++)
	{
		logger << itr->GetManeuver() << '\t' << itr->GetSelf().GetState() << logger.EndL();
	}
	
}

Agent NaiveObserver::InterpolateSelf(const TimedContainer<Agent>::const_iterator& p1, const TimedContainer<Agent>::const_iterator& p2)
{
	Agent interpolated;
	interpolated.SetID(p1.value().GetID());
	AgentParameters pars = p1.value().GetParameters();
	interpolated.SetParameters(pars);
	
	const State& oldState = p1.value().GetState();
	const State& newState = p2.value().GetState();
	
	double deltaTime = p2.time() - p1.time();
		
	Require(deltaTime > 0, "NaiveObserver::InterpolateSelf", "Delta time between sensor data must be > 0");
	
	interpolated.SetState(oldState + (newState - oldState)/deltaTime*(lastPredictedTime - p1.time()));
	
	return interpolated;
}

AgentVector NaiveObserver::InterpolateOthers(TimedContainer<AgentVector>::const_iterator p1, TimedContainer<AgentVector>::const_iterator p2)
{
	AgentVector interpolatedAgents;
	
	// FIXME It could happen that sensors do not see same things at subsequent instants. How to deal with it?
	Require(p1.value().size() == p2.value().size(), "NaiveObserver::InterpolateOthers", "FIXME!!!");
	
	try
	{
		for (auto itr = p1.value().begin(); itr != p1.value().end(); itr++)
		{
			Agent interpolated;	
	
			const Agent& oldAgent = itr->second;
			const Agent& newAgent = p2.value().at(itr->first);
		
			interpolated.SetID(itr->first);
			interpolated.SetParameters(itr->second.GetParameters());
	
			const State& oldState = oldAgent.GetState();
			const State& newState = newAgent.GetState();
	
			double deltaTime = p2.time() - p1.time();
			Require(deltaTime > 0, "NaiveObserver::InterpolateOthers", "Delta time between sensor data must be > 0");
	
			interpolated.SetState(oldState + (newState - oldState)/deltaTime*(lastPredictedTime - p1.time()));
	
			interpolatedAgents[itr->first] = interpolated;
		}
	}
	catch(out_of_range&)
	{
		Error("NaiveObserver::InterpolateOthers", "Probably happend because of new/disappeared agents, see FIXME");
	}
	
	return interpolatedAgents;
}

EnvironmentParameters NaiveObserver::InterpolateEnvironment(TimedContainer<EnvironmentParameters>::const_iterator p1, TimedContainer<EnvironmentParameters>::const_iterator p2)
{
	//FIXME for now do not do anything here
	return p1.value();
}



