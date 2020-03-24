//
// Created by brachwal on 22.03.2020.
//

#include <typeinfo>
#include <iostream>
#include "ConfigModule.hh"

////////////////////////////////////////////////////////////////////////////////
///
ConfigModule::ConfigModule(const std::string& name): m_name(name){}


////////////////////////////////////////////////////////////////////////////////
///
bool ConfigModule::IsUnitDefined(const std::string& unit) const {
    return m_units.find(unit) != m_units.end() ? true : false;
}

////////////////////////////////////////////////////////////////////////////////
///
void ConfigModule::SetValue(const std::string& unit, std::any value) {
    if (IsUnitDefined(unit)) {
        if (m_units[unit].type() != value.type()) {
            throw std::invalid_argument("ConfigModule::SetValue::"
                                        "Module( "+m_name+" ): Given unit ("+unit+") is of wrong type value");
        } else {
            m_units.at(unit) = value;
            if(m_status_of_units_initialisation.at(unit)) // check if this unit was initialised to default value
                m_status_of_units.at(unit) = true;
            else
                m_status_of_units_initialisation.at(unit) = true;
        }
    } else {
        throw std::invalid_argument("ConfigModule::SetValue::"
                                    "Module( "+m_name+" ): Given unit ("+unit+") is not defined");
    }
}

////////////////////////////////////////////////////////////////////////////////
///
std::any ConfigModule::GetValue(const std::string& unit) const {
    if (IsUnitDefined(unit)) {
        return m_units.at(unit);
    } else {
        throw std::invalid_argument("ConfigModule::GetValue::"
                                    "Module( " + m_name + " ): "
                                    "Given unit (" + unit + ") is not defined.");
    }
}

////////////////////////////////////////////////////////////////////////////////
///
bool ConfigModule::GetStatus() const {
    for (auto status : m_status_of_units) {
        if (status.second) return true;  // if any of the parameter is modified
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////
///
bool ConfigModule::GetStatus(const std::string& unit) const {
    if (IsUnitDefined(unit)) {
        return m_status_of_units.at(unit);
    } else {
        throw std::invalid_argument("ConfigModule::GetValue::"
                                    "Module( " + m_name + " ): "
                                    "Given unit (" + unit + ") is not defined.");
    }
}

////////////////////////////////////////////////////////////////////////////////
///
void ConfigModule::SetStatus(bool status_new) {
    for (auto status : m_status_of_units) {
        status.second = status_new;
    }
}

////////////////////////////////////////////////////////////////////////////////
///
std::vector<std::string> ConfigModule::GetUnitsNames(){
    std::vector<std::string> names;
    for(const auto& unit : m_units){
        names.emplace_back(unit.first);
    }
    return names;
}

////////////////////////////////////////////////////////////////////////////////
///
void ConfigModule::Print() const {
    std::cout << "[ConfigModule]:: Configuration of the \"" << m_name << "\" module:" << std::endl;
    for(const auto& unit : m_units){
        std::cout << "[ConfigModule]:: " << unit.first << " = ";
        m_unit_streamers.at(unit.second.type())(unit.second, std::cout);
        m_status_of_units.at(unit.first) ? std::cout << "\t[modified]" : std::cout << "\t[default]";
        std::cout<<std::endl;
    }
}