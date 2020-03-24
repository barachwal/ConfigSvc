//
// Created by brachwal on 23.03.2020.
//

#ifndef CONFIGSVC_CONFIGSVC_HH
#define CONFIGSVC_CONFIGSVC_HH

#include <memory>
#include "Configurable.hh"

////////////////////////////////////////////////////////////////////////////////
///
///\class ConfigSvc
///\brief The main configuration service to manage the desired system configurable.
/// It is a singleton like pattern
// TODO:: przekierowanie ostream

class ConfigSvc {
    private:
        ///
        ConfigSvc() = default;
        
        ///
        ~ConfigSvc() = default;
    
        // Delete the copy and move constructors
        ConfigSvc(const ConfigSvc&) = delete;
        ConfigSvc& operator=(const ConfigSvc&) = delete;
        ConfigSvc(ConfigSvc&&) = delete;
        ConfigSvc& operator=(ConfigSvc&&) = delete;

        ///\brief Module name and pointer to actual module mapping.
        std::map<std::string, std::shared_ptr<Configurable>> m_config_modules;
    
    public:
        ///\brief Static method to get instance of this singleton object.
        static ConfigSvc* GetInstance();

        ///\brief The main comunication method for changing the actual value of the particular unit of a given module.
        void SetValue(const std::string& module, const std::string& unit, std::any value);

        ///\brief The main communication method for getting the actual value of the particular unit of a given module.
        template <typename T> T GetValue(const std::string& module, const std::string& unit) const;

        ///\brief The wrapper of the Configuration::GetStatus
        bool GetStatus(const std::string& module) const;

        ///\brief The wrapper of the Configuration::GetStatus
        bool GetStatus(const std::string& module, const std::string& unit) const;

        ///\brief The wrapper of the Configuration::SetStatus
        void SetStatus(const std::string& module, bool status);

        ///\brief Register new module configuraiton in the main service.
        void Register(const std::string& module, std::shared_ptr<Configurable>);

        ///\brief Check if the specified module is registered already in the service.
        bool IsRegistered(const std::string& module) const;

        ///\brief Remove from the service given module configuration.
        void Unregister(const std::string& module);

        ///\brief Function setting to defaults
        ///for a given module and its unit for which the value is defined
        void SetDefault(const std::string& module, const std::string& unit);

        ///
        std::shared_ptr<ConfigModule> GetConfigModule(const std::string& module);

};

template <typename T> T ConfigSvc::GetValue(const std::string& module, const std::string& unit) const {
    if (IsRegistered(module)) {
        return m_config_modules.at(module)->Config()->GetValue<T>(unit);
    } else {
        throw std::invalid_argument("ConfigSvc::GetValue:: Module( " + module + " ) is not defined.");
    }
}

#endif //CONFIGSVC_CONFIGSVC_HH
