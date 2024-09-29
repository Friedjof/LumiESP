#ifndef ABSTRACTMODE_H
#define ABSTRACTMODE_H


#include <functional>

#include "ControllerService.h"


class AbstractMode {
    private:
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

        // helper methods
        bool isFirstRun();
        void enableFirstRun();
        void disableFirstRun();

        // abstract methods
        virtual void customSetup() = 0;
        virtual void customLoop(unsigned long long steps) = 0;

        void loop(unsigned long long steps);

    public:
        AbstractMode(ControllerService* controllerService);
        ~AbstractMode();

        // setup method called once when the mode is initialized in the main.cpp
        void setup();

        // mode properties
        String getModeTitle();
        String getModeInternalName();
        String getModeDescription();
        String getModeAuthor();
        String getModeContact();
        String getModeVersion();
        String getModeLicense();
};

#endif