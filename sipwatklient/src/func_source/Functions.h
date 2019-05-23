/*
 * Functions.cc
 *
 *  Created on: 29 gru 2017
 *      Author: Krzysztof Œwidrak
 */
//includes
#include "header.h"

using namespace std;
typedef map<string, string> TypeCid_Map;

std::string random_string(std::string::size_type length);
int calculateLengthOfMsg(RequestSIP* req);
int calculateLengthOfMsg(AnswerSIP* ans);
void *createControlTu(RequestSIP *ans, int type);
void *createControlTu(AnswerSIP *ans, int type);
void printRequest(RequestSIP* req, const char* file, const char* host);
void printAnswer(AnswerSIP* req, const char* file, const char* host);
bool checkIsDouble(const char* x);
bool checkIsAddress(const char* x);
