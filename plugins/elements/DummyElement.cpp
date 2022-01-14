/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.cc to edit this template
 */

/* 
 * File:   DummyElement.cpp
 * Author: rlcancian
 * 
 * Created on 11 de janeiro de 2022, 22:26
 */

#include "DummyElement.h"

DummyElement::DummyElement(Model* model, std::string name) : ModelElement(model, Util::TypeOf<DummyElement>(), name) {
}

// static 

ModelElement* DummyElement::LoadInstance(Model* model, std::map<std::string, std::string>* fields) {
	DummyElement* newElement = new DummyElement(model);
	try {
		newElement->_loadInstance(fields);
	} catch (const std::exception& e) {

	}
	return newElement;
}

PluginInformation* DummyElement::GetPluginInformation() {
	PluginInformation* info = new PluginInformation(Util::TypeOf<DummyElement>(), &DummyElement::LoadInstance);
	info->setDescriptionHelp("//@TODO");
	//info->setDescriptionHelp("");
	//info->setObservation("");
	//info->setMinimumOutputs();
	//info->setDynamicLibFilenameDependencies();
	//info->setFields();
	// ...
	return info;
}

//

std::string DummyElement::show() {
	return ModelElement::show();
}

// must be overriden by derived classes

bool DummyElement::_loadInstance(std::map<std::string, std::string>* fields) {
	bool res = ModelElement::_loadInstance(fields);
	if (res) {
		try {
			//this->_attributeName = LoadField(fields, "attributeName", DEFAULT.attributeName);
			//this->_orderRule = static_cast<OrderRule> (LoadField(fields, "orderRule", static_cast<int> (DEFAULT.orderRule)));
		} catch (...) {
		}
	}
	return res;
}

std::map<std::string, std::string>* DummyElement::_saveInstance(bool saveDefaultValues) {
	std::map<std::string, std::string>* fields = ModelElement::_saveInstance(saveDefaultValues); //Util::TypeOf<Queue>());
	//SaveField(fields, "orderRule", static_cast<int> (this->_orderRule), static_cast<int> (DEFAULT.orderRule));
	//SaveField(fields, "attributeName", this->_attributeName, DEFAULT.attributeName);
	return fields;
}

// could be overriden by derived classes

bool DummyElement::_check(std::string* errorMessage) {
	bool resultAll = true;
	//resultAll &= _parentModel->getElements()->check(Util::TypeOf<Station>(), _station, "Station", errorMessage);
	return resultAll;
}

ParserChangesInformation* DummyElement::_getParserChangesInformation() {
}

void DummyElement::_initBetweenReplications() {
}

void DummyElement::_createInternalElements() {
	if (_reportStatistics) {
		//if (_cstatNumberInQueue == nullptr) {
		//	_cstatNumberInQueue = new StatisticsCollector(_parentModel, getName() + "." + "NumberInQueue", this); /* @TODO: ++ WHY THIS INSERT "DISPOSE" AND "10ENTITYTYPE" STATCOLL ?? */
		//	_childrenElements->insert({"NumberInQueue", _cstatNumberInQueue});
		//}
	} else { //if (_cstatNumberInQueue != nullptr) {
		//_removeChildrenElements();
	}
}
