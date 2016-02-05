#include "eventmanager.h"

int main() {
	if (evm_init()) return 1;
	else return evm_run();
}