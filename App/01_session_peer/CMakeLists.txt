# 같은 소스에 서로 다른 설정을 전달하여 실행 파일 4개 생성
function(add_session_example name transport mode role node_id)
    zpl_add_example(${name} main.c app_cfg.h)
    # build/<예제 폴더>/<tcp 또는 udp>/<실행 파일>
    set_target_properties(${name} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/${transport}"
    )
    target_compile_definitions(${name} PRIVATE
        ZP_SESSION_COMMU_MODE=${mode}
        ZP_SESSION_ROLE=${role}
        ZP_SESSION_NODE_ID=${node_id}
    )
endfunction()

add_session_example(tcp_listener  tcp ZP_SESSION_COMMU_TCP ZP_SESSION_ROLE_LISTENER  0)
add_session_example(tcp_connector tcp ZP_SESSION_COMMU_TCP ZP_SESSION_ROLE_CONNECTOR 1)
add_session_example(udp_peer0     udp ZP_SESSION_COMMU_UDP ZP_SESSION_ROLE_UDP_PEER  0)
add_session_example(udp_peer1     udp ZP_SESSION_COMMU_UDP ZP_SESSION_ROLE_UDP_PEER  1)

# 기본 빌드에 포함. --target session_examples로 이 네 개만 빌드 가능.
add_custom_target(session_examples ALL
    DEPENDS tcp_listener tcp_connector udp_peer0 udp_peer1)
