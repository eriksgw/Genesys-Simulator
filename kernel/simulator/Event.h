/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Event.h
 * Author: rafael.luiz.cancian
 *
 * Created on 21 de Junho de 2018, 19:41
 */

#ifndef EVENT_H
#define EVENT_H

#include <string>

#include "ModelDataDefinition.h"
#include "Entity.h"
#include "ModelComponent.h"

//namespace GenesysKernel {

/*!
 * An an instantaneaous event, triggered at a certain moment by an entity upon reaching a component. The simulated time advances in discrete points in time and that are the instants that an event is triggered.
 */
class Event {//: public ModelDataDefinition {
public:
	Event(double time, Entity* entity, ModelComponent* component, unsigned int componentInputNumber = 0);
	Event(double time, Entity* entity, Connection* connection);
	virtual ~Event() = default;
public:
	double getTime() const;
	ModelComponent* getComponent() const;
	Entity* getEntity() const;
	unsigned int getComponentInputNumber() const;
public:
	std::string show();
private:
	double _time;
	Entity* _entity;
	ModelComponent* _component;
	unsigned int _componentInputNumber;
};
//namespace\\}
#endif /* EVENT_H */

