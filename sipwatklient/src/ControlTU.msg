//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

//
// TODO generated message class
//
cplusplus
{{
     #include "inet/networklayer/common/L3Address.h"
}}    
class noncobject inet::L3Address;
namespace inet;
//Where is message destinated
//This message can be sended in two relations 
//SUPERIORED
//from TU to SipEngine module (UAC or UAS)
//INFERIORED
//from SipEngine module (UAC or UAS) to TU
//there is no possibility to community between UAC and UAS.
enum DestOfMsg
    {
        Tu=30;	//INFERIORED UAC/UAS->TU
        Uas=20;	//SUPERIORED TU->UAS
        Uac=10;	//SUPERIORED TU->UAC
    };
//What info message is carrying
enum InfoCarried
    {
        //for both relations requests
        CALL=1;
        TERMINATE=2;
        CANTALK=3;
        //for relation INFERIORED
        CANCELreq=3;
        //for both relations responses
        OKansw=200;
        BUSYansw=486;
        RINGINGansw=180;
        NOT_ACCEPTABLE4answ=406;
        NOT_ACCEPTABLE6answ=606;
        NOT_FOUNDansw=404;
    };

message ControlTU {
    
    int infoCarried @enum(InfoCarried);
    int destOfMsg @enum(DestOfMsg);
    
    L3Address call2Address; 	//where to call/from where the callee is calling  
    string myVoiceCodec=""; 	//codec i want to negotiate/ codec which callee is proposing
    string call2User="";		//the name of user where i want to call/ which are calling
   	string localUser="";		//the name of user whos is using the device
}
