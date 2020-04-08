//
// Created by brachwal on 22.03.2020.
//

#include <iostream>
#include "ConfigModule.hh"
#include "ConfigSvc.hh"

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
        if(IsPublic(unit)){
            if (m_units[unit].type() == value.type()) {
                m_units.at(unit) = value;
                UnitStateUpdate(unit);
            } else
                ConfigSvc::ARGUMENT_ERROR("ConfigModule::SetValue",m_name,"Given unit (\""+ unit+"\" is of wrong type value");
        } else
            ConfigSvc::LOGIC_ERROR("ConfigModule::SetValue",m_name,"Given unit ("+ unit+") is read-only!!!");
    } else
        ConfigModule::NOT_DEFINED_UNIT_ERROR(m_name,unit);
}

////////////////////////////////////////////////////////////////////////////////
///
std::any ConfigModule::GetValue(const std::string& unit) const {
    if (IsUnitDefined(unit))
        return m_units.at(unit);
    else
        ConfigModule::NOT_DEFINED_UNIT_ERROR(m_name,unit);
}

////////////////////////////////////////////////////////////////////////////////
///
bool ConfigModule::GetStatus() const {
    for (auto status : m_units_state) {
        // if any of the parameter is modified
        if (!status.second.IsDefaultValue()) return true;
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////
///
bool ConfigModule::GetStatus(const std::string& unit) const {
    if (IsUnitDefined(unit))
        return m_units_state.at(unit).IsDefaultValue();
    else
        ConfigModule::NOT_DEFINED_UNIT_ERROR(m_name,unit);
}

////////////////////////////////////////////////////////////////////////////////
///
std::vector<std::string> ConfigModule::GetUnitsNames() const {
    std::vector<std::string> names;
    for(const auto& unit : m_units)
        names.emplace_back(unit.first);
    return names;
}

////////////////////////////////////////////////////////////////////////////////
///
void ConfigModule::Print() const {
    std::cout << "[ConfigModule]:: Configuration of the \"" << m_name << "\" module:" << std::endl;
    for(const auto& unit : m_units){
        std::cout << "[ConfigModule]:: " << unit.first << " = ";
        m_unit_streamers.at(unit.second.type())(unit.second, std::cout);
        m_units_state.at(unit.first).IsDefaultValue() ? std::cout << "\t[modified]" : std::cout << "\t[default]";
        std::cout<<std::endl;
    }
}

////////////////////////////////////////////////////////////////////////////////
///
bool ConfigModule::IsPublic(const std::string& unit) const {
    if (IsUnitDefined(unit))
        return m_units_state.at(unit).IsPublic();
    else
        ConfigModule::NOT_DEFINED_UNIT_ERROR(m_name,unit);
}

////////////////////////////////////////////////////////////////////////////////
///
void ConfigModule::UnitStateUpdate(const std::string& unit){
    if (IsUnitDefined(unit)) {
        // check if this unit was initialised to default value
        if (!m_units_state.at(unit).IsInitialized()) {
            // after first call of this method the unit is assumed being initialized
            m_units_state.at(unit).IsInitialized(true);
            // assume first call of this method by the default configuration process
            m_units_state.at(unit).IsDefaultValue(true);
        } else {
            m_units_state.at(unit).IsDefaultValue(false);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
///
bool ConfigModule::IsInitialized() const {
    for(const auto& unit : m_units_state)
        if (!unit.second.IsInitialized()) return false;
    return true;
}

////////////////////////////////////////////////////////////////////////////////
///
void ConfigModule::NOT_DEFINED_UNIT_ERROR(const std::string& module, const std::string& unit){
    ConfigSvc::ARGUMENT_ERROR("ConfigModule::IsPublic",module,"Given unit (" + unit + ") is not defined.");
}