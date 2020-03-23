//
// Created by brachwal on 23.03.2020.
//

#include "Configurable.hh"

//#include "ConfigSvc.hh"

////////////////////////////////////////////////////////////////////////////////
///
Configurable::Configurable():m_config(std::make_shared<ModuleConfig>()){
}

////////////////////////////////////////////////////////////////////////////////
///
Configurable::Configurable(const std::string& name):m_config(std::make_shared<ModuleConfig>(name)){
}
