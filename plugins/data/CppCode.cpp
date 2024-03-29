/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.cc to edit this template
 */

/* 
 * File:   CppCode.cpp
 * Author: rlcancian
 * 
 * Created on 11 de janeiro de 2022, 22:24
 */

#include "CppCode.h"

#ifdef PLUGINCONNECT_DYNAMIC

extern "C" StaticGetPluginInformation GetPluginInformation() {
	return &CppCode::GetPluginInformation;
}
#endif

ModelDataDefinition* CppCode::NewInstance(Model* model, std::string name) {
	return new CppCode(model, name);
}

CppCode::CppCode(Model* model, std::string name) : ModelDataDefinition(model, Util::TypeOf<CppCode>(), name) {
}

// static 

ModelDataDefinition* CppCode::LoadInstance(Model* model, std::map<std::string, std::string>* fields) {
	CppCode* newElement = new CppCode(model);
	try {
		newElement->_loadInstance(fields);
	} catch (const std::exception& e) {

	}
	return newElement;
}

PluginInformation* CppCode::GetPluginInformation() {
	PluginInformation* info = new PluginInformation(Util::TypeOf<CppCode>(), &CppCode::LoadInstance, &CppCode::NewInstance);
	info->setDescriptionHelp("//@TODO");
	//info->setObservation("");
	//info->setMinimumOutputs();
	//info->setDynamicLibFilenameDependencies();
	//info->setFields();
	// ...
	return info;
}

//

std::string CppCode::show() {
	return ModelDataDefinition::show();
}

void CppCode::setCode(std::string _code) {
	this->_code = _code;
}

std::string CppCode::getCode() const {
	return _code;
}
// must be overriden by derived classes

bool CppCode::_loadInstance(std::map<std::string, std::string>* fields) {
	bool res = ModelDataDefinition::_loadInstance(fields);
	if (res) {
		// @TODO: not implemented yet
	}
	return res;
}

std::map<std::string, std::string>* CppCode::_saveInstance(bool saveDefaultValues) {
	std::map<std::string, std::string>* fields = ModelDataDefinition::_saveInstance(saveDefaultValues);
	// @TODO: not implemented yet
	return fields;
}

// could be overriden by derived classes

bool CppCode::_check(std::string* errorMessage) {
	return true;
}

//ParserChangesInformation* CppCode::_getParserChangesInformation() {}

//void CppCode::_initBetweenReplications() {}

void CppCode::_createInternalData() {
}

CppCode::CodeResult CppCode::compile() {
	CppCode::CodeResult result();
	return result;
}

CppCode::CodeResult CppCode::loadLibrary() {
	CppCode::CodeResult result();
	return result;
}

CppCode::CodeResult CppCode::unloadLibrary() {
	CppCode::CodeResult result();
	return result;
}

bool CppCode::IsLibraryLoaded() const {
	return libraryLoaded;
}

