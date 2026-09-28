#pragma once 

#include <zenoh-pico.h>

#ifndef ZP_SESSION_MAX_PEER_NUM     
#define ZP_SESSION_MAX_PEER_NUM     (10)                        // 필수 기입사항
#endif

#define ZP_SESSION_COMMU_UDP        (0)
#define ZP_SESSION_COMMU_TCP        (1)

#ifndef ZP_SESSION_COMMU_MODE // CMake 에서 -D 로 덮어쓸 수 있게
#define ZP_SESSION_COMMU_MODE       (ZP_SESSION_COMMU_TCP)      // 필수 기입사항
#endif

#define ZP_SESSION_MODE             (Z_CONFIG_MODE_PEER)
#define ZP_SESSION_PEER_PORT        "7447"

#define ZP_SESSION_ROLE_DEFALT      (0)
#if (ZP_SESSION_COMMU_MODE == ZP_SESSION_COMMU_UDP)
#define ZP_SESSION_ROLE_UDP_PEER    (1)
#elif (ZP_SESSION_COMMU_MODE == ZP_SESSION_COMMU_TCP)
#define ZP_SESSION_ROLE_CONNECTOR   (2)
#define ZP_SESSION_ROLE_LISTENER    (3)
#define ZP_SESSION_CHK_TCP(role)    ((role & ZP_SESSION_ROLE_CONNECTOR) == 0x1) // TCP 확인가능
#endif

#ifndef ZP_SESSION_ROLE
#define ZP_SESSION_ROLE             (ZP_SESSION_ROLE_DEFALT)    // 필수 기입사항
#endif

#if (ZP_SESSION_COMMU_MODE == ZP_SESSION_COMMU_UDP)
#define ZP_SESSION_PEER_LOCATOR     "udp/"
#define ZP_SESSION_UDP_IP           "224.0.0.224"
#elif (ZP_SESSION_COMMU_MODE == ZP_SESSION_COMMU_TCP)
#define ZP_SESSION_PEER_LOCATOR     "tcp/"
#define ZP_SESSION_TCP_IP           "127.0.0.1"
#else
#error "ZP_SESSION_COMMU_MODE is not defined"
#endif

#define ZP_SESSION_KEYEXPR_NODE0    "test/session_peer/0"     // 필수 기입사항
#define ZP_SESSION_KEYEXPR_NODE1    "test/session_peer/1"     // 필수 기입사항
#define ZP_SESSION_KEYEXPR_NODE2    "test/session_peer/2"     // 필수 기입사항
#define ZP_SESSION_KEYEXPR_NODE3    "test/session_peer/3"     // 필수 기입사항
#define ZP_SESSION_KEYEXPR_NODE4    "test/session_peer/4"     // 필수 기입사항
#define ZP_SESSION_KEYEXPR_NODE5    "test/session_peer/5"     // 필수 기입사항