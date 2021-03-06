/*
 * Copyright (C) 2007-2013 FreeIPMI Core Team
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

#if HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <stdlib.h>
#if STDC_HEADERS
#include <string.h>
#endif /* STDC_HEADERS */
#include <assert.h>

#include "ipmi-pef-config.h"
#include "ipmi-pef-config-map.h"
#include "ipmi-pef-config-utils.h"
#include "ipmi-pef-config-validate.h"

#include "freeipmi-portability.h"
#include "pstdout.h"

/* convenience struct */
struct alert_policy_table {
  uint8_t policy_type;
  uint8_t policy_enabled;
  uint8_t policy_number;
  uint8_t destination_selector;
  uint8_t channel_number;
  uint8_t alert_string_set_selector;
  uint8_t event_specific_alert_string;
};

static config_err_t
_get_alert_policy_table (struct ipmi_pef_config_state_data *state_data,
                         const char *section_name,
                         struct alert_policy_table *apt)
{
  fiid_obj_t obj_cmd_rs = NULL;
  uint64_t val;
  config_err_t rv = CONFIG_ERR_FATAL_ERROR;
  uint8_t alert_policy_entry_number;

  assert (state_data);
  assert (section_name);
  assert (apt);

  alert_policy_entry_number = atoi (section_name + strlen ("Alert_Policy_"));

  if (!(obj_cmd_rs = fiid_obj_create (tmpl_cmd_get_pef_configuration_parameters_alert_policy_table_rs)))
    {
      pstdout_fprintf (state_data->pstate,
                       stderr,
                       "fiid_obj_create: %s\n",
                       strerror (errno));
      goto cleanup;
    }

  if (ipmi_cmd_get_pef_configuration_parameters_alert_policy_table (state_data->ipmi_ctx,
                                                                    IPMI_GET_PEF_PARAMETER,
                                                                    alert_policy_entry_number,
                                                                    IPMI_PEF_CONFIGURATION_PARAMETERS_NO_BLOCK_SELECTOR,
                                                                    obj_cmd_rs) < 0)
    {
      config_err_t ret;

      if (state_data->prog_data->args->config_args.common_args.debug)
        pstdout_fprintf (state_data->pstate,
                         stderr,
                         "ipmi_cmd_get_pef_configuration_parameters_alert_policy_table: %s\n",
                         ipmi_ctx_errormsg (state_data->ipmi_ctx));

      if (config_is_config_param_non_fatal_error (state_data->ipmi_ctx,
                                                  obj_cmd_rs,
                                                  &ret))
        rv = ret;

      goto cleanup;
    }

#if 0
  if (FIID_OBJ_GET (obj_cmd_rs, "alert_policy_entry_number", &val) < 0)
    {
      pstdout_fprintf (state_data->pstate,
                       stderr,
                       "fiid_obj_get: 'alert_policy_entry_number': %s\n",
                       fiid_obj_errormsg (obj_cmd_rs));
      goto cleanup;
    }
#endif
  if (FIID_OBJ_GET (obj_cmd_rs, "policy_number.policy_type", &val) < 0)
    {
      pstdout_fprintf (state_data->pstate,
                       stderr,
                       "fiid_obj_get: 'policy_number.policy_type': %s\n",
                       fiid_obj_errormsg (obj_cmd_rs));
      goto cleanup;
    }
  apt->policy_type = val;

  if (FIID_OBJ_GET (obj_cmd_rs, "policy_number.enabled", &val) < 0)
    {
      pstdout_fprintf (state_data->pstate,
                       stderr,
                       "fiid_obj_get: 'policy_number.enabled': %s\n",
                       fiid_obj_errormsg (obj_cmd_rs));
      goto cleanup;
    }
  apt->policy_enabled = val;

  if (FIID_OBJ_GET (obj_cmd_rs, "policy_number.policy_number", &val) < 0)
    {
      pstdout_fprintf (state_data->pstate,
                       stderr,
                       "fiid_obj_get: 'policy_number.policy_number': %s\n",
                       fiid_obj_errormsg (obj_cmd_rs));
      goto cleanup;
    }
  apt->policy_number = val;

  if (FIID_OBJ_GET (obj_cmd_rs, "channel_destination.destination_selector", &val) < 0)
    {
      pstdout_fprintf (state_data->pstate,
                       stderr,
                       "fiid_obj_get: 'channel_destination.destination_selector': %s\n",
                       fiid_obj_errormsg (obj_cmd_rs));
      goto cleanup;
    }
  apt->destination_selector = val;

  if (FIID_OBJ_GET (obj_cmd_rs, "channel_destination.channel_number", &val) < 0)
    {
      pstdout_fprintf (state_data->pstate,
                       stderr,
                       "fiid_obj_get: 'channel_destination.channel_number': %s\n",
                       fiid_obj_errormsg (obj_cmd_rs));
      goto cleanup;
    }
  apt->channel_number = val;

  if (FIID_OBJ_GET (obj_cmd_rs, "alert_string_key.alert_string_set_selector", &val) < 0)
    {
      pstdout_fprintf (state_data->pstate,
                       stderr,
                       "fiid_obj_get: 'alert_string_key.alert_string_set_selector': %s\n",
                       fiid_obj_errormsg (obj_cmd_rs));
      goto cleanup;
    }
  apt->alert_string_set_selector = val;

  if (FIID_OBJ_GET (obj_cmd_rs, "alert_string_key.event_specific_alert_string", &val) < 0)
    {
      pstdout_fprintf (state_data->pstate,
                       stderr,
                       "fiid_obj_get: 'alert_string_key.event_specific_alert_string': %s\n",
                       fiid_obj_errormsg (obj_cmd_rs));
      goto cleanup;
    }
  apt->event_specific_alert_string = val;

  rv = CONFIG_ERR_SUCCESS;
 cleanup:
  fiid_obj_destroy (obj_cmd_rs);
  return (rv);
}

static config_err_t
_set_alert_policy_table (struct ipmi_pef_config_state_data *state_data,
                         const char *section_name,
                         struct alert_policy_table *apt)
{
  fiid_obj_t obj_cmd_rs = NULL;
  config_err_t rv = CONFIG_ERR_FATAL_ERROR;
  uint8_t alert_policy_entry_number;

  assert (state_data);
  assert (section_name);
  assert (apt);

  alert_policy_entry_number = atoi (section_name + strlen ("Alert_Policy_"));

  if (!(obj_cmd_rs = fiid_obj_create (tmpl_cmd_set_pef_configuration_parameters_rs)))
    {
      pstdout_fprintf (state_data->pstate,
                       stderr,
                       "fiid_obj_create: %s\n",
                       strerror (errno));
      goto cleanup;
    }

  if (ipmi_cmd_set_pef_configuration_parameters_alert_policy_table (state_data->ipmi_ctx,
                                                                    alert_policy_entry_number,
                                                                    apt->policy_type,
                                                                    apt->policy_enabled,
                                                                    apt->policy_number,
                                                                    apt->destination_selector,
                                                                    apt->channel_number,
                                                                    apt->alert_string_set_selector,
                                                                    apt->event_specific_alert_string,
                                                                    obj_cmd_rs) < 0)
    {
      config_err_t ret;

      if (state_data->prog_data->args->config_args.common_args.debug)
        pstdout_fprintf (state_data->pstate,
                         stderr,
                         "ipmi_cmd_set_pef_configuration_parameters_alert_policy_table: %s\n",
                         ipmi_ctx_errormsg (state_data->ipmi_ctx));

      /* IPMI Workaround
       *
       * Fujitsu RX 100 S5
       *
       * Inventec 5441/Dell Xanadu II
       *
       * All fields have to be applied simultaneously, the motherboard
       * does not appear to like configuration of one field of a time,
       * always leading to invalid input errors.  This isn't a
       * compliance issue, but makes it tough to make a portable/good
       * interface.
       */
      if (ipmi_ctx_errnum (state_data->ipmi_ctx) == IPMI_ERR_COMMAND_INVALID_OR_UNSUPPORTED
          && (ipmi_check_completion_code (obj_cmd_rs,
                                          IPMI_COMP_CODE_INVALID_DATA_FIELD_IN_REQUEST) == 1))
        {
          struct config_section *section;
          struct config_keyvalue *kv;
          
          section = state_data->sections;
          while (section)
            {
              if (!strcasecmp (section_name, section->section_name))
                break;
              section = section->next;
            }

          /* shouldn't be possible */
          if (!section)
            goto cleanup;
          
          if ((kv = config_find_keyvalue (section,
                                          "Policy_Type")))
            apt->policy_type = policy_type_number (kv->value_input);

          if ((kv = config_find_keyvalue (section,
                                          "Policy_Enabled")))
            apt->policy_enabled = same (kv->value_input, "yes");

          if ((kv = config_find_keyvalue (section,
                                          "Policy_Number")))
            apt->policy_number = atoi (kv->value_input);

          if ((kv = config_find_keyvalue (section,
                                          "Destination_Selector")))
            apt->destination_selector = atoi (kv->value_input);
          
          if ((kv = config_find_keyvalue (section,
                                          "Channel_Number")))
            apt->channel_number = atoi (kv->value_input);

          if ((kv = config_find_keyvalue (section,
                                          "Alert_String_Set_Selector")))
            apt->alert_string_set_selector = atoi (kv->value_input);
          
          if ((kv = config_find_keyvalue (section,
                                          "Event_Specific_Alert_String")))
            apt->event_specific_alert_string = same (kv->value_input, "yes");

          if (state_data->prog_data->args->config_args.common_args.debug)
            pstdout_fprintf (state_data->pstate,
                             stderr,
                             "ipmi_cmd_set_pef_configuration_parameters_alert_policy_table: attempting workaround\n");

          if (ipmi_cmd_set_pef_configuration_parameters_alert_policy_table (state_data->ipmi_ctx,
                                                                            alert_policy_entry_number,
                                                                            apt->policy_type,
                                                                            apt->policy_enabled,
                                                                            apt->policy_number,
                                                                            apt->destination_selector,
                                                                            apt->channel_number,
                                                                            apt->alert_string_set_selector,
                                                                            apt->event_specific_alert_string,
                                                                            obj_cmd_rs) < 0)
            {
              if (state_data->prog_data->args->config_args.common_args.debug)
                pstdout_fprintf (state_data->pstate,
                                 stderr,
                                 "ipmi_cmd_set_pef_configuration_parameters_alert_policy_table: %s\n",
                                 ipmi_ctx_errormsg (state_data->ipmi_ctx));

              if (config_is_config_param_non_fatal_error (state_data->ipmi_ctx,
                                                          obj_cmd_rs,
                                                          &ret))
                rv = ret;

              goto cleanup;
            }

          /* success */
          goto out;
        }
      else if (config_is_config_param_non_fatal_error (state_data->ipmi_ctx,
                                                       obj_cmd_rs,
                                                       &ret))
        rv = ret;

      goto cleanup;
    }

 out:
  rv = CONFIG_ERR_SUCCESS;
 cleanup:
  fiid_obj_destroy (obj_cmd_rs);
  return (rv);
}

static config_err_t
policy_type_checkout (const char *section_name,
                      struct config_keyvalue *kv,
                      void *arg)
{
  ipmi_pef_config_state_data_t *state_data;
  struct alert_policy_table apt;
  config_err_t ret;

  assert (section_name);
  assert (kv);
  assert (arg);
  
  state_data = (ipmi_pef_config_state_data_t *)arg;

  if ((ret = _get_alert_policy_table (state_data,
                                      section_name,
                                      &apt)) != CONFIG_ERR_SUCCESS)
    return (ret);

  if (config_section_update_keyvalue_output (state_data->pstate,
                                             kv,
                                             policy_type_string (apt.policy_type)) < 0)
    return (CONFIG_ERR_FATAL_ERROR);

  return (CONFIG_ERR_SUCCESS);
}

static config_err_t
policy_type_commit (const char *section_name,
                    const struct config_keyvalue *kv,
                    void *arg)
{
  ipmi_pef_config_state_data_t *state_data;
  struct alert_policy_table apt;
  config_err_t ret;

  assert (section_name);
  assert (kv);
  assert (arg);
  
  state_data = (ipmi_pef_config_state_data_t *)arg;

  if ((ret = _get_alert_policy_table (state_data,
                                      section_name,
                                      &apt)) != CONFIG_ERR_SUCCESS)
    return (ret);

  apt.policy_type = policy_type_number (kv->value_input);

  return (_set_alert_policy_table (state_data,
                                   section_name,
                                   &apt));
}

static config_err_t
policy_enabled_checkout (const char *section_name,
                         struct config_keyvalue *kv,
                         void *arg)
{
  ipmi_pef_config_state_data_t *state_data;
  struct alert_policy_table apt;
  config_err_t ret;

  assert (section_name);
  assert (kv);
  assert (arg);
  
  state_data = (ipmi_pef_config_state_data_t *)arg;

  if ((ret = _get_alert_policy_table (state_data,
                                      section_name,
                                      &apt)) != CONFIG_ERR_SUCCESS)
    return (ret);

  if (config_section_update_keyvalue_output (state_data->pstate,
                                             kv,
                                             apt.policy_enabled ? "Yes" : "No") < 0)
    return (CONFIG_ERR_FATAL_ERROR);

  return (CONFIG_ERR_SUCCESS);
}

static config_err_t
policy_enabled_commit (const char *section_name,
                       const struct config_keyvalue *kv,
                       void *arg)
{
  ipmi_pef_config_state_data_t *state_data;
  struct alert_policy_table apt;
  config_err_t ret;

  assert (section_name);
  assert (kv);
  assert (arg);
  
  state_data = (ipmi_pef_config_state_data_t *)arg;

  if ((ret = _get_alert_policy_table (state_data,
                                      section_name,
                                      &apt)) != CONFIG_ERR_SUCCESS)
    return (ret);

  apt.policy_enabled = same (kv->value_input, "yes");

  return (_set_alert_policy_table (state_data,
                                   section_name,
                                   &apt));
}

static config_err_t
policy_number_checkout (const char *section_name,
                        struct config_keyvalue *kv,
                        void *arg)
{
  ipmi_pef_config_state_data_t *state_data;
  struct alert_policy_table apt;
  config_err_t ret;

  assert (section_name);
  assert (kv);
  assert (arg);
  
  state_data = (ipmi_pef_config_state_data_t *)arg;

  if ((ret = _get_alert_policy_table (state_data,
                                      section_name,
                                      &apt)) != CONFIG_ERR_SUCCESS)
    return (ret);

  if (config_section_update_keyvalue_output_unsigned_int (state_data->pstate,
                                                          kv,
                                                          apt.policy_number) < 0)
    return (CONFIG_ERR_FATAL_ERROR);

  return (CONFIG_ERR_SUCCESS);
}

static config_err_t
policy_number_commit (const char *section_name,
                      const struct config_keyvalue *kv,
                      void *arg)
{
  ipmi_pef_config_state_data_t *state_data;
  struct alert_policy_table apt;
  config_err_t ret;

  assert (section_name);
  assert (kv);
  assert (arg);
  
  state_data = (ipmi_pef_config_state_data_t *)arg;

  if ((ret = _get_alert_policy_table (state_data,
                                      section_name,
                                      &apt)) != CONFIG_ERR_SUCCESS)
    return (ret);

  apt.policy_number = atoi (kv->value_input);

  return (_set_alert_policy_table (state_data,
                                   section_name,
                                   &apt));
}

static config_err_t
destination_selector_checkout (const char *section_name,
                               struct config_keyvalue *kv,
                               void *arg)
{
  ipmi_pef_config_state_data_t *state_data;
  struct alert_policy_table apt;
  config_err_t ret;

  assert (section_name);
  assert (kv);
  assert (arg);
  
  state_data = (ipmi_pef_config_state_data_t *)arg;

  if ((ret = _get_alert_policy_table (state_data,
                                      section_name,
                                      &apt)) != CONFIG_ERR_SUCCESS)
    return (ret);

  if (config_section_update_keyvalue_output_unsigned_int (state_data->pstate,
                                                          kv,
                                                          apt.destination_selector) < 0)
    return (CONFIG_ERR_FATAL_ERROR);

  return (CONFIG_ERR_SUCCESS);
}

static config_err_t
destination_selector_commit (const char *section_name,
                             const struct config_keyvalue *kv,
                             void *arg)
{
  ipmi_pef_config_state_data_t *state_data;
  struct alert_policy_table apt;
  config_err_t ret;

  assert (section_name);
  assert (kv);
  assert (arg);
  
  state_data = (ipmi_pef_config_state_data_t *)arg;

  if ((ret = _get_alert_policy_table (state_data,
                                      section_name,
                                      &apt)) != CONFIG_ERR_SUCCESS)
    return (ret);

  apt.destination_selector = atoi (kv->value_input);

  return (_set_alert_policy_table (state_data,
                                   section_name,
                                   &apt));
}

static config_err_t
channel_number_checkout (const char *section_name,
                         struct config_keyvalue *kv,
                         void *arg)
{
  ipmi_pef_config_state_data_t *state_data;
  struct alert_policy_table apt;
  config_err_t ret;

  assert (section_name);
  assert (kv);
  assert (arg);
  
  state_data = (ipmi_pef_config_state_data_t *)arg;

  if ((ret = _get_alert_policy_table (state_data,
                                      section_name,
                                      &apt)) != CONFIG_ERR_SUCCESS)
    return (ret);

  if (config_section_update_keyvalue_output_unsigned_int (state_data->pstate,
                                                          kv,
                                                          apt.channel_number) < 0)
    return (CONFIG_ERR_FATAL_ERROR);

  return (CONFIG_ERR_SUCCESS);
}

static config_err_t
channel_number_commit (const char *section_name,
                       const struct config_keyvalue *kv,
                       void *arg)
{
  ipmi_pef_config_state_data_t *state_data;
  struct alert_policy_table apt;
  config_err_t ret;

  assert (section_name);
  assert (kv);
  assert (arg);
  
  state_data = (ipmi_pef_config_state_data_t *)arg;

  if ((ret = _get_alert_policy_table (state_data,
                                      section_name,
                                      &apt)) != CONFIG_ERR_SUCCESS)
    return (ret);

  apt.channel_number = atoi (kv->value_input);

  return (_set_alert_policy_table (state_data,
                                   section_name,
                                   &apt));
}

static config_err_t
alert_string_set_selector_checkout (const char *section_name,
                                    struct config_keyvalue *kv,
                                    void *arg)
{
  ipmi_pef_config_state_data_t *state_data;
  struct alert_policy_table apt;
  config_err_t ret;

  assert (section_name);
  assert (kv);
  assert (arg);
  
  state_data = (ipmi_pef_config_state_data_t *)arg;

  if ((ret = _get_alert_policy_table (state_data,
                                      section_name,
                                      &apt)) != CONFIG_ERR_SUCCESS)
    return (ret);

  if (config_section_update_keyvalue_output_unsigned_int (state_data->pstate,
                                                          kv,
                                                          apt.alert_string_set_selector) < 0)
    return (CONFIG_ERR_FATAL_ERROR);

  return (CONFIG_ERR_SUCCESS);
}

static config_err_t
alert_string_set_selector_commit (const char *section_name,
                                  const struct config_keyvalue *kv,
                                  void *arg)
{
  ipmi_pef_config_state_data_t *state_data;
  struct alert_policy_table apt;
  config_err_t ret;

  assert (section_name);
  assert (kv);
  assert (arg);
  
  state_data = (ipmi_pef_config_state_data_t *)arg;

  if ((ret = _get_alert_policy_table (state_data,
                                      section_name,
                                      &apt)) != CONFIG_ERR_SUCCESS)
    return (ret);

  apt.alert_string_set_selector = atoi (kv->value_input);

  return (_set_alert_policy_table (state_data,
                                   section_name,
                                   &apt));
}

static config_err_t
event_specific_alert_string_checkout (const char *section_name,
                                      struct config_keyvalue *kv,
                                      void *arg)
{
  ipmi_pef_config_state_data_t *state_data;
  struct alert_policy_table apt;
  config_err_t ret;

  assert (section_name);
  assert (kv);
  assert (arg);
  
  state_data = (ipmi_pef_config_state_data_t *)arg;

  if ((ret = _get_alert_policy_table (state_data,
                                      section_name,
                                      &apt)) != CONFIG_ERR_SUCCESS)
    return (ret);

  if (config_section_update_keyvalue_output (state_data->pstate,
                                             kv,
                                             apt.event_specific_alert_string ? "Yes" : "No") < 0)
    return (CONFIG_ERR_FATAL_ERROR);

  return (CONFIG_ERR_SUCCESS);
}

static config_err_t
event_specific_alert_string_commit (const char *section_name,
                                    const struct config_keyvalue *kv,
                                    void *arg)
{
  ipmi_pef_config_state_data_t *state_data;
  struct alert_policy_table apt;
  config_err_t ret;

  assert (section_name);
  assert (kv);
  assert (arg);
  
  state_data = (ipmi_pef_config_state_data_t *)arg;

  if ((ret = _get_alert_policy_table (state_data,
                                      section_name,
                                      &apt)) != CONFIG_ERR_SUCCESS)
    return (ret);

  apt.event_specific_alert_string = same (kv->value_input, "yes");

  return (_set_alert_policy_table (state_data,
                                   section_name,
                                   &apt));
}

struct config_section *
ipmi_pef_config_alert_policy_table_section_get (ipmi_pef_config_state_data_t *state_data, unsigned int num)
{
  struct config_section *section = NULL;
  char section_name[CONFIG_MAX_SECTION_NAME_LEN];
  char description[CONFIG_MAX_DESCRIPTION_LEN];

  assert (state_data);
  assert (num);

  snprintf (section_name, CONFIG_MAX_SECTION_NAME_LEN, "Alert_Policy_%u", num);

  if (!(section = config_section_create (state_data->pstate,
                                         section_name,
                                         NULL,
                                         NULL,
                                         0,
                                         NULL,
                                         NULL)))
    goto cleanup;

  if (config_section_add_key (state_data->pstate,
                              section,
                              "Policy_Type",
                              "Possible values: Always_Send_To_This_Destination/Proceed_To_Next_Entry/Do_Not_Proceed_Any_More_Entries/Proceed_To_Next_Entry_Different_Channel/Proceed_To_Next_Entry_Different_Destination_Type",
                              0,
                              policy_type_checkout,
                              policy_type_commit,
                              policy_type_validate) < 0)
    goto cleanup;

  if (config_section_add_key (state_data->pstate,
                              section,
                              "Policy_Enabled",
                              "Possible values: Yes/No",
                              0,
                              policy_enabled_checkout,
                              policy_enabled_commit,
                              config_yes_no_validate) < 0)
    goto cleanup;

  if (config_section_add_key (state_data->pstate,
                              section,
                              "Policy_Number",
                              "Give a valid number",
                              0,
                              policy_number_checkout,
                              policy_number_commit,
                              config_number_range_four_bits) < 0)
    goto cleanup;

  if (config_section_add_key (state_data->pstate,
                              section,
                              "Destination_Selector",
                              "Give a valid number",
                              0,
                              destination_selector_checkout,
                              destination_selector_commit,
                              config_number_range_four_bits) < 0)
    goto cleanup;

  if (!state_data->lan_channel_numbers_loaded)
    {
      if (load_lan_channel_numbers (state_data) != CONFIG_ERR_SUCCESS)
	goto cleanup;
    }

  if (state_data->lan_channel_numbers_count > 0)
    {
      char tempbuf[CONFIG_MAX_DESCRIPTION_LEN];
      int i;

      snprintf (description,
		CONFIG_MAX_DESCRIPTION_LEN,
		"Give a valid number (LAN = %u",
		state_data->lan_channel_numbers[0]);

      for (i = 1; i < state_data->lan_channel_numbers_count; i++)
	{
	  snprintf (tempbuf,
		    CONFIG_MAX_DESCRIPTION_LEN,
		    ", %u",
		    state_data->lan_channel_numbers[i]);
	  
	  strcat (description, tempbuf);
	}

      strcat (description, ")");
    }
  else
    snprintf (description,
	      CONFIG_MAX_DESCRIPTION_LEN,
	      "Give a valid number");

  if (config_section_add_key (state_data->pstate,
                              section,
                              "Channel_Number",
                              description,
                              0,
                              channel_number_checkout,
                              channel_number_commit,
                              config_number_range_four_bits) < 0)
    goto cleanup;

  if (config_section_add_key (state_data->pstate,
                              section,
                              "Alert_String_Set_Selector",
                              "Give a valid number",
                              0,
                              alert_string_set_selector_checkout,
                              alert_string_set_selector_commit,
                              config_number_range_seven_bits) < 0)
    goto cleanup;

  if (config_section_add_key (state_data->pstate,
                              section,
                              "Event_Specific_Alert_String",
                              "Possible values: Yes/No",
                              0,
                              event_specific_alert_string_checkout,
                              event_specific_alert_string_commit,
                              config_yes_no_validate) < 0)
    goto cleanup;

  return (section);

 cleanup:
  if (section)
    config_section_destroy (section);
  return (NULL);
}

