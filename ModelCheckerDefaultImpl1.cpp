/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ModelCheckerDefaultImpl1.cpp
 * Author: rafael.luiz.cancian
 * 
 * Created on 8 de Agosto de 2018, 18:44
 */

#include "ModelCheckerDefaultImpl1.h"
#include "SourceModelComponent.h"
#include "SinkModelComponent.h"

ModelCheckerDefaultImpl1::ModelCheckerDefaultImpl1(Model* model) {
    _model = model;
}

ModelCheckerDefaultImpl1::ModelCheckerDefaultImpl1(const ModelCheckerDefaultImpl1& orig) {
}

ModelCheckerDefaultImpl1::~ModelCheckerDefaultImpl1() {
}

bool ModelCheckerDefaultImpl1::checkAll() {
    bool res = true;
    res &= checkConnected();
    //res &= checkPathway();
    res &= checkSymbols();
    //res &= checkAndAddInternalLiterals();
    //res &= checkActivationCode();
    return res;
}

//bool ModelCheckerDefaultImpl1::checkAndAddInternalLiterals() {
//    /* TODO +-: not implemented yet */
//    return true;
//}

bool ModelCheckerDefaultImpl1::_recursiveConnectedTo(ModelComponent* comp, List<ModelComponent*>* visited, List<ModelComponent*>* unconnected, bool* drenoFound) {
    visited->insert(comp);
    _model->getTracer()->trace(Util::TraceLevel::mostDetailed, "Connected to component " + comp->getName());
    if (dynamic_cast<SinkModelComponent*> (comp) != nullptr) { // it is a sink
        *drenoFound = true;
    } else { // it is not a sink
        if (comp->getNextComponents()->size() == 0) {
            unconnected->insert(comp);
            _model->getTracer()->trace(Util::TraceLevel::errors, "Component \"" + comp->getName() + "\" is unconnected (not a sink with no next componentes connected to)");
            *drenoFound = false;
        } else {
            ModelComponent* nextComp;
            for (std::list<ModelComponent*>::iterator it = comp->getNextComponents()->getList()->begin(); it != comp->getNextComponents()->getList()->end(); it++) {
                nextComp = (*it);
                if (visited->find(nextComp) == visited->getList()->end()) { // not visited yet
                    *drenoFound = false;
                    this->_recursiveConnectedTo(nextComp, visited, unconnected, drenoFound);
                } else {
                    *drenoFound = true;
                }
            }
        }
    }
}

/*
procedure TModel._RecursiveConnectedTo(thisModule:TModule; var visited:TStringList; var unconnected: TStringList; var drenofound:boolean);
var plugin: TPlugin;
    index: integer;
    nextModule: TModule;
    i: integer;
begin
   visited.Add(IntToStr(thisModule.ID)); {include this module as visited}
   index := Genesys.PlugInIndexOf(thisModule.Kind);
   Trace(cTLModuleDetail, Genesys.AuxFunctions.StrIdent(2)+'- Connected to ' + thisModule.Kind +': '+ thisModule.Name+' ('+IntToStr(thisModule.ID)+')');
   if index = -1 then begin
      {the module has no associated plugin. Impossible. Should do more, but will mark module as unconnected}
      unconnected.Add(IntToStr(thisModule.ID));
      Trace(cTLError,'Error: Module named "'+thisModule.Name+'" id "'+IntToStr(thisModule.ID)+'" has no valid plugin "'+thisModule.Kind+'").');
      end
   else begin
      plugin := Genesys.PlugIn[index];
      if plugin.Dreno then begin
         index := 0; {singal to stop searching. Found a dreno}
         i := 0;
         while (i < thisModule.NextCount) and (index = 0) do begin
            if thisModule.NextID[i] <> thisModule.ID then
               index := 1; {need continue searching}
            i := i + 1;
         end;
         if index = 0 then begin
            {found a dreno that has no connection with other modules. Stop walking}
            drenoFound := true;
         end;
      end;
      if not drenoFound then begin
         if thisModule.NextCount = 0 then begin
            //this module is not a DRENO and it has no next modules - it is a dead end
            unconnected.Add(IntToStr(thisModule.ID));
            Trace(cTLError,'Error: Module named "'+thisModule.Name+'" id "'+IntToStr(thisModule.ID)+'" is unconnected (no next module).');
            end
         else begin
            //continue the pathway throught the next modules
            for index := 0 to thisModule.NextCount - 1 do begin
               drenoFound := false;
               nextModule := ModuleByID(thisModule.NextID[index]);
               if (nextModule = nil) then begin
                  {can be an invalid ID or a queue associated with a module}
                  i := SIMAN.QueueIndex(thisModule.NextID[index]);
                  if (i >= 0) then begin
                     {is a queue. Try to get the module associated}
                     nextModule := ModuleByID(SIMAN.Queue[i].ModuleID);
                  end;
                  if (nextModule = nil) then begin
                     {invalid ID}
                     unconnected.Add(IntToStr(thisModule.ID));
                     Trace(cTLError,'Error: Module named "'+thisModule.Name+'" id "'+IntToStr(thisModule.ID)+'" is unconnected (invalid next id "'+IntToStr(thisModule.NextID[index])+'").');
                     DrenoFound := true; {only one error is suficient}
                  end;
               end;
               if (nextModule <> nil) then begin
                  if visited.IndexOf(IntToStr(nextModule.ID)) = -1 then
                     _RecursiveConnectedTo(nextModule, visited, unconnected, drenoFound) {continue searching}
                  else
                     drenoFound := true; {arrived at an already visited module}
               end;
            end;
            if not DrenoFound then begin
               Trace(cTLError,'Error: Module named "'+thisModule.Name+'" id "'+IntToStr(thisModule.ID)+'" is unconnected (takes to no dreno).');
               DrenoFound := true;
            end;
         end;
      end;
   end;
end;
 */

bool ModelCheckerDefaultImpl1::checkConnected() {
    /* TODO +-: not implemented yet */
    _model->getTracer()->trace(Util::TraceLevel::blockArrival, "Checking connected");
    bool resultAll = true;
    Util::IncIndent();
    {
        List<ModelComponent*>* visited = new List<ModelComponent*>();
        List<ModelComponent*>* unconnected = new List<ModelComponent*>();
        ModelComponent* comp;
        for (std::list<ModelComponent*>::iterator it = _model->getComponents()->getList()->begin(); it != _model->getComponents()->getList()->end(); it++) {
            comp = (*it);
            if (dynamic_cast<SourceModelComponent*> (comp) != nullptr) {
                // it is a source component
                bool drenoFound = false;
                _recursiveConnectedTo(comp, visited, unconnected, &drenoFound);
            }
        }
        // check if any component remais unconnected
        for (std::list<ModelComponent*>::iterator it = _model->getComponents()->getList()->begin(); it != _model->getComponents()->getList()->end(); it++) {
            comp = (*it);
            if (visited->find(comp) == visited->getList()->end()) { //not found
                resultAll = false;
                _model->getTracer()->trace(Util::TraceLevel::errors, "Component \"" + comp->getName() + "\" is unconnected.");
            }
        }

    }
    Util::DecIndent();
    return resultAll;
}

/*
function TModel._CheckConnected: integer;
var i, j: integer;
    kind: string;
    moduleID: word;
    criacoes,
    visited, unconnected,nonvisual: TStringList;
    thisModule: TModule;
    drenoFound: boolean;
begin
   { Returns 0 if model is correctlly connected, or the -ID of the first unconnected module}
   result := -1;  {is wrong by default}
   Trace(cTLModuleIntern, Genesys.AuxFunctions.StrIdent(1)+'- Verifying modules connections');
   visited := TStringList.Create;
   visited.Sorted := true;
   unconnected := TStringList.Create;
   unconnected.Sorted := true;
   {takes all source modules in the model}
   i := 0;
   while (i < Genesys.PlugInCount) do begin
      if Genesys.PlugIn[i].Source then begin
         kind := Genesys.PlugIn[i].Kind;
         //A SOURCE block has been found. Start searching a pathway
         Criacoes := ModulesOfKind[kind];
         if Criacoes <> nil then begin
            for j := 0 to Criacoes.Count - 1 do begin
               //for each SOURCE block, searchs for a pathway throught a DRENO
               drenoFound := false;
               thisModule := TModule(Criacoes.Objects[j]);
               _RecursiveConnectedTo(thisModule,visited,unconnected, drenoFound);
            end;
         end;
      end;
      // Non-visual blocks don�t need to be connected
      if not Genesys.PlugIn[i].Visual then begin
         kind := Genesys.PlugIn[i].Kind;
         nonvisual := ModulesOfKind[kind];
         if nonvisual <> nil then begin
            for j := 0 to nonvisual.Count - 1 do begin
               visited.Add(IntToStr(TModule(nonvisual.Objects[j]).ID));
            end;
         end;
      end;
      i := i + 1;
   end;
   {finished to walk throught the model}
   {verify if there are modules that were not visited}
   for i := 0 to aModules.Count - 1 do begin
      criacoes := TStringList(aModules.Objects[i]);
      for j := 0 to criacoes.Count - 1 do begin
         moduleID := TModule(criacoes.Objects[j]).ID;
         if visited.IndexOf(IntToStr(moduleID)) = -1 then begin
            {this module couldn't be visited by any source module. It is inaccessible. Mark as unconnected}
            unconnected.Add(IntToStr(moduleID));
            Trace(cTLError,'Error: Module id "'+IntToStr(moduleID)+'" is unconnected (not accessible).');
         end;
      end;
   end;
   if unconnected.Count = 0 then
      result := 0 {ok}
   else
      result := -1;  {model has problems}
end;
 
 */



bool ModelCheckerDefaultImpl1::checkSymbols() {
    bool res = true;
    _model->getTracer()->trace(Util::TraceLevel::blockArrival, "Checking symbols");
    Util::IncIndent();
    {
        // check components
        _model->getTracer()->trace(Util::TraceLevel::blockInternal, "Components:");
        Util::IncIndent();
        {
            List<ModelComponent*>* components = _model->getComponents();
            for (std::list<ModelComponent*>::iterator it = components->getList()->begin(); it != components->getList()->end(); it++) {
                res &= (*it)->Check((*it));
            }
        }
        Util::DecIndent();

        // check elements
        _model->getTracer()->trace(Util::TraceLevel::blockInternal, "Elements:");
        Util::IncIndent();
        {
            std::string elementType;
            bool result;
            ModelElement* element;
            std::string* errorMessage = new std::string();
            std::list<std::string>* elementTypes = _model->getElementManager()->getElementTypenames();
            for (std::list<std::string>::iterator typeIt = elementTypes->begin(); typeIt != elementTypes->end(); typeIt++) {
                elementType = (*typeIt);
                List<ModelElement*>* elements = _model->getElementManager()->getElements(elementType);
                for (std::list<ModelElement*>::iterator it = elements->getList()->begin(); it != elements->getList()->end(); it++) {
                    element = (*it);
                    // copyed from modelCOmponent. It is not inside the ModelElement::Check because ModelElement has no access to Model to call Tracer
                    _model->getTracer()->trace(Util::TraceLevel::mostDetailed, "Checking " + element->getTypename() + ": " + element->getName()); //std::to_string(component->_id));
                    Util::IncIndent();
                    {
                        try {
                            result = element->Check((*it), errorMessage);
                            res &= result;
                            if (!res) {
                                _model->getTracer()->trace(Util::TraceLevel::errors, "Error: Checking has failed with message '" + *errorMessage + "'");
                            }
                        } catch (const std::exception& e) {
                            _model->getTracer()->traceError(e, "Error verifying component " + element->show());
                        }
                    }
                    Util::DecIndent();

                    /*
                    _model->getTracer()->trace(Util::TraceLevel::mostDetailed, "Verifying symbols of " + elementType + " " + element->getName()); //std::to_string(component->_id));
                    result = element->Check((*it), errorMessage);
                    res &= result;
                    if (!result) {
                        _model->getTracer()->trace(Util::TraceLevel::mostDetailed, "Verification of symbols of component \"" + element->getName() + "\" has failed with message " + *errorMessage);
                    }
                     */
                }
            }
        }
        Util::DecIndent();
    }
    Util::DecIndent();

    return res;
}

bool ModelCheckerDefaultImpl1::checkActivationCode() {
    /* TODO +-: not implemented yet */
    return true;
}

/*
bool ModelCheckerDefaultImpl1::verifySymbol(std::string componentName, std::string expressionName, std::string expression, std::string expressionResult, bool mandatory) {
    bool res = true;
    if (mandatory && expression == "") {
        _model->getTracer()->getErrorMessages()->insert("Error verifying symbol \"" + expressionName + "\" of component \"" + componentName + "\n: Mandatory symbol is empty");
        res = false;
    }
    // TODO: Not implemented yet 
    return res;
}
 */