#include QMK_KEYBOARD_H

#include "0xCD.h"

kb_state current_kb_state;

void state_sync_init(void) {
    memset(&current_kb_state, 0, sizeof(kb_state));
}

void state_sync_kb_state_update(void) {
    if (is_keyboard_master()) {
        current_kb_state.ac_on = autocorrect_is_enabled();
        current_kb_state.capsword_on = is_caps_word_on();
    }
}

void kb_state_sync(void) {
    // if(!is_transport_connected()) {
    //     return;
    // }

    if (is_keyboard_master()) {
        static kb_state last_state;
        static uint32_t last_sync;

        bool needs_sync = false;

        // check for state differences
        if (memcmp(&current_kb_state, &last_state, sizeof(kb_state))) {
            needs_sync = true;
            memcpy(&last_state, &current_kb_state, sizeof(kb_state));
        }

        // Send once per second anyway
        if (timer_elapsed32(last_sync) > 1000) needs_sync = true;

        if (needs_sync) {
            if (transaction_rpc_send(RPC_ID_SYNC_STATE_KB, sizeof(kb_state), &current_kb_state)) {
                last_sync = timer_read32();
            }
        }
    }
}

void kb_state_sync_slave(uint8_t m2s_size, const void* m2s_buffer, uint8_t s2m_size, void* s2m_buffer) {
    if (m2s_size == sizeof(kb_state)) {
        memcpy(&current_kb_state, m2s_buffer, sizeof(kb_state));
    }
}

bool state_sync_is_ac_on(void) {
    return current_kb_state.ac_on;
}


bool state_sync_capsword_on(void) {
    return current_kb_state.capsword_on;
}
