//
// Created by brachwal on 23.03.2020.
//

#include "Configurable.hh"
#include "ConfigSvc.hh"

////////////////////////////////////////////////////////////////////////////////
///
Configurable::Configurable(const std::string& name):m_config(std::make_shared<ConfigModule>(name)),m_configSvc(ConfigSvc::GetInstance()){
    // NOTE: It is permitted to call shared_from_this only on a previously shared object.
    // The solution is to create a temporary shared_ptr while making sure it will not destroy
    // the instance when it goes out of scope. For this, you supply a lambda function
    // as a custom deleter that does nothing:
    auto wptr = std::shared_ptr<Configurable>( this, [](Configurable*){} );
    m_configSvc->Register(Config()->GetName(), shared_from_this());
}

////////////////////////////////////////////////////////////////////////////////
///
void Configurable::DefaultConfig(){
    auto units = m_config->GetUnitsNames();
    for (const auto& unit : units) DefaultConfig(unit);
}

