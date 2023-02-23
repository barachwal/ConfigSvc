//
// Created by barachwal on 22.03.2020.
//

#ifndef CONFIGSVC_CONFIGMODULE_HH
#define CONFIGSVC_CONFIGMODULE_HH

#include <ostream>
#include <typeindex>
#include <string>
#include <sstream>
#include <any>
#include <map>
#include <tuple>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include "UnitState.hh"
#include "toml.hh"

class Configurable;

////////////////////////////////////////////////////////////////////////////////
///
///\class Configuration
///\brief The single module-like configuration class definition.
///The ConfigModule instance is intended to keep series of units and their value information.
class ConfigModule {
    private:
        ///
        std::string m_name = "NoName";

        /// \brief Unit and its value mapping
        std::map<std::string, std::any> m_units;

        /// \brief Store for each unit its type
        std::unordered_map<std::type_index, void(*)(std::any const&, std::ostream&)> m_unit_streamers;

        /// \brief Store for keep track series of status flag
        std::map<std::string, UnitState> m_units_state;

        ///
        toml::parse_result* m_toml_config;

        ///
        bool m_toml = false;

        /// \brief Get value for a single unit
        std::any GetValue(const std::string& unit) const;

        ///
        template<class T> void AddUnitStreamer();

        ///
        bool IsPublic(const std::string& unit) const;

        ///
        void UnitStateUpdate(const std::string& unit);

        /// \brief Get status of the value initialization for all units for the given module.
        bool IsInitialized() const;
        
        ///
        bool IsInitialized(const std::string& unit) const;

        ///
        static void NOT_DEFINED_UNIT_ERROR(const std::string& module, const std::string& unit);

        ///
        friend class Configurable;

    public:
        ///
        ConfigModule() = default;

        ///
        ConfigModule(const std::string& name);

        ///
        ~ConfigModule() = default;

        /// \brief Check if the specified unit already exists in Configuration instance.
        bool IsUnitDefined(const std::string& unit) const;

        ///
        template <typename T> void DefineUnit(const std::string& unit, bool isPublic=true);

        /// \brief Set value for a single unit
        void SetValue(const std::string& unit, std::any value);

        /// \brief Check and set value from TOML file if it's loaded
        template <typename T> void SetTValue(const std::string& unit, std::any value);

        /// \brief Check the global status of a given module configuration
        /// Whether it's different from the default configuration.
        bool GetStatus() const;

        /// \brief Check the unit-like status of a given module configuration.
        bool GetStatus(const std::string& unit) const;

        ///
        std::string GetName() const { return m_name; }

        ///
        std::vector<std::string> GetUnitsNames() const;

        ///
        void SetName(const std::string& name) { m_name=name; }

        ///\brief The direct method for getting the actual value of the particular unit.
        template <typename T> T GetValue(const std::string& unit) const;

        ///
        void Print() const;

        ///
        void SetTomlConfig();
};
#include <iostream>

template <typename T> void ConfigModule::SetTValue(const std::string& unit, std::any value){
    if(IsUnitDefined(unit) && IsPublic(unit) ){
        if(!IsInitialized(unit))
            SetValue(unit,value); // Set default value first
        if(m_toml){
            if((*m_toml_config)[m_name][unit].is_value()){  // found value in TOML file
                auto toml_value = (*m_toml_config)[m_name][unit].value_or<T>(T());
                SetValue(unit,toml_value); // Set again with new value
            }

        }
    }
}

template <typename T> T ConfigModule::GetValue(const std::string& unit) const {
    if (IsUnitDefined(unit)) {
        return std::any_cast<T>(GetValue(unit));
    } else {
        // can't use here ConfigSvc::ARGUMENT_ERROR (circular dependency)
        throw std::invalid_argument("ConfigModule::GetValue:: "
                                    "Module( " + m_name + " ): Given unit (" + unit + ") is not defined.");
    }
}

template <typename T> void ConfigModule::DefineUnit(const std::string& unit, bool isPublic){
    m_units.insert(std::make_pair(unit,T()));
    m_units_state.insert(std::make_pair(unit,UnitState()));
    m_units_state.at(unit).IsInitialized(false);
    if(isPublic) m_units_state.at(unit).IsPublic(isPublic);
    AddUnitStreamer<T>();
}

template<class T> void ConfigModule::AddUnitStreamer() {
    if(m_unit_streamers.find(typeid(T)) == m_unit_streamers.end()) {
        m_unit_streamers[typeid(T)] = [](std::any const &a, std::ostream &os) {
            os << std::any_cast<T>(a);
        };
    }
}

#endif //CONFIGSVC_CONFIGMODULE_HH
