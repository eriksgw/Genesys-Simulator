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
#include "Model.h"
#include "EntityType.h"
#include "ElementManager.h"
#include "Attribute.h"
#include "Assign.h"

Create::Create(Model* model, std::string name) : SourceModelComponent(model, Util::TypeOf<Create>(),name) {
    _numberOut = new Counter(_parentModel, "Count number in", this);
    _parentModel->elements()->insert(_numberOut);
    GetterMember getter = DefineGetterMember<SourceModelComponent>(this, &Create::getEntitiesPerCreation);
    SetterMember setter = DefineSetterMember<SourceModelComponent>(this, &Create::setEntitiesPerCreation);
    model->controls()->insert(new SimulationControl(Util::TypeOf<Create>(), "Entities Per Creation", getter,setter));
    /*
    model->getControls()->insert(new SimulationControl(Util::TypeOf<Create>(), "Time Between Creations",
	    DefineGetterMember<SourceModelComponent>(this, &Create::getTimeBetweenCreationsExpression),
	    DefineSetterMember<SourceModelComponent>(this, &Create::setTimeBetweenCreationsExpression))
	    );
     */
}


std::string Create::show() {
    return SourceModelComponent::show();
}

void Create::_execute(Entity* entity) {
    double tnow = _parentModel->simulation()->simulatedTime();
    entity->setAttributeValue("Entity.ArrivalTime", tnow); // ->find("Entity.ArrivalTime")->second->setValue(tnow);
    //entity->setAttributeValue("Entity.Picture", 1); // ->find("Entity.ArrivalTime")->second->setValue(tnow);
    double timeBetweenCreations, timeScale, newArrivalTime;
    unsigned int _maxCreations = _parentModel->parseExpression(this->_maxCreationsExpression);
    for (unsigned int i = 0; i<this->_entitiesPerCreation; i++) {
	if (_entitiesCreatedSoFar < _maxCreations) {
	    _entitiesCreatedSoFar++;
	    Entity* newEntity = new Entity(_parentModel);
	    newEntity->setEntityType(entity->getEntityType());
	    _parentModel->elements()->insert(newEntity); // ->getEntities()->insert(newEntity);
	    timeBetweenCreations = _parentModel->parseExpression(this->_timeBetweenCreationsExpression);
	    timeScale = Util::TimeUnitConvert(this->_timeBetweenCreationsTimeUnit, _parentModel->infos()->replicationLengthTimeUnit());
	    newArrivalTime = tnow + timeBetweenCreations*timeScale;
	    Event* newEvent = new Event(newArrivalTime, newEntity, this);
	    _parentModel->futureEvents()->insert(newEvent);
	    _parentModel->tracer()->trace("Arrival of entity " + std::to_string(newEntity->getEntityNumber()) + " schedule for time " + std::to_string(newArrivalTime));
	    //_model->getTrace()->trace("Arrival of entity "+std::to_string(entity->getId()) + " schedule for time " +std::to_string(newArrivalTime));
	}
    }
    _numberOut->incCountValue();
    _parentModel->sendEntityToComponent(entity, this->nextComponents()->frontConnection(), 0.0);
}

PluginInformation* Create::GetPluginInformation() {
    PluginInformation* info = new PluginInformation(Util::TypeOf<Create>(), &Create::LoadInstance);
    info->setSource(true);
    info->insertDynamicLibFileDependence("attribute.so");
    info->insertDynamicLibFileDependence("entitytype.so");
    info->insertDynamicLibFileDependence("statisticscollector.so");
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

std::map<std::string, std::string>* Create::_saveInstance() {
    std::map<std::string, std::string>* fields = SourceModelComponent::_saveInstance();
    return fields;
}

bool Create::_check(std::string* errorMessage) {
    bool resultAll = SourceModelComponent::_check(errorMessage);
    return resultAll;
}
