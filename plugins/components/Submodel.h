/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Submodel.h
 * Author: rlcancian
 *
 * Created on 11 de Setembro de 2019, 13:04
 */

#ifndef SUBMODEL_H
#define SUBMODEL_H

#include "../../kernel/simulator/ModelComponent.h"

/*!
 This component ...
 */
class Submodel : public ModelComponent {
public: // constructors
	Submodel(Model* model, std::string name = "");
	virtual ~Submodel() = default;
public: // virtual
	virtual std::string show();
public: // static
	static PluginInformation* GetPluginInformation();
	static ModelComponent* LoadInstance(Model* model, std::map<std::string, std::string>* fields);
	static ModelDataDefinition* NewInstance(Model* model, std::string name = "");
protected: // virtual
	virtual void _onDispatchEvent(Entity* entity);
	virtual bool _loadInstance(std::map<std::string, std::string>* fields);
	virtual std::map<std::string, std::string>* _saveInstance(bool saveDefaultValues);
protected: // virtual
	//virtual void _initBetweenReplications();
	virtual bool _check(std::string* errorMessage);
private: // methods
private: // attributes 1:1
private: // attributes 1:n
};


#endif /* SUBMODEL_H */

