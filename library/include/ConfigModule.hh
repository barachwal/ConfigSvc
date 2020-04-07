//
// Created by barachwal on 22.03.2020.
//

#ifndef CONFIGSVC_CONFIGMODULE_HH
#define CONFIGSVC_CONFIGMODULE_HH

#include <ostream>
#include <typeindex>
#include <string>
#include <any>
#include <map>
#include <tuple>
#include <vector>
#include <unordered_map>
#include <stdexcept>

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

        /// \brief Store for keep track series of status flag (easly extensible)
        /// vector[0] - configuration status: false: unchanged (default value)
        ///                                    true: changed   (modified from default)
        /// vector[1] - unit initialisation status
        /// vector[2] - unit global scope status (is it private or not)
        std::map<std::string, std::vector<bool>> m_unit_status;

        /// \brief Status of the configuration.
        /// false: unchanged (default value)
        /// true: changed   (modified from default)
        std::map<std::string, bool> m_status_of_units;

        ///
        std::map<std::string, bool> m_status_of_units_initialisation;

        /// \brief Get value for a single unit
        std::any GetValue(const std::string& unit) const;

        ///
        template<class T> void AddUnitStreamer();

        ///
        void SetGlobalScope(const std::string& unit, bool value){}

        ///
        void IsGlobal(const std::string& unit){}

        /// \brief Change status of the current value for specific unit the given module.
        void SetStatus(const std::string& unit, bool status);

        /// \brief Change status of the value initialization for specific unit for the given module.
        void SetInitializationStatus(const std::string& unit, bool status);

        /// \brief Get status of the value initialization for specific unit for the given module.
        bool IsInitialized(const std::string& unit) const;

        /// \brief Get status of the value initialization for all units for the given module.
        bool IsInitialized() const;

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
        template <typename T> void DefineUnit(const std::string& unit);

        /// \brief Set value for a single unit
        void SetValue(const std::string& unit, std::any value);

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
};

template <typename T> T ConfigModule::GetValue(const std::string& unit) const {
    if (IsUnitDefined(unit)) {
        return std::any_cast<T>(GetValue(unit));
    } else {
        throw std::invalid_argument("ConfigModule::GetValue:: "
                                    "Module( " + m_name + " ): Given unit (" + unit + ") is not defined.");
    }
}

template <typename T> void ConfigModule::DefineUnit(const std::string& unit){
    m_units.insert(std::make_pair(unit,T()));
    m_status_of_units.insert(std::make_pair(unit,false));
    m_status_of_units_initialisation.insert(std::make_pair(unit,false));
    m_unit_status.insert(std::make_pair(unit,std::vector<bool>{false,false,false}));
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
