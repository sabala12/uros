#ifndef UROS_FAULT_H
#define UROS_FAULT_H

#include <arch/x86_64/core/stub.h>

#define no_mangling	extern "C"

no_mangling void fault_handler()
{
}

#endif // UROS_FAULT_H
