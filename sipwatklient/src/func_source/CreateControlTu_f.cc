/*
 * CreateControlTu_f.cc
 *
 *  Created on: 5 sty 2018
 *      Author: Krzysztof Œwidrak
 */

#include "Functions.h"

void *createControlTu(RequestSIP *ans, int type) {
    ControlTU* controlTu = new ControlTU();
    string str;
    controlTu->setInfoCarried(type);
    controlTu->setDestOfMsg(Tu);
    controlTu->setCall2Address((L3Address) ans->getToIpAddress());
    controlTu->setCall2User(ans->getToUser());
    controlTu->setLocalUser(ans->getFromUser());
    controlTu->setMyVoiceCodec(ans->getMyVoiceCodec());
    return controlTu;
}
void *createControlTu(AnswerSIP *ans, int type) {
    ControlTU* controlTu = new ControlTU();
    controlTu->setInfoCarried(type);
    controlTu->setDestOfMsg(Tu);
    controlTu->setCall2Address((L3Address) ans->getToIpAddress());
    controlTu->setCall2User(ans->getToUser());
    controlTu->setLocalUser(ans->getFromUser());
    return controlTu;
}

