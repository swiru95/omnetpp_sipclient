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
import inet.applications.contract.IUDPApp;
import ned.IdealChannel;


module SipClient like IUDPApp
{
    parameters:
        bool write_logs=default(true);	//Par which decides if the logs are printed into txt file.
        								//If true-> the msgs and status are print into simluations/simlogs/
        								//else -> logs wont be created
    //The form of modules name:
    // type typeDestinationCompoundModule
    //example: 
    // inUacSipEngine means that the gate is destinated 
    // to connect the Uac module which is the part 
    // of SipEngine compound module and the gate is an Input type
    // (look at SipDTBR.ned)
    // the exception is the MUST BE IUDPApp gates.
    gates:
		//An UdpApp must be gates
        input udpIn @labels(UDPcontrollInfoSIPClient/up);
        output udpOut @labels(UDPCotrollInfoSIPClient/down);

    submodules:
        sipEngine: SipEngine {
            @display("p=145,122;is=vl");
        }
        sipDTBR: SipDTBR {
            @display("p=145,211");
        }
        sipTU: SipTU {
            @display("p=145,36");
        }
    connections:

        udpIn  --> sipDTBR.inSipClient;
        sipDTBR.outSipClient  --> udpOut;
        sipDTBR.outUacSipEngine  --> sipEngine.inUacDtbr;
        sipDTBR.outUasSipEngine  --> sipEngine.inUasDtbr;
        sipEngine.outUacDtbr  --> sipDTBR.inUacSipEngine;
        sipEngine.outUasDtbr  --> sipDTBR.inUasSipEngine;
        sipEngine.outUacTu  --> sipTU.inUacSipEngine;
        sipEngine.outUasTu  --> sipTU.inUasSipEngine;
        sipTU.outUacSipEngine  --> sipEngine.inUacTu;
        sipTU.outUasSipEngine  --> sipEngine.inUasTu;
}
