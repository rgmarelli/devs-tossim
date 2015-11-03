/*
 * Copyright (c) 2013-2014 Ricardo Guido Marelli
 * All rights reserved.
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
