#ifndef OFFMODE_H
#define OFFMODE_H


#include <functional>

#include "AbstractMode.h"


class OffMode : public AbstractMode {
    private:
        // internal properties
        String hexColor = "#a300a3";
        String newHexColor = this->hexColor;

        byte brightness = 0;
        byte newBrightness = this->brightness;

    public:
        OffMode(ControllerService* controllerService);

        void customSetup() override;
        void customLoop(unsigned long long steps) override;
};

#endif