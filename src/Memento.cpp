#include "Memento.h"
#include <sstream>
#include <iomanip>

ConcreteMemento::ConcreteMemento(const Data& data) : data(data) {
    std::time_t now = std::time(nullptr);
    std::stringstream date_ss;
    date_ss << std::put_time(std::localtime(&now), "%Y-%m-%d");
    date = date_ss.str();
    time = "";
}

ConcreteMemento::ConcreteMemento(const Data& data, const std::string& date) : 
                                                    data(data), date(date) {}

ConcreteMemento::ConcreteMemento(const Data& data, const std::string& date, const std::string& time)
    : data(data), date(date), time(time) {}

std::string ConcreteMemento::display() const {
    return "Map ID: " + std::to_string(data.mapID) + "\n" +
           "Score: " + std::to_string(data.score) + "\n" +
           "Lives: " + std::to_string(data.lives) + "\n" +
           "Date: " + date + "\n" +
           "Time: " + time + "\n";
}

Data ConcreteMemento::getData() const {
    return data;
}

std::string ConcreteMemento::getDate() const {
    return date;
}

std::string ConcreteMemento::getTime() const {
    return time;
}