/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Stop.cpp
 * Author: rlcancian
 * 
 * Created on 11 de Setembro de 2019, 13:15
 */

#include "Stop.h"

#include "../../kernel/simulator/Model.h"

#ifdef PLUGINCONNECT_DYNAMIC 

extern "C" StaticGetPluginInformation GetPluginInformation() {
	return &Stop::GetPluginInformation;
}
#endif

ModelDataDefinition* Stop::NewInstance(Model* model, std::string name) {
	return new Stop(model, name);
}

Stop::Stop(Model* model, std::string name) : ModelComponent(model, Util::TypeOf<Stop>(), name) {
}

std::string Stop::show() {
	return ModelComponent::show() + "";
}

ModelComponent* Stop::LoadInstance(Model* model, std::map<std::string, std::string>* fields) {
	Stop* newComponent = new Stop(model);
	try {
		newComponent->_loadInstance(fields);
	} catch (const std::exception& e) {

	}
	return newComponent;
}

void Stop::_onDispatchEvent(Entity* entity) {
	_parentModel->getTracer()->trace("I'm just a dummy model and I'll just send the entity forward");
	this->_parentModel->sendEntityToComponent(entity, this->getConnections()->getFrontConnection());
}

bool Stop::_loadInstance(std::map<std::string, std::string>* fields) {
	bool res = ModelComponent::_loadInstance(fields);
	if (res) {
		// @TODO: not implemented yet
	}
	return res;
}

//void Stop::_initBetweenReplications() {}

std::map<std::string, std::string>* Stop::_saveInstance(bool saveDefaultValues) {
	std::map<std::string, std::string>* fields = ModelComponent::_saveInstance(saveDefaultValues);
	// @TODO: not implemented yet
	return fields;
}

bool Stop::_check(std::string* errorMessage) {
	bool resultAll = true;
	// @TODO: not implemented yet
	*errorMessage += "";
	return resultAll;
}

PluginInformation* Stop::GetPluginInformation() {
	PluginInformation* info = new PluginInformation(Util::TypeOf<Stop>(), &Stop::LoadInstance, &Stop::NewInstance);
	info->setCategory("Material Handling");
	// ...
	return info;
}


