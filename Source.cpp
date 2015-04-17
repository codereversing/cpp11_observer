#include <iostream>
#include "Observer.h"

enum class EventTypes
{
    EventTypeFirst = 0,
    EventTypeSecond
};

struct DispatchArgs
{
    int i;
};

class DispatcherNoArgs : public Observable<EventTypes>
{
public:
    DispatcherNoArgs() = default;
    ~DispatcherNoArgs() = default;

    void Send()
    {
        Notify(EventTypes::EventTypeFirst);
    }
};

class DispatcherArgs : public Observable<EventTypes, DispatchArgs &>
{
public:
    DispatcherArgs() = default;
    ~DispatcherArgs() = default;

    void Send()
    {
        DispatchArgs args{ 123 };
        Notify(EventTypes::EventTypeSecond, args);
    }
};

void DispatchNoArgsReceiver()
{
    std::cout << "DispatchNoArgsReceiver (function) called " << std::endl;
}

void DispatchArgsReceiver(DispatchArgs &args)
{
    std::cout << "DispatchArgsReceiver called -- args: " << args.i << std::endl;
}

int main()
{
    DispatcherNoArgs noArgs;
    auto functionInfo1 = noArgs.Register(EventTypes::EventTypeFirst, DispatchNoArgsReceiver);
    auto functionInfo2 = noArgs.Register(EventTypes::EventTypeFirst, [] { std::cout << "DispatchNoArgsReceiver (lambda) called " << std::endl; });

    noArgs.Send(); //Calls DispatchNoArgsReceiver and lambda
    noArgs.Remove(functionInfo1);
    noArgs.Send(); //Calls just lambda

    DispatcherArgs args;
    auto functionInfo3 = args.Register(EventTypes::EventTypeSecond, std::bind(DispatchArgsReceiver, std::placeholders::_1));
    args.Send(); //Calls bound function

    return 0;
}