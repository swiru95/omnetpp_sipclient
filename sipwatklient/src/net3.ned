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


network Net3
{
    @display("bgb=503,350");
    submodules:
        host2: StandardHost {
            @display("p=235,299");
        }
        host3: StandardHost {
            @display("p=449,35");
        }
        host1: StandardHost {
            @display("p=39,34");
        }
        router: Router {
            @display("p=235,153");
        }
        configurator: IPv4NetworkConfigurator {
            @display("p=81,247");
        }
    connections:
        host1.ethg++ <--> Eth100M <--> router.ethg++;
        host3.ethg++ <--> Eth100M <--> router.ethg++;
        host2.ethg++ <--> Eth100M <--> router.ethg++;
}
