#ifndef ABSTRACTAPP_H
#define ABSTRACTAPP_H


#include <functional>

#include "ControllerService.h"


class AbstractApp {
    protected:
        ControllerService* controllerService;

        // mode properties
        String modeTitle;
        String modeInternalName;
        String modeDescription;
        String modeAuthor;
        String modeContact;
        String modeVersion;
        String modeLicense;

    public:
        AbstractApp(ControllerService* controllerService);
        ~AbstractApp();

        void setup();
        void loop(int steps);

        // mode properties
        String getModeTitle();
        String getModeInternalName();
        String getModeDescription();
        String getModeAuthor();
        String getModeContact();
        String getModeVersion();
        String getModeLicense();

        virtual void customSetup() = 0;
        virtual void customLoop(const int steps) = 0;
};

#endif