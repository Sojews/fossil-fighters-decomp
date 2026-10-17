// NitroSDK SND: alarms, which the ARM7 driver signals through the PXI FIFO. Each carries an id so a signal
// for an alarm that has since been set up again is ignored.

#include "nitro/snd.h"

#define SND_ALARM_NUM 8

typedef struct {
    SNDAlarmHandler func;
    void *arg;
    u8 id;
} SNDAlarm;

static SNDAlarm sAlarm[SND_ALARM_NUM];

void SND_AlarmInit(void) {
    int i;
    for (i = 0; i < SND_ALARM_NUM; i++) {
        sAlarm[i].func = NULL;
        sAlarm[i].arg = NULL;
        sAlarm[i].id = 0;
    }
}

void SNDi_IncAlarmId(int alarmNo) {
    SNDAlarm *alarm = &sAlarm[alarmNo];
    alarm->id++;
}

int SNDi_SetAlarmHandler(int alarmNo, SNDAlarmHandler handler, void *arg) {
    SNDAlarm *alarm = &sAlarm[alarmNo];
    alarm->func = handler;
    alarm->arg = arg;
    alarm->id++;
    return alarm->id;
}

void SNDi_CallAlarmHandler(int alarmNo) {
    SNDAlarm *alarm = &sAlarm[alarmNo & 0xff];
    u8 id = (u8) (alarmNo >> 8);
    if (id != alarm->id) {
        return;
    }
    if (alarm->func == NULL) {
        return;
    }
    alarm->func(alarm->arg);
}
