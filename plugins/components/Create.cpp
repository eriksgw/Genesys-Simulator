/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   Create.cpp
 * Author: rafael.luiz.cancian
 *
 * Created on 21 de Junho de 2018, 20:12
 */

#include "Create.h"
#include "../../kernel/simulator/Model.h"
#include "../../kernel/simulator/EntityType.h"
#include "../../kernel/simulator/ModelDataManager.h"
#include "../../kernel/simulator/Attribute.h"
#include "Assign.h"

#ifdef PLUGINCONNECT_DYNAMIC

extern "C" StaticGetPluginInformation GetPluginInformation() {
	return &Create::GetPluginInformation;
}
#endif

ModelDataDefinition* Create::NewInstance(Model* model, std::string name) {
	return new Create(model, name);
}

Create::Create(Model* model, std::string name) : SourceModelComponent(model, Util::TypeOf<Create>(), name) {
	PropertyT<unsigned int>* prop1= new PropertyT<unsigned int>(Util::TypeOf<Create>(), "Entities Per Creation",
			DefineGetter<Create,unsigned int>(this, &Create::getEntitiesPerCreation),
			DefineSetter<Create,unsigned int>(this, &Create::setEntitiesPerCreation));
	model->getControls()->insert(prop1);
	_addProperty(prop1);
	PropertyT<std::string>* prop2 = new PropertyT<std::string>(Util::TypeOf<Create>(), "Time Between Creations",
			DefineGetter<Create,std::string>(this, &Create::getTimeBetweenCreationsExpression),
			DefineSetter<Create,std::string>(this, &Create::setTimeBetweenCreationsExpression));
	model->getControls()->insert(prop2);
	_addProperty(prop2);
	PropertyT<Util::TimeUnit>* prop3 = new PropertyT<Util::TimeUnit>(Util::TypeOf<Create>(), "Time Unit",
			DefineGetter<Create,Util::TimeUnit>(this, &Create::getTimeUnit),
			DefineSetter<Create,Util::TimeUnit>(this, &Create::setTimeUnit));
	model->getControls()->insert(prop3);
	_addProperty(prop3);
}

std::string Create::show() {
	return SourceModelComponent::show();
}

void Create::_onDispatchEvent(Entity* entity, unsigned int inputNumber) {
	_parentModel->getDataManager()->insert(entity->getClassname(), entity);
	double tnow = _parentModel->getSimulation()->getSimulatedTime();
	entity->setAttributeValue("Entity.ArrivalTime", tnow); // ->find("Entity.ArrivalTime")->second->setValue(tnow);
	//entity->setAttributeValue("Entity.Picture", 1); // ->find("Entity.ArrivalTime")->second->setValue(tnow);
	double timeBetweenCreations, timeScale, newArrivalTime;
	unsigned int _maxCreations = _parentModel->parseExpression(this->_maxCreationsExpression);
	if (tnow != _lastArrival) {
		_lastArrival = tnow;
		timeBetweenCreations = _parentModel->parseExpression(this->_timeBetweenCreationsExpression);
		timeScale = Util::TimeUnitConvert(this->_timeBetweenCreationsTimeUnit, _parentModel->getSimulation()->getReplicationBaseTimeUnit());
		newArrivalTime = tnow + timeBetweenCreations*timeScale;
		for (unsigned int i = 0; i<this->_entitiesPerCreation; i++) {
			if (_entitiesCreatedSoFar < _maxCreations) {
				_entitiesCreatedSoFar++;
				Entity* newEntity = _parentModel->createEntity(entity->getEntityType()->getName() + "_%", false);
				newEntity->setEntityType(entity->getEntityType());
				Event* newEvent = new Event(newArrivalTime, newEntity, this);
				_parentModel->getFutureEvents()->insert(newEvent);
				_parentModel->getTracer()->traceSimulation(this, "Arrival of "/*entity " + std::to_string(newEntity->entityNumber())*/ + newEntity->getName() + " schedule for time " + std::to_string(newArrivalTime) + Util::StrTimeUnitShort(_parentModel->getSimulation()->getReplicationBaseTimeUnit()));
			}
		}
	}
	if (_reportStatistics)
		_numberOut->incCountValue();
	_parentModel->sendEntityToComponent(entity, this->getConnections()->getFrontConnection());
}

PluginInformation* Create::GetPluginInformation() {
	PluginInformation* info = new PluginInformation(Util::TypeOf<Create>(), &Create::LoadInstance, &Create::NewInstance);
	info->setSource(true);
	info->setMaximumInputs(0);
	info->setMinimumInputs(0);
	std::string text = "This module is intended as the starting point for entities in a simulation model.";
	text += "	Entities are created using a schedule or based on a time between arrivals. Entities then leave the module to begin processing through the system.";
	text += "	The entity type is specified in this module.";
	text += "	Animation showing the number of entities created is displayed when the module is placed.";
	text += "	TYPICAL USES: (1) The start of a part’s production in a manufacturing line;";
	text += "	(2) A document’s arrival (for example, order, check, application) into a business process;";
	text += "	(3)A customer’s arrival at a service process (for example, retail store, restaurant, information desk)";
	info->setDescriptionHelp(text);
	//info->insertDynamicLibFileDependence("attribute.so");
	//info->insertDynamicLibFileDependence("entitytype.so");
	//info->insertDynamicLibFileDependence("statisticscollector.so");

	return info;
}

ModelComponent* Create::LoadInstance(Model* model, std::map<std::string, std::string>* fields) {
	Create* newComponent = new Create(model);
	try {
		newComponent->_loadInstance(fields);
	} catch (const std::exception& e) {

	}

	return newComponent;
}

bool Create::_loadInstance(std::map<std::string, std::string>* fields) {

	return SourceModelComponent::_loadInstance(fields);
}

void Create::_initBetweenReplications() {

	SourceModelComponent::_initBetweenReplications();
}

std::map<std::string, std::string>* Create::_saveInstance(bool saveDefaultValues) {
	std::map<std::string, std::string>* fields = SourceModelComponent::_saveInstance(saveDefaultValues);

	return fields;
}

bool Create::_check(std::string* errorMessage) {
	bool resultAll = SourceModelComponent::_check(errorMessage);

	return resultAll;
}

void Create::_createInternalData() {
	SourceModelComponent::_createInternalData();
	if (_reportStatistics && _numberOut == nullptr) {
		_numberOut = new Counter(_parentModel, getName() + "." + "CountNumberOut", this);
		_internalData->insert({"CountNumberOut", _numberOut});
	} else if (!_reportStatistics && _numberOut != nullptr) {
		this->_removeInternalDatas();
		// @TODO _internelElements->remove("Count_number_in");
		//_numberOut->~Counter();
		_numberOut = nullptr;
	}
}
