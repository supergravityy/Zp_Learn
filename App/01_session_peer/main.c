#include <stdio.h>
#include <stdlib.h>
#include <zenoh-pico.h>
#include "app_cfg.h"

// 현재 확인된 상대 peer마다 호출되는 콜백함수. context는 peer_count의 주소.
static void on_peer(const z_id_t *id, void *context) // z_owned_closure_zid_t에 등록하는 함수원형 "void func(const z_id_t *id, void *context);"
{
    int *peer_count = (int *)context;
    (*peer_count)++;

    z_owned_string_t id_string;
    if (z_id_to_string(id, &id_string) < 0) {
        printf("[ERROR] z_id_to_string() failed\n");
        return;
    }

    printf("[PEER] ID=%.*s\n",
           (int)z_string_len(z_loan(id_string)),
           z_string_data(z_loan(id_string)));
    z_drop(z_move(id_string));
}

// 30초 동안 현재 세션이 알고 있는 상대를 조회. scouting 요청은 아님.
static int seek_partner(const z_loaned_session_t *session)
{
    int time_sec = 0, retVal = 0, peer_count = 0;
    z_owned_closure_zid_t callback;

    while (time_sec < 30 && peer_count == 0) {
        
        z_closure(&callback, on_peer, NULL, &peer_count); // 호출될 함수와 함수에 전달할 데이터를 묶어서 객체로 만드는 매크로

        // 콜백은 이 조회가 반환하기 전에 실행됨.
        retVal = z_info_peers_zid(session, z_move(callback));
        if (retVal < 0) {
            printf("[ERROR] z_info_peers_zid() failed, retVal=%d\n", retVal);
            return retVal;
        }

        if (peer_count == 0) {
            printf("[PEER] elapsed=%ds, waiting for remote peer...\n", time_sec);
        } else {
            printf("[PEER] elapsed=%ds, count=%d\n", time_sec, peer_count);
        }
        z_sleep_s(1);
        time_sec++;
    }
    return 0;
}

int main(void)
{
    int retVal = 0;
    z_owned_config_t config;
    z_owned_session_t session;

    printf("[CONFIG] nodeID=%d, commuMode=%d, role=%d\n", ZP_SESSION_NODE_ID, ZP_SESSION_COMMU_MODE, ZP_SESSION_ROLE);

    // 1. config 생성
    retVal = z_config_default(&config);
    if (retVal < 0) {
        printf("[ERROR] z_config_default() failed, retVal=%d\n", retVal);
        z_drop(z_move(config));
        return retVal;
    }

    // 2. config 설정
    retVal = zp_config_insert(z_loan_mut(config), Z_CONFIG_MODE_KEY, ZP_SESSION_MODE);
    if (retVal < 0) {
        printf("[ERROR] zp_config_insert()_1 failed, retVal=%d\n", retVal);
        z_drop(z_move(config));
        return retVal;
    }
    retVal = zp_config_insert(z_loan_mut(config), APP_ENDPOINT_KEY, APP_ENDPOINT);
    if (retVal < 0) {
        printf("[ERROR] zp_config_insert()_2 failed, retVal=%d\n", retVal);
        z_drop(z_move(config));
        return retVal;
    }

    // 3. session 생성
    retVal = z_open(&session, z_move(config), NULL); 
    if (retVal < 0) {
        printf("[ERROR] z_open() failed, retVal=%d\n", retVal);
        // config는 z_open에 소유권을 넘겼으므로 다시 해제하지 않음.
        return retVal;
    }
    else {
        printf("[INFO] z_open() success, session=%p\n", (void *)z_loan(session));
    }

    // 4. 백그라운드 통신 태스크 시작
    retVal = zp_start_read_task(z_loan_mut(session), NULL); // polling방식으로 수신된 메시지를 처리하는 백그라운드 태스크 시작
    if (retVal < 0) {
        printf("[ERROR] zp_start_read_task() failed, retVal=%d\n", retVal);
        z_drop(z_move(session));
        return retVal;
    }
    retVal = zp_start_lease_task(z_loan_mut(session), NULL); // 생존신고용 백그라운드 태스크 시작
    if (retVal < 0) {
        printf("[ERROR] zp_start_lease_task() failed, retVal=%d\n", retVal);
        z_drop(z_move(session));
        return retVal;
    }
    else{
        printf("[INFO] zp_start_read_task() and zp_start_lease_task() success!\nChecking peers for 30 seconds...\n");
    }

    // 5. 상대 peer 확인 (오류가 발생해도 세션은 아래에서 정리)
    retVal = seek_partner(z_loan(session));

    // 6. 종료
    z_drop(z_move(session));
    printf("[INFO] session released\n");

    return retVal;
}
