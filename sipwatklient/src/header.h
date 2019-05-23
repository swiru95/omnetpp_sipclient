/*
 * header.h
 *
 *  Created on: 14 gru 2017
 *      Author: Krzysztof Œwidrak
 */

#ifndef HEADER_H_
#define HEADER_H_
// //All includes
#include <fstream>
#include <stdio.h>
#include <random>
#include <map>
#include <random>
#include <iostream>
#include <string>
#include <string.h>
#include <set>
//omnet
#include <omnetpp.h>
//inet
#include "inet/applications/base/ApplicationBase.h"
#include "inet/networklayer/common/L3Address.h"
#include "inet/transportlayer/contract/udp/UDPSocket.h"
#include "inet/networklayer/common/L3AddressResolver.h"
#include "inet/transportlayer/udp/UDPPacket.h"
//sipclientwat
//msgs
#include "AnswerSIP_m.h"
#include "RequestSIP_m.h"
#include "ControlTU_m.h"
//namespaces
using namespace omnetpp;
using namespace inet;
using namespace std;
enum StateOfSipEngine {
    TRYING = 11, PROCEEDING = 12, COMPLETE = 13, PROCESS = 14
};

//End
#endif /* HEADER_H_ */
