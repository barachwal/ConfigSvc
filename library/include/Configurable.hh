//
// Created by brachwal on 23.03.2020.
//

#ifndef CONFIGSVC_CONFIGURABLE_HH
#define CONFIGSVC_CONFIGURABLE_HH

#include <memory>
#include "ConfigModule.hh"

class ConfigSvc;

///\class Configurable
///\brief The base abstract class for any object that is expected to be configurable.
class Configurable : public std::enable_shared_from_this<Configurable> {
    private:

    protected:
        ///
        Configurable() = delete;

        ///
        explicit Configurable(const std::string& name);

        ///
        virtual ~Configurable() = default;

        /// Simple wrapper to  ConfigModule::DefineUnit
        template <typename T> void DefineUnit(const std::string&, bool isPublic=true);

        ///\brief Pointer to the actual Configuration object being created for a given model.
        std::shared_ptr<ConfigModule> m_config;

        ///\brief Raw pointer to the global configuration service (singleton object).
        ConfigSvc* m_configSvc;

    public:

        ///\brief Pure virtual method for defining the list of units(parameters) constituting given module.
        virtual void Configure() = 0;
        /// EXAMPLE OVERRIDE:
        /// void MyModuleClass::Configure() override {
        ///     DefineUnit<float>("UnitParameter1");
        ///     DefineUnit<MyClass>("UnitParameter2");
        ///     DefineUnit<std::shared_ptr<MyClass>>("UnitParameter3");
        ///     DefineUnit<std::tuple<double, char, std::string>>("UnitParameter4");
        ///     ...
        ///     // The following have to be called from here!
        ///     Configurable::DefaultConfig();   // setup the default configuration for all defined units/parameters
        ///     Configurable::PrintConfig();     // print or not to print...
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

        ///\brief Call default configuration for all defined units
        void DefaultConfig();

        /// /// Simple wrapper to  ConfigModule::DefineUnit
        void PrintConfig() const { m_config->Print(); }

        ///\brief Get pointer to the actual Configuration object being created for a given module.
        std::shared_ptr<ConfigModule> thisConfig() { return m_config; }

        ///
        ConfigSvc* configSvc() const { return m_configSvc; }
};

template <typename T> void Configurable::DefineUnit(const std::string& unit, bool isPublic){
    m_config->DefineUnit<T>(unit,isPublic);
}

#endif //CONFIGSVC_CONFIGURABLE_HH