# ApiRestArduino
El uso de la Api Rest se usa para realizar conexiones por medio de arduino a un servidor por medio de la URL

Api Rest es desarrollada para realizar las diferentes conexiones por medio de WiFiClientSecure ya que es necesario
realizar conexiones a servidores que contiene un protocolo segúro.

Al momento de crear esto 06/06/2020 fue desarrollado para la creación de un método de tipo GET para consumir este
servidor, y realizarlo por medio de protocolo seguro.

Para realizar esto se necesita el fingerprint(const char *fingerprint[] = "00 00 00 00 00 00 00 00 00 00 00 00 00 00")
el cual es el certificado SHA1 de la coneción del servidor al cual se esta conectando el arduino.
