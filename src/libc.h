extern "C" void outb(uint16_t port, uint8_t val);
extern "C" uint8_t inb(uint16_t port);
extern "C" void enterRealMode();
extern "C" void enterProtectedMode();