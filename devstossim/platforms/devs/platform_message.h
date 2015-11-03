/*
 * Copyright (c) 2013-2014 Ricardo Guido Marelli
 * All rights reserved.
 *
 *
 * Copyright (c) 2005-2006 Intel Corporation
 * All rights reserved.
 *
 * This file is distributed under the terms in the attached INTEL-LICENSE     
 * file. If you do not find these files, copies can be found by writing to
 * Intel Research Berkeley, 2150 Shattuck Avenue, Suite 1300, Berkeley, CA, 
 * 94704.  Attention:  Intel License Inquiry.
 */
/**
 * Implementation to support the devs platform.
 */

#ifndef PLATFORM_MESSAGE_H
#define PLATFORM_MESSAGE_H

#include "Serial.h"

typedef struct {
  nxle_uint8_t length;
  nxle_uint16_t dest;
  nxle_uint16_t src;
  nxle_uint8_t type;
} devs_am_header_t;

typedef union message_header {
  devs_am_header_t devs_am_header;
  serial_header_t serial;
} message_header_t;

typedef union message_footer {
  nx_uint8_t dummy;
} message_footer_t;

typedef union message_metadata {
  nx_uint8_t dummy;
} message_metadata_t;

#endif
