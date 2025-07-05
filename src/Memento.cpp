#include "Memento.h"

ConcreteMemento::ConcreteMemento(const Data& data) : data(data) {
    std::time_t now = std::time(nullptr);
    date = std::ctime(&now);
}

ConcreteMemento::ConcreteMemento(const Data& data, const std::string& date) : 
                                                    data(data), date(date) {}

std::string ConcreteMemento::display() const {
    return "Map ID: " + std::to_string(data.mapID) + "\n" +
           "Remaining Time: " + std::to_string(data.remainingTime) + "\n" +
           "Score: " + std::to_string(data.score) + "\n" +
           "Lives: " + std::to_string(data.lives) + "\n" +
           "Date: " + date;
}

Data ConcreteMemento::getData() const {
    return data;
}

std::string ConcreteMemento::getDate() const {
    return date;
}