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

import inet.networklayer.configurator.ipv4.IPv4NetworkConfigurator;
import inet.node.ethernet.Eth100M;
import inet.node.inet.Router;
import inet.node.inet.StandardHost;


network Net2
{
    @display("bgb=284,249");
    submodules:
        host1: StandardHost {
            @display("p=37,190");
        }
        host2: StandardHost {
            @display("p=237,52");
        }
        configurator: IPv4NetworkConfigurator {
            @display("p=165,149");
        }
        router: Router {
            @display("p=51,48");
        }
    connections:
        router.ethg++ <--> Eth100M <--> host1.ethg++;
        router.ethg++ <--> Eth100M <--> host2.ethg++;
}
