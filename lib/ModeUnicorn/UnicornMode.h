#ifndef OFFMODE_H
#define OFFMODE_H


#include <functional>

#include "AbstractMode.h"


class UnicornMode : public AbstractMode {
    private:
        // internal properties
        String hexColor = "#a300a3";
        String newHexColor = this->hexColor;

        byte brightness = 0;
        byte newBrightness = this->brightness;

    public:
        UnicornMode(ControllerService* controllerService);

        void customSetup() override;
        void customLoop(unsigned long long steps) override;
};

#endif