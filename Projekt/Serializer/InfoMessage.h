#ifndef INFO_MESSAGE_H_
#define INFO_MESSAGE_H_

#include "Serializer/Serializer.h"
#include "Logger.h"

#define BAND 2

#define RUTSCHE_VOLL 0x01
#define QUITIERUNGS_BIT 0x04
#define BAND_FREI 0x08
#define ESTOP_BIT 0x02
#define LB_INTERRUPTED 0x10


class InfoMessage
{
public:
    static InfoMessage* getInfoMessage()
    {
        static InfoMessage instances;
        return &instances;
    }
    bool istBand1RutscheVoll();
    bool istBand2RutscheVoll();
    bool istBand2Frei();
    void update(struct info_package_without_ch* infoUpdate);

    void setBand1RutscheVoll();
    void setBand1RutscheLeer();
    void setBand2RutscheVoll();
    void setBand2RutscheLeer();
    void setBand2Frei();
    void setBand2NichtFrei();
    void setLBinterruptedBit();
    void resetisinterruptedBit();
    void setQuittierung();
    void InhaltdesPaketesausgeben();

    void setESTOP();
    bool isESTOPGedrueckt();
    void removeESTOP();
    bool wurdeUeberallQuitiert();
    bool isLbNextConveyorInterrupted();
    struct info_package* getMessage();

private:
  InfoMessage();
   ~InfoMessage();
  struct info_package info;
};

#endif /* SERIALIZER_H_ */
