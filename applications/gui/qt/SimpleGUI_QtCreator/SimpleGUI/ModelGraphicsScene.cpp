/*
 * The MIT License
 *
 * Copyright 2022 rlcancian.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/*
 * File:   ModelGraphicsScene.cpp
 * Author: rlcancian
 *
 * Created on 16 de fevereiro de 2022, 09:52
 */

#include <qt5/QtWidgets/qgraphicssceneevent.h>
#include <QTreeWidget>
#include <QMessageBox>
#include "ModelGraphicsScene.h"
#include "GraphicalModelComponent.h"
#include "ModelGraphicsView.h"
#include "GraphicalComponentPort.h"
#include "GraphicalConnection.h"

ModelGraphicsScene::ModelGraphicsScene(qreal x, qreal y, qreal width, qreal height, QObject *parent) : QGraphicsScene(x, y, width, height, parent) {
	// grid
	_grid.pen.setWidth(1);
	_grid.pen.setStyle(Qt::DotLine);
	showGrid();
}

ModelGraphicsScene::ModelGraphicsScene(const ModelGraphicsScene& orig) {
}

ModelGraphicsScene::~ModelGraphicsScene() {
	//
}


//-----------------------------------------------------------------------

GraphicalModelComponent* ModelGraphicsScene::addGraphicalModelComponent(Plugin* plugin, ModelComponent* component, QPointF position, QColor color) {
	GraphicalModelComponent* graphComp = new GraphicalModelComponent(plugin, component, position, color);
	addItem(graphComp);
	_graphicalModelComponents->append(graphComp);
	if (selectedItems().size()==1 && plugin->getPluginInfo()->getMinimumInputs()>0) { // check if there is selected component and crate a connection between them
		GraphicalModelComponent* otherGraphComp = dynamic_cast<GraphicalModelComponent*>(selectedItems().at(0));
		if (otherGraphComp != nullptr) { // a component is selected
			ModelComponent* otherComp = otherGraphComp->getComponent();
			unsigned int i=otherComp->getConnections()->size();
			if (otherGraphComp->getGraphicalOutputPorts().size()>i) {
				// create connection (both model and grapically, since model is being built
				// model
				otherGraphComp->getComponent()->getConnections()->insertAtRank(i, new Connection({component,0}));
				//graphically
				_sourceGraphicalComponentPort = ((GraphicalModelComponent*)selectedItems().at(0))->getGraphicalOutputPorts().at(i);
				GraphicalComponentPort* destport = graphComp->getGraphicalInputPorts().at(0);
				addGraphicalConnection(_sourceGraphicalComponentPort, destport);
			}
		} else {
			GraphicalComponentPort* sourceGraphPort = dynamic_cast<GraphicalComponentPort*>(selectedItems().at(0));
			if (sourceGraphPort != nullptr) { // a specific output port of a component is selected.
				// create connection (both model and grapically, since model is being built
				otherGraphComp = sourceGraphPort->graphicalComponent();
				ModelComponent* otherComp = otherGraphComp->getComponent();
				// create connection (both model and grapically, since model is being built (ALMOST REPEATED CODE -- REFACTOR)
				// model
				otherGraphComp->getComponent()->getConnections()->insertAtRank(sourceGraphPort->portNum(), new Connection({component,0}));
				//graphically
				_sourceGraphicalComponentPort = sourceGraphPort;
				GraphicalComponentPort* destport = graphComp->getGraphicalInputPorts().at(0);
				addGraphicalConnection(_sourceGraphicalComponentPort, destport);
			}
		}
	}
	//notify graphical model change
	GraphicalModelEvent* modelGraphicsEvent = new GraphicalModelEvent(GraphicalModelEvent::EventType::CREATE, GraphicalModelEvent::EventObjectType::COMPONENT, graphComp);
	dynamic_cast<ModelGraphicsView*>(views().at(0))->notifySceneGraphicalModelEventHandler(modelGraphicsEvent);
}

void ModelGraphicsScene::addGraphicalConnection(GraphicalComponentPort* sourcePort, GraphicalComponentPort* destinationPort) {
	GraphicalConnection* graphicconnection = new GraphicalConnection(sourcePort, destinationPort);
	addItem(graphicconnection);
	_graphicalConnections->append(graphicconnection);
	//notify graphical model change
	GraphicalModelEvent* modelGraphicsEvent = new GraphicalModelEvent(GraphicalModelEvent::EventType::CREATE, GraphicalModelEvent::EventObjectType::CONNECTION, graphicconnection);
	dynamic_cast<ModelGraphicsView*>(views().at(0))->notifySceneGraphicalModelEventHandler(modelGraphicsEvent);

}
void ModelGraphicsScene::addDrawing(){

}

void ModelGraphicsScene::addAnimation(){

}

void ModelGraphicsScene::removeGraphicalModelComponent(GraphicalModelComponent* gmc){
	// remove graphically
	/// first remove connections
	for(GraphicalComponentPort* port: gmc->getGraphicalInputPorts()) {
		for (GraphicalConnection* graphConn: *port->getConnections()) {
			removeGraphicalConnection(graphConn);
		}
	}
	for(GraphicalComponentPort* port: gmc->getGraphicalOutputPorts()) {
		for (GraphicalConnection* graphConn: *port->getConnections()) {
			removeGraphicalConnection(graphConn);
		}
	}
	/// then remove the component
	removeItem(gmc);
	_graphicalModelComponents->removeOne(gmc);
	ModelComponent* component = gmc->getComponent();
	gmc->~GraphicalModelComponent();
	// remove in model
	_simulator->getModels()->current()->getComponents()->remove(component);
	//notify graphical model change
	GraphicalModelEvent* modelGraphicsEvent = new GraphicalModelEvent(GraphicalModelEvent::EventType::REMOVE, GraphicalModelEvent::EventObjectType::COMPONENT, nullptr); // notify AFTER destroy or BEFORE it?
	dynamic_cast<ModelGraphicsView*>(views().at(0))->notifySceneGraphicalModelEventHandler(modelGraphicsEvent);
}
void ModelGraphicsScene::removeGraphicalConnection(GraphicalConnection* gc){
	// remove in model
	ModelComponent* sourceComp = gc->getSource()->first;
	sourceComp->getConnections()->removeAtRank(gc->getSource()->second);
	// remove graphically
	removeItem(gc);
	_graphicalConnections->removeOne(gc);
	////////gc->~GraphicalConnection();
	//notify graphical model change
	GraphicalModelEvent* modelGraphicsEvent = new GraphicalModelEvent(GraphicalModelEvent::EventType::REMOVE, GraphicalModelEvent::EventObjectType::CONNECTION, nullptr); // notify AFTER destroy or BEFORE it?
	dynamic_cast<ModelGraphicsView*>(views().at(0))->notifySceneGraphicalModelEventHandler(modelGraphicsEvent);
}

void ModelGraphicsScene::removeDrawing(){

}

void ModelGraphicsScene::removeAnimation(){

}


//------------------------------------------------------------------------

void ModelGraphicsScene::showGrid() {
	// remove xisting grid
	for (QGraphicsLineItem* line : *_grid.lines) {
		removeItem((QGraphicsItem*) line);
	}
	_grid.lines->clear();
	// add new grid
	for ( int i = sceneRect().left(); i < sceneRect().right(); i += _grid.interval) {
		QGraphicsLineItem* line = addLine(i, sceneRect().top(), i, sceneRect().bottom(), _grid.pen);
		_grid.lines->insert(_grid.lines->end(), line);
	}
	for (int j = sceneRect().top(); j < sceneRect().bottom(); j += _grid.interval) {
		QGraphicsLineItem* line = addLine(sceneRect().left(), j, sceneRect().right(), j, _grid.pen);
		_grid.lines->insert(_grid.lines->end(), line);
	}
}

void ModelGraphicsScene::beginConnection(){
	_connectingStep = 1;
	((QGraphicsView* )this->parent())->setCursor(Qt::CrossCursor);
}


//-------------------------
// PROTECTED VIRTUAL FUNCTIONS
//-------------------------

void ModelGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent){
	QGraphicsScene::mousePressEvent(mouseEvent);
	if (_connectingStep>0 && mouseEvent->button()==Qt::LeftButton){
		QGraphicsItem* item = this->itemAt(mouseEvent->scenePos(), QTransform());
		if (item!=nullptr) {
			GraphicalComponentPort* port = dynamic_cast<GraphicalComponentPort*>(item);
			if (port != nullptr) {
				if (_connectingStep==1 && !port->isInputPort()){
					_sourceGraphicalComponentPort = port;
					_connectingStep = 2;
					return;
				 } else if (_connectingStep==2 && port->isInputPort()) {
					_connectingStep = 3;
					// create connection
					// in the model
					ModelComponent* sourceComp = _sourceGraphicalComponentPort->graphicalComponent()->getComponent();
					ModelComponent* destComp = port->graphicalComponent()->getComponent();
					sourceComp->getConnections()->insertAtRank(_sourceGraphicalComponentPort->portNum(), new Connection({destComp, port->portNum()}));
					// graphically
					GraphicalConnection* graphicconnection = new GraphicalConnection(_sourceGraphicalComponentPort, port);
					_sourceGraphicalComponentPort->addGraphicalConnection(graphicconnection);
					port->addGraphicalConnection(graphicconnection);
					addItem(graphicconnection);
					//
					((ModelGraphicsView *) (this->parent()))->unsetCursor();
					_connectingStep = 0;
					return;
				}
			}
		}
		((ModelGraphicsView *) (this->parent()))->unsetCursor();
		_connectingStep = 0;
	}
}

void ModelGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) {
	QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void ModelGraphicsScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent){
	QGraphicsScene::mouseDoubleClickEvent(mouseEvent);
	GraphicalComponentPort* port = dynamic_cast<GraphicalComponentPort*>(this->itemAt(mouseEvent->scenePos(), QTransform()));
	if (port!=nullptr) {
		if (!port->isInputPort() && this->_connectingStep==0) {
			_sourceGraphicalComponentPort = port;
			_connectingStep = 2;

		}
	}
}
void ModelGraphicsScene::wheelEvent(QGraphicsSceneWheelEvent *wheelEvent) {
	QGraphicsScene::wheelEvent(wheelEvent);
}

void ModelGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) {
	QGraphicsScene::mouseMoveEvent(mouseEvent);
	((ModelGraphicsView *) (this->parent()))->notifySceneMouseEventHandler(mouseEvent); // to show coords
	if (_connectingStep>0){
		QGraphicsItem* item = this->itemAt(mouseEvent->scenePos(), QTransform());
		if (item!=nullptr) {
			GraphicalComponentPort* port = dynamic_cast<GraphicalComponentPort*>(item);
			if (port != nullptr) {
				if (_connectingStep==1 && !port->isInputPort()) {
					((ModelGraphicsView *) (this->parent()))->setCursor(Qt::PointingHandCursor);
				} else if (_connectingStep==2 && port->isInputPort()) {
					((ModelGraphicsView *) (this->parent()))->setCursor(Qt::PointingHandCursor);
				}
				return;
			}
		}
		if (_connectingStep>1) {
			((ModelGraphicsView *) (this->parent()))->setCursor(Qt::ClosedHandCursor);
		} else {
			((ModelGraphicsView *) (this->parent()))->setCursor(Qt::CrossCursor);
		}
	}
}
void ModelGraphicsScene::dropEvent(QGraphicsSceneDragDropEvent *event){
	QGraphicsScene::dropEvent(event);
	if (this->_objectBeingDragged != nullptr) {
		QTreeWidgetItem* treeItem = /*dynamic_cast<QTreeWidgetItem*>*/(_objectBeingDragged);
		if (treeItem != nullptr) {
			QColor color = treeItem->textColor(0);
			QString pluginname = treeItem->whatsThis(0);
			Plugin* plugin = _simulator->getPlugins()->find(pluginname.toStdString());
			if(plugin != nullptr) {
				if (plugin->getPluginInfo()->isComponent()) {
					event->setDropAction(Qt::IgnoreAction);
					event->accept();
					// create component in the model
					ModelComponent* component = (ModelComponent*) plugin->newInstance(_simulator->getModels()->current());
					// create graphically
					addGraphicalModelComponent(plugin, component, event->scenePos(), color);
					return;
				}
			}
		}
	}
	event->setAccepted(false);
}

void ModelGraphicsScene::dragEnterEvent(QGraphicsSceneDragDropEvent *event){
	QGraphicsScene::dragEnterEvent(event);
	//QString name;
	//Plugin* plugin = _simulator->getPlugins()->find(name.toStdString());
	//if(true) {//(plugin != nullptr) {
	event->setDropAction(Qt::CopyAction);
	event->accept();
	//}
}

void ModelGraphicsScene::dragLeaveEvent(QGraphicsSceneDragDropEvent *event){
	QGraphicsScene::dragLeaveEvent(event);
	event->setDropAction(Qt::CopyAction);
	event->accept();
}

void ModelGraphicsScene::dragMoveEvent(QGraphicsSceneDragDropEvent *event){
	QGraphicsScene::dragMoveEvent(event);
	event->setDropAction(Qt::CopyAction);
	event->accept();
}

void ModelGraphicsScene::keyPressEvent(QKeyEvent *keyEvent){
	QList<QGraphicsItem*> selected = this->selectedItems();
	if (keyEvent->key()==Qt::Key_Delete && selected.size()>0) {
		QMessageBox::StandardButton reply = QMessageBox::question(this->_parentWidget, "Delete Component", "Are you sure you want to delete the selected components?", QMessageBox::Yes | QMessageBox::No);
		if (reply == QMessageBox::No) {
			return;
		}
		Model* model = _simulator->getModels()->current();
		for (QGraphicsItem* item: selected) {
			GraphicalModelComponent* gmc = dynamic_cast<GraphicalModelComponent*>(item);
			if (gmc != nullptr) {
				removeGraphicalModelComponent(gmc);
			} else {
				GraphicalConnection* gc = dynamic_cast<GraphicalConnection*>(item);
				if (gc != nullptr) {
					removeGraphicalConnection(gc);
				} else {
					//Drawing* gc = dynamic_cast<GraphicalConnection*>(item);
					//if (gc != nullptr) {
					//	removeItem(item);
					//	gc->~Drawing();
					//}
				}
			}
		}
	}
}

//--------------------------
//
//--------------------------

void ModelGraphicsScene::setObjectBeingDragged(QTreeWidgetItem* objectBeingDragged)
{
	_objectBeingDragged = objectBeingDragged;
}

void ModelGraphicsScene::setSimulator(Simulator *simulator){
	_simulator = simulator;
}


unsigned short ModelGraphicsScene::connectingStep() const
{
	return _connectingStep;
}

void ModelGraphicsScene::setConnectingStep(unsigned short connectingStep)
{
	_connectingStep = connectingStep;
}

void ModelGraphicsScene::setParentWidget(QWidget *parentWidget)
{
	_parentWidget = parentWidget;
}

QList<GraphicalModelComponent*>* ModelGraphicsScene::graphicalModelMomponentItems(){
	QList<GraphicalModelComponent*>* list = new QList<GraphicalModelComponent*>();
	for(QGraphicsItem* item: this->items()) {
		GraphicalModelComponent* gmc = dynamic_cast<GraphicalModelComponent*>(item);
		if (gmc != nullptr) {
			list->append(gmc);
		}
	}
	return list;
}

//------------------------
// Private
//------------------------

