#ifndef ADC_H
#define ADC_H

class Adc
{
    public:
        virtual void StartAdc() {};
        virtual float GetResult() { return 0; };
};



#endif // ADC_H
