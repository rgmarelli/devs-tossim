# DEVS-TOSSIM

Copyright (c) 2013-2014 Ricardo Guido Marelli
All rights reserved.

------------------------------------------------------------------------------------------

## 1. Manual de usuario


### 1.1. Estructura de directorios e instalación

El framework de simulación __DEVS-TOSSIM__ fue desarrollado y probado con TinyOS 2.1.2 bajo 
plataforma Linux. Para poder compilar la interfaz de usuario de DEVS es necesario instalar 
la biblioteca *readline* junto a sus archivos para desarrollo (headers). 

La instalación del framework consiste en la copia del código fuente a algún directorio del 
usuario. No se requiere realizar pasos adicionales. 

El framework se compone de dos directorios principales: *devsforcpp* y *devstossim*.

El directorio *devsforcpp* contiene la implementación de DEVS. Esta es una implementación 
genérica que se puede utilizar en forma independiente de los modelos para motes 
desarrollados.

El directorio *devstossim* contiene los modelos para simulación de redes de sensores y 
aplicaciones cliente y servidor básicas. Se compone de los siguientes subdirectorios:

* __client_mote__: es una aplicación cliente básica que se compila junto con las aplicaciones de TinyOS. Más adelante se detalla su uso.

* __interface__: contiene la interfase del simulador con TinyOS y la implementación del modelo TinyOSProcessor.

* __make__: contiene los Makefiles que permiten integrar el framework de simulación en el sistema de construcción (build system) de TinyOS; esto hace posible compilar las aplicaciones de TinyOS para ser ejecutadas en el simulador.

* __model__: contiene las implementaciones de los modelos DEVS descriptos en este trabajo (con la excepción del modelo TinyOSProcessor). Estás son las clases base que modelan el mote y se pueden extender para agregar comportamiento o tomar como ejemplo para generar nuevas implementaciones.

* __platforms__: este directorio contiene la contraparte TinyOS del framework de simulación.

* __server__: es un programa servidor básico que ejecuta el modelo acoplado *RadioMedium* junto a la interfaz de usuario DEVS. A este servidor se conectan los motes cliente (aplicaciones TinyOS). Se puede tomar como ejemplo para correr cualquier modelo acoplado de DEVS ya que el código es muy reducido. Más adelante se detalla su uso.

------------------------------------------------------------------------------------------

### 1.2. Compilación de aplicaciones TinyOS con DEVS-TOSSIM

Para compilar aplicaciones TinyOS en DEVS-TOSSIM es necesario exportar las variables de 
entorno *DEVSBASE* y *TOSMAKE_PATH*. Suponiendo que se haya instalado el simulador en el 
directorio /opt/devs se debe ejecutar:

    export DEVSBASE=/opt/devs/src
    export TOSMAKE_PATH=$DEVSBASE/devstossim/make/

DEVS-TOSSIM agrega una plataforma "*devs*" a TinyOS. Para compilar una aplicación contra el 
simulador se debe cambiar al directorio de la aplicación TinyOS que se desee compilar y 
ejecutar: *make devs*. Por ejemplo:

    cd /opt/tinyos-2.1.2/apps/RadioCountToLeds/
    make devs

Si la compilación fue exitosa se genera el directorio simbuild conteniendo un programa 
cliente mote y se lee el siguiente mensaje:

    *** Successfully built DEVS-TOSSIM (DEVS TinyOS Simulator).

------------------------------------------------------------------------------------------

### 1.3. Compilación del servidor

El programa servidor de ejemplo ejecuta el modelo acoplado *RadioMedium* en la interfaz de 
usuario de DEVS. Este programa es necesario recompilarlo solamente en caso de realizar 
cambios sobre el citado modelo. Para compilarlo se debe hacer:

    cd /opt/devs/devstossim/server
    make

Esto genera un binario server.

------------------------------------------------------------------------------------------

### 1.4. La interfaz de usuario de DEVS-TOSSIM

El framework DEVS incluye una interfaz de usuario básica que se compila junto al simulador 
DEVS de mayor jerarquía. Un ejemplo es el programa server descripto precedentemente.

La interfaz provee los siguientes comandos:

* __dump__. registra la cola de eventos de DEVS en la bitácora (log).

* __help__. Muestra los comandos disponibles.

* __info__. Imprime información básica de la simulación. Esto incluye:

  * *Model*: El nombre del modelo de mayor jerarquía.

  * *Childs*: cantidad de nodos conectados a ese modelo.

  * *nextTN*: tiempo del próximo evento DEVS.

  * *Event queue size*: cantidad de elementos en la cola de eventos de DEVS.

  * *Total Exec. time*: tiempo total que se utilizó para correr la simulación.

  * *Avg. Exec. time*: tiempo promedio de ejecución de cada evento DEVS.

  * *Time factor*: la forma en que la simulación está manejando el tiempo. Ver comando *time* más adelante.

  * *Iterations*: cantidad de iteraciones o pasos de simulación ejecutados.

* __loop__. Inicia un ciclo infinito de simulación.

* __step [N]__. Si se especifica un argumento, ejecuta esa cantidad de pasos de simulación. Si no se especifica un argumento, ejecuta un único paso de simulación.

* __stop__. Detiene la ejecución de la simulación.

* __time [T]__. Controla el tiempo durante la simulación. Si se especifica 1, el simulador ejecuta en tiempo real. Si se especifica 0, ejecuta tan rápido como sea posible. Si se especifica un número mayor a 1, se desacelera la simulación (expansión del tiempo). Si se especifica un número menor a 1, se acelera la simulación (contracción del tiempo).

* __quit__. Cierra el simulador y la conexión con todos los nodos conectados (childs).


La simulación se registra utilizando syslog y se utiliza la facilidad *USER*. Todos los logs
están precedidos por la cadena "*DEVSTOSSIM*" y se indica el PID del proceso que lo originó
y una cadena adicional que distingue si la línea de log fue generada por un modelo DEVS o
por TinyOS.

------------------------------------------------------------------------------------------

### 1.5. Ejecución de una simulación con DEVS-TOSSIM

Para ejecutar una simulación con DEVS-TOSSIM, primero es necesario ejecutar el programa 
servidor como sigue (el parámetro especifica en que puerto TCP escucha el servidor):

    cd /opt/devs/devstossim/server
    ./server 5001

Luego, se debe ejecutar el programa mote que se generó al compilar la aplicación TinyOS 
utilizando la plataforma *devs*. Cada instancia de este programa representa un nodo sensor 
independiente. El programa recibe los siguientes argumentos:

* __server__: nombre del equipo y puerto donde se encuentra el servidor. Por ejemplo: localhost:5001.

* __name__: nombre del nodo sensor.

* __id__: id del nodo sensor en TinyOS.

* __duty-cycle (opcional)__: ciclo de trabajo que utilizará el transceiver especificado como un número entre 0 y 1. Por defecto, se utiliza 1 con lo que el transceiver no se suspende.

* __x,y,z (opcional)__: posición del nodo sensor en el espacio.

* __serial port (opcional)__: si se especifica un número distinto de cero, se activa el *serial forwarder* en ese puerto.

* __sensor value (opcional)__: si se especifica un valor, el sensor oscilará alrededor de ese valor; caso contrario, el valor será al azar.

* __data sensor port (opcional)__: si se especifica este parámetro, se ignora sensor value y se instancia en su lugar un sensor del tipo *DataSensor* que esperará información de sensado en el puerto especificado.

Por ejemplo, para ejecutar los siguientes dos nodos:

    * un nodo llamado root con id 0, ciclo de trabajo del 50% (0.5), en la posición 
      [0,0,0], con serial forwarder activado en el puerto 9002 y cuyo sensor oscile por 
      sobre 5000.

    * un nodo llamado node1 con id 1, ciclo de trabajo del 50% (0.5), en la posición 
      [0,0,50] y cuyo sensor oscile por sobre 7000.

Se debe hacer:

    cd /opt/tinyos-2.1.2/apps/RadioCountToLeds/simbuild
    ./mote localhost:5001 root 0 0.5 0,0,0 9002 5000 &
    ./mote localhost:5001 node1 1 0.5 0,0,50 0 7000 &

Adicionalmente, se incluye un script Bash llamado *nodes.sh* que puede encontrarse en el directorio *devstossim/client_mote* y que genera una grilla de nodos con valores de sensor al azar. Este script recibe los siguientes argumentos:
 
* __binary__: ruta al programa de TinyOS que se desea a ejecutar.

* __nodes__: cantidad total de nodos

* __deep__: cantidad de filas en la grilla. Es decir: si se especifican 25 nodos y deep 5 se genera una grilla de 5 x 5.

* __x step__: distancia entre nodos sobre el eje X.

* __y step__: distancia entre nodos sobre el eje Y.

Por ejemplo, si se ejecuta el script con los siguientes argumentos:

    ./nodes.sh /opt/tinyos-2.1.2/apps/RadioCountToLeds/simbuild/mote 25 5 30 20

se genera una grilla de 5 x 5 donde los nodos están espaciados 30 metros sobre el eje X y 20 metros sobre el eje Y. El programa muestra la grilla generada donde puede observarse la posición en la primer fila y la primer columna y el identificador del nodo en el resto de las celdas:

    Y\X	 30	 60	 90	120	150	
     20	  1	  2	  3	  4	  5	
     40	  6	  7	  8	  9	 10	
     60	 11	 12	 13	 14	 15	
     80	 16	 17	 18	 19	 20	
    100	 21	 22	 23	 24	 25


------------------------------------------------------------------------------------------

## 2. Manual de desarrollo


### 2.1. Introducción

Este manual explica brevemente como utilizar el framework DEVS y generar nuevos modelos. 
El framework DEVS es genérico y puede utilizarse para realizar otros simuladores que no 
tengan relación con redes de sensores inalámbricas.

Entre los modelos DEVS generados para implementar DEVS-TOSSIM hay modelos relativamente 
simples, como ser la clase *Timer*, que se pueden tomar como referencia.

------------------------------------------------------------------------------------------

### 2.2. Creación de nuevos modelos atómicos DEVS

Para generar un nuevo modelo atómico DEVS se debe heredar de la clase abstracta 
*AtomicModel*. Esta clase define los siguientes tres métodos abstractos para los cuales 
debe proveerse una implementación:


##### virtual void internalTransition()

    Función de transición interna.

##### virtual void externalTransition(DEVS::ExternalMessage* message)

    Función de transición externa. Recibe como parámetro el mensaje externo. Este puntero 
    no se debe liberar ya que la memoria es liberada automáticamente por el framework.

##### virtual DEVS::OutputMessage* outputFunction()

    Función de salida. Esta función se invoca para generar el mensaje de salida que 
    depende del estado del modelo. Para devolver un mensaje de salida, se debe alocar 
    memoria que será liberada automáticamente por el framework DEVS.
    En caso de que no se requiera generar ningún mensaje de salida, el método debe 
    devolver NULL.


Por otro lado, la función de duración de estado tiene una implementación por defecto que devuelve el valor de la variable de estado sigma como se detalla a continuación:

##### virtual TIME timeAdvanceFunction()

    La variable de estado sigma se puede asignar invocando el método setSigma() en el 
    método que corresponda (por ejemplo, en internalTransition).
     
    El método timeAdvanceFunction() se puede sobrecargar en las clases derivadas con el 
    fin de proveer una implementación diferente.
     
     La clase TIME representa el tiempo en DEVS y admite la asignación de un valor infinito
    para pasivar el modelo: DEVS::TIME::infinity(). Tiene precisión de nanosegundos.


Respecto al control de las fases o estados en los que puede estar el modelo se proveen los siguientes métodos:

##### void registerPhase(std::string phase)

    Esté método registra una fase. Todas las fases o estados que el modelo puede tomar 
    deben registrarse en el constructor.

##### std::string phase()

    Devuelve la fase actual.

##### setPhase(std::string phase)

    Se utiliza para cambiar la fase del modelo durante una transición externa, durante una
    transición interna, o en construcción para asignar la fase inicial. El método valida
    que la fase que se está intentando asignar haya sido registrada previamente.

##### bool phaseIs(std::string phase)

    Permite validar si el modelo se encuentra en la fase especificada.


Finalmente, al construir un modelo atómico se deben registrar los puertos de los que dispone ese modelo. Los puertos de entrada y salida del modelo son accesibles mediante los métodos *outputPorts()* e *inputPorts()*. Por ejemplo:

    this->outputPorts().add( DEVS::Port( this-> name(),
                                         "TimerFire") );
                                         
     Agrega al modelo un puerto de salida llamado TimerFire. Notar que el puerto se 
     construye especificando el nombre del modelo atómico.
     
    this->inputPorts().add( DEVS::Port(this->name(),
                          "TimerStart"));
                          
     Agrega al modelo un puerto de entrada llamado TimerStart.

------------------------------------------------------------------------------------------
 
### 2.3. Creación de nuevos modelos acoplados DEVS 

Para generar nuevos modelos acoplados se utiliza la clase CoupledModel. Esta clase puede 
instanciarse directamente o derivarse para definir el modelo acoplado en el constructor o 
si se requiere sobrecargar la función de traducción de la salida. La definición de un 
modelo acoplado implica  los siguientes cuatro puntos:

* __Registrar los puertos de entrada y salida del modelo acoplado__.

     Esto se realiza del mismo modo que para los modelos atómicos.

* __Registrar los modelos que componen el modelo acoplado__.

      Los modelos componentes pueden ser modelos atómicos o modelos acoplados. Como puede 
      verse en el ejemplo siguiente, los modelos componente se registran utilizando el 
      nombre del modelo con lo que ese nombre debe ser único en el contexto del modelo 
      acoplado.

        DEVS::CoupledModel coupled ( "UnModeloAcoplado" );
        coupled.add( "modelo1" );
        coupled.add( "modelo2" );

* __Registrar los acoplamientos__.

      Se pueden establecer acoplamientos entre los modelos componentes o acoplar los 
      puertos del modelo acoplado a los modelos componentes. Un mismo puerto puede 
      acoplarse a varios puertos. El framework no verifica que el puerto efectivamente 
      exista en el modelo de destino al realizar el acoplamiento; no obstante esto se 
      valida y registra en el log al ejecutar la simulación. Ejemplos de acoplamientos:

        coupled.addCoupling( model1.outputPorts().getPortByName( "Out"),
                             model2.inputPorts().getPortByName( "In") );

          Acopla el puerto de salida Out de model1 al puerto de entrada In de model2. 
          Este ejemplo supone que se tiene acceso a los modelos componente.

        coupled.addCoupling( DEVS::Port("modelo1","Out"), DEVS::Port("modelo2","In"));

          Este acoplamiento es similar al anterior con la diferencia que no requiere 
          disponer de las instancias de los modelos componentes sino que utiliza 
          directamente el nombre de los mismos.

        coupled.addCoupling( DEVS::Port("modelo1","Out), 
                             coupled.outputPorts().getPortByName("Out") );

          Acopla el puerto de salida Out del modelo modelo1 al puerto de salida Out del 
          modelo acoplado.


* __Determinar si se necesita una función de traducción de la salida especifica__.

      Por defecto, el framework provee una función de traducción de la salida genérica. 
      No obstante, puede requerirse realizar una función específica para el modelo 
      acoplado como se hizo en DEVS-TOSSIM para implementar la ecuación de Friis en el 
      modelo de radio. El método que debe sobrecargarse es el siguiente:

        virtual ExternalMessage* translate( OutputMessage *message, Port &dstPort )

          Este método se invoca una vez por cada puerto dstPort que esté acoplado al
          puerto de salida especificado en el mensaje. Se permite que la función devuelva 
          NULL lo que indica que para ese puerto se está suprimiendo la salida. Para 
          devolver un mensaje, se debe alocar memoria que será liberada automáticamente 
          por el framework DEVS.

------------------------------------------------------------------------------------------

### 2.4. Instanciación de simuladores

Para realizar una simulación, es necesario instanciar los simuladores correspondientes. 
Cada modelo DEVS debe asociarse con un simulador encargado de ejecutar ese modelo. 
Por ejemplo: 

    SimpleAtomic atomic_model1( "modelo1" );
    SimpleAtomic atomic_model2( "modeol2" );
    DEVS::CoupledModel coupled( "UnModeloAcoplado" );
    
    DEVS::AtomicSimulator simulator1( &atomic_model1);
    DEVS::AtomicSimulator simulator2( &atomic_model2);
    DEVS::CoupledSimulator coupled_simulator( &coupled );

Las líneas de código precedentes instancian dos modelos atómicos y un modelo acoplado e instancian los simuladores correspondientes a cada modelo.

    coupled_simulator.addSimulator( &simulator1 );
    coupled_simulator.addSimulator( &simulator2 );

Una vez que se han instanciado los simuladores se deben registrar los simuladores de los modelos componentes con el simulador del modelo acoplado que componen.

    for( int i=0; i < 20; i++ ) {
        coupled_simulator.simulate();
    }

El código anterior ejecuta 20 pasos de simulación en tiempo virtual (tan rápido como sea posible).

Alternativamente, se pueden utilizar las clases *CoupledCompositeModel* y *CoupledCompositeSimulator* para generar toda la jerarquía de simuladores como se muestra en el ejemplo siguiente:

     DEVS::CoupledCompositeModel coupled( "UnModeloAcoplado" );
     SimpleModel* model1 = new SimpleModel( "modelo1" );
     SimpleModel* model2 = new SimpleModel( "modelo2" );
     
     coupled.add( model1 );
     coupled.add( model2 );

Los modelos se registran en el modelo acoplado. Al usar *CoupledCompositeModel*, el registro se realiza utilizando directamente el objeto en lugar del nombre del modelo.

El modelo registrado se destruye automáticamente cuando se destruye el modelo acoplado. Los modelos componentes pueden crearse y registrarse directamente en el constructor del modelo acoplado.

    DEVS::CoupledCompositeSimulator coupled_simulator(&coupled);

Al instanciar la clase *CoupledCompositeSimulator* se instancian automáticamente todos los simuladores de los modelos componente.

------------------------------------------------------------------------------------------
 
### 2.5. Código que puede utilizarse como referencia

Los siguientes archivos del simulador se pueden utilizar como ejemplo de uso de otras 
clases y funcionalidades del framework DEVS-TOSSIM:

* __Interfaz de usuario y simuladores remotos (servidor)__

  * *devstossim/server/main.cpp*

     Este programa muestra el uso de las clases RemoteSimulatorAcceptor y RemoteBinding 
     para generar un simulador de un modelo acoplado que acepta conexiones de simuladores 
     remotos. Además, muestra como se instancia y activa la interfaz de línea de comandos 
     del framework DEVS. 

* __Simuladores remotos (cliente)__

  * *devstossim/client_mote/mote.cpp*

     Este programa muestra el uso de la clase RemoteSimulatorConnector para conectarse a un
     simulador remoto incluyendo el envío de parámetros en la conexión. Además, es un 
     ejemplo de instanciación y activación de la clase DataCollector.

* __Integración con sistemas externos (DataCollector)__

  * *devstossim/model/Data_Sensor.h*

     Este modelo utiliza la clase DataCollector para leer línea a línea valores de sensor. 
     Muestra como inspeccionar el stream de datos sin retirar la información del buffer.

  * *devstossim/model/Connected_Serial.h*

     Este modelo utiliza la clase DataCollector para intercambiar información 
     (envío y recepción) con un sistema externo. Además, muestra como intercambiar mensajes
     durante la conexión con el sistema externo (clase Serial_Handshake).

* __Sobrecarga de la función de traducción de la salida__

  * *devstossim/model/RadioMedium.h*

     El modelo de radio sobrecarga la función de la salida para aplicar la ecuación de 
     Friis. En este sentido, muestra como generar un mensaje de transición externo 
     (ExternalMessage) a partir de un mensaje de salida (OutputMessage). Adicionalmente, 
     este modelo utiliza el método addCouplingForAllModels para generar automáticamente los
     acoplamientos de los modelos componentes.

* __Modelo de nodos sensores__

  * *devstossim/model/Mote.h*

     El modelo Mote es el modelo básico de nodo sensor. Este modelo puede usarse como base
     para generar nodos sensores que tengan otros componentes (otro tipo de transceiver, 
     diferentes sensores, incorporar un modelo de batería, etc.).
