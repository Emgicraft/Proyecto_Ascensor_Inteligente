# Proyecto_Ascensor_Inteligente
El proyecto consta en controlar por voz el piso al que deseo llegar, esto por medio de una aplicación por celular y conexión bluetooth.

Para esto, se hizo uso del microcontrolador ESP32 en su presentación como DevKit v1 para el control del ascensor y para establecer la conexión bluetooth con el equipo móvil (smartphone), además de otros componentes más como el módulo de 4 relés para Arduino, una librería específica para el control bluetooth del ESP32-DevKit y otra librería más para el control de una pantalla LCD, el cual mostraría el estado actual del ascensor.

## Materiales usados
Como visión general, se usaron engranajes y rodamientos para la transmisión de movimiento del motor al ascensor. Se usaron también 4 varillas de fierro en ángulo como soporte de toda la estructura y tablas de madera o tripley para un mejor acabado. En resumen, usamos lo que tuvimos a la mano:

    1. Engranajes
    2. Rodamientos
    3. Motor de 12V con reductor
    4. Varillas de fierro en ángulo recto (90°)
    5. Plancas de tripley y/o madera

## Librerías adicionales
Se hizo uso de dos librerías específicas para el ESP32, las cuales adjunto en este respositorio con los nombres [ESP32_BluetoothSerial](ESP32_BluetoothSerial/readme.md) _(Evandro Copercini, 2018)_ y [library-LCD-I2C-master](library-LCD-I2C-master/readme.md) _(Desconocido pero basado en DFROBOT)_. Por supuesto, siempre dando el agradecimiento al trabajo de aquellos quienes crearon dichas librerias.

## Advertencia
Por último, indicar que el material adjunto en este repositorio ha sido subido pasado ya varios años de haber hecho el proyecto, por lo que no garantizo que todo este completo u ordenado. Sin embargo, espero pueda servir como referencia, en especial el informe que a pesar de no ser muy riguroso, si explica y adjunta capturas del proyecto.
