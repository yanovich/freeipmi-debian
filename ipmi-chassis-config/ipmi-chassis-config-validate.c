/*
 * Copyright (C) 2008-2013 FreeIPMI Core Team
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

#include "ipmi-chassis-config.h"
#include "ipmi-chassis-config-map.h"
#include "ipmi-chassis-config-validate.h"

#include "freeipmi-portability.h"

config_validate_t
power_restore_policy_number_validate (const char *section_name,
                                      const char *key_name,
                                      const char *value,
                                      void *arg)
{
  assert (section_name);
  assert (key_name);
  assert (value);

  if (power_restore_policy_number (value) != -1)
    return (CONFIG_VALIDATE_VALID_VALUE);
  return (CONFIG_VALIDATE_INVALID_VALUE);
}

config_validate_t
bios_boot_type_number_validate (const char *section_name,
                                const char *key_name,
                                const char *value,
                                void *arg)
{
  assert (section_name);
  assert (key_name);
  assert (value);

  if (bios_boot_type_number (value) != -1)
    return (CONFIG_VALIDATE_VALID_VALUE);
  return (CONFIG_VALIDATE_INVALID_VALUE);
}

config_validate_t
boot_device_number_validate (const char *section_name,
                             const char *key_name,
                             const char *value,
                             void *arg)
{
  assert (section_name);
  assert (key_name);
  assert (value);

  if (boot_device_number (value) != -1)
    return (CONFIG_VALIDATE_VALID_VALUE);
  return (CONFIG_VALIDATE_INVALID_VALUE);
}

config_validate_t
device_instance_selector_number_validate (const char *section_name,
                                          const char *key_name,
                                          const char *value,
                                          void *arg)
{
  assert (section_name);
  assert (key_name);
  assert (value);

  if (device_instance_selector_number (value) != -1)
    return (CONFIG_VALIDATE_VALID_VALUE);
  return (CONFIG_VALIDATE_INVALID_VALUE);
}

config_validate_t
firmware_bios_verbosity_number_validate (const char *section_name,
                                         const char *key_name,
                                         const char *value,
                                         void *arg)
{
  assert (section_name);
  assert (key_name);
  assert (value);

  if (firmware_bios_verbosity_number (value) != -1)
    return (CONFIG_VALIDATE_VALID_VALUE);
  return (CONFIG_VALIDATE_INVALID_VALUE);
}

config_validate_t
console_redirection_number_validate (const char *section_name,
                                     const char *key_name,
                                     const char *value,
                                     void *arg)
{
  assert (section_name);
  assert (key_name);
  assert (value);

  if (console_redirection_number (value) != -1)
    return (CONFIG_VALIDATE_VALID_VALUE);
  return (CONFIG_VALIDATE_INVALID_VALUE);
}
