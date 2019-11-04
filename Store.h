/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Store.h
 * Author: rlcancian
 *
 * Created on 11 de Setembro de 2019, 13:07
 */

#ifndef STORE_H
#define STORE_H

#include "ModelComponent.h"

/*!
 This component ...
 */
class Store : public ModelComponent {
public: // constructors
    Store(Model* model, std::string name="");
    virtual ~Store() = default;
public:  // virtual
    virtual std::string show();
public:  // static
    static PluginInformation* GetPluginInformation();
    static ModelComponent* LoadInstance(Model* model, std::map<std::string, std::string>* fields);
protected:  // virtual
    virtual void _execute(Entity* entity);
    virtual void _initBetweenReplications();
    virtual bool _loadInstance(std::map<std::string, std::string>* fields);
    virtual std::map<std::string, std::string>* _saveInstance();
    virtual bool _check(std::string* errorMessage);
private: // methods
private: // attributes 1:1
private: // attributes 1:n
};


#endif /* STORE_H */

