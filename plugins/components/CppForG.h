/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.h to edit this template
 */

/* 
 * File:   CppForG.h
 * Author: rlcancian
 *
 * Created on 11 de janeiro de 2022, 22:37
 */

#pragma once

#include "../../kernel/simulator/ModelComponent.h"
#include "../data/CppCode.h"

/*!
 This component ...
 */
class CppForG : public ModelComponent {
public: // constructors
	CppForG(Model* model, std::string name = "");
	virtual ~CppForG() = default;
public: // virtual
	virtual std::string show();
public: // static
	static PluginInformation* GetPluginInformation();
	static ModelComponent* LoadInstance(Model* model, std::map<std::string, std::string>* fields);
	static ModelDataDefinition* NewInstance(Model* model, std::string name = "");
public:
	void setSourceCode(std::string _sourceCode);
	std::string getSourceCode() const;
protected: // virtual
	virtual void _onDispatchEvent(Entity* entity);
	virtual bool _loadInstance(std::map<std::string, std::string>* fields);
	virtual std::map<std::string, std::string>* _saveInstance(bool saveDefaultValues);
protected: // virtual
	//virtual void _initBetweenReplications();
	virtual bool _check(std::string* errorMessage);
	virtual void _createInternalData();
private: // methods
private: // attributes 1:1
	// internel
	CppCode* _cppcode = nullptr;
private: // attributes 1:n
};

