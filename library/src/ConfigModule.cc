//
// Created by brachwal on 22.03.2020.
//

#include "ConfigModule.hh"

////////////////////////////////////////////////////////////////////////////////
///
bool ConfigModule::IsDefined(const std::string& unit) const {
    return m_unit.find(unit) != m_unit.end() ? true : false;
}

////////////////////////////////////////////////////////////////////////////////
///
void ConfigModule::SetValue(const std::string& unit, std::any value) {
    if (IsDefined(unit)) {
        if (m_unit[unit].type() != value.type()) {
            throw std::invalid_argument("ConfigModule::SetValue::"
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
std::any ConfigModule::GetValue(const std::string& unit) const {
    if (IsDefined(unit)) {
        return m_unit.at(unit);
    } else {
        throw std::invalid_argument("ConfigModule::GetValue::"
                                    "Module( " + m_name + " ): "
                                    "Given unit (" + unit + ") is not defined.");
    }
}

////////////////////////////////////////////////////////////////////////////////
///
bool ConfigModule::GetStatus() const {
    for (auto status : m_status) {
        if (status.second) return true;  // if any of the parameter is modified
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////
///
bool ConfigModule::GetStatus(const std::string& unit) const {
    if (IsDefined(unit)) {
        return m_status.at(unit);
    } else {
        throw std::invalid_argument("ConfigModule::GetValue::"
                                    "Module( " + m_name + " ): "
                                    "Given unit (" + unit + ") is not defined.");
    }
}

////////////////////////////////////////////////////////////////////////////////
///
void ConfigModule::SetStatus(bool status_new) {
    for (auto status : m_status) {
        status.second = status_new;
    }
}