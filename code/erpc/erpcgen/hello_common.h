/* 定义项目名称,也是所有生成的源码文件名前缀 */

/*
 * Generated by erpcgen 1.13.0 on Mon Feb 10 20:01:52 2025.
 *
 * AUTOGENERATED - DO NOT EDIT
 */


#if !defined(_hello_common_h_)
#define _hello_common_h_


#if defined(__cplusplus)
extern "C"
{
#endif
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "erpc_version.h"

#if 11300 != ERPC_VERSION_NUMBER
#error "The generated shim code version is different to the rest of eRPC code."
#endif


#if !defined(ERPC_TYPE_DEFINITIONS_HELLO)
#define ERPC_TYPE_DEFINITIONS_HELLO

// Enumerators data types declarations
typedef enum lockErrors_t
{
    lErrorOk_c = 0,
    lErrorOutofMemory_c = 1,
    lErrorMaxError_c = 2
} lockErrors_t;

#endif // ERPC_TYPE_DEFINITIONS_HELLO

#if defined(__cplusplus)
}
#endif

#endif // _hello_common_h_
