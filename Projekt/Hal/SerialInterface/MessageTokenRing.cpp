/*
 * MessageTokenRing.cpp
 *
 *  Created on: 23.10.2016
 *      Author: marvin


#include <SerialInterface/MessageTokenRing.h>

MessageTokenRing::MessageTokenRing(Serial* input,Serial* output):input(input),output(output),serializer()
{
    // TODO Auto-generated constructor stub

}

MessageTokenRing::~MessageTokenRing()
{
    // TODO Auto-generated destructor stub
}

uint8_t MessageTokenRing::sendMessage(RingMessage* message)
{
    string str = message->toString();
    string::iterator it;

    Packet pack = {};
    for (it = str.begin(); it < str.end(); ++it) {
        pack.data = *it;
        output->sendPacket(&pack);
    }
    pack.data = 0x04;
    output->sendPacket(&pack);
    return 0;
}

uint8_t MessageTokenRing::receiveMessage()
{
    string content = "";
    Packet package = { };

    //todo separat method.
    while (package.data != (uint8_t) '\0') {
        input->recvPacket(&package);
        char rawMessage = (char) (package.data);
        LOG_DEBUG << "MessageTokenRing::recieveMessage(): Uebertragendes bit " << rawMessage
                << "\n";
        content.push_back(rawMessage);
    }
    LOG_DEBUG << "MessageTokenRing::recieveMessage(): Beenden des zuhoerens \n";
    LOG_DEBUG << "MessageTokenRing::recieveMessage(): Message:"<< content <<"\n";

    if (!(content.empty())) {
        string::iterator it;
        uint8_t boolean = 0;
        uint8_t isValue = 0;
        string values = "";
        string key = "";
        std::map<string,string> allPairs;
        for (it = content.begin(); it < content.end(); ++it) {
            char symbol = (char) (*it);

            if (boolean) {
                if (isValue) {
                    values.push_back(symbol);
                }
                if (!isValue) {
                    key.push_back(symbol);
                }
            }

            // string guard;
            if (symbol == '"') {
                if (boolean == 0) {
                    boolean = 1;
                } else {
                    boolean = 0;
                }
            }

            // string guard;
            if (symbol == ':' || symbol == ','|| symbol == '}') {
                if (isValue == 0) {
                    isValue = 1;
                } else {
                    isValue = 0;
                    allPairs[key] = values;

                }
            }
        }
LOG_DEBUG << "MessageTokenRing::receiveMessage Groesse der eingabe: "<<allPairs.size();
        // show content:
        for (std::map<string,string>::iterator it=allPairs.begin(); it!=allPairs.end(); ++it){
          std::cout << it->first << " => " << it->second << '\n';}
    }
return 0;
}
 */
