# 🏭 Simulación Multihilo de Sistema de Control de Producción

Sistema ejecutable escrito en **C** que simula el entorno de producción de una fábrica mediante programación concurrente, hilos POSIX y mecanismos avanzados de sincronización.

---

## 📌 Descripción General

El programa simula el flujo de producción y montaje utilizando una arquitectura jerárquica multihilo:

* **`factory_manager` (Orquestador principal):** Lee un fichero de configuración, valida los datos e inicializa y coordina múltiples gestores de procesos.
* **`process_manager` (Línea de producción):** Gestiona una cinta transportadora virtual operada por hilos independientes de tipo **Productor-Consumidor**[cite: 2].
* **Cola Circular (`queue`):** Estructura de datos compartida que actúa como búfer/cinta donde los productos son insertados y extraídos de forma segura[cite: 2].

---

## 🛠️ Arquitectura y Sincronización

* **Hilos POSIX (`pthread`):** Coordinación entre el hilo principal (`factory_manager`), los gestores (`process_manager`) y las parejas de *producer/consumer*[cite: 2].
* **Exclusión Mutua y Sincronización:** Uso de `pthread_mutex` y variables de condición (`pthread_cond`) para evitar condiciones de carrera en el llenado/vaciado de las colas[cite: 2].
* **Semáforos POSIX (`sem_t`):** Coordinación del orden de ejecución y ordenación estricta de las trazas de salida en consola entre múltiples procesos[cite: 2].

---

## 📁 Estructura del Proyecto

```text
.
├── factory_manager.c   # Orquestador del sistema (punto de entrada)[cite: 2]
├── process_manager.c   # Gestión de líneas y lógica Productor-Consumidor[cite: 2]
├── process_manager.h   # Definición de parámetros y cabeceras de procesos[cite: 2]
├── queue.c             # Implementación de la cola circular compartida[cite: 2]
├── queue.h             # Definición de la estructura de la cola y elementos[cite: 2]
└── Makefile            # Script de compilación del proyecto[cite: 2]
````
Readme hecho por gemini
