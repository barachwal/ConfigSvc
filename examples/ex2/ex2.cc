//
// Created by barachwal on 21.02.2023.
//

#include <iostream>
#include "ConfigSvc.hh"

class SuperCar : public Configurable {
public:
    SuperCar(const std::string& name):Configurable(name){Configure();}
    ~SuperCar() = default;
    void Configure() override {
        DefineUnit<std::string>("Color");
        DefineUnit<std::string>("Type");
        DefineUnit<int>("SpareTireNumber");

        Configurable::DefaultConfig();   // setup the default configuration for all defined units/parameters
        // Configurable::PrintConfig();
    }
    void DefaultConfig(const std::string& unit) override {
        // if (unit.compare("Label") == 0) {
        //     m_config->SetValue(unit, std::string("This is very nice car"));
        // }
        if (unit.compare("Color") == 0) {
            m_config->SetTValue<std::string>(unit, std::string("White"));
        }
        if (unit.compare("Type") == 0) {
            m_config->SetTValue<std::string>(unit, std::string("C"));
        }
        if (unit.compare("SpareTireNumber") == 0) {
            m_config->SetTValue<int>(unit, int(2));
        }
    }
    bool ValidateConfig() const override {
        auto stn = thisConfig()->GetValue<int>("SpareTireNumber");
        if(stn<3){
            std::cout << "[SuperCar]:: To less spare tires in the care!!!" << std::endl;
            return false;
        }
        return true;
    }
};

int main(){
    std::cout << "[Ex2]:: Example 2" << std::endl;

    std::cout << std::endl;
    auto configSvc = ConfigSvc::GetInstance();  // we can initialize ConfigSvc and pars TOML file
                                                // before the given module definition and default configuration happen
    std::string ex_file = EXAMPLES;
    ex_file+="/ex2/super_car.toml";
    configSvc->ParseTomlFile(ex_file);
    // configSvc->PrintTomlConfig();

    auto myCar = SuperCar("Bolid"); // Once the configuration is called the TOML config is being checked for "Bolid" module
    std::cout << std::endl;
    // configSvc->GetConfigModule("Bolid")->Print();
    std::cout << std::endl;
    // configSvc->ParseTomlFile(ex_file);       // or we can parse TOML after the given module is instatiant and configured
    configSvc->GetConfigModule("Bolid")->Print();
    configSvc->ValidateConfiguration();


}
