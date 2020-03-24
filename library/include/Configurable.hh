//
// Created by brachwal on 23.03.2020.
//

#ifndef CONFIGSVC_CONFIGURABLE_HH
#define CONFIGSVC_CONFIGURABLE_HH

#include <vector>
#include <memory>
#include "ConfigModule.hh"

class ConfigSvc;

///\class Configurable
///\brief The base abstract class for any object that is expected to be configurable.
class Configurable {
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
        std::shared_ptr<ConfigModule> m_config;

        ///\brief Raw pointer to the global configuration service (singleton object).
        ConfigSvc* m_configSvc;

    public:

        ///\brief Pure virtual method for defining the list of units(parameters) constituting given module.
        virtual void Configure() = 0;
        /// EXAMPLE OVERRIDE:
        /// void MyModuleClass::Configure() override {
        ///     SetName("MyModuleName") // once it was not given at the ctr call
        ///     m_units.emplace_back("UnitParameter1");
        ///     m_units.emplace_back("UnitParameter2");
        ///     m_units.emplace_back("UnitParameter3");
        ///     m_units.emplace_back("UnitParameter4");
        ///     ...
        ///     for (const auto& unit : m_units)
        ///          DefaultConfig(unit);   // setup the default configuration for all defined units/parameters
        ///     m_configSvc->Register(GetName(), this); // register this module to the main service
        /// }

        ///\brief Pure virtual method for implementing the default set of values for all units of given module.
        virtual void DefaultConfig(const std::string&) = 0;
        /// EXAMPLE OVERRIDE:
        /// void MyModuleClass::DefaultConfig(const std::string& unit) override {
        ///     if (unit.compare("UnitParameter1") == 0) {
        ///         m_config->SetValue(unit, float(0.2)); // or any built-in type
        ///     }
        ///     if (unit.compare("UnitParameter2") == 0) {
        ///         m_config->SetValue(unit, MyClass()); // any usr type
        ///     }
        ///     if (unit.compare("UnitParameter3") == 0) {  // smart ptrs
        ///         auto myClassSharedPtr = std::make_shared<MyClass>();    // create here
        ///         // auto myClassSharedPtr = anyOtherMyObj->GetValue();   // or take from any other place
        ///         m_config->SetValue(unit, myClassSharedPtr);
        ///     }
        ///     if (unit.compare("UnitParameter4") == 0) {  // std library objs
        ///         auto myTuple = std::make_tuple((3.8, 'A', "Lisa Simpson");
        ///         m_config->SetValue(unit, myTuple);
        ///     }
        /// }

        ///\brief Get pointer to the actual Configuration object being created for a given model.
        std::shared_ptr<ConfigModule> Config() { return m_config; }

        ///\brief Get name of the module - configurable object
        std::string GetName() const { return m_config->GetName(); }

        ///\brief Get name of the module - configurable object
        void SetName(const std::string& name ) { return m_config->SetName(name); }

    };

#endif //CONFIGSVC_CONFIGURABLE_HH
