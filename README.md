# incubadora-neonatal-esp32
Sistema de monitoreo y control ambiental para incubadora neonatal basado en ESP32
# Sistema de Monitoreo y Control Ambiental para Incubadora Neonatal

**Proyecto:** Diseño Biomédico I  
**Institución:** Universidad Autónoma de Manizales  
**Autoras:** Isabella Buitrago Ceballos, Isabela Rivillas Quiguanás

## Descripción
Sistema embebido basado en ESP32 que monitorea temperatura y humedad en una incubadora neonatal. Incluye control automático mediante relés, señalización visual/sonora y control manual por Bluetooth.

## Componentes principales
- Microcontrolador ESP32
- Sensor DHT22
- Pantalla OLED 0.96"
- Módulos relé, buzzer, LEDs

## Comandos Bluetooth
| Comando | Acción |
|---------|--------|
| A / B | Ventilador ON / OFF |
| C / D | Calefactor ON / OFF |
| E / F | Buzzer activar / silenciar |
| M | Modo automático |
| R | Reiniciar sistema |
