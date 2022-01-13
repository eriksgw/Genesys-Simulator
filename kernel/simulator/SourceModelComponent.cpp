/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   SourceModelCOmponent.cpp
 * Author: rafael.luiz.cancian
 *
 * Created on 21 de Junho de 2018, 19:50
 */

#include "SourceModelComponent.h"
#include "Model.h"
#include "Attribute.h"

//using namespace GenesysKernel;

SourceModelComponent::SourceModelComponent(Model* model, std::string componentTypename, std::string name) : ModelComponent(model, componentTypename, name) {
}

std::string SourceModelComponent::show() {
	std::string text = ModelComponent::show() +
			",entityType=\"" + _entityType->getName() + "\"" +
			",firstCreation=" + std::to_string(_firstCreation);
	return text;
}

bool SourceModelComponent::_loadInstance(std::map<std::string, std::string>* fields) {
	bool res = ModelComponent::_loadInstance(fields);
	if (res) {
		this->_entitiesPerCreation = LoadField(fields, "entitiesPerCreation", DEFAULT.entitiesPerCreation);
		this->_firstCreation = LoadField(fields, "firstCreation", DEFAULT.firstCreation);
		this->_timeBetweenCreationsExpression = LoadField(fields, "timeBetweenCreations", DEFAULT.timeBetweenCreationsExpression);
		this->_timeBetweenCreationsTimeUnit = LoadField(fields, "timeBetweenCreationsTimeUnit", DEFAULT.timeBetweenCreationsTimeUnit);
		this->_maxCreationsExpression = LoadField(fields, "maxCreations", DEFAULT.maxCreationsExpression);
		std::string entityTypename = LoadField(fields, "EntityType");
		this->_entityType = dynamic_cast<EntityType*> (_parentModel->getElements()->getElement(Util::TypeOf<EntityType>(), entityTypename));
	}
	return res;
}

void SourceModelComponent::_initBetweenReplications() {
	this->_entitiesCreatedSoFar = 0;
}

std::map<std::string, std::string>* SourceModelComponent::_saveInstance(bool saveDefaultValues) {
	std::map<std::string, std::string>* fields = ModelComponent::_saveInstance();
	SaveField(fields, "entitiesPerCreation", _entitiesPerCreation, DEFAULT.entitiesPerCreation);
	SaveField(fields, "firstCreation", _firstCreation, DEFAULT.firstCreation);
	SaveField(fields, "timeBetweenCreations", _timeBetweenCreationsExpression, DEFAULT.timeBetweenCreationsExpression);
	SaveField(fields, "timeBetweenCreationsTimeUnit", _timeBetweenCreationsTimeUnit, DEFAULT.timeBetweenCreationsTimeUnit);
	SaveField(fields, "maxCreations", _maxCreationsExpression, DEFAULT.maxCreationsExpression);
	SaveField(fields, "EntityType", _entityType->getName());
	return fields;
}

bool SourceModelComponent::_check(std::string* errorMessage) {
	/* include attributes needed */
	ElementManager* elements = _parentModel->getElements();
	std::vector<std::string> neededNames = {"Entity.ArrivalTime"};
	std::string neededName;
	for (unsigned int i = 0; i < neededNames.size(); i++) {
		neededName = neededNames[i];
		if (elements->getElement(Util::TypeOf<Attribute>(), neededName) == nullptr) {
			Attribute* attr1 = new Attribute(_parentModel, neededName);
			elements->insert(attr1);
		}
	}
	bool resultAll = true;
	resultAll &= _parentModel->getElements()->check(Util::TypeOf<EntityType>(), _entityType, "entitytype", errorMessage);
	resultAll &= _parentModel->checkExpression(this->_timeBetweenCreationsExpression, "time between creations", errorMessage);
	return resultAll;
}

void SourceModelComponent::setFirstCreation(double _firstCreation) {
	this->_firstCreation = _firstCreation;
}

double SourceModelComponent::getFirstCreation() const {
	return _firstCreation;
}

//void SourceModelComponent::setCollectStatistics(bool _collectStatistics) {
//    this->_collectStatistics = _collectStatistics;
//}
//
//bool SourceModelComponent::isCollectStatistics() const {
//    return _collectStatistics;
//}

void SourceModelComponent::setEntityType(EntityType* entityType) {
	_entityType = entityType;
}

EntityType* SourceModelComponent::getEntityType() const {
	return _entityType;
}

void SourceModelComponent::setTimeUnit(Util::TimeUnit _timeUnit) {
	this->_timeBetweenCreationsTimeUnit = _timeUnit;
}

Util::TimeUnit SourceModelComponent::getTimeUnit() const {
	return this->_timeBetweenCreationsTimeUnit;
}

void SourceModelComponent::setTimeBetweenCreationsExpression(std::string _timeBetweenCreations) {
	this->_timeBetweenCreationsExpression = _timeBetweenCreations;
}

std::string SourceModelComponent::getTimeBetweenCreationsExpression() const {
	return _timeBetweenCreationsExpression;
}

void SourceModelComponent::setMaxCreations(std::string _maxCreationsExpression) {
	this->_maxCreationsExpression = _maxCreationsExpression;
}

void SourceModelComponent::setMaxCreations(unsigned long _maxCreations) {
	this->_maxCreationsExpression = std::to_string(_maxCreations);
}

std::string SourceModelComponent::getMaxCreations() const {
	return _maxCreationsExpression;
}

unsigned int SourceModelComponent::getEntitiesCreated() const {
	return _entitiesCreatedSoFar;
}

void SourceModelComponent::setEntitiesCreated(unsigned int _entitiesCreated) {
	this->_entitiesCreatedSoFar = _entitiesCreated;
}

void SourceModelComponent::setEntitiesPerCreation(unsigned int _entitiesPerCreation) {
	this->_entitiesPerCreation = _entitiesPerCreation;
	//this->_entitiesCreatedSoFar = _entitiesPerCreation; // that's because "entitiesPerCreation" entities are included in the future events list BEFORE replication starts (to initialize events list)
}

unsigned int SourceModelComponent::getEntitiesPerCreation() const {
	return _entitiesPerCreation;
}