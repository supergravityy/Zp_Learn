#include <stdio.h>
#include <zenoh-pico.h>

int main(void) {
    printf("zenoh-pico %s\n", ZENOH_PICO);
    printf("ADVANCED_PUBLICATION=%d RX_CACHE=%d BATCH_UNICAST=%d FRAG_MAX=%d\n",
           Z_FEATURE_ADVANCED_PUBLICATION, Z_FEATURE_RX_CACHE,
           Z_BATCH_UNICAST_SIZE, Z_FRAG_MAX_SIZE);
    z_owned_config_t cfg;
    z_config_default(&cfg);
    z_drop(z_move(cfg));
    printf("OK\n");
    return 0;
}
