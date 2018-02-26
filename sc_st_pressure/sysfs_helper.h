/*
 $License:
    Copyright (C) 2016 SanCloud all rights reserved.
 $
 */

/*******************************************************************************
 *
 * $Id$
 *
 ******************************************************************************/

#ifndef SC_SYSFS_HELPER_H__
#define SC_SYSFS_HELPER_H__

#ifdef __cplusplus
extern "C" {
#endif


int find_type_by_name(const char *name, const char *type);
sc_error_t sc_get_sysfs_path(char *name);
sc_error_t sc_get_sysfs_abs_path(char *name);
sc_error_t sc_get_dmpfile(char *name);
sc_error_t sc_get_chip_name(char *name);
sc_error_t sc_get_sysfs_key(unsigned char *key);
sc_error_t sc_get_handler_number(const char *name, int *num);
sc_error_t sc_get_input_number(const char *name, int *num);
sc_error_t sc_get_iio_trigger_path(char *name);
sc_error_t sc_get_iio_device_node(char *name);

#ifdef __cplusplus
}
#endif
#endif	/* SC_SYSFS_HELPER_H__ */
