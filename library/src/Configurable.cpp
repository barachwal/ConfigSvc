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
    auto it = std::find (units.begin(), units.end(), "Label");
    if (it != units.end()){
        std::string message = "User's Configure implementation";
        message+=" Doesn't have to include the DefineUnit<>(\"Label\")";
        ConfigSvc::WARNING("Configurable::DefaultConfig",thisConfig()->GetName(),message);
    } else {
        thisConfig()->DefineUnit<std::string>("Label",true);
    }

    bool gotUsersLabel = false;
    for (const auto& unit : units){
        if(unit.compare("Name")!=0 && unit.compare("name")!=0){
            DefaultConfig(unit);
            if(unit.compare("Label")==0 || unit.compare("label")==0)
                gotUsersLabel = true;
        }
        else {
            std::string message = "User's DefaultConfig implementation";
            message+=" shouldn't inlcude the \"Name\" unit.";
            message+=" Consider usage of the \"Label\" instead";
            message+=" (predefined by default with the \"Name\" of given module)";
            ConfigSvc::WARNING("Configurable::DefaultConfig",thisConfig()->GetName(),message);
        }
    }

    if(!gotUsersLabel){
        std::string message = "User's DefaultConfig doesn't inlcude \"Label\"";
        message+=" it's being predefined with the \"Name\" of given module.";
        ConfigSvc::WARNING("Configurable::DefaultConfig",thisConfig()->GetName(),message);
        thisConfig()->SetValue("Label",thisConfig()->GetName());
    }

    if(!thisConfig()->IsInitialized())
        ConfigSvc::LOGIC_ERROR("Configurable::DefaultConfig",thisConfig()->GetName(),"is not fully initialized!");
}

