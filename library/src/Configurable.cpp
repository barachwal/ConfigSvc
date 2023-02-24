//
// Created by brachwal on 23.03.2020.
//

#include "Configurable.hh"
#include "ConfigSvc.hh"
#include <algorithm>

////////////////////////////////////////////////////////////////////////////////
///
Configurable::Configurable(const std::string& name):m_config(std::make_shared<ConfigModule>(name)),m_configSvc(ConfigSvc::GetInstance()){
    // NOTE: It is permitted to call shared_from_this only on a previously shared object.
    // The solution is to create a temporary shared_ptr while making sure it will not destroy
    // the instance when it goes out of scope. For this, you supply a lambda function
    // as a custom deleter that does nothing:
    auto wptr = std::shared_ptr<Configurable>( this, [](Configurable*){} );
    m_configSvc->Register(thisConfig()->GetName(), shared_from_this());
}

////////////////////////////////////////////////////////////////////////////////
///
void Configurable::DefaultConfig(){
    auto units = thisConfig()->GetUnitsNames();

    // User's configuration implementation doesn't have to include the DefineUnit<>(\"Label\")"
    // but it is being initialized by default to be same as name of given module.
    // The 'Label' unit is being used in number of contexts...
    auto it = std::find (units.begin(), units.end(), "Label");
    if (it == units.end()){
        thisConfig()->DefineUnit<std::string>("Label",true);
    } 
    units = thisConfig()->GetUnitsNames();
    for (const auto& unit : units){
        if(unit.compare("Name")!=0 && unit.compare("name")!=0){
            DefaultConfig(unit);
        }
        else {
            std::string message = "User's DefaultConfig implementation";
            message+=" can't inlcude the \"Name\" unit.";
            message+=" Consider usage of the \"Label\" instead";
            message+=" (predefined by default with the \"Name\" of given module)";
            ConfigSvc::WARNING("Configurable::DefaultConfig",thisConfig()->GetName(),message);
        }
    }

    if(!thisConfig()->IsInitialized("Label"))
        thisConfig()->SetValue("Label",thisConfig()->GetName());

    /*if(!gotUsersLabel){
        std::string message = "User's DefaultConfig doesn't inlcude \"Label\"";
        message+=" it's being predefined with the \"Name\" of given module.";
        ConfigSvc::WARNING("Configurable::DefaultConfig",thisConfig()->GetName(),message);
        
    }*/

    if(!thisConfig()->IsInitialized())
        ConfigSvc::LOGIC_ERROR("Configurable::DefaultConfig",thisConfig()->GetName(),"is not fully initialized!");
}

