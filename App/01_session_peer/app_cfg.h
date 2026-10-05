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
#define ZP_SESSION_ROLE_UDP_PEER    (1)
#define ZP_SESSION_ROLE_CONNECTOR   (2)
#define ZP_SESSION_ROLE_LISTENER    (3)
#define ZP_SESSION_CHK_TCP(role)    (((role) == ZP_SESSION_ROLE_CONNECTOR) || ((role) == ZP_SESSION_ROLE_LISTENER))

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



// 각 실행 파일의 노드 번호는 CMake에서 전달. -> ex) -DZP_SESSION_NODE_ID=0
// 제노세션을 열 때, 각 노드별 key expression을 선택하여 사용.
#ifndef ZP_SESSION_NODE_ID
#error "ZP_SESSION_NODE_ID must be supplied by CMake"
#endif

#if (ZP_SESSION_COMMU_MODE == ZP_SESSION_COMMU_TCP)
    #define APP_TRANSPORT_NAME "tcp/"
    #define APP_ENDPOINT "tcp/" ZP_SESSION_TCP_IP ":" ZP_SESSION_PEER_PORT

    #if (ZP_SESSION_ROLE == ZP_SESSION_ROLE_CONNECTOR)
        #define APP_ROLE_NAME "connector"
        #define APP_ENDPOINT_KEY Z_CONFIG_CONNECT_KEY
    #elif (ZP_SESSION_ROLE == ZP_SESSION_ROLE_LISTENER)
        #define APP_ROLE_NAME "listener"
        #define APP_ENDPOINT_KEY Z_CONFIG_LISTEN_KEY
    #else
        #error "TCP requires listener or connector role"
    #endif
#elif (ZP_SESSION_COMMU_MODE == ZP_SESSION_COMMU_UDP)

    #if (ZP_SESSION_ROLE != ZP_SESSION_ROLE_UDP_PEER)
        #error "UDP requires peer role"
    #endif
    #define APP_TRANSPORT_NAME "udp/"
    #define APP_ROLE_NAME "multicast peer"
    // 동일 WSL 안에서 실습: 두 프로세스 모두 loopback 인터페이스 사용.
    #define APP_ENDPOINT "udp/" ZP_SESSION_UDP_IP ":" ZP_SESSION_PEER_PORT "#iface=lo"
    #define APP_ENDPOINT_KEY Z_CONFIG_LISTEN_KEY

#endif

// 각 노드별 key expression 정의. (실습용으로 6개만 정의)
#define ZP_SESSION_KEYEXPR_NODE0    "test/session_peer/0"     // 필수 기입사항
#define ZP_SESSION_KEYEXPR_NODE1    "test/session_peer/1"     // 필수 기입사항
#define ZP_SESSION_KEYEXPR_NODE2    "test/session_peer/2"     // 필수 기입사항
#define ZP_SESSION_KEYEXPR_NODE3    "test/session_peer/3"     // 필수 기입사항
#define ZP_SESSION_KEYEXPR_NODE4    "test/session_peer/4"     // 필수 기입사항
#define ZP_SESSION_KEYEXPR_NODE5    "test/session_peer/5"     // 필수 기입사항