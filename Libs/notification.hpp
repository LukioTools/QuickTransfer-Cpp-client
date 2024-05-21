#pragma once

#include "glib.h"
#include <cstddef>
#include <functional>
#include <iostream>
#include <libnotify/notification.h>
#include <libnotify/notify.h>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace Notification {

    class Button {
        std::string action;
        std::string label;
        NotifyNotification *notification;
        GMainLoop *loop;
    public:

        using Func = std::function<void(NotifyNotification*, char*)>;
        struct userdata{
            GMainLoop* loop;
            Func fn;
            userdata(GMainLoop* ml, Func f):loop(ml), fn(std::move(f)){};
        };
        static void callback(NotifyNotification *n, char *action, gpointer user_data) {
            auto ud = std::unique_ptr<userdata>(reinterpret_cast<userdata*>(user_data));
            g_main_loop_quit(ud->loop);
            ud->fn(n, action);
            //std::cout << (void*) user_data << std::endl;
            //auto t = reinterpret_cast<Button*>(user_data);
            //std::cout << t->label << std::endl;
        }

        Button(NotifyNotification *noti, std::string act, std::string lab, Func f, GMainLoop *l)
            : action(std::move(act)), notification(noti), label(std::move(lab)), loop(l) {
            auto ptr = new userdata(l,f);
            notify_notification_add_action(notification,
                                           action.c_str(), // Action name
                                           label.c_str(),  // Label for the action
                                           callback,
                                           ptr,           // User data
                                           nullptr);       // Free func
        }
    };

    class Notification {
    private:
        std::string name;
        std::string title;
        std::string message;

        NotifyNotification *notification;
        std::vector<Button> btns;
        GMainLoop *loop;
        void (*finalCallback)();

        static void on_notification_closed(NotifyNotification *n, gpointer user_data) {
            Notification *notif = static_cast<Notification*>(user_data);
            if (notif->loop) {
                g_main_loop_quit(notif->loop);
            }
            if (notif->finalCallback) {
                notif->finalCallback();
            }
        }

        bool show() {
            g_signal_connect(notification, "closed", G_CALLBACK(on_notification_closed), this);
            return notify_notification_show(notification, nullptr);
        }

    public:
        Notification(std::string n, std::string t, std::string m, void (*callback)())
            : name(std::move(n)), title(std::move(t)), message(std::move(m)),
              notification(notify_notification_new(name.c_str(), title.c_str(), nullptr)),
              finalCallback(callback) {
            loop = g_main_loop_new(nullptr, FALSE);
        }

        void AddButton(std::string label, std::string action, Button::Func f) {
            btns.emplace_back(Button(notification, std::move(action), std::move(label), std::move(f), loop));
        }

        void setTimeout(int timeout) {
            notify_notification_set_timeout(notification, timeout);
        }

        bool ShowNotification() {
            if (!show()) {
                std::cerr << "Failed to show notification" << std::endl;
                g_object_unref(G_OBJECT(notification));
                notify_uninit();
                return false;
            }
            g_main_loop_run(loop);
            return true;
        }

        ~Notification() {
            g_main_loop_quit(loop);
            g_main_loop_unref(loop);
        }
    };

}
