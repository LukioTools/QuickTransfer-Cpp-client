#include <iostream>
#include <libnotify/notify.h>
#include "./Libs/notification.hpp"

void callback_acc(NotifyNotification* n, char* t)
{
    std::cout << "Accepting file" << std::endl;
}

void callback_closing()
{
    std::cout << "Accepting file" << std::endl;
}


int main(int argc, char *argv[])
{

    if (!notify_init("Test Notification")) {
        std::cerr << "Failed to initialize libnotify" << std::endl;
        return 1;
    }

    Notification::Notification notification(std::string(""),std::string(""),std::string(""), callback_closing);
    notification.setTimeout(10000);
    notification.AddButton("lol", "lol", callback_acc);
    notification.ShowNotification();
    notify_uninit();
    return 0;
}
