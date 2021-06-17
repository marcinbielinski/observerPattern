#include <iostream>
#include <list>
#include <memory>

    /* one object sometimes changes the state
    second object wants to know about the changes to the first object state

    ----------------
    Observer pattern
    ---------------- */

    /* Subscriber could be POLLING for state of a Publisher but it's generally a bad idea
    ONE to MANY is a much cleaner and logical solution.
    Subscribers are observers that first need to register to get the notifications */

class Subscriber {
public:
    virtual void updateMe (const std::string& message) = 0;
};

    /* Publisher on the other hand is an observable object (sometimes referred as a subject)
    Publisher should be responsible for telling all the Subscribers when state has changed
    PUSHING notification about the change only when state changes! */

class Publisher {
private:
    std::list<std::shared_ptr<Subscriber>> subscriberList;
    void notifyAllSubs() {
        for (auto& itr : subscriberList)
        {
            itr -> updateMe("LOW_POWER");
        }
    }

public:
    void regSubscriber(const std::shared_ptr<Subscriber>& observer) {
        subscriberList.emplace_back(observer);
    }
    void unregSubscriber(const std::shared_ptr<Subscriber>& observer) {
        subscriberList.remove(observer);
    }

    void batteryLevel(int val) {
        int BATTERY_THR = 30;
        int AUDIO_ONLY = 15;

        std::cout << "Battery level: " << val << std::endl;
        if (val <= BATTERY_THR)
        {
            notifyAllSubs();
        }
    }
};

class Display : public Subscriber {
public:
    Display() {
        name = __func__;
    }

    void updateMe(const std::string &message) override {
        std::cout << name << " :: " << message << std::endl;
    }
private:
    std::string name;
};

class Audio : public Subscriber {
public:
    Audio() {
        name = __func__;
    }

    void updateMe(const std::string &message) override {
        std::cout << name << " :: " << message << std::endl;
    }
private:
    std::string name;
};

int main() {
    // instantiating subject to register the subscribers
    std::unique_ptr<Publisher> publisher = std::make_unique<Publisher>();

    std::shared_ptr<Subscriber> display = std::make_shared<Display>();
    std::shared_ptr<Subscriber> audio = std::make_shared<Audio>();

    publisher->regSubscriber(display);
    publisher->regSubscriber(audio);

    publisher->batteryLevel(50);
    publisher->batteryLevel(40);
    publisher->batteryLevel(31);
    publisher->batteryLevel(20);

    // unregister display to give only audio notification to save more battery
    publisher->unregSubscriber(display);

    publisher->batteryLevel(15);
    publisher->batteryLevel(10);

    return 0;
}
