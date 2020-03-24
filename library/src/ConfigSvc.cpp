//
// Created by brachwal on 23.03.2020.
//

#include <iostream>
#include "ConfigSvc.hh"

///////////////////////////////////////////////////////////////////////////////
///
ConfigSvc* ConfigSvc::GetInstance() {
    static ConfigSvc instance;
    return &instance;
}

////////////////////////////////////////////////////////////////////////////////
///
bool ConfigSvc::IsRegistered(const std::string& module) const {
    return m_config_modules.find(module) != m_config_modules.end() ? true : false;
}

////////////////////////////////////////////////////////////////////////////////
///
void ConfigSvc::SetValue(const std::string& module, const std::string& unit, std::any value) {
    if (IsRegistered(module)) {
        return m_config_modules.at(module)->Config()->SetValue(unit, value);
    } else {
        throw std::invalid_argument("ConfigSvc::SetValue:: Module( " + module + " ) is not registered.");
    }
}

////////////////////////////////////////////////////////////////////////////////
///
void ConfigSvc::SetStatus(const std::string& module, bool status) {
    if (IsRegistered(module)) {
        return m_config_modules[module]->Config()->SetStatus(status);
    } else {
        throw std::invalid_argument("ConfigSvc::SetValue:: Module( " + module + " ) is not registered.");
    }
}
////////////////////////////////////////////////////////////////////////////////
///
bool ConfigSvc::GetStatus(const std::string& module) const {
    if (IsRegistered(module)) {
        return m_config_modules.at(module)->Config()->GetStatus();
    } else {
        throw std::invalid_argument("ConfigSvc::GetStatus:: Module( " + module + " ) is not registered.");
    }
}

////////////////////////////////////////////////////////////////////////////////
///
bool ConfigSvc::GetStatus(const std::string& module, const std::string& unit) const {
    if (IsRegistered(module)) {
        return m_config_modules.at(module)->Config()->GetStatus(unit);
    } else {
        throw std::invalid_argument("ConfigSvc::GetStatus:: Module( " + module + " ) is not registered.");
    }
}

////////////////////////////////////////////////////////////////////////////////
///
void ConfigSvc::Register(const std::string& module, std::shared_ptr<Configurable> config_volume) {
    if (IsRegistered(module)) {
        // Each module name should be unique
        throw std::logic_error("ConfigSvc::Register:: Module( \"" + module + "\" ) is already registered.");
    } else {
        std::cout << "[ConfigSvc]::  Register new module : \"" << module <<"\""<< std::endl;
        m_config_modules.insert(make_pair(module, config_volume));
    }
}

////////////////////////////////////////////////////////////////////////////////
///
void ConfigSvc::Unregister(const std::string& module) {
    // Erasing By Key
    m_config_modules.erase(module);
}

////////////////////////////////////////////////////////////////////////////////
///
void ConfigSvc::SetDefault(const std::string& module, const std::string& unit) {
    if (IsRegistered(module)) {
        m_config_modules.at(module)->DefaultConfig(unit);
    } else {
        throw std::invalid_argument("ConfigSvc::GetStatus:: Module( " + module + " ) is not registered.");
    }
}

////////////////////////////////////////////////////////////////////////////////
///
std::shared_ptr<ConfigModule> ConfigSvc::GetConfigModule(const std::string& module){
    if (IsRegistered(module)) {
        return m_config_modules.at(module)->Config();
    }
    else {
        throw std::logic_error("ConfigSvc::GetConfigModule:: Module( \"" + module + "\" ) is not registered.");
    }
}
