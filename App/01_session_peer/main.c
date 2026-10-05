#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <zenoh-pico.h>
#include "app_cfg.h"

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
        z_drop(z_move(config));
        // config를 session에 소유권 이동했기에 drop 해줄 이유가 없음.
        return retVal;
    }
    else {
        printf("[INFO] z_open() success, session=%p\n", z_loan(session));
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
        printf("[INFO] zp_start_read_task() and zp_start_lease_task() success!\nWaiting for 5 seconds..\n");
    }

    // 5. 종료
    z_sleep_s(5); // 5초간 대기
    printf("[INFO] z_drop() success, session=%p\n",  (void *)z_loan(session));
    z_drop(z_move(session));

    return retVal;
}