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
 */

module PlatformLedsC
{
  provides interface GeneralIO as Led0;
  provides interface GeneralIO as Led1;
  provides interface GeneralIO as Led2;
  uses interface Init;
}
implementation
{

  bool led0_is_on;
  bool led1_is_on;
  bool led2_is_on;

  async command void Led0.set() {
     led0_is_on = TRUE;
  }

  async command void Led0.clr() {
     led0_is_on = FALSE;
  }

  async command void Led0.toggle() {
     led0_is_on = !led0_is_on;
  }

  async command bool Led0.get() {
    return led0_is_on;
  }

  async command void Led0.makeInput() {
  }

  async command void Led0.makeOutput() {
    call Init.init();
  }

  async command void Led1.set() {
     led1_is_on = TRUE;
  }

  async command void Led1.clr() {
     led1_is_on = FALSE;
  }

  async command void Led1.toggle() {
     led1_is_on = !led1_is_on;
  }

  async command bool Led1.get() {
    return led1_is_on;
  }

  async command void Led1.makeInput() {
  }

  async command void Led1.makeOutput() {
    call Init.init();
  }

  async command void Led2.set() {
     led2_is_on = TRUE;
  }

  async command void Led2.clr() {
     led2_is_on = FALSE;
  }

  async command void Led2.toggle() {
     led2_is_on = !led2_is_on;
  }

  async command bool Led2.get() {
    return led2_is_on;
  }

  async command void Led2.makeInput() {
  }

  async command void Led2.makeOutput() {
    call Init.init();
  }

  async command bool Led0.isInput() { 
    return FALSE;
  }

  async command bool Led0.isOutput() { 
    return FALSE;
  }

  async command bool Led1.isInput() { 
    return FALSE;
  }

  async command bool Led1.isOutput() { 
    return FALSE;
  }

  async command bool Led2.isInput() { 
    return FALSE;
  }

  async command bool Led2.isOutput() { 
    return FALSE;
  }

}
