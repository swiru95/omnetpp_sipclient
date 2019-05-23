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

package sipwatklient;

//
// TODO auto-generated module
//
simple SipTU
{
    parameters:
        //Client and Call parameters
        string localUser=default("");							//MUST BE//user which is using the client
        string myVoiceCodec = default("PCMU/8000");	//Codec carried by SDP
        double delayTime @unit(s) = default(300ms);	//The period of sim Time, uses to simulate the User duration while handling a call

        string call2Users = default("");		//SIP addresses of SIP to whom the sessions are going to be established
        string startCallingTimes = default("");	//Sim Times when the current sessions are going to be established
        string stopCallingTimes = default("");	//Sim Times when the current sessions are going to be deestablished
        //statistics
        @signal[StateS](type=long);
        @statistic[StateT](source=StateS; record=vector);

    //The form of modules name:
    // type typeDestinationCompoundModule, 
    // the rule is described in the SipClient compound module
    // (SipClient.ned)
    gates:
        ///To SipEngine
        //to UAC (the element of SipEngine)
        input inUacSipEngine;
        output outUacSipEngine;
        //to UAS (the element of SipEngine)
        input inUasSipEngine;
        output outUasSipEngine;


}
