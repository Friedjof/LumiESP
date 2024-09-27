#ifndef ABSTRACTMODE_H
#define ABSTRACTMODE_H


#include <functional>

#include "ControllerService.h"


class AbstractMode {
    private:
        unsigned long long lastSteps = 0;
        bool firstRun = true;

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
        AbstractMode(ControllerService* controllerService);
        ~AbstractMode();

        void setup();
        void loop(unsigned long long steps);

        bool isFirstRun();

        // mode properties
        String getModeTitle();
        String getModeInternalName();
        String getModeDescription();
        String getModeAuthor();
        String getModeContact();
        String getModeVersion();
        String getModeLicense();

        virtual void customSetup() = 0;
        virtual void customLoop(unsigned long long steps) = 0;
};

#endif