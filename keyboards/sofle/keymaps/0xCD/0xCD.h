#pragma once

#include QMK_KEYBOARD_H

// state_sync.c
void kb_state_sync_slave(uint8_t m2s_size, const void* m2s_buffer, uint8_t s2m_size, void* s2m_buffer);
void state_sync_init(void);
void state_sync_kb_state_update(void);
void kb_state_sync(void);

bool state_sync_is_ac_on(void);
bool state_sync_capsword_on(void);

typedef struct _kb_state {
    bool ac_on;
    bool capsword_on;
} kb_state;
