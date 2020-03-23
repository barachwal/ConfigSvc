//
// Created by brachwal on 22.03.2020.
//

#include "ModuleConfig.hh"
#include  <stdexcept>

////////////////////////////////////////////////////////////////////////////////
///
bool ModuleConfig::IsDefined(const std::string& unit) {
    return m_unit.find(unit) != m_unit.end() ? true : false;
}

////////////////////////////////////////////////////////////////////////////////
///
void ModuleConfig::SetValue(const std::string& unit, std::any value) {
    if (IsDefined(unit)) {
        if (m_unit[unit].type() != value.type()) {
            throw std::invalid_argument("ModuleConfig::SetValue::"
                                        "Module( " +m_name+ " ): "
                                        "Given unit ("+unit+") is of wrong type value");
        } else {
            m_unit.at(unit) = value;
            m_status.at(unit) = true;
        }
    } else {
        // Define and register new unit for this module
        m_unit[unit] = value;
        m_status[unit] = false;
    }
}

////////////////////////////////////////////////////////////////////////////////
///
std::any ModuleConfig::GetValue(const std::string& unit) {
    if (IsDefined(unit)) {
        return m_unit.at(unit);
    } else {
        throw std::invalid_argument("ModuleConfig::GetValue::"
                                    "Module( " + m_name + " ): "
                                    "Given unit (" + unit + ") is not defined.");
    }
}

////////////////////////////////////////////////////////////////////////////////
///
bool ModuleConfig::GetStatus() {
    for (auto status : m_status) {
        if (status.second) return true;  // if any of the parameter is modified
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////
///
bool ModuleConfig::GetStatus(const std::string& unit) {
    if (IsDefined(unit)) {
        return m_status.at(unit);
    } else {
        throw std::invalid_argument("ModuleConfig::GetValue::"
                                    "Module( " + m_name + " ): "
                                    "Given unit (" + unit + ") is not defined.");
    }
}

////////////////////////////////////////////////////////////////////////////////
///
void ModuleConfig::SetStatus(bool status_new) {
    for (auto status : m_status) {
        status.second = status_new;
    }
}