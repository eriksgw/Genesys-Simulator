/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   FirstExampleOfSimulation.cpp
 * Author: rlcancian
 * 
 * Created on 3 de Setembro de 2019, 18:34
 */

#include "Example_RouteStation.h"

// you have to included need libs

// GEnSyS Simulator
#include "../../kernel/simulator/Simulator.h"

// Model Components
#include "../../plugins/components/Create.h"
#include "../../plugins/components/Route.h"
#include "../../plugins/components/Enter.h"
#include "../../plugins/components/Delay.h"
#include "../../plugins/components/Dispose.h"

// Model elements
//#include "../../kernel/simulator/EntityType.h"

Example_RouteStation::Example_RouteStation() {
}

/**
 * This is the main function of the application. 
 * It instanciates the simulator, builds a simulation model and then simulate that model.
 */
int Example_RouteStation::main(int argc, char** argv) {
	Simulator* genesys = new Simulator();
	this->insertFakePluginsByHand(genesys);
	this->setDefaultTraceHandlers(genesys->getTracer());
	genesys->getTracer()->setTraceLevel(Util::TraceLevel::L9_mostDetailed);
	Model* model = genesys->getModels()->newModel();
	Create* create1 = new Create(model);
	create1->setEntityType(new EntityType(model));
	create1->setTimeBetweenCreationsExpression("4.0");
	Station* station1 = new Station(model);
	Route* route1 = new Route(model);
	route1->setStation(station1);
	route1->setRouteTimeExpression("0.1");
	//
	Enter* enter1 = new Enter(model);
	enter1->setStation(station1);
	Delay* delay1 = new Delay(model);
	Station* station2 = new Station(model);
	Route* route2 = new Route(model);
	route2->setStation(station2);
	route2->setRouteTimeExpression("0.2");
	//
	Enter* enter2 = new Enter(model);
	enter2->setStation(station2);
	Delay* delay2 = new Delay(model);
	Station* station3 = new Station(model);
	Route* route3 = new Route(model);
	route3->setStation(station3);
	route3->setRouteTimeExpression("0.3");
	//
	Enter* enter3 = new Enter(model);
	enter3->setStation(station3);
	Dispose* dispose1 = new Dispose(model);
	//
	create1->getNextComponents()->insert(route1);
	enter1->getNextComponents()->insert(delay1);
	delay1->getNextComponents()->insert(route2);
	enter2->getNextComponents()->insert(delay2);
	delay2->getNextComponents()->insert(route3);
	enter3->getNextComponents()->insert(dispose1);
	//
	model->getSimulation()->setReplicationLength(20);
	model->save("./models/Example_RouteStation.txt");
	do {
		model->getSimulation()->step();
		std::cin.ignore(std::numeric_limits <std::streamsize> ::max(), '\n');
	} while (model->getSimulation()->isPaused());
	return 0;
};
