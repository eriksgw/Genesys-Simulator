/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Assign.h
 * Author: rafael.luiz.cancian
 *
 * Created on 31 de Agosto de 2018, 10:10
 */

#ifndef ASSIGN_H
#define ASSIGN_H

#include "../../kernel/simulator/ModelComponent.h"
#include "../../kernel/simulator/Model.h"
#include "../../kernel/simulator/Plugin.h"
#include "../../kernel/simulator/Attribute.h"
#include "../data/Variable.h"

/*!
 Assign module
DESCRIPTION
This module is used for assigning new values to variables, entity attributes, entity
types, entity pictures, or other system variables. Multiple assignments can be made
with a single Assign module.
TYPICAL USES
* Accumulate the number of subassemblies added to a part
* Change an entity’s type to represent the customer copy of a multi-page form
* Establish a customer’s priority
PROMPTS
 Prompt Description
Name Unique module identifier displayed on the module shape.
Assignments Specifies the one or more assignments that will be made when an
entity executes the module.
Type Type of assignment to be made. Other can include system
variables, such as resource capacity or simulation end time.
Variable Name Name of the variable that will be assigned a new value when an
entity enters the module. Applies only when Type is Variable,
Variable Array (1D), or Variable Array (2D).
Row Specifies the row index for a variable array.
Column Specifies the column index for a variable array.
Attribute Name Name of the entity attribute that will be assigned a new value
when the entity enters the module. Applies only when Type is
Attribute.
Entity Type New entity type that will be assigned to the entity when the
entity enters the module. Applies only when Type is Entity Type.
Entity Picture New entity picture that will be assigned to the entity when the
entity enters the module. Applies only when Type is Entity
Picture.
Other Identifies the special system variable that will be assigned a new
value when an entity enters the module. Applies only when Type
is Other.
New Value Assignment value of the attribute, variable, or other system
variable. Does not apply when Type is Entity Type or Entity
Picture.
 */
class Assign : public ModelComponent {
public:

	/*!
	 * While the assign class allows you to perform multiple assignments, the assignment class defines an assignment itself.
	 */
	class Assignment {
	public:
		Assignment(Model* model, std::string destination, std::string expression, bool isAttributeNotVariable = true) {
			this->_destination = destination;
			this->_expression = expression;
			if (isAttributeNotVariable) {
				if (model->getDataManager()->getDataDefinition(Util::TypeOf<Attribute>(), destination) == nullptr) {
					model->getDataManager()->insert(Util::TypeOf<Attribute>(), new Attribute(model, destination));
				}
				} else {
				if (model->getDataManager()->getDataDefinition(Util::TypeOf<Variable>(), destination) == nullptr) {
					model->getDataManager()->insert(Util::TypeOf<Variable>(), new Attribute(model, destination));
				}
			}
		}
		Assignment(std::string destination, std::string expression) {
			this->_destination = destination;
			this->_expression = expression;
			// an assignment is always in the form:
			// (destinationType) destination = expression
		};
		void setDestination(std::string _destination) {
			this->_destination = _destination;
		}
		std::string getDestination() const {
			return _destination;
		}
		void setExpression(std::string _expression) {
			this->_expression = _expression;
		}
		std::string getExpression() const {
			return _expression;
		}
	private:
		std::string _destination = "";
		std::string _expression = "";

	};
public:
	Assign(Model* model, std::string name = "");
	virtual ~Assign() = default;
public:
	virtual std::string show();
public:
	static PluginInformation* GetPluginInformation();
	static ModelComponent* LoadInstance(Model* model, std::map<std::string, std::string>* fields);
	static ModelDataDefinition* NewInstance(Model* model, std::string name = "");
public:
	List<Assignment*>* getAssignments() const;
protected:
	virtual void _onDispatchEvent(Entity* entity);
	virtual bool _loadInstance(std::map<std::string, std::string>* fields);
	virtual std::map<std::string, std::string>* _saveInstance(bool saveDefaultValues);
protected:
	//virtual void _initBetweenReplications();
	virtual bool _check(std::string* errorMessage);
private:
private:

	const struct DEFAULT_VALUES {
		unsigned int assignmentsSize = 1;
	} DEFAULT;
	List<Assignment*>* _assignments = new List<Assignment*>();
};

#endif /* ASSIGN_H */

