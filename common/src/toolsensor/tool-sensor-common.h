/*
  Copyright (C) 2003-2010 FreeIPMI Core Team

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2, or (at your option)
  any later version.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA
*/

#ifndef _TOOL_SENSOR_COMMON_H
#define _TOOL_SENSOR_COMMON_H

#include <stdio.h>
#include <stdint.h>

#include <freeipmi/freeipmi.h>

#include "pstdout.h"

#define UNRECOGNIZED_SENSOR_TYPE            "Unrecognized"

#define SENSORS_HEADER_RECORD_ID_STR        "ID"
#define SENSORS_HEADER_NAME_STR             "Name"
#define SENSORS_HEADER_TYPE_STR             "Type"
#define SENSORS_HEADER_STATE_STR            "State"
#define SENSORS_HEADER_READING_STR          "Reading"
#define SENSORS_HEADER_UNITS_STR            "Units"
#define SENSORS_HEADER_EVENT_STR            "Event"

/* achu:
 *
 * The stack on cygwin is smaller than on unixes, and the ability for
 * me to get a bigger stack in cygwin is difficult.  The normal unix
 * ways (i.e. ulimit -s unlimited) don't work.  Some posts online
 * indicate it's b/c of the way windows works with stack limits.
 *
 * So we're just going to use smaller values to deal with the problem.
 */
#ifdef __CYGWIN__
#define MAX_SENSOR_RECORD_IDS               128
#define MAX_SENSOR_TYPES                    16
#else /* !__CYGWIN__ */
/* record id is 16 bits - 65536 */
#define MAX_SENSOR_RECORD_IDS               65536
#define MAX_SENSOR_TYPES                    256
#endif /* !__CYGWIN__ */
#define MAX_SENSOR_TYPES_STRING_LENGTH      256

#define SENSOR_PARSE_ALL_STRING             "all"
#define SENSOR_PARSE_NONE_STRING            "none"

/* entity_id is a 8 bit field */
#define MAX_ENTITY_IDS                      256

#define MAX_ENTITY_ID_SENSOR_NAME_STRING    256

struct sensor_column_width
{
  int record_id;
  int sensor_name;
  int sensor_type;
  int sensor_units;
};

struct sensor_entity_id_counts
{
  unsigned int count[MAX_ENTITY_IDS];
};

const char * get_sensor_type_output_string (unsigned int sensor_type);

void get_sensor_type_cmdline_string (char *sensor_type);

int get_entity_sensor_name_string (pstdout_state_t pstate,
                                   ipmi_sdr_parse_ctx_t sdr_parse_ctx,
                                   const void *sdr_record,
                                   unsigned int sdr_record_len,
                                   struct sensor_entity_id_counts *entity_id_counts,
                                   uint8_t *sensor_number,
                                   char *sensor_name_buf,
                                   unsigned int sensor_name_buf_len);

int get_entity_sensor_name_string_by_record_id (pstdout_state_t pstate,
                                                ipmi_sdr_parse_ctx_t sdr_parse_ctx,
                                                ipmi_sdr_cache_ctx_t sdr_cache_ctx,
                                                uint16_t record_id,
                                                struct sensor_entity_id_counts *entity_id_counts,
                                                uint8_t *sensor_number,
                                                char *sensor_name_buf,
                                                unsigned int sensor_name_buf_len);

int display_sensor_type_cmdline (pstdout_state_t pstate, 
                                 unsigned int sensor_type);

int display_string_cmdline (pstdout_state_t pstate, 
                            const char *str);

int sensor_type_strcmp (pstdout_state_t pstate,
                        const char *sensor_type_str_input,
                        unsigned int sensor_type);

int get_sensor_units_output_string (pstdout_state_t pstate,
                                    ipmi_sdr_parse_ctx_t sdr_parse_ctx,
                                    const void *sdr_record,
                                    unsigned int sdr_record_len,
                                    char *sensor_units_buf,
                                    unsigned int sensor_units_buflen,
                                    unsigned int abbreviated_units_flag);

void output_sensor_headers (pstdout_state_t pstate,
                            int quiet_readings,
                            int output_sensor_state,
                            int comma_separated_output,
                            int no_sensor_type_output,
                            struct sensor_column_width *column_width);

int calculate_entity_id_counts (pstdout_state_t pstate,
                                ipmi_sdr_cache_ctx_t sdr_cache_ctx,
                                ipmi_sdr_parse_ctx_t sdr_parse_ctx,
                                struct sensor_entity_id_counts *entity_id_counts);

/* use normal names, set entity_id_counts to NULL */
int calculate_column_widths (pstdout_state_t pstate,
                             ipmi_sdr_cache_ctx_t sdr_cache_ctx,
                             ipmi_sdr_parse_ctx_t sdr_parse_ctx,
                             char sensor_types[][MAX_SENSOR_TYPES_STRING_LENGTH+1],
                             unsigned int sensor_types_length,
                             unsigned int record_ids[],
                             unsigned int record_ids_length,
                             unsigned int abbreviated_units,
                             unsigned int shared_sensors,
                             unsigned int count_event_only_records,
                             unsigned int count_device_locator_records,
                             unsigned int count_oem_records,
                             struct sensor_entity_id_counts *entity_id_counts,
                             struct sensor_column_width *column_width);

int calculate_record_ids (pstdout_state_t pstate,
                          ipmi_sdr_cache_ctx_t sdr_cache_ctx,
                          ipmi_sdr_parse_ctx_t sdr_parse_ctx,
                          char sensor_types[][MAX_SENSOR_TYPES_STRING_LENGTH+1],
                          unsigned int sensor_types_length,
                          char exclude_sensor_types[][MAX_SENSOR_TYPES_STRING_LENGTH+1],
                          unsigned int exclude_sensor_types_length,
                          unsigned int record_ids[],
                          unsigned int record_ids_length,
                          unsigned int exclude_record_ids[],
                          unsigned int exclude_record_ids_length,
                          unsigned int output_record_ids[MAX_SENSOR_RECORD_IDS],
                          unsigned int *output_record_ids_length);

#endif