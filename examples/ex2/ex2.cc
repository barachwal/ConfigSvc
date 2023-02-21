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
        DefineUnit<char>("Type");
        DefineUnit<int>("SpareTireNumber");

        Configurable::DefaultConfig();   // setup the default configuration for all defined units/parameters
        Configurable::PrintConfig();
    }
    void DefaultConfig(const std::string& unit) override {
        // if (unit.compare("Label") == 0) {
        //     m_config->SetValue(unit, std::string("This is very nice car"));
        // }
        if (unit.compare("Color") == 0) {
            m_config->SetValue(unit, std::string("White"));
        }
        if (unit.compare("Type") == 0) {
            m_config->SetValue(unit, char('C'));
        }
        if (unit.compare("SpareTireNumber") == 0) {  // smart ptrs
            m_config->SetValue(unit, int(2));
        }
    }
};

int main(){
    std::cout << "[Ex1]:: Example 1" << std::endl;
    auto myCar = SuperCar("Bolid"); // it's already exists int the ConfigSvc!

    std::cout << std::endl;
    auto configSvc = ConfigSvc::GetInstance();
    std::string ex_file = EXAMPLES;
    ex_file+="/ex2/super_car.toml";
    // std::cout << ex_file <<  std::endl;

    configSvc->ParseTomlFile(ex_file);
    configSvc->PrintTomlConfig();
    // std::cout << "[Ex1]:: myCar color       : "
    //           << configSvc->GetValue<std::string>("Bolid","Color") << std::endl;
    // std::cout << "[Ex1]:: myCar type        : "
    //           << configSvc->GetValue<char>("Bolid","Type") << std::endl;
    // std::cout << "[Ex1]:: myCar #spare tires: "
    //           << configSvc->GetValue<int>("Bolid","SpareTireNumber") << std::endl;

    // std::cout << std::endl;
    // configSvc->SetValue("Bolid","Color",std::string("Red"));
    // configSvc->GetConfigModule("Bolid")->Print();
}
