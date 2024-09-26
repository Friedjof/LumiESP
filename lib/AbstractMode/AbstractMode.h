#ifndef ABSTRACTMODE_H
#define ABSTRACTMODE_H


#include <functional>

#include "ControllerService.h"


class AbstractMode {
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

        bool isHexColor(String hex);

    public:
        AbstractMode(ControllerService* controllerService);
        ~AbstractMode();

        void setup();
        void loop(int steps);

        virtual void customSetup() = 0;
        virtual void customLoop(const int steps) = 0;
};

#endif