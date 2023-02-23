//
// Created by brachwal on 23.03.2020.
//

#include <iostream>
#include "ConfigSvc.hh"
#include "colors.hh"

///////////////////////////////////////////////////////////////////////////////
///
ConfigSvc* ConfigSvc::GetInstance() {
    static ConfigSvc instance;
    return &instance;
}

////////////////////////////////////////////////////////////////////////////////
///
void ConfigSvc::LOGIC_ERROR(const std::string& caller, const std::string& module, const std::string& message){
    throw std::logic_error(caller+": Module( \""+module+"\"): "+message);
}

////////////////////////////////////////////////////////////////////////////////
///
void ConfigSvc::ARGUMENT_ERROR(const std::string& caller, const std::string& module, const std::string& message){
    throw std::invalid_argument(caller+": Module( \""+module+"\"): "+message);
}

////////////////////////////////////////////////////////////////////////////////
///
void ConfigSvc::NOT_REGISTERED_MODULE_ERROR(const std::string& caller, const std::string& module){
    throw std::invalid_argument(caller+": Module \""+module+"\": is not registered.");
}

////////////////////////////////////////////////////////////////////////////////
///
void ConfigSvc::WARNING(const std::string& caller, const std::string& module, const std::string& message){
    ConfigSvc::WARNING(caller+": Module \""+module+"\": "+message);
}

////////////////////////////////////////////////////////////////////////////////
///
void ConfigSvc::WARNING(const std::string& message){
    std::cout << "[ConfigSvc]"<< FYEL("[WARNING]")<<"::"<< message << std::endl;
}

////////////////////////////////////////////////////////////////////////////////
///
void ConfigSvc::ERROR(const std::string& message){
    std::cout << "[ConfigSvc]"<< FRED("[ERROR]")<<"::"<< message << std::endl;
}

////////////////////////////////////////////////////////////////////////////////
///
void ConfigSvc::INFO(const std::string& message){
    std::cout << "[ConfigSvc]"<< FGRN("[INFO]")<<"::"<< message << std::endl;
}

////////////////////////////////////////////////////////////////////////////////
///
bool ConfigSvc::IsRegistered(const std::string& module) const {
    return m_config_modules.find(module) != m_config_modules.end() ? true : false;
}

////////////////////////////////////////////////////////////////////////////////
///
void ConfigSvc::SetValue(const std::string& module, const std::string& unit, std::any value) {
    if (!IsRegistered(module))
        ConfigSvc::NOT_REGISTERED_MODULE_ERROR("ConfigSvc::SetValue",module);
    m_config_modules.at(module)->thisConfig()->SetValue(unit, value);
}

////////////////////////////////////////////////////////////////////////////////
///
bool ConfigSvc::GetStatus(const std::string& module) const {
    if (!IsRegistered(module))
        ConfigSvc::NOT_REGISTERED_MODULE_ERROR("ConfigSvc::GetStatus",module);
    return m_config_modules.at(module)->thisConfig()->GetStatus();    
}

////////////////////////////////////////////////////////////////////////////////
///
bool ConfigSvc::GetStatus(const std::string& module, const std::string& unit) const {
    if (!IsRegistered(module))
        ConfigSvc::NOT_REGISTERED_MODULE_ERROR("ConfigSvc::GetStatus",module);
    return m_config_modules.at(module)->thisConfig()->GetStatus(unit);
}

////////////////////////////////////////////////////////////////////////////////
///
void ConfigSvc::Register(const std::string& module, std::shared_ptr<Configurable> config_volume) {
    if (IsRegistered(module)) {
        // Each module name should be unique
        ConfigSvc::LOGIC_ERROR("ConfigSvc::Register",module," this module is already registered.");
    } else {
        ConfigSvc::INFO("Register new module : \""+module+"\"");
        m_config_modules.insert(make_pair(module, config_volume));
    }
}

////////////////////////////////////////////////////////////////////////////////
///
void ConfigSvc::Unregister(const std::string& module) {
    // Erasing By Key
    m_config_modules.erase(module);
}

////////////////////////////////////////////////////////////////////////////////
///
void ConfigSvc::SetDefault(const std::string& module, const std::string& unit) {
    if (!IsRegistered(module))
        ConfigSvc::NOT_REGISTERED_MODULE_ERROR("ConfigSvc::GetStatus",module);
    m_config_modules.at(module)->DefaultConfig(unit);
}

////////////////////////////////////////////////////////////////////////////////
///
std::shared_ptr<ConfigModule> ConfigSvc::GetConfigModule(const std::string& module){
    if (!IsRegistered(module))
        ConfigSvc::NOT_REGISTERED_MODULE_ERROR("ConfigSvc::GetStatus",module);
    return m_config_modules.at(module)->thisConfig();
}

////////////////////////////////////////////////////////////////////////////////
///
toml::parse_result& ConfigSvc::ParseTomlFile(const std::string& file){
    m_toml_config = toml::parse_file(file);
    m_toml = true;
    if(!m_config_modules.empty())
        ReloadConfiguration();
}

////////////////////////////////////////////////////////////////////////////////
///
void ConfigSvc::PrintTomlConfig() const{
    if(m_toml){
        ConfigSvc::INFO("PrintTomlConfig...");
        //std::cout << m_toml_config << "\n";
        std::cout << toml::json_formatter{ m_toml_config } << "\n"; // re-serialize as JSON
    }
    else {
        std::cout << "[ConfigSvc]"<<FYEL("[WARNING]")<<":: PrintTomlConfig : any file was parsed.."<< std::endl;
    }
}

////////////////////////////////////////////////////////////////////////////////
///
void ConfigSvc::ReloadConfiguration()const{
    ConfigSvc::INFO(" Reload configuration...");
    for(auto& module : m_config_modules){
        module.second->thisConfig()->SetTomlConfig();
        module.second->DefaultConfig();
    }
}