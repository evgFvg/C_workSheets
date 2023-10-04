#include "fsm.h"

#define STATE_NUMBER (4)
#define EVENT_NUMBER (2)

typedef enum state
{
    START,
    BETWEEN,
    ACCEPT,
    REJECT
} state_t;

typedef enum event
{
    ZERO,
    ONE
} event_t;

typedef state_t (*f_ptr)();

static state_t ReturnAccept(void);
static state_t ReturnBetween(void);
static state_t ReturnReject(void);


int IsValidString(const char *str)
{
    state_t state = START;
    event_t event = 0;

    static f_ptr event_matrix[STATE_NUMBER][EVENT_NUMBER] =
        {
            {ReturnBetween, ReturnReject},
            {ReturnAccept, ReturnBetween},
            {ReturnAccept, ReturnBetween},
            {ReturnReject, ReturnReject}
        };

    while ('\0' != *str && state != REJECT)
    {
        event = (event_t)(*str - '0');
        state = event_matrix[state][event]();
        ++str;
    }

    return state == ACCEPT;
}

static state_t ReturnBetween()
{
    return BETWEEN;
}
static state_t ReturnReject()
{
    return REJECT;
}
static state_t ReturnAccept()
{
    return ACCEPT;
}

/*

int IsValidString(const char *str)
{
    state_t state = START;
    event_t event = 0;

    static state_t event_matrix[REJECT + 1][ONE + 1] =
        {
            {BETWEEN, REJECT},
            {ACCEPT, BETWEEN},
            {ACCEPT, BETWEEN},
            {REJECT, REJECT}};

    while ('\0' != *str && event != REJECT)
    {
        event = (event_t)(*str - '0');
        state = event_matrix[state][event];
        ++str;
    }

    return state == ACCEPT;
}

*/


