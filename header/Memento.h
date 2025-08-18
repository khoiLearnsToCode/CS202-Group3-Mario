#pragma once

#include <iostream>
#include <string>
#include <ctime>
#include "Data.h"


class Memento {
    public:
    virtual ~Memento() = default;
    virtual std::string display() const = 0;
    virtual Data getData() const = 0;
    virtual std::string getDate() const = 0;
    virtual std::string getTime() const = 0;
};

class ConcreteMemento : public Memento {
    private:
    Data data;
    std::string date;
    std::string time; 

    public:
    ConcreteMemento(const Data& data);
    ConcreteMemento::ConcreteMemento(const Data& data, const std::string& date, const std::string& time);
    ConcreteMemento(const Data& data, const std::string& date);

    std::string display() const override;
    Data getData() const override;
    std::string getDate() const override;
    std::string getTime() const override;
};