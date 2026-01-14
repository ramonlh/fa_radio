
#include <7Semi_MCP23017.h>

// MCP23017 at default address 0x20 on default Wire bus
MCP23017_7Semi mcp(0x20);

// Pines
const uint8_t BUTTON_PIN = 0;
const uint8_t LED_PIN    = 8;

// Mapas para tus pines de multiplexor
const uint8_t muxA[8] = {0,  1,  2,  3,  4,  5,  6,  7};
                      // 80m 40m 30m 20m 15m 10m USB LSB
const uint8_t muxB[8] = {8, 9, 10, 11, 12, 13, 14, 15};
                      // CW PTT

// -----------------------------------------------------------------------------
// Inicialización del MCP23017
// -----------------------------------------------------------------------------
int init_MCP23017()
{
  // Iniciar MCP23017 en dirección I2C 0x20
  if (!mcp.begin(SDA, SCL)) {
    return 1;  // No encontrado
  }


  // Configurar pines de multiplexor como salidas
  for (int i = 0; i < 8; i++) {
    mcp.pinMode((pins)muxA[i], OUTPUT);
    mcp.pinMode((pins)muxB[i], OUTPUT);
  }
    mcp.pinMode((pins)muxB[1], INPUT_PULLUP); // PTT


  return 0;  // OK
}

void digitalWriteMux(int pin, int value)
{
  s2("pin:"); s2(pin); 
  s2("  value:"); s2(value); s2("\n");
  mcp.digitalWrite((pins)pin, value ? HIGH : LOW);
}
