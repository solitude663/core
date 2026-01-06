#ifndef BASE_ENTRY_H
#define BASE_ENTRY_H

internal void BaseInit();
internal void BaseShutdown();

internal void BaseMainThreadEntry(void (*entry)(int, char**), int argc, char **argv);
internal void BaseThreadEntry(void (*entry)(void *p), void *params);

#endif // Header guard
