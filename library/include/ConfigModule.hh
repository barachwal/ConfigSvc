//
// Created by barachwal on 22.03.2020.
//

#ifndef CONFIGSVC_CONFIGMODULE_HH
#define CONFIGSVC_CONFIGMODULE_HH

#include <any>
#include <map>
#include  <stdexcept>

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
        std::map<std::string, std::any> m_unit;

        /// \brief Status of the configuration.
        /// false: unchanged
        /// true: changed
        std::map<std::string, bool> m_status;

        /// \brief Get value for a single unit
        std::any GetValue(const std::string& unit) const;

    public:
        ///
        ConfigModule() = default;

        ///
        ConfigModule(const std::string& name): m_name(name){};

        ///
        ~ConfigModule() = default;

        /// \brief Check if the specified unit already exists in Configuration instance.
        bool IsDefined(const std::string& unit) const;

        /// \brief Set value for a single unit
        void SetValue(const std::string& unit, std::any value);

        /// \brief Check the global status of a given module configuration
        /// Whether it's different from the default configuration.
        bool GetStatus() const;

        /// \brief Check the unit-like status of a given module configuration.
        bool GetStatus(const std::string& unit) const;

        /// \brief Change global status for the given module.
        void SetStatus(bool status_new);

        ///
        std::string GetName() const { return m_name; }

        ///
        void SetName(const std::string& name) { m_name=name; }

        ///\brief The direct method for getting the actual value of the particular unit.
        template <typename T> T GetValue(const std::string& unit) const {
            return std::any_cast<T>(GetValue(unit));
        }

};

#endif //CONFIGSVC_CONFIGMODULE_HH
