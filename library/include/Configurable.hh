//
// Created by brachwal on 23.03.2020.
//

#ifndef CONFIGSVC_CONFIGURABLE_HH
#define CONFIGSVC_CONFIGURABLE_HH

#include <vector>
#include <memory>
#include "ModuleConfig.hh"

class ConfigSvc;

///\class Configurable
///\brief The base abstract class for any object that is expected to be configurable.
class Configurable {
    public:

        ///\brief Pure virtual method for defining the list of units(parameters) constituting given module.
        virtual void Configure() = 0;

        ///\brief Pure virtual method for implementing the default set of values for all units of given module.
        virtual void DefaultConfig(const std::string&) = 0;

        ///\brief Get pointer to the actual Configuration object being created for a given model.
        std::shared_ptr<ModuleConfig> Config() { return m_config; }

        ///\brief Get name of the module - configurable object
        std::string Name() const { return m_config->Name(); }

    protected:
        ///
        Configurable();

        ///
        Configurable(const std::string& name);

        ///
        virtual ~Configurable() = default;

        ///\brief List of units(parameters) constituting given module.
        std::vector<std::string> m_units;

        ///\brief Pointer to the actual Configuration object being created for a given model.
        std::shared_ptr<ModuleConfig> m_config;

        ///\brief Raw pointer to the global configuration service (singleton object).
        ConfigSvc* configSvc;
    };

#endif //CONFIGSVC_CONFIGURABLE_HH
