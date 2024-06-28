#if !defined(__dami_tasks_h__)
#define __dami_tasks_h__

void ui_task(void *args);

void reboot_watchdog_task(void *);

void update_qr_code_task(void *);

#endif // __dami_tasks_h__
