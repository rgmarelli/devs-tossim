/* 
 * DEVS-TOSSIM - a DEVS framework for simulation of TinyOS wireless sensor networks
 * Copyright (c) 2013 Ricardo Guido Marelli
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * ---
 * DEVS-TOSSIM sensor implementation.
 */

module DevsSensorC
{
    provides interface Init;
    provides interface Read<uint16_t>;
}

implementation
{

    void readDone(const void* port_name, const void* content, unsigned int content_size) @C() @spontaneous() {
        unsigned int val=*((unsigned int*)content);
        signal Read.readDone(SUCCESS, val);
    }

    command error_t Init.init() {
        registerExternalEvent("SensorReadDone",&readDone);
        return SUCCESS;
    }

    command error_t Read.read() {
        putOutputMessage("Sensor_Read","SensorRead",NULL,0);
        return SUCCESS;
    }
}
