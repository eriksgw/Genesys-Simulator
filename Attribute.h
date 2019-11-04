/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Attribute.h
 * Author: rafael.luiz.cancian
 *
 * Created on 25 de Setembro de 2018, 16:37
 */

#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include <string>
#include <list>
#include "List.h"
#include "ModelElement.h"
#include "ElementManager.h"
#include "Plugin.h"

class Attribute : public ModelElement {
public:
    //Attribute(Model* model);
    Attribute(Model* model, std::string name="");
    virtual ~Attribute() = default;
public:
    virtual std::string show();
public:
    static PluginInformation* GetPluginInformation();
    static ModelElement* LoadInstance(Model* model, std::map<std::string, std::string>* fields);
protected:
    virtual bool _loadInstance(std::map<std::string, std::string>* fields);
    virtual std::map<std::string, std::string>* _saveInstance();
    virtual bool _check(std::string* errorMessage);
private:
    //List<unsigned int>* _dimensionSizes = new List<unsigned int>();
};

#endif /* ATTRIBUTE_H */

