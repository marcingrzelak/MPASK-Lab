#ifndef NET_SNMP_CONFIG_API_H
#define NET_SNMP_CONFIG_API_H

    /**
     *  Library API routines concerned with configuration and control
     *    of the behaviour of the library, agent and other applications.
     */

#include "types.h"

#ifdef __cplusplus
extern          "C" {
#endif

    struct config_line *register_config_handler(const char *filePrefix,
                                                const char *token,
                                                void (*parser) (const char *, char *),
                                                void (*releaser) (void),
                                                const char *usageLine);
    struct config_line *register_app_config_handler(
                                                const char *token,
                                                void (*parser) (const char *, char *),
                                                void (*releaser) (void),
                                                const char *usageLine);

/* These two are documented as "register{,_app}_premib_handler() */
/*?*/    struct config_line *register_prenetsnmp_mib_handler(const char *filePrefix,
                                                const char *token,
                                                void (*parser) (const char *, char *),
                                                void (*releaser) (void),
                                                const char *usageLine);
/*?*/    struct config_line *register_app_prenetsnmp_mib_handler(
                                                const char *token,
                                                void (*parser) (const char *, char *),
                                                void (*releaser) (void),
                                                const char *usageLine);
                                                            
    void            unregister_config_handler(const char *filePrefix, const char *token);
    void            unregister_app_config_handler(                    const char *token);
    void            unregister_all_config_handlers(void);

/*?*/ void register_mib_handlers(void);
    void            read_configs(void);
    void            read_premib_configs(void);

    void            read_config_print_usage(const char *lead);
    void            config_perror(const char *);
    void            config_pwarn(const char *);

#ifdef __cplusplus
}
#endif

    /*
     *  For the initial release, this will just refer to the
     *  relevant UCD header files.
     *    In due course, the routines relevant to this area of the
     *  API will be identified, and listed here directly.
     *
     *  But for the time being, this header file is a placeholder,
     *  to allow application writers to adopt the new header file names.
     */
#include "library\snmp_api.h"
		 
#include "library\read_config.h"
#include "library\default_store.h"
		 
#include <stdio.h>              /* for FILE definition */
#include "library\snmp_parse_args.h"
#include "library\snmp_enum.h"
#include "library\vacm.h"

#endif                          /* NET_SNMP_CONFIG_API_H */
